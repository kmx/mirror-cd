/** \file
 * \brief OpenGL Base Driver
 *
 * See Copyright Notice in cd.h
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifdef WIN32
#include <windows.h>
#else
#include <iconv.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

#include <FTGL/ftgl.h>

#include "cd.h"
#include "cd_private.h"
#include "cdgl.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define NUM_HATCHES  6
#define HATCH_WIDTH  8
#define HATCH_HEIGHT 8

/* 
** 6 predefined patterns to be accessed through cdHatch(
CD_HORIZONTAL | CD_VERTICAL | CD_FDIAGONAL | CD_BDIAGONAL |
CD_CROSS      | CD_DIAGCROSS)

*/
static char hatches[NUM_HATCHES][8] = {
  {0x00,0x00,0xFF,0x00,0x00,0x00,0xFF,0x00},  /* HORIZONTAL */
  {0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22},  /* VERTICAL */
  {0x08,0x10,0x20,0x40,0x80,0x01,0x02,0x04},  /* FDIAGONAL */
  {0x10,0x08,0x04,0x02,0x01,0x80,0x40,0x20},  /* BDIAGONAL */
  {0x22,0x22,0xFF,0x22,0x22,0x22,0xFF,0x22},  /* CROSS */
  {0x18,0x18,0x24,0x42,0x81,0x81,0x42,0x24}   /* DIAGCROSS */
};

struct _cdCtxImage
{
  unsigned int w, h, depth;
  GLubyte* img;
};

struct _cdCtxCanvas
{
  cdCanvas* canvas;

  FTGLfont *font;
  char fontfilename[10240];

  char* glLastConvertUTF8;

  float rotate_angle;
  int rotate_center_x;
  int rotate_center_y;

  int poly_holes[500];
  int holes;
};

/******************************************************/

static char* cdglStrConvertToUTF8(cdCtxCanvas *ctxcanvas, const char* str, unsigned int len)
{
  if (ctxcanvas->glLastConvertUTF8)
    free(ctxcanvas->glLastConvertUTF8);

#ifdef WIN32
  {
    wchar_t* toUnicode;

    toUnicode = (wchar_t*)malloc((len+1) * sizeof(wchar_t));
    MultiByteToWideChar(CP_ACP, 0, str, len+1, toUnicode, (len+1));

    len = WideCharToMultiByte(CP_UTF8, 0, toUnicode, -1, NULL, 0, NULL, NULL);
    if(!len)
      return (char*)str;

    ctxcanvas->glLastConvertUTF8 = (char*)malloc(len * sizeof(char));
    WideCharToMultiByte(CP_UTF8, 0, toUnicode, -1, ctxcanvas->glLastConvertUTF8, len, NULL, NULL);

    free(toUnicode);
  }
#else
  {
	  /* Based on http://www.lemoda.net/c/iconv-example/iconv-example.html
		   Last access: June 15th, 2010. */
    iconv_t cd;
    unsigned int utf8len = len*2;
    char* utf8 = calloc(utf8len, 1);

    cd = iconv_open("UTF-8", "ISO-8859-1");
    if(cd == (iconv_t)-1)
      return (char*)str;

    ctxcanvas->glLastConvertUTF8 = utf8;
		iconv(cd, (char**)&str, &len, &utf8, &utf8len);

		iconv_close(cd);
  }
#endif

  return ctxcanvas->glLastConvertUTF8;
}

/******************************************************/

static void cdkillcanvas(cdCtxCanvas *ctxcanvas)
{
  if(ctxcanvas->font)
    ftglDestroyFont(ctxcanvas->font);

  if (ctxcanvas->glLastConvertUTF8)
    free(ctxcanvas->glLastConvertUTF8);

  free(ctxcanvas);
}

/******************************************************/

static void cdflush(cdCtxCanvas *ctxcanvas)
{
  glFlush();
  (void)ctxcanvas;
}

/******************************************************/

static int cdclip(cdCtxCanvas *ctxcanvas, int clip_mode)
{
  switch (clip_mode)
  {
  case CD_CLIPOFF:
    if(glIsEnabled(GL_SCISSOR_TEST))
      glDisable(GL_SCISSOR_TEST);
    break;
  case CD_CLIPAREA:
    {
      glEnable(GL_SCISSOR_TEST);
      glScissor(ctxcanvas->canvas->clip_rect.xmin, ctxcanvas->canvas->clip_rect.ymin,
                (ctxcanvas->canvas->clip_rect.xmax - ctxcanvas->canvas->clip_rect.xmin),
                (ctxcanvas->canvas->clip_rect.ymax - ctxcanvas->canvas->clip_rect.ymin));
      break;
    }
  case CD_CLIPPOLYGON:
    break;
  case CD_CLIPREGION:
    break;
  }

  return clip_mode;
}

static void cdfcliparea(cdCtxCanvas *ctxcanvas, double xmin, double xmax, double ymin, double ymax)
{
  if (ctxcanvas->canvas->clip_mode == CD_CLIPAREA) 
  {
    ctxcanvas->canvas->clip_rect.xmin = (int)xmin;
    ctxcanvas->canvas->clip_rect.ymin = (int)ymin;
    ctxcanvas->canvas->clip_rect.xmax = (int)xmax;
    ctxcanvas->canvas->clip_rect.ymax = (int)ymax;
    cdclip(ctxcanvas, CD_CLIPAREA);
  }
}

static void cdcliparea(cdCtxCanvas *ctxcanvas, int xmin, int xmax, int ymin, int ymax)
{
  cdfcliparea(ctxcanvas, (double)xmin, (double)xmax, (double)ymin, (double)ymax);
}

