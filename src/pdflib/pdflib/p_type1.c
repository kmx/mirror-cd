/*---------------------------------------------------------------------------*
 |              PDFlib - A library for generating PDF on the fly             |
 +---------------------------------------------------------------------------+
 | Copyright (c) 1997-2006 Thomas Merz and PDFlib GmbH. All rights reserved. |
 +---------------------------------------------------------------------------+
 |                                                                           |
 |    This software is subject to the PDFlib license. It is NOT in the       |
 |    public domain. Extended versions and commercial licenses are           |
 |    available, please check http://www.pdflib.com.                         |
 |                                                                           |
 *---------------------------------------------------------------------------*/

/* $Id$
 *
 * PDFlib Type1 font handling routines
 *
 */

#include "pc_ctype.h"

#include "p_intern.h"
#include "p_font.h"


/* Type 1 font portions: ASCII, encrypted, zeros */
typedef enum { t1_ascii, t1_encrypted, t1_zeros } pdf_t1portion;

typedef struct {
    pdf_t1portion  portion;
    size_t         length[4];
    pdc_file      *fontfile;
    pdc_byte      *img;            /* in-core Type1 font file image   */
    pdc_byte      *end;            /* first byte above image buf   */
    pdc_byte      *pos;            /* current "file" position      */
} t1_private_data;

#define PFA_TESTBYTE    4

#define PDF_CURRENTFILE "currentfile eexec"


/* ---------------------------- General platforms --------------------------- */

#define LINEBUFLEN      256

/*
 * PFA files are assumed to be encoded in host format. Therefore
 * we must use literal strings and characters for interpreting the
 * font file.
 */

static int
PFA_data_fill(PDF *p, PDF_data_source *src)
{
    static const char *fn = "PFA_data_fill";
#ifndef PDFLIB_EBCDIC
    static const char HexToBin['F' - '0' + 1] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0,
        0, 10, 11, 12, 13, 14, 15
    };
#else
#endif
    char *s, *c;
    int i;
    int len;
    t1_private_data *t1_private;
    pdf_t1portion t1portion;

    t1_private = (t1_private_data *) src->private_data;

    if (src->buffer_start == NULL)
    {
        src->buffer_start = (pdc_byte *) pdc_malloc(p->pdc, LINEBUFLEN + 1, fn);
        src->buffer_length = LINEBUFLEN;
    }

    s = pdc_fgetline((char *) src->buffer_start, LINEBUFLEN,
                     t1_private->fontfile);
    if (s == NULL)
        return pdc_false;

    /* set unix line end */
    len = (int) strlen(s);
    s[len] = '\n';
    len++;
    s[len] = 0;

    /* check for line of zeros: set t1_zero flag if found */
    if (*s == '0')
    {
        for (i = 0; s[i] == '0'; i++)
        {
            /* */ ;
        }
        if (s[i] == '\n')
            t1_private->portion = t1_zeros;
    }

    /* check whether font data portion follows: set t1_encrypted flag later */
    t1portion = t1_private->portion;
    if (t1_private->portion != t1_encrypted &&
        !strncmp((const char *)s, PDF_CURRENTFILE, strlen(PDF_CURRENTFILE)))
        t1portion = t1_encrypted;

    src->next_byte = src->buffer_start;

    switch (t1_private->portion)
    {
        case t1_ascii:
        {
            t1_private->length[1] += (size_t) len;
            src->bytes_available = (size_t) len;
        }
        break;

        case t1_encrypted:
        {
            src->bytes_available = 0;

            /* Convert to upper case for safe binary conversion */
            for (c = s; *c != '\n'; c++)
            {
                *c = (char) pdc_toupper(*c);
            }

            /* convert ASCII to binary in-place */
            for (i = 0; s[i] != '\n'; i += 2)
            {
                if ((!pdc_isxdigit(s[i]) && !pdc_isspace(s[i])) ||
                    (!pdc_isxdigit(s[i+1]) && !pdc_isspace(s[i+1])))
                {
                    pdc_fclose(t1_private->fontfile);
                    pdc_error(p->pdc, PDF_E_FONT_CORRUPT, "PFA", "?", 0, 0);
                }
#ifndef PDFLIB_EBCDIC
                s[i/2] = (char) (16*HexToBin[s[i]-'0'] + HexToBin[s[i+1]-'0']);
#else
#endif
                src->bytes_available++;
            }
            t1_private->length[2] += src->bytes_available;
        }
        break;

        case t1_zeros:
        {
            t1_private->length[3] += (size_t) len;
            src->bytes_available = (size_t) len;
        }
        break;
    }

    t1_private->portion = t1portion;

    return pdc_true;
}

