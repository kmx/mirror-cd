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
 * EBCDIC conversion routines
 *
 */

#ifndef PC_EBCDIC_H
#define PC_EBCDIC_H


void    pdc_ebcdic2ascii(char *s);
void    pdc_ebcdic2ascii_len(char *s, size_t len);
void    pdc_ascii2ebcdic_len(char *s, size_t len);
void    pdc_ascii2ebcdic(char *s);
void    pdc_ascii2ebcdic_char(char *c);
void    pdc_ascii2ebcdic_int(int *i);
void    pdc_ebcdic2ascii_int(int *i);
void	pdc_ebcdic2ascii_byte(pdc_byte *c);
void    pdc_ebcdic2pdfascii(char *s);
void    pdc_ebcdic2pdfascii_len(char *s, size_t len);

#endif	/* PC_EBCDIC_H */