/******************************************************/

static int cdwritemode(cdCtxCanvas *ctxcanvas, int write_mode)
{
  if(glIsEnabled(GL_COLOR_LOGIC_OP))
    glDisable(GL_COLOR_LOGIC_OP);

  switch (write_mode)
  {
  case CD_REPLACE:
    glLogicOp(GL_COPY);
    break;
  case CD_XOR:
    glLogicOp(GL_XOR);
    break;
  case CD_NOT_XOR:
    glLogicOp(GL_EQUIV);
    break;
  }

  glEnable(GL_COLOR_LOGIC_OP);

  (void)ctxcanvas;
  return write_mode;
}

static int cdhatch(cdCtxCanvas *ctxcanvas, int hatch_style)
{
  GLubyte pattern[128];
  int x, y, pos = 0;
 
  glEnable(GL_POLYGON_STIPPLE);
 
  for (y = 0; y < 128; y+=8)
  {
    for (x = 0; x < 8; x++)
      pattern[x+y] = hatches[hatch_style][pos];
    pos++;

    if(pos > 7) /* repeat the pattern */
      pos = 0;
  }
  glPolygonStipple(pattern);

  (void)ctxcanvas;
  return hatch_style;
}

static void cdglEnableBackOpacity(cdCtxCanvas* ctxcanvas)
{
  if (ctxcanvas->canvas->back_opacity == CD_OPAQUE)
  {
    glColor4ub(cdRed(ctxcanvas->canvas->foreground), cdGreen(ctxcanvas->canvas->foreground), cdBlue(ctxcanvas->canvas->foreground), 255);

    glDepthMask(GL_TRUE);

    if(glIsEnabled(GL_DEPTH_TEST))
      glDisable(GL_DEPTH_TEST);

    if(glIsEnabled(GL_BLEND))
      glDisable(GL_BLEND);
  }
  else
  {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    glColor4ub(cdRed(ctxcanvas->canvas->foreground), cdGreen(ctxcanvas->canvas->foreground), cdBlue(ctxcanvas->canvas->foreground), 0);
  }
}

static int cdinteriorstyle(cdCtxCanvas *ctxcanvas, int style)
{
  switch (style)
  {
  case CD_HOLLOW:
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    break;
  case CD_SOLID:
  case CD_HATCH :
  case CD_STIPPLE:
  case CD_PATTERN:
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    break;
  }

  switch (style)
  {
  case CD_HOLLOW:
  case CD_SOLID:
    if(glIsEnabled(GL_POLYGON_STIPPLE))
      glDisable(GL_POLYGON_STIPPLE);
    break;
  case CD_HATCH:
    cdglEnableBackOpacity(ctxcanvas);
    cdhatch(ctxcanvas, ctxcanvas->canvas->hatch_style);
    break;
  case CD_STIPPLE:
  case CD_PATTERN:
    break;
  }

  return style;
}

static int cdlinestyle(cdCtxCanvas *ctxcanvas, int style)
{
  switch (style)
  {
  case CD_CONTINUOUS:
    if(glIsEnabled(GL_LINE_STIPPLE))
      glDisable(GL_LINE_STIPPLE);
    return style;
    break;
  case CD_DASHED:
  case CD_DOTTED:
  case CD_DASH_DOT:
  case CD_DASH_DOT_DOT:
  case CD_CUSTOM:
    glEnable(GL_LINE_STIPPLE);
    cdglEnableBackOpacity(ctxcanvas);
    break;
  }

  switch (style)
  {
  case CD_DASHED:
    glLineStipple(1, 0x3F);
    break;
  case CD_DOTTED:
    glLineStipple(1, 0x33);
    break;
  case CD_DASH_DOT:
    glLineStipple(1, 0x33F);
    break;
  case CD_DASH_DOT_DOT:
    glLineStipple(1, 0x333F);
    break;
  case CD_CUSTOM:
    /* style patterns more than 16 bits are not completely drawed */
    glLineStipple(1, (GLushort)*ctxcanvas->canvas->line_dashes);
    break;
  }

  return style;
}

static int cdlinewidth(cdCtxCanvas *ctxcanvas, int width)
{
  if (width == 0) 
    width = 1;

  glLineWidth((GLfloat)width);

  (void)ctxcanvas;
  return width;
}

static int cdbackopacity(cdCtxCanvas *ctxcanvas, int opaque)
{
  ctxcanvas->canvas->back_opacity = opaque;
  cdinteriorstyle(ctxcanvas, ctxcanvas->canvas->interior_style);
  cdlinestyle(ctxcanvas, ctxcanvas->canvas->line_style);
  return opaque;
}

/***********************************************************************************/
/* Functions to get the font name path                                             */
/* Base source = https://www.h3dapi.org:8090/H3DAPI/trunk/H3DAPI/src/FontStyle.cpp */
/***********************************************************************************/
#ifdef WIN32
static LONG cdglWGetNextNameValue(HKEY key, LPCTSTR subkey, LPTSTR szName, LPTSTR szData)
{
  static HKEY hkey = NULL;
  static DWORD dwIndex = 0;
  LONG retval;

  if (subkey == NULL && szName == NULL && szData == NULL)
  {
    if (hkey)
      RegCloseKey(hkey);
  
    hkey = NULL;
    return ERROR_SUCCESS;
  }

  if (subkey && subkey[0] != 0)
  {
    retval = RegOpenKeyEx(key, subkey, 0, KEY_READ, &hkey);
    if (retval != ERROR_SUCCESS)
      return retval;

    dwIndex = 0;
  }
  else
    dwIndex++;

  *szName = 0;
  *szData = 0;

  {
    char szValueName[MAX_PATH];
    DWORD dwValueNameSize = sizeof(szValueName)-1;
    BYTE szValueData[MAX_PATH];
    DWORD dwValueDataSize = sizeof(szValueData)-1;
    DWORD dwType = 0;

    retval = RegEnumValue(hkey, dwIndex, szValueName, &dwValueNameSize, NULL, &dwType, szValueData, &dwValueDataSize);
    if (retval == ERROR_SUCCESS)
    {
      lstrcpy(szName, (char *)szValueName);
      lstrcpy(szData, (char *)szValueData);
    }
  }

  return retval;
}