#define PFB_MARKER      0x80
#define PFB_ASCII       1
#define PFB_BINARY      2
#define PFB_EOF         3

static int
pdf_t1getc(t1_private_data *t1)
{
    int val;

    if (t1->fontfile)
    {
        return pdc_fgetc(t1->fontfile);
    }
    val = (int) *t1->pos;
    t1->pos++;

    return val;
}

static pdc_bool
pdf_read_pfb_segment(PDF *p, PDF_data_source *src, t1_private_data *t1, int i)
{
    static const char *fn = "pdf_read_pfb_segment";
    size_t length, len;

    length  = (size_t) (pdf_t1getc(t1) & 0xff);
    length |= (size_t) (pdf_t1getc(t1) & 0xff) << 8;
    length |= (size_t) (pdf_t1getc(t1) & 0xff) << 16;
    length |= (size_t) (pdf_t1getc(t1) & 0xff) << 24;

    if (src->buffer_start)
        pdc_free(p->pdc, (void *) src->buffer_start);
    src->buffer_start = (pdc_byte *) pdc_malloc(p->pdc, length, fn);

    if (t1->fontfile) {
        len = pdc_fread(src->buffer_start, 1, length, t1->fontfile);
    } else {
        len = length;
        if (t1->pos + len > t1->end)
            len = (unsigned int)(t1->end - t1->pos);
        memcpy(src->buffer_start, t1->pos, len);
        t1->pos += len;
    }

    t1->length[i] = len;
    src->next_byte = src->buffer_start;
    src->bytes_available = len;

    return (len != length) ? pdc_true : pdc_false;;
}

static int
PFB_data_fill(PDF *p, PDF_data_source *src)
{
    t1_private_data *t1;
    unsigned char c, type;
    pdc_bool err = pdc_false;

    t1 = (t1_private_data *) src->private_data;

    c    = (unsigned char) pdf_t1getc(t1);
    type = (unsigned char) pdf_t1getc(t1);

    if (t1->length[1] == (size_t) 0) {
        if (c != PFB_MARKER || type != PFB_ASCII) {
            err = pdc_true;
        } else {
            err = pdf_read_pfb_segment(p, src, t1, 1);
        }

    } else if (t1->length[2] == (size_t) 0) {
        if (c != PFB_MARKER || type != PFB_BINARY) {
            err = pdc_true;
        } else {
            err = pdf_read_pfb_segment(p, src, t1, 2);
        }

    } else if (t1->length[3] == 0) {
        if (c != PFB_MARKER || type != PFB_ASCII) {
            err = pdc_true;
        } else {
            err = pdf_read_pfb_segment(p, src, t1, 3);
        }
    } else if (c != PFB_MARKER || type != PFB_EOF) {
        err = pdc_true;
    } else {
        return pdc_false;
    }

    if (err) {
        if (t1->fontfile)
            pdc_fclose(t1->fontfile);
	pdc_error(p->pdc, PDF_E_FONT_CORRUPT, "PFB", "?", 0, 0);
    }

    return pdc_true;
}

static void
t1data_terminate(PDF *p, PDF_data_source *src)
{
    pdc_free(p->pdc, (void *) src->buffer_start);
}

static void
t1data_init(PDF *p, PDF_data_source *src)
{
    t1_private_data *t1_private;

    (void) p;

    t1_private = (t1_private_data *) src->private_data;

    t1_private->portion = t1_ascii;
    t1_private->length[1] = (size_t) 0;
    t1_private->length[2] = (size_t) 0;
    t1_private->length[3] = (size_t) 0;

    src->buffer_start = NULL;
}