static int cdglWGetFontFileName(LPCTSTR lpszFontName, int bold, int italic, char* fileName)
{
  TCHAR szName[2 * MAX_PATH];
  TCHAR szData[2 * MAX_PATH];
  TCHAR displayName[2 * MAX_PATH];
  LPCTSTR strFont = "Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts";
  char localFontName[256];
  int bResult = 0;

  sprintf(localFontName, "%s", lpszFontName);

  if( bold )
    strcat(localFontName, " Bold");

  if( italic )
    strcat(localFontName, " Italic");

  while (cdglWGetNextNameValue(HKEY_LOCAL_MACHINE, strFont, szName, szData) == ERROR_SUCCESS)
  {
    if (_strnicmp(localFontName, szName, strlen(localFontName)) == 0)
    {
      sprintf(displayName, "%s", szName);
      sprintf(fileName, "%s", szData);
      bResult = 1;
      break;
    }
    strFont = "";
  }
  /* close the registry key */
  cdglWGetNextNameValue(HKEY_LOCAL_MACHINE, NULL, NULL, NULL);

  return bResult;
}
#else
#include <fontconfig/fontconfig.h>

static int cdglXGetFontFileName(const char *font_name, int bold, int italic, char* fileName)
{
  char styles[4][20];
  int style_size;
  FcObjectSet *os = 0;
  FcFontSet *fs;
  FcPattern *pat;
  int bResult = 0;

  if( bold && italic )
  {
    sprintf(styles[0], "%s", "BoldItalic");
    sprintf(styles[1], "%s", "Bold Italic");
    sprintf(styles[2], "%s", "Bold Oblique");
    sprintf(styles[3], "%s", "BoldOblique");
    style_size = 4;
  }
  else if( bold )
  {
    sprintf(styles[0], "%s", "Bold");
    style_size = 1;
  }
  else if( italic )
  {
    sprintf(styles[0], "%s", "Italic");
    sprintf(styles[1], "%s", "Oblique");
    style_size = 2;
  }
  else
  {
    sprintf(styles[0], "%s", "Regular");
    sprintf(styles[1], "%s", "Normal");
    sprintf(styles[2], "%s", "Medium");
    style_size = 3;
  }

  pat = FcPatternCreate();
  os = FcObjectSetBuild(FC_FAMILY, FC_FILE, FC_STYLE, NULL);
  fs = FcFontList(NULL, pat, os);
  if (pat)
    FcPatternDestroy(pat);

  if(fs)
  {
    int j, s;

    for (j = 0; j < fs->nfont; j++)
    {
      FcChar8 *file;
      FcChar8 *style;
      FcChar8 *family;

      FcPatternGetString(fs->fonts[j], FC_FILE, 0, &file); 
      FcPatternGetString(fs->fonts[j], FC_STYLE, 0, &style );
      FcPatternGetString(fs->fonts[j], FC_FAMILY, 0, &family );

      if (strncasecmp(font_name, (char*)family, strlen(font_name)) == 0)
      {
        /* check if the font is of the correct type. */
        for(s = 0; s < style_size; s++ )
        {
          if (strcasecmp(styles[s], (char*)style ) == 0)
          {
            sprintf(fileName, "%s", (char*)file);
            bResult = 1;
            FcFontSetDestroy (fs);

            return bResult;
          }

          /* set value to use if no more correct font of same family is found. */
          sprintf(fileName, "%s", (char*)file);
          bResult = 1;
        }
      }
    }
    FcFontSetDestroy (fs);
  }

  return bResult;
}
#endif

static int cdfont(cdCtxCanvas *ctxcanvas, const char *typeface, int style, int size)
{
  int is_italic = 0, is_bold = 0;   /* default is CD_PLAIN */
  char strFontFileName[256];

  if (style & CD_BOLD)
    is_bold = 1;

  if (style & CD_ITALIC)
    is_italic = 1;

#ifdef WIN32
  if (cdStrEqualNoCase(typeface, "Courier") || cdStrEqualNoCase(typeface, "Monospace"))
    typeface = "Courier New";
  else if (cdStrEqualNoCase(typeface, "Times") || cdStrEqualNoCase(typeface, "Serif"))
    typeface = "Times New Roman";
  else if (cdStrEqualNoCase(typeface, "Helvetica") || cdStrEqualNoCase(typeface, "Sans"))
    typeface = "Arial";

  if(!cdglWGetFontFileName(typeface, is_bold, is_italic, strFontFileName))
    return 0;

  sprintf(ctxcanvas->fontfilename, "%s\\fonts\\%s", getenv("windir"), strFontFileName);
#else
  if (cdStrEqualNoCase(typeface, "Courier") || cdStrEqualNoCase(typeface, "Courier New") || cdStrEqualNoCase(typeface, "Monospace"))
    typeface = "freemono";
  else if (cdStrEqualNoCase(typeface, "Times") || cdStrEqualNoCase(typeface, "Times New Roman")|| cdStrEqualNoCase(typeface, "Serif"))
    typeface = "freeserif";
  else if (cdStrEqualNoCase(typeface, "Helvetica") || cdStrEqualNoCase(typeface, "Arial") || cdStrEqualNoCase(typeface, "Sans"))
    typeface = "freesans";

  if(!cdglXGetFontFileName(typeface, is_bold, is_italic, strFontFileName))
    return 0;

  sprintf(ctxcanvas->fontfilename, "%s", strFontFileName);
#endif

  ctxcanvas->font = ftglCreateBufferFont(ctxcanvas->fontfilename);
  
  if (!ctxcanvas->font)
    return 0;

  if (size < 0)
    size = cdGetFontSizePoints(ctxcanvas->canvas, size);

  ftglSetFontFaceSize(ctxcanvas->font, size, 72);
  ftglSetFontCharMap(ctxcanvas->font, ft_encoding_unicode);

  return 1;
}

static void cdgetfontdim(cdCtxCanvas *ctxcanvas, int *max_width, int *height, int *ascent, int *descent)
{
  if(!ctxcanvas->font)
    return;

  if (max_width) *max_width = (int)ftglGetFontAdvance(ctxcanvas->font, "W");
  if (height)    *height    = (int)ftglGetFontLineHeight(ctxcanvas->font);
  if (ascent)    *ascent    = (int)ftglGetFontAscender(ctxcanvas->font);
  if (descent)   *descent   = (int)ftglGetFontDescender(ctxcanvas->font);
}

static long int cdforeground(cdCtxCanvas *ctxcanvas, long int color)
{
  unsigned char r, g, b;
  (void)ctxcanvas;

  //TODO
  if(glIsEnabled(GL_POLYGON_STIPPLE))
    glDisable(GL_POLYGON_STIPPLE);

  cdDecodeColor(color, &r, &g, &b);
  glColor3ub(r, g, b);

  return color;
}

static void cdclear(cdCtxCanvas* ctxcanvas)
{
  unsigned char r, g, b;
  cdDecodeColor(ctxcanvas->canvas->background, &r, &g, &b);
  glClearColor((GLclampf)((double)r/255.0), (GLclampf)((double)g/255.0), (GLclampf)((double)b/255.0), 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

static void cdfline(cdCtxCanvas *ctxcanvas, double x1, double y1, double x2, double y2)
{
  glBegin(GL_LINES);
    glVertex2d(x1, y1);
    glVertex2d(x2, y2);
  glEnd();

  (void)ctxcanvas;
}

static void cdline(cdCtxCanvas *ctxcanvas, int x1, int y1, int x2, int y2)
{
  cdfline(ctxcanvas, (double)x1, (double)y1, (double)x2, (double)y2);
}

static void cdfrect(cdCtxCanvas *ctxcanvas, double xmin, double xmax, double ymin, double ymax)
{
  glBegin(GL_LINE_LOOP);
    glVertex2d(xmin, ymin);
    glVertex2d(xmax, ymin);
    glVertex2d(xmax, ymax);
    glVertex2d(xmin, ymax);
  glEnd();

  (void)ctxcanvas;
}

static void cdrect(cdCtxCanvas *ctxcanvas, int xmin, int xmax, int ymin, int ymax)
{
  cdfrect(ctxcanvas, (double)xmin, (double)xmax, (double)ymin, (double)ymax);
}

static void cdfbox(cdCtxCanvas *ctxcanvas, double xmin, double xmax, double ymin, double ymax)
{
  glBegin(GL_QUADS);
    glVertex2d(xmin, ymin);
    glVertex2d(xmax, ymin);
    glVertex2d(xmax, ymax);
    glVertex2d(xmin, ymax);
  glEnd();

  (void)ctxcanvas;
}

static void cdbox(cdCtxCanvas *ctxcanvas, int xmin, int xmax, int ymin, int ymax)
{
  cdfbox(ctxcanvas, (double)xmin, (double)xmax, (double)ymin, (double)ymax);
}

static void cdftext(cdCtxCanvas *ctxcanvas, double x, double y, const char *s, int len)
{
  int dir = -1;
  float bounds[6];
  int w, h, desc;
  double x_origin = x;
  double y_origin = y;

  if (!ctxcanvas->font)
    return;

  s = cdglStrConvertToUTF8(ctxcanvas, s, len);
  ftglGetFontBBox(ctxcanvas->font, s, len, bounds);

  desc = (int)ftglGetFontDescender(ctxcanvas->font);
  w = (int)ceil(bounds[3] - bounds[0]);
  h = (int)ceil(bounds[4] - bounds[1]);

  switch (ctxcanvas->canvas->text_alignment)
  {
    case CD_BASE_RIGHT:
    case CD_NORTH_EAST:
    case CD_EAST:
    case CD_SOUTH_EAST:
      x = x - w;
      break;
    case CD_BASE_CENTER:
    case CD_CENTER:
    case CD_NORTH:
    case CD_SOUTH:
      x = x - w/2;
      break;
    case CD_BASE_LEFT:
    case CD_NORTH_WEST:
    case CD_WEST:
    case CD_SOUTH_WEST:
      x = x;
      break;
  }

  if (ctxcanvas->canvas->invert_yaxis)
    dir = 1;

  switch (ctxcanvas->canvas->text_alignment)
  {
    case CD_BASE_LEFT:
    case CD_BASE_CENTER:
    case CD_BASE_RIGHT:
      y = y;
      break;
    case CD_SOUTH_EAST:
    case CD_SOUTH_WEST:
    case CD_SOUTH:
      y = y + dir * desc;
      break;
    case CD_NORTH_EAST:
    case CD_NORTH:
    case CD_NORTH_WEST:
      y = y + dir * (h + desc);
      break;
    case CD_CENTER:
    case CD_EAST:
    case CD_WEST:
      y = y + dir * (h / 2);
      break;
  }

  if (ctxcanvas->canvas->text_orientation != 0)
  {
    double angle = CD_DEG2RAD * ctxcanvas->canvas->text_orientation;
    double cos_angle = cos(angle);
    double sin_angle = sin(angle);
    cdfRotatePoint(ctxcanvas->canvas, x, y, x_origin, y_origin, &x, &y, sin_angle, cos_angle);
  }

  glPushMatrix();
    glTranslated(x, y, 0.0);
    glRotated(ctxcanvas->canvas->text_orientation, 0, 0, 1);

    /* FTGL not operate as expected when a logical pixel operation (write mode) is enabled. */
    if(glIsEnabled(GL_COLOR_LOGIC_OP))
    {
      glDisable(GL_COLOR_LOGIC_OP);
      ftglRenderFont(ctxcanvas->font, s, FTGL_RENDER_ALL);
      glEnable(GL_COLOR_LOGIC_OP);
    }
    else
      ftglRenderFont(ctxcanvas->font, s, FTGL_RENDER_ALL);
  glPopMatrix();
}

static void cdtext(cdCtxCanvas *ctxcanvas, int x, int y, const char *s, int len)
{
  cdftext(ctxcanvas, (double)x, (double)y, s, len);
}

static void cdgettextsize(cdCtxCanvas *ctxcanvas, const char *s, int len, int *width, int *height)
{
  float bounds[6];

  if (!ctxcanvas->font)
    return;

  s = cdglStrConvertToUTF8(ctxcanvas, s, len);
  ftglGetFontBBox(ctxcanvas->font, s, len, bounds);

  if (width)  *width  = (int)ceil(bounds[3] - bounds[0]);
  if (height) *height = (int)ceil(bounds[4] - bounds[1]);
}

static void cdpoly(cdCtxCanvas *ctxcanvas, int mode, cdPoint* poly, int n)
{
  int i;
  
  if (mode == CD_CLIP)
    return;

  if (mode == CD_BEZIER)
  {
    int i, prec = 100;
    float (*points)[3] = malloc(n * sizeof(*points));

    for(i = 0; i < n; i++)
    {
      points[i][0] = (float)poly[i].x;
      points[i][1] = (float)poly[i].y;
      points[i][2] = 0;
    }

    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, n, &points[0][0]);
    glEnable(GL_MAP1_VERTEX_3);
    glMapGrid1f(prec, 0.0, 1.0);
    glEvalMesh1(GL_LINE, 0, prec);
    glDisable(GL_MAP1_VERTEX_3);

    return;
  }

  if (mode == CD_PATH)
  {
    cdSimPolyPath(ctxcanvas->canvas, poly, n);
    return;
  }

  switch (mode)
  {
  case CD_CLOSED_LINES :
    glBegin(GL_LINE_LOOP);
    break;
  case CD_OPEN_LINES :
    glBegin(GL_LINE_STRIP);
    break;
  case CD_FILL :
    glBegin(GL_POLYGON);
    break;
  }

  for(i = 0; i < n; i++)
    glVertex2i(poly[i].x, poly[i].y);
  glEnd();

  (void)ctxcanvas;
}

static void cdfpoly(cdCtxCanvas *ctxcanvas, int mode, cdfPoint* poly, int n)
{
  int i;

  if (mode == CD_CLIP)
    return;

  if (mode == CD_BEZIER)
  {
    int i, prec = 100;
    double (*points)[3] = malloc(n * sizeof(*points));

    for(i = 0; i < n; i++)
    {
      points[i][0] = poly[i].x;
      points[i][1] = poly[i].y;
      points[i][2] = 0;
    }

    glMap1d(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, n, &points[0][0]);
    glEnable(GL_MAP1_VERTEX_3);
    glMapGrid1d(prec, 0.0, 1.0);
    glEvalMesh1(GL_LINE, 0, prec);
    glDisable(GL_MAP1_VERTEX_3);

    return;
  }

  if (mode == CD_PATH)
  {
    cdfSimPolyPath(ctxcanvas->canvas, poly, n);
    return;
  }

  switch (mode)
  {
  case CD_CLOSED_LINES :
    glBegin(GL_LINE_LOOP);
    break;
  case CD_OPEN_LINES :
    glBegin(GL_LINE_STRIP);
    break;
  case CD_FILL :
    glBegin(GL_POLYGON);
    break;
  }

  for(i = 0; i < n; i++)
    glVertex2d(poly[i].x, poly[i].y);
  glEnd();

  (void)ctxcanvas;
}

/******************************************************/

static void cdglGetImageData(GLubyte* glImage, unsigned char *r, unsigned char *g, unsigned char *b, int w, int h)
{
  int y, x;
  unsigned char *pixline_data;
  int rowstride, channels = 3;

  rowstride = w * channels;

  /* planes are separated in image data */
  for (y = 0; y < h; y++)
  {
    int lineoffset = y * w;
    pixline_data = (unsigned char*)glImage + y * rowstride;
    for(x = 0; x < w; x++)
    {
      int pos = x*channels;
      r[lineoffset+x] = pixline_data[pos];
      g[lineoffset+x] = pixline_data[pos+1];
      b[lineoffset+x] = pixline_data[pos+2];
    }
  }
}

static GLubyte* cdglCreateImageRGBA(int width, int height, const unsigned char *r, const unsigned char *g, const unsigned char *b, const unsigned char *a, int image_width)
{
  GLubyte* pixline_data;
  GLubyte* glImage;
  int x, y;
  int channels = a ? 4 : 3;
  int rowstride = width * channels;
  int lineoffset;

  glImage = (GLubyte*)malloc(rowstride * height);

  /* planes are separated in image data */
  for (y = 0; y < height; y++)
  {
    lineoffset = y * image_width;
    pixline_data = glImage + y * rowstride;

    for(x=0;x<width;x++)
    {
      int pos = x*channels;
      pixline_data[pos]   = r[lineoffset+x];
      pixline_data[pos+1] = g[lineoffset+x];
      pixline_data[pos+2] = b[lineoffset+x];

      if (a)
        pixline_data[pos+3] = a[lineoffset+x];
    }
  }

  return glImage;
}

static GLubyte* cdglCreateImageMap(int width, int height, const long* colors, const unsigned char *map, int image_width)
{
  const GLubyte *line_data;
  GLubyte *pixline_data;
  GLubyte *glImage;
  int x, y, channels = 3;
  int rowstride = width * channels;

  glImage = (GLubyte*)malloc(rowstride * height);

  for (y = 0; y < height; y++)
  {
    pixline_data = glImage + y * rowstride;
    line_data = map + y * image_width;

    for (x=0; x<width; x++)
    {
      GLubyte index = line_data[x];
      long c = colors[index];
      GLubyte *r = &pixline_data[channels*x],
              *g = r+1,
              *b = g+1;

      *r = cdRed(c);
      *g = cdGreen(c);
      *b = cdBlue(c);
    }
  }

  return glImage;
}

static void cdgetimagergb(cdCtxCanvas *ctxcanvas, unsigned char *r, unsigned char *g, unsigned char *b, int x, int y, int w, int h)
{
  GLubyte* glImage = (GLubyte*)malloc((w*3)*h);  /* each pixel uses 3 bytes (RGB) */

  glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
  glReadPixels(x, y, w, h, GL_RGB, GL_UNSIGNED_BYTE, glImage);
  if (!glImage)
    return;

  cdglGetImageData(glImage, r, g, b, w, h);

  (void)ctxcanvas;

  free(glImage);
}

static void cdputimagerectrgb(cdCtxCanvas *ctxcanvas, int iw, int ih, const unsigned char *r, const unsigned char *g, const unsigned char *b, int x, int y, int w, int h, int xmin, int xmax, int ymin, int ymax)
{
  /* Images are bitmaps, and cannot be directly rotated or scaled */
  GLubyte* glImage;
  int rw = xmax-xmin+1;
  int rh = ymax-ymin+1;

  glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

  glImage = cdglCreateImageRGBA(rw, rh, r, g, b, NULL, iw);
  if (!glImage)
    return;

  /* adjusts when the initial position (x,y) are less than 0 */
  if(x < 0)
  {
    w -= x;
    x = 0;
  }

  if(y < 0)
  {
    h -= y;
    y = 0;
  }

  if (w != rw || w != rh)
    glPixelZoom((GLfloat)w/rw, (GLfloat)h/rh);

  glRasterPos2i(x, y);
  glDrawPixels(rw, rh, GL_RGB, GL_UNSIGNED_BYTE, glImage);

  (void)ih;
  (void)ctxcanvas;

  free(glImage);
}

static void cdputimagerectrgba(cdCtxCanvas *ctxcanvas, int iw, int ih, const unsigned char *r, const unsigned char *g, const unsigned char *b, const unsigned char *a, int x, int y, int w, int h, int xmin, int xmax, int ymin, int ymax)
{
  /* Images are bitmaps, and cannot be directly rotated or scaled */
  GLubyte* glImage;
  int rw = xmax-xmin+1;
  int rh = ymax-ymin+1;

  glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

  glImage = cdglCreateImageRGBA(rw, rh, r, g, b, a, iw);
  if (!glImage)
    return;

  /* adjusts when the initial position (x,y) are less than 0 */
  if(x < 0)
  {
    w -= x;
    x = 0;
  }

  if(y < 0)
  {
    h -= y;
    y = 0;
  }

  if (w != rw || h != rh)
    glPixelZoom((GLfloat)w/rw, (GLfloat)h/rh);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glRasterPos2i(x, y);

  if(glIsEnabled(GL_COLOR_LOGIC_OP))
  {
    glDisable(GL_COLOR_LOGIC_OP);
    glDrawPixels(rw, rh, GL_RGBA, GL_UNSIGNED_BYTE, glImage);
    glEnable(GL_COLOR_LOGIC_OP);
  }
  else
    glDrawPixels(rw, rh, GL_RGBA, GL_UNSIGNED_BYTE, glImage);

  glDisable(GL_BLEND);

  (void)ih;
  (void)ctxcanvas;

  free(glImage);
}

static void cdputimagerectmap(cdCtxCanvas *ctxcanvas, int iw, int ih, const unsigned char *index, const long int *colors, int x, int y, int w, int h, int xmin, int xmax, int ymin, int ymax)
{
  /* Images are bitmaps, and cannot be directly rotated or scaled */
  GLubyte* glImage;
  int rw = xmax-xmin+1;
  int rh = ymax-ymin+1;

  glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

  glImage = cdglCreateImageMap(rw, rh, colors, index, iw);
  if (!glImage)
    return;

  /* adjusts when the initial position (x,y) are less than 0 */
  if(x < 0)
  {
    w -= x;
    x = 0;
  }

  if(y < 0)
  {
    h -= y;
    y = 0;
  }

  if (w != rw || h != rh)
    glPixelZoom((GLfloat)w/rw, (GLfloat)h/rh);

  glRasterPos2i(x, y);
  glDrawPixels(rw, rh, GL_RGB, GL_UNSIGNED_BYTE, glImage);

  (void)ih;
  (void)ctxcanvas;

  free(glImage);
}

static void cdpixel(cdCtxCanvas *ctxcanvas, int x, int y, long int color)
{
  glColor3ub(cdRed(color), cdGreen(color), cdBlue(color));

  /* Draw pixel */
  glPointSize(1);
  glBegin(GL_POINTS);
    glVertex2i(x, y);
  glEnd();

  /* restore the foreground color */
  glColor3ub(cdRed(ctxcanvas->canvas->foreground), cdGreen(ctxcanvas->canvas->foreground), cdBlue(ctxcanvas->canvas->foreground));

  (void)ctxcanvas;
}

static cdCtxImage *cdcreateimage (cdCtxCanvas *ctxcanvas, int w, int h)
{
  cdCtxImage *ctximage = (cdCtxImage *)malloc(sizeof(cdCtxImage));

  ctximage->w = w;
  ctximage->h = h;
  ctximage->depth = ctxcanvas->canvas->bpp;

  ctximage->img = (GLubyte*)malloc(w*h*4);  /* each pixel uses 4 bytes (RGBA) */

  if (!ctximage->img)
  {
    free(ctximage);
    return (void*)0;
  }

  return (void*)ctximage;
}

static void cdgetimage (cdCtxCanvas *ctxcanvas, cdCtxImage *ctximage, int x, int y)
{
  glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
  glReadPixels(x, y - ctximage->h+1, ctximage->w, ctximage->h, GL_RGBA, GL_UNSIGNED_BYTE, ctximage->img);

  (void)ctxcanvas;
}

static void cdputimagerect (cdCtxCanvas *ctxcanvas, cdCtxImage *ctximage, int x, int y, int xmin, int xmax, int ymin, int ymax)
{
  glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
  glRasterPos2i(x, y);
  glDrawPixels(xmax-xmin+1, ymax-ymin+1, GL_RGBA, GL_UNSIGNED_BYTE, ctximage->img);

  (void)ctxcanvas;
}

static void cdkillimage (cdCtxImage *ctximage)
{
  free(ctximage->img);
  free(ctximage);
}

static void cdscrollarea (cdCtxCanvas *ctxcanvas, int xmin, int xmax, int ymin, int ymax, int dx, int dy)
{
  glRasterPos2i(xmin+dx, ymin+dy);
  glCopyPixels(xmin, ymin, xmax-xmin+1, ymax-ymin+1, GL_RGBA);

  (void)ctxcanvas;
}

static void cdtransform(cdCtxCanvas *ctxcanvas, const double* matrix)
{
  if (matrix)
  {
    GLdouble transformMTX[4][4];

    transformMTX[0][0] = matrix[0];   transformMTX[0][1] = matrix[1];   transformMTX[0][2] = 0.0;         transformMTX[0][3] = 0.0;
    transformMTX[1][0] = matrix[2];   transformMTX[1][1] = matrix[3];   transformMTX[1][2] = 0.0;         transformMTX[1][3] = 0.0;
    transformMTX[2][0] = 0.0;         transformMTX[2][1] = 0.0;         transformMTX[2][2] = 1.0;         transformMTX[2][3] = 0.0;
    transformMTX[3][0] = matrix[4];   transformMTX[3][1] = matrix[5];   transformMTX[3][2] = 0.0;         transformMTX[3][3] = 1.0;

    glLoadIdentity();
    glMultMatrixd(&transformMTX[0][0]);
  }

  (void)ctxcanvas;
}

/******************************************************************/

static void set_poly_attrib(cdCtxCanvas *ctxcanvas, char* data)
{
  int hole;

  if (data == NULL)
  {
    ctxcanvas->holes = 0;
    return;
  }

  sscanf(data, "%d", &hole);
  ctxcanvas->poly_holes[ctxcanvas->holes] = hole;
  ctxcanvas->holes++;
}

static char* get_poly_attrib(cdCtxCanvas *ctxcanvas)
{
  static char holes[10];
  sprintf(holes, "%d", ctxcanvas->holes);
  return holes;
}

static cdAttribute poly_attrib =
{
  "POLYHOLE",
  set_poly_attrib,
  get_poly_attrib
}; 

static void set_rotate_attrib(cdCtxCanvas* ctxcanvas, char* data)
{
  if (data)
  {
    sscanf(data, "%g %d %d", &ctxcanvas->rotate_angle,
                             &ctxcanvas->rotate_center_x,
                             &ctxcanvas->rotate_center_y);

    cdCanvasTransformTranslate(ctxcanvas->canvas, ctxcanvas->rotate_center_x, ctxcanvas->rotate_center_y);
    cdCanvasTransformRotate(ctxcanvas->canvas, ctxcanvas->rotate_angle);
    cdCanvasTransformTranslate(ctxcanvas->canvas, -ctxcanvas->rotate_center_x, -ctxcanvas->rotate_center_y);
  }
  else
  {
    ctxcanvas->rotate_angle = 0;
    ctxcanvas->rotate_center_x = 0;
    ctxcanvas->rotate_center_y = 0;

    cdCanvasTransform(ctxcanvas->canvas, NULL);
  }
}

static char* get_rotate_attrib(cdCtxCanvas* ctxcanvas)
{
  static char data[100];

  if (!ctxcanvas->rotate_angle)
    return NULL;

  sprintf(data, "%g %d %d", (double)ctxcanvas->rotate_angle,
                            ctxcanvas->rotate_center_x,
                            ctxcanvas->rotate_center_y);

  return data;
}

static cdAttribute rotate_attrib =
{
  "ROTATE",
  set_rotate_attrib,
  get_rotate_attrib
};

static void set_size_attrib(cdCtxCanvas* ctxcanvas, char* data)
{
  if (data)
  {
    cdCanvas* canvas = ctxcanvas->canvas;
    float res = (float)canvas->xres;
    sscanf(data, "%dx%d %g", &canvas->w, &canvas->h, &res);
    canvas->yres = canvas->xres = res;
    canvas->w_mm = ((double)canvas->w) / canvas->xres;
    canvas->h_mm = ((double)canvas->h) / canvas->yres;
  }
}

static cdAttribute size_attrib =
{
  "SIZE",
  set_size_attrib,
  NULL
};

static char* get_version_attrib(cdCtxCanvas* ctxcanvas)
{
  (void)ctxcanvas;
  return (char*)glGetString(GL_VERSION);
}

static cdAttribute version_attrib =
{
  "GLVERSION",
  NULL,
  get_version_attrib
};

static void cdcreatecanvas(cdCanvas* canvas, void *data)
{
  cdCtxCanvas* ctxcanvas;
  int w = 0, h = 0;
  float res = (float)3.78;
  char* str_data = (char*)data;

  sscanf(str_data, "%dx%d %g", &w, &h, &res);

  if (w == 0 || h == 0)
    return;

  ctxcanvas = (cdCtxCanvas *)malloc(sizeof(cdCtxCanvas));
  memset(ctxcanvas, 0, sizeof(cdCtxCanvas));

  canvas->xres = res;
  canvas->yres = res;

  canvas->w_mm = ((double)canvas->w) / canvas->xres;
  canvas->h_mm = ((double)canvas->h) / canvas->yres;

  ctxcanvas->canvas = canvas;
  canvas->ctxcanvas = ctxcanvas;

  ctxcanvas->glLastConvertUTF8 = NULL;

  cdRegisterAttribute(canvas, &rotate_attrib);
  cdRegisterAttribute(canvas, &version_attrib);
  cdRegisterAttribute(canvas, &poly_attrib);
  cdRegisterAttribute(canvas, &size_attrib);
}

static void cdinittable(cdCanvas* canvas)
{
  canvas->cxFlush = cdflush;
  canvas->cxClear = cdclear;
  
  canvas->cxPixel  = cdpixel;
  canvas->cxLine   = cdline;
  canvas->cxPoly   = cdpoly;
  canvas->cxRect   = cdrect;
  canvas->cxBox    = cdbox;
  canvas->cxArc = cdSimArc;
  canvas->cxSector = cdSimSector;
  canvas->cxChord = cdSimChord;

  canvas->cxText = cdtext;
  canvas->cxFont = cdfont;
  canvas->cxGetFontDim  = cdgetfontdim;
  canvas->cxGetTextSize = cdgettextsize;

  canvas->cxClip = cdclip;
  canvas->cxClipArea = cdcliparea;
  canvas->cxWriteMode = cdwritemode;
  canvas->cxLineStyle = cdlinestyle;
  canvas->cxLineWidth = cdlinewidth;
  canvas->cxBackOpacity   = cdbackopacity;
  canvas->cxInteriorStyle = cdinteriorstyle;
  canvas->cxHatch = cdhatch;
  canvas->cxForeground = cdforeground;
  canvas->cxTransform  = cdtransform;

  canvas->cxFLine = cdfline;
  canvas->cxFPoly = cdfpoly;
  canvas->cxFRect = cdfrect;
  canvas->cxFBox = cdfbox;
  canvas->cxFArc = cdfSimArc;
  canvas->cxFSector = cdfSimSector;
  canvas->cxFChord = cdfSimChord;
  canvas->cxFText = cdftext;
  canvas->cxFClipArea = cdfcliparea;

  canvas->cxScrollArea = cdscrollarea;
  canvas->cxCreateImage = cdcreateimage;
  canvas->cxGetImage = cdgetimage;
  canvas->cxPutImageRect = cdputimagerect;
  canvas->cxKillImage = cdkillimage;

  canvas->cxGetImageRGB = cdgetimagergb;
  canvas->cxPutImageRectRGB = cdputimagerectrgb;
  canvas->cxPutImageRectMap = cdputimagerectmap;
  canvas->cxPutImageRectRGBA = cdputimagerectrgba;

  canvas->cxKillCanvas = cdkillcanvas;
}

static cdContext cdGLContext =
{
  CD_CAP_ALL & ~(CD_CAP_PLAY | CD_CAP_PALETTE | CD_CAP_LINEJOIN | CD_CAP_LINECAP |
                 CD_CAP_REGION | CD_CAP_STIPPLE | CD_CAP_PATTERN),
  0,
  cdcreatecanvas,
  cdinittable,
  NULL,              
  NULL,
};

cdContext* cdContextGL(void)
{
  return &cdGLContext;
}