pdc_bool
pdf_t1open_fontfile(PDF *p, pdf_font *font, const char *filename,
                    PDF_data_source *t1src, pdc_bool requested)
{
    static const char *fn = "pdf_t1open_fontfile";
    t1_private_data  *t1_private = NULL;
    pdc_file *fp = NULL;
    const char *stemp = NULL;
    unsigned char magic[PFA_TESTBYTE];
    char fullname[PDC_FILENAMELEN];
    int fflags = PDC_FILE_BINARY;
    int ispfb = pdc_true;

    if (filename)
    {

        fp = pdc_fsearch_fopen(p->pdc, filename, fullname, "PostScript Type1 ",
                               fflags);
        if (fp == NULL)
        {
            if (t1src)
                pdc_error(p->pdc, -1, 0, 0, 0, 0);
            return pdc_check_fopen_errmsg(p->pdc, requested);
        }

        pdc_logg_cond(p->pdc, 1, trc_font,
            "\tLoading PostScript Type1 fontfile \"%s\":\n", fullname);

    }

    if (fp)
    {
        pdc_fread(magic, 1, PFA_TESTBYTE, fp);
        stemp = filename;
    }
    else if (font->ft.img)
    {
        strncpy((char *) magic, (const char *)font->ft.img, PFA_TESTBYTE);
        stemp = font->ft.name;
    }

    /* try to identify PFA files */
    if (magic[0] != PFB_MARKER)
    {
        char startsequ[PFA_TESTBYTE + 1];

        strcpy(startsequ, FNT_PFA_STARTSEQU);

        if (strncmp((const char *) magic, startsequ, PFA_TESTBYTE))
        {
            if (fp)
                pdc_fclose(fp);
            pdc_set_errmsg(p->pdc, PDF_E_T1_NOFONT, stemp, 0, 0, 0);
            if (t1src)
                pdc_error(p->pdc, -1, 0, 0, 0, 0);
            return pdc_false;
        }
        ispfb = pdc_false;
    }

    pdc_logg_cond(p->pdc, 1, trc_font,
        "\tPostScript Type1 font of format \"%s\" detected\n",
        ispfb ? "PFB" : "PFA");

    if (t1src)
    {
        t1src->private_data = (unsigned char *)
                pdc_malloc(p->pdc, sizeof(t1_private_data), fn);
        t1_private = (t1_private_data *) t1src->private_data;

        if (filename)
        {
            pdc_fclose(fp);
            if (ispfb)
            {
                t1_private->fontfile =
                    pdc_fsearch_fopen(p->pdc, fullname, NULL, "PFB ", fflags);
            }
            else
            {
                t1_private->fontfile =
                    pdc_fsearch_fopen(p->pdc, fullname, NULL, "PFA ",
                                      PDC_FILE_TEXT);
            }

            if (t1_private->fontfile == NULL)
                pdc_error(p->pdc, -1, 0, 0, 0, 0);
        }
        else if (font->ft.img)
        {
            t1_private->fontfile = NULL;
            t1_private->img = font->ft.img;
            t1_private->pos = font->ft.img;
            t1_private->end = font->ft.img + font->ft.filelen;
        }

        t1src->init = t1data_init;
        t1src->fill = ispfb ? PFB_data_fill : PFA_data_fill;
        t1src->terminate = t1data_terminate;
    }
    else if (fp != NULL)
    {
        if (pdc_file_isvirtual(fp) == pdc_true)
        {
            if (ispfb)
                font->ft.img =
                    (pdc_byte *) pdc_freadall(fp, &font->ft.filelen, NULL);
            font->ft.imgname = pdc_strdup(p->pdc, fullname);
            pdc_lock_pvf(p->pdc, font->ft.imgname);
        }
        font->ft.filename = pdc_strdup(p->pdc, fullname);
        pdc_fclose(fp);
    }

    return pdc_true;
}

pdc_bool
pdf_make_t1src (PDF *p, pdf_font *font, PDF_data_source *t1src)
{
    return pdf_t1open_fontfile(p, font, font->filename, t1src, pdc_true);
}

void
pdf_put_length_objs(PDF *p, PDF_data_source *t1src,
                    pdc_id length1_id, pdc_id length2_id, pdc_id length3_id)
{
    pdc_begin_obj(p->out, length1_id);              /* Length1 object */
    pdc_printf(p->out, "%ld\n",
            (long) ((t1_private_data *) t1src->private_data)->length[1]);
    pdc_end_obj(p->out);

    pdc_begin_obj(p->out, length2_id);              /* Length2 object */
    pdc_printf(p->out, "%ld\n",
            (long) ((t1_private_data *) t1src->private_data)->length[2]);
    pdc_end_obj(p->out);

    pdc_begin_obj(p->out, length3_id);              /* Length3 object */
    pdc_printf(p->out, "%ld\n",
            (long) ((t1_private_data *) t1src->private_data)->length[3]);
    pdc_end_obj(p->out);

    if (((t1_private_data *) t1src->private_data)->fontfile)
        pdc_fclose(((t1_private_data *) t1src->private_data)->fontfile);

    pdc_free(p->pdc, (void *) t1src->private_data);
}
