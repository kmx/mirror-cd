/** \file
 * \brief SVG driver
 *
 * See Copyright Notice in cd.h
 */

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <limits.h> 
#include <math.h>
#include <locale.h>

#include "cd.h"
#include "wd.h"
#include "cd_private.h"
#include "cdsvg.h"

#include "lodepng.h"
#include "base64.h"

struct _cdCtxCanvas 
{
  cdCanvas* canvas;
  char* filename;       

  char bgColor[20];
  char fgColor[20]; 
  char linecap[10];
  char linejoin[10];
  char linestyle[20];
  char poly[256];
  char pattern[30];

  char font_weight[15];
  char font_style[15];
  char font_decoration[15];
  char font_family[256];
  char font_size[10];

  int backopacity;
  int writemode;
  int linewidth;
  int hatchboxsize;

  double xmatrix[6];         /* Transformation matrix that includes axis inversion */

  /* private */
  int last_fill_mode;
  
  int last_clip_path;
  int clip_off_control;
  int clip_polygon;

  int transform_control;

  FILE* file;
};

static void cdkillcanvas(cdCtxCanvas* ctxcanvas)
{
  while(ctxcanvas->clip_off_control > 0)
  {
    fprintf(ctxcanvas->file, "</g>\n");
    --ctxcanvas->clip_off_control;
  }

  if(ctxcanvas->transform_control)
    fprintf(ctxcanvas->file, "</g>\n");

  fprintf(ctxcanvas->file, "</g>\n");
  fprintf(ctxcanvas->file, "</svg>\n");

  free(ctxcanvas->filename);
  fclose(ctxcanvas->file);

  memset(ctxcanvas, 0, sizeof(cdCtxCanvas));
  free(ctxcanvas);
}

static void setclip_area(cdCtxCanvas *ctxcanvas)
{
  cdRect* clip_rect = &ctxcanvas->canvas->clip_rect;

  fprintf(ctxcanvas->file, "<clipPath id=\"clippath%d\">\n", ++ctxcanvas->last_clip_path);

  if (ctxcanvas->canvas->use_matrix)
  {
    cdPoint poly[4];
    poly[0].x = clip_rect->xmin; poly[0].y = clip_rect->ymin;
    poly[1].x = clip_rect->xmin; poly[1].y = clip_rect->ymax;
    poly[2].x = clip_rect->xmax; poly[2].y = clip_rect->ymax;
    poly[3].x = clip_rect->xmax; poly[3].y = clip_rect->ymin;

    sprintf(ctxcanvas->poly,    "%g,%g",                  (double)poly[0].x, (double)poly[0].y);
    sprintf(ctxcanvas->poly, "%s %g,%g", ctxcanvas->poly, (double)poly[1].x, (double)poly[1].y);
    sprintf(ctxcanvas->poly, "%s %g,%g", ctxcanvas->poly, (double)poly[2].x, (double)poly[2].y);
    sprintf(ctxcanvas->poly, "%s %g,%g", ctxcanvas->poly, (double)poly[3].x, (double)poly[3].y);

    fprintf(ctxcanvas->file, "<polygon points=\"%s\" />\n", ctxcanvas->poly);
  }
  else
  {
    double x, y, w, h;
    x = (double)clip_rect->xmin;
    y = (double)clip_rect->ymin;
    w = (double)(clip_rect->xmax - clip_rect->xmin + 1);
    h = (double)(clip_rect->ymax - clip_rect->ymin + 1);

    fprintf(ctxcanvas->file, "<rect x=\"%g\" y=\"%g\" width=\"%g\" height=\"%g\" />\n", x, y, w, h);
  }

  fprintf(ctxcanvas->file, "</clipPath>\n");

  fprintf(ctxcanvas->file, "<g clip-path=\"url(#clippath%d)\">\n", ctxcanvas->last_clip_path);
  ++ctxcanvas->clip_off_control;
}

int cdclip(cdCtxCanvas *ctxcanvas, int clip_mode)
{
  switch (clip_mode)
  {
    case CD_CLIPOFF:
      if(ctxcanvas->clip_off_control > 0)
      {
        fprintf(ctxcanvas->file, "</g>\n");
        --ctxcanvas->clip_off_control;
      }
      break;
    case CD_CLIPAREA:
      setclip_area(ctxcanvas);
      break;
    case CD_CLIPPOLYGON:
      if (ctxcanvas->clip_polygon)
      {
        fprintf(ctxcanvas->file, "<g clip-path=\"url(#clippath%d)\">\n", ctxcanvas->last_clip_path);
        ++ctxcanvas->clip_off_control;
      }
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

static void cdtransform(cdCtxCanvas *ctxcanvas, const double* matrix)
{
  if (matrix)
  {
    /* Matrix identity */
    ctxcanvas->xmatrix[0] = 1; 
    ctxcanvas->xmatrix[1] = 0;
    ctxcanvas->xmatrix[2] = 0; 
    ctxcanvas->xmatrix[3] = -1; 
    ctxcanvas->xmatrix[4] = 0; 
    ctxcanvas->xmatrix[5] = (ctxcanvas->canvas->h-1);

    if(ctxcanvas->transform_control)
    {
      fprintf(ctxcanvas->file, "</g>\n");
      --ctxcanvas->transform_control;
    }

    cdMatrixMultiply(matrix, ctxcanvas->xmatrix);

    ++ctxcanvas->transform_control;
    fprintf(ctxcanvas->file, "<g transform=\"matrix(%g %g %g %g %g %g)\">\n", ctxcanvas->xmatrix[0], ctxcanvas->xmatrix[1], ctxcanvas->xmatrix[2], ctxcanvas->xmatrix[3], ctxcanvas->xmatrix[4], ctxcanvas->xmatrix[5]);

    ctxcanvas->canvas->invert_yaxis = 0;
  }
  else
  {
    ctxcanvas->canvas->invert_yaxis = 1;
  }
}

static void cdfline(cdCtxCanvas *ctxcanvas, double x1, double y1, double x2, double y2)
{
  fprintf(ctxcanvas->file, "<line x1=\"%g\" y1=\"%g\" x2=\"%g\" y2=\"%g\" style=\"fill:none; stroke:%s; stroke-width:%d; stroke-linecap:%s; stroke-linejoin:%s; stroke-dasharray:%s; opacity:%d\" />\n",
                                     x1, y1, x2, y2, ctxcanvas->fgColor, ctxcanvas->linewidth, ctxcanvas->linecap, ctxcanvas->linejoin, ctxcanvas->linestyle, ctxcanvas->backopacity);
}

static void cdline(cdCtxCanvas *ctxcanvas, int x1, int y1, int x2, int y2)
{
  cdfline(ctxcanvas, (double)x1, (double)y1, (double)x2, (double)y2);
}

static void cdfrect(cdCtxCanvas *ctxcanvas, double xmin, double xmax, double ymin, double ymax)
{
  fprintf(ctxcanvas->file, "<rect x=\"%g\" y=\"%g\" width=\"%g\" height=\"%g\" style=\"fill:none; stroke:%s; stroke-width:%d; stroke-linecap:%s; stroke-linejoin:%s; stroke-dasharray:%s; opacity:%d\" />\n",
    xmin, ymin, xmax-xmin, ymax-ymin, ctxcanvas->fgColor, ctxcanvas->linewidth, ctxcanvas->linecap, ctxcanvas->linejoin, ctxcanvas->linestyle, ctxcanvas->backopacity);
}

static void cdrect(cdCtxCanvas *ctxcanvas, int xmin, int xmax, int ymin, int ymax)
{
  cdfrect(ctxcanvas, (double)xmin, (double)xmax, (double)ymin, (double)ymax);
}

static void cdfbox(cdCtxCanvas *ctxcanvas, double xmin, double xmax, double ymin, double ymax)
{
  fprintf(ctxcanvas->file, "<rect x=\"%g\" y=\"%g\" width=\"%g\" height=\"%g\" style=\"fill:%s; stroke:%s; stroke-width:%d; stroke-linecap:%s; stroke-linejoin:%s; stroke-dasharray:%s; opacity:%d\" />\n",
    xmin, ymin, xmax-xmin, ymax-ymin, (ctxcanvas->canvas->interior_style == CD_SOLID) ? ctxcanvas->fgColor : ctxcanvas->pattern,
    (ctxcanvas->canvas->interior_style == CD_SOLID) ? ctxcanvas->fgColor : "none", ctxcanvas->linewidth, ctxcanvas->linecap, ctxcanvas->linejoin, ctxcanvas->linestyle, ctxcanvas->backopacity);
}

static void cdbox(cdCtxCanvas *ctxcanvas, int xmin, int xmax, int ymin, int ymax)
{
  cdfbox(ctxcanvas, (double)xmin, (double)xmax, (double)ymin, (double)ymax);
}

static void cdfarc(cdCtxCanvas *ctxcanvas, double xc, double yc, double w, double h, double a1, double a2)
{
  double arcStartX, arcStartY, arcEndX, arcEndY;
  int largeArc = 0;

  if((a1 == 0.0) && (a2 == 360.0)) /* an ellipse/circle */
  {
    fprintf(ctxcanvas->file, "<ellipse cx=\"%g\" cy=\"%g\" rx=\"%g\" ry=\"%g\" style=\"fill:none; stroke:%s; stroke-width:%d; stroke-linecap:%s; stroke-linejoin:%s; stroke-dasharray:%s; opacity:%d\" />\n",
      xc, yc, w/2, h/2, ctxcanvas->fgColor, ctxcanvas->linewidth, ctxcanvas->linecap, ctxcanvas->linejoin, ctxcanvas->linestyle, ctxcanvas->backopacity);

    return;
  }

  if (ctxcanvas->canvas->use_matrix)  /* Transformation active */
  {
    double temp = 360 - a1;
    a1 = 360 - a2;
    a2 = temp;
  }

  arcStartX = (xc+(w/2)*cos(a1*CD_DEG2RAD));
  arcStartY = (yc-(h/2)*sin(a1*CD_DEG2RAD));
  arcEndX   = (xc+(w/2)*cos(a2*CD_DEG2RAD));
  arcEndY   = (yc-(h/2)*sin(a2*CD_DEG2RAD));

  if((a2-a1) > 180.0)
    largeArc = 1;

  fprintf(ctxcanvas->file, "<path d=\"M%g,%g A%g,%g 0 %d,0 %g,%g\" style=\"fill:none; stroke:%s; stroke-width:%d; stroke-linecap:%s; stroke-linejoin:%s; stroke-dasharray:%s; opacity:%d\" />\n",
    arcStartX, arcStartY, w/2, h/2, largeArc, arcEndX, arcEndY, ctxcanvas->fgColor, ctxcanvas->linewidth, ctxcanvas->linecap, ctxcanvas->linejoin, ctxcanvas->linestyle, ctxcanvas->backopacity);
}

static void cdarc(cdCtxCanvas *ctxcanvas, int xc, int yc, int w, int h, double a1, double a2)
{
  cdfarc(ctxcanvas, (double)xc, (double)yc, (double)w, (double)h, a1, a2);
}

static void cdfsector(cdCtxCanvas *ctxcanvas, double xc, double yc, double w, double h, double a1, double a2)
{
  double arcStartX, arcStartY, arcEndX, arcEndY;
  int largeArc = 0;

  if((a1 == 0.0) && (a2 == 360.0)) /* an ellipse/circle */
  {
    fprintf(ctxcanvas->file, "<ellipse cx=\"%g\" cy=\"%g\" rx=\"%g\" ry=\"%g\" style=\"fill:%s; stroke:%s; stroke-width:%d; stroke-linecap:%s; stroke-linejoin:%s; stroke-dasharray:%s; opacity:%d\" />\n",
      xc, yc, w/2, h/2, (ctxcanvas->canvas->interior_style == CD_SOLID) ? ctxcanvas->fgColor : ctxcanvas->pattern,
      (ctxcanvas->canvas->interior_style == CD_SOLID) ? ctxcanvas->fgColor : "none", ctxcanvas->linewidth, ctxcanvas->linecap, ctxcanvas->linejoin, ctxcanvas->linestyle, ctxcanvas->backopacity);

    return;
  }

  if (ctxcanvas->canvas->use_matrix)  /* Transformation active */
  {
    double temp = 360 - a1;
    a1 = 360 - a2;
    a2 = temp;
  }

  arcStartX = (xc+(w/2)*cos(a1*CD_DEG2RAD));
  arcStartY = (yc-(h/2)*sin(a1*CD_DEG2RAD));
  arcEndX   = (xc+(w/2)*cos(a2*CD_DEG2RAD));
  arcEndY   = (yc-(h/2)*sin(a2*CD_DEG2RAD));

  if((a2-a1) > 180.0)
    largeArc = 1;

  fprintf(ctxcanvas->file, "<path d=\"M%g,%g L%g,%g A%g,%g 0 %d,0 %g,%g Z\" style=\"fill:%s; stroke:%s; stroke-width:%d; stroke-linecap:%s; stroke-linejoin:%s; stroke-dasharray:%s; opacity:%d\" />\n",
    xc, yc, arcStartX, arcStartY, w/2, h/2, largeArc, arcEndX, arcEndY, (ctxcanvas->canvas->interior_style == CD_SOLID) ? ctxcanvas->fgColor : ctxcanvas->pattern, 
    (ctxcanvas->canvas->interior_style == CD_SOLID) ? ctxcanvas->fgColor : "none", ctxcanvas->linewidth, ctxcanvas->linecap, ctxcanvas->linejoin, ctxcanvas->linestyle, ctxcanvas->backopacity);
}

static void cdsector(cdCtxCanvas *ctxcanvas, int xc, int yc, int w, int h, double a1, double a2)
{
  cdfsector(ctxcanvas, (double)xc, (double)yc, (double)w, (double)h, a1, a2);
}

static void cdfchord(cdCtxCanvas *ctxcanvas, double xc, double yc, double w, double h, double a1, double a2)
{
  double arcStartX, arcStartY, arcEndX, arcEndY;
  int largeArc = 0;

  if (ctxcanvas->canvas->use_matrix)  /* Transformation active */
  {
    double temp = 360 - a1;
    a1 = 360 - a2;
    a2 = temp;
  }

  arcStartX = (xc+(w/2)*cos(a1*CD_DEG2RAD));
  arcStartY = (yc-(h/2)*sin(a1*CD_DEG2RAD));
  arcEndX   = (xc+(w/2)*cos(a2*CD_DEG2RAD));
  arcEndY   = (yc-(h/2)*sin(a2*CD_DEG2RAD));

  if((a2-a1) > 180.0)
    largeArc = 1;

  fprintf(ctxcanvas->file, "<path d=\"M%g,%g A%g,%g 0 %d,0 %g,%g Z\" style=\"fill:%s; stroke:%s; stroke-width:%d; stroke-linecap:%s; stroke-linejoin:%s; stroke-dasharray:%s; opacity:%d\" />\n",
    arcStartX, arcStartY, w/2, h/2, largeArc, arcEndX, arcEndY, (ctxcanvas->canvas->interior_style == CD_SOLID) ? ctxcanvas->fgColor : ctxcanvas->pattern, 
    (ctxcanvas->canvas->interior_style == CD_SOLID) ? ctxcanvas->fgColor : "none", ctxcanvas->linewidth, ctxcanvas->linecap, ctxcanvas->linejoin, ctxcanvas->linestyle, ctxcanvas->backopacity);
}

static void cdchord(cdCtxCanvas *ctxcanvas, int xc, int yc, int w, int h, double a1, double a2)
{
  cdfchord(ctxcanvas, (double)xc, (double)yc, (double)w, (double)h, a1, a2);
}

static void cdftext(cdCtxCanvas *ctxcanvas, double x, double y, const char *text, int len)
{
  char anchor[10];
  char alignment[20];
  int i;

  switch (ctxcanvas->canvas->text_alignment)
  {
    case CD_NORTH:
    case CD_NORTH_EAST:
    case CD_NORTH_WEST:
      sprintf(alignment, "%s", "text-before-edge");
      break;
    case CD_SOUTH:
    case CD_SOUTH_EAST:
    case CD_SOUTH_WEST:
      sprintf(alignment, "%s", "text-after-edge");
      break;
    case CD_CENTER:
    case CD_EAST:
    case CD_WEST:
      sprintf(alignment, "%s", "middle");
      break;
    case CD_BASE_CENTER:
    case CD_BASE_LEFT:
    case CD_BASE_RIGHT:
      sprintf(alignment, "%s", "baseline");
      break;
  }

  switch (ctxcanvas->canvas->text_alignment)
  {
    case CD_WEST:
    case CD_NORTH_WEST:
    case CD_SOUTH_WEST:
    case CD_BASE_LEFT:
      sprintf(anchor, "%s", "start");
      break;
    case CD_CENTER:
    case CD_NORTH:
    case CD_SOUTH:
    case CD_BASE_CENTER:
      sprintf(anchor, "%s", "middle");
      break;
    case CD_EAST:
    case CD_NORTH_EAST:
    case CD_SOUTH_EAST:
    case CD_BASE_RIGHT:
      sprintf(anchor, "%s", "end");
      break;
  }

  /* Characters are putting in file using hexadecimal representations */
  /* <text> string </text> no print special characters, like cedilla and acutes */
  /* Future solution: use glyphs in embedded fonts (TO DO)*/

  if (ctxcanvas->canvas->text_orientation != 0)
  {
    double text_cos = cos(ctxcanvas->canvas->text_orientation*CD_DEG2RAD);
    double text_sin = sin(ctxcanvas->canvas->text_orientation*CD_DEG2RAD);

    if (ctxcanvas->canvas->use_matrix)  /* Transformation active */
      fprintf(ctxcanvas->file, "<text transform=\"matrix(%g %g %g %g %g %g)\" font-family=\"%s\" font-size=\"%s\" font-style=\"%s\" font-weight=\"%s\" text-decoration=\"%s\" text-anchor=\"%s\" dominant-baseline=\"%s\" fill=\"%s\">\n", 
      text_cos, text_sin, text_sin, -text_cos, x, y, ctxcanvas->font_family, ctxcanvas->font_size, ctxcanvas->font_style, ctxcanvas->font_weight, ctxcanvas->font_decoration, anchor, alignment, ctxcanvas->fgColor);
    else
      fprintf(ctxcanvas->file, "<text transform=\"matrix(%g %g %g %g %g %g)\" font-family=\"%s\" font-size=\"%s\" font-style=\"%s\" font-weight=\"%s\" text-decoration=\"%s\" text-anchor=\"%s\" dominant-baseline=\"%s\" fill=\"%s\">\n", 
      text_cos, -text_sin, text_sin, text_cos, x, y, ctxcanvas->font_family, ctxcanvas->font_size, ctxcanvas->font_style, ctxcanvas->font_weight, ctxcanvas->font_decoration, anchor, alignment, ctxcanvas->fgColor);
    
    for(i = 0; i < len; i++)
      fprintf(ctxcanvas->file, "&#x%02X;", (unsigned char)text[i]);

    fprintf(ctxcanvas->file, "\n</text>\n");
  }
  else
  {
    fprintf(ctxcanvas->file, "<text x=\"%g\" y=\"%g\" font-family=\"%s\" font-size=\"%s\" font-style=\"%s\" font-weight=\"%s\" text-decoration=\"%s\" text-anchor=\"%s\" dominant-baseline=\"%s\" fill=\"%s\">\n", 
      x, y, ctxcanvas->font_family, ctxcanvas->font_size, ctxcanvas->font_style, ctxcanvas->font_weight, ctxcanvas->font_decoration, anchor, alignment, ctxcanvas->fgColor);

    for(i = 0; i < len; i++)
      fprintf(ctxcanvas->file, "&#x%02X;", (unsigned char)text[i]);

    fprintf(ctxcanvas->file, "\n</text>\n");
  }
}

static void cdtext(cdCtxCanvas *ctxcanvas, int x, int y, const char *text, int len)
{
  cdftext(ctxcanvas, (double)x, (double)y, text, len);
}

static void cdfpoly(cdCtxCanvas *ctxcanvas, int mode, cdfPoint* poly, int n)
{
  int i, m = 0;
  char rule[8];

  if(mode == CD_BEZIER)
    m = 1;

  sprintf(ctxcanvas->poly, "%g,%g", poly[m].x, poly[m].y);
  for(i = m+1; i<n; i++)
    sprintf(ctxcanvas->poly, "%s %g,%g", ctxcanvas->poly, poly[i].x, poly[i].y);

  switch (mode)
  {
  case CD_CLOSED_LINES :
    fprintf(ctxcanvas->file, "<polygon style=\"fill:none; stroke:%s; stroke-width:%d; stroke-linecap:%s; stroke-linejoin:%s; stroke-dasharray:%s; opacity:%d\" points=\"%s\" />\n",
      ctxcanvas->fgColor, ctxcanvas->linewidth, ctxcanvas->linecap, ctxcanvas->linejoin, ctxcanvas->linestyle, ctxcanvas->backopacity, ctxcanvas->poly);
    break;
  case CD_OPEN_LINES :
    fprintf(ctxcanvas->file, "<polyline style=\"fill:none; stroke:%s; stroke-width:%d; stroke-linecap:%s; stroke-linejoin:%s; stroke-dasharray:%s; opacity:%d\" points=\"%s\" />\n",
      ctxcanvas->fgColor, ctxcanvas->linewidth, ctxcanvas->linecap, ctxcanvas->linejoin, ctxcanvas->linestyle, ctxcanvas->backopacity, ctxcanvas->poly);
    break;
  case CD_BEZIER :
    fprintf(ctxcanvas->file, "<path d=\"M%g,%g C%s\" style=\"fill:none; stroke:%s; stroke-width:%d; stroke-linecap:%s; stroke-linejoin:%s; stroke-dasharray:%s; opacity:%d\" />\n",
      poly[0].x, poly[0].y, ctxcanvas->poly, ctxcanvas->fgColor, ctxcanvas->linewidth, ctxcanvas->linecap, ctxcanvas->linejoin, ctxcanvas->linestyle, ctxcanvas->backopacity);
    break;
  case CD_FILL :
    if(ctxcanvas->canvas->fill_mode==CD_EVENODD)
      sprintf(rule, "%s", "evenodd");
    else
      sprintf(rule, "%s", "nonzero");

    fprintf(ctxcanvas->file, "<polygon style=\"fill:%s; fill-rule:%s; stroke:%s; stroke-width:%d; stroke-linecap:%s; stroke-linejoin:%s; stroke-dasharray:%s; opacity:%d\" points=\"%s\" />\n",
      (ctxcanvas->canvas->interior_style == CD_SOLID) ? ctxcanvas->fgColor : ctxcanvas->pattern, rule, (ctxcanvas->canvas->interior_style == CD_SOLID) ? ctxcanvas->fgColor : "none",
      ctxcanvas->linewidth, ctxcanvas->linecap, ctxcanvas->linejoin, ctxcanvas->linestyle, ctxcanvas->backopacity, ctxcanvas->poly);
    break;
  case CD_CLIP:
    fprintf(ctxcanvas->file, "<clipPath id=\"clippath%d\">\n", ++ctxcanvas->last_clip_path);

    fprintf(ctxcanvas->file, "<polygon points=\"%s\" />\n", ctxcanvas->poly);

    fprintf(ctxcanvas->file, "</clipPath>\n");
    
    ctxcanvas->clip_polygon = 1;

    if (ctxcanvas->canvas->clip_mode == CD_CLIPPOLYGON)
      cdclip(ctxcanvas, CD_CLIPPOLYGON);

    break;

  }
}

static void cdpoly(cdCtxCanvas *ctxcanvas, int mode, cdPoint* poly, int n)
{
  int i;
  cdfPoint* newPoly = NULL;
  
  newPoly = (cdfPoint*)malloc(sizeof(cdfPoint)*(n+1));

  for(i = 0; i < n; i++)
  {
    newPoly[i].x = (double)poly[i].x;
    newPoly[i].y = (double)poly[i].y;
  }

  cdfpoly(ctxcanvas, mode, (cdfPoint*)newPoly, n);

  free(newPoly);
}

static int cdbackopacity(cdCtxCanvas *ctxcanvas, int opacity)
{
  ctxcanvas->backopacity = opacity;
  return opacity;
}

static int cdlinestyle(cdCtxCanvas *ctxcanvas, int style)
{
  switch (style)
  {
    case CD_CONTINUOUS : /* empty dash */
      sprintf(ctxcanvas->linestyle, "%s", "0");
      break;
    case CD_DASHED :
      sprintf(ctxcanvas->linestyle, "%s", "6,2");
      break;
    case CD_DOTTED :
      sprintf(ctxcanvas->linestyle, "%s", "2,2");
      break;
    case CD_DASH_DOT :
      sprintf(ctxcanvas->linestyle, "%s", "6,2,2,2");
      break;
    case CD_DASH_DOT_DOT :
      sprintf(ctxcanvas->linestyle, "%s", "6,2,2,2,2,2");
      break;
    case CD_CUSTOM :
      {
        int i;
        sprintf(ctxcanvas->linestyle, "%d", ctxcanvas->canvas->line_dashes[0]);

        for (i = 1; i < ctxcanvas->canvas->line_dashes_count; i++)
          sprintf(ctxcanvas->linestyle, "%s, %d", ctxcanvas->linestyle, ctxcanvas->canvas->line_dashes[i]);
      }
      break;
  }

  return style;
}

static int cdlinewidth(cdCtxCanvas *ctxcanvas, int width)
{
  ctxcanvas->linewidth = width;

  return width;
}

static int cdlinecap(cdCtxCanvas *ctxcanvas, int cap)
{
  if(cap == CD_CAPROUND)
    sprintf(ctxcanvas->linecap, "%s", "round");
  else if(cap == CD_CAPSQUARE)
    sprintf(ctxcanvas->linecap, "%s", "square");
  else  /* CD_CAPFLAT */
    sprintf(ctxcanvas->linecap, "%s", "butt");

  return cap;
}

static int cdlinejoin(cdCtxCanvas *ctxcanvas, int join)
{
  if(join == CD_ROUND)
    sprintf(ctxcanvas->linejoin, "%s", "round");
  else if(join == CD_BEVEL)
    sprintf(ctxcanvas->linejoin, "%s", "bevel");
  else  /* CD_MITER */
    sprintf(ctxcanvas->linejoin, "%s", "miter");

  return join;
}

static int cdhatch(cdCtxCanvas *ctxcanvas, int style)
{
  int hsize = ctxcanvas->hatchboxsize - 1;
  int hhalf = hsize / 2;

  ctxcanvas->canvas->interior_style = CD_HATCH;
  sprintf(ctxcanvas->pattern, "url(#pattern%d)", ++ctxcanvas->last_fill_mode);
  fprintf(ctxcanvas->file, "<pattern id=\"pattern%d\" patternUnits=\"userSpaceOnUse\" x=\"0\" y=\"0\" width=\"%d\" height=\"%d\">\n", ctxcanvas->last_fill_mode, hsize, hsize);

  switch(style)
  {
  case CD_HORIZONTAL:
    fprintf(ctxcanvas->file, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" style=\"fill:none; stroke:%s; stroke-width:%d; stroke-linecap:%s; stroke-linejoin:%s; opacity:%d\" />\n",
      0, hhalf, hsize, hhalf, (ctxcanvas->canvas->back_opacity != CD_OPAQUE) ? ctxcanvas->fgColor : ctxcanvas->bgColor, ctxcanvas->linewidth, ctxcanvas->linecap, ctxcanvas->linejoin, ctxcanvas->backopacity);
    break;
  case CD_VERTICAL:
    fprintf(ctxcanvas->file, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" style=\"fill:none; stroke:%s; stroke-width:%d; stroke-linecap:%s; stroke-linejoin:%s; opacity:%d\" />\n",
      hhalf, 0, hhalf, hsize, (ctxcanvas->canvas->back_opacity != CD_OPAQUE) ? ctxcanvas->fgColor : ctxcanvas->bgColor, ctxcanvas->linewidth, ctxcanvas->linecap, ctxcanvas->linejoin, ctxcanvas->backopacity);
    break;
  case CD_BDIAGONAL:
    fprintf(ctxcanvas->file, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" style=\"fill:none; stroke:%s; stroke-width:%d; stroke-linecap:%s; stroke-linejoin:%s; opacity:%d\" />\n",
      0, hsize, hsize, 0, (ctxcanvas->canvas->back_opacity != CD_OPAQUE) ? ctxcanvas->fgColor : ctxcanvas->bgColor, ctxcanvas->linewidth, ctxcanvas->linecap, ctxcanvas->linejoin, ctxcanvas->backopacity);
    break;
  case CD_FDIAGONAL:
    fprintf(ctxcanvas->file, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" style=\"fill:none; stroke:%s; stroke-width:%d; stroke-linecap:%s; stroke-linejoin:%s; opacity:%d\" />\n",
      0, 0, hsize, hsize, (ctxcanvas->canvas->back_opacity != CD_OPAQUE) ? ctxcanvas->fgColor : ctxcanvas->bgColor, ctxcanvas->linewidth, ctxcanvas->linecap, ctxcanvas->linejoin, ctxcanvas->backopacity);
    break;
  case CD_CROSS:
    fprintf(ctxcanvas->file, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" style=\"fill:none; stroke:%s; stroke-width:%d; stroke-linecap:%s; stroke-linejoin:%s; opacity:%d\" />\n",
      hsize, 0, hsize, hsize, (ctxcanvas->canvas->back_opacity != CD_OPAQUE) ? ctxcanvas->fgColor : ctxcanvas->bgColor, ctxcanvas->linewidth, ctxcanvas->linecap, ctxcanvas->linejoin, ctxcanvas->backopacity);
    fprintf(ctxcanvas->file, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" style=\"fill:none; stroke:%s; stroke-width:%d; stroke-linecap:%s; stroke-linejoin:%s; opacity:%d\" />\n",
      0, hhalf, hsize, hhalf, (ctxcanvas->canvas->back_opacity != CD_OPAQUE) ? ctxcanvas->fgColor : ctxcanvas->bgColor, ctxcanvas->linewidth, ctxcanvas->linecap, ctxcanvas->linejoin, ctxcanvas->backopacity);
    break;
  case CD_DIAGCROSS:
    fprintf(ctxcanvas->file, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" style=\"fill:none; stroke:%s; stroke-width:%d; stroke-linecap:%s; stroke-linejoin:%s; opacity:%d\" />\n",
      0, 0, hsize, hsize, (ctxcanvas->canvas->back_opacity != CD_OPAQUE) ? ctxcanvas->fgColor : ctxcanvas->bgColor, ctxcanvas->linewidth, ctxcanvas->linecap, ctxcanvas->linejoin, ctxcanvas->backopacity);
    fprintf(ctxcanvas->file, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" style=\"fill:none; stroke:%s; stroke-width:%d; stroke-linecap:%s; stroke-linejoin:%s; opacity:%d\" />\n",
      hsize, 0, 0, hsize, (ctxcanvas->canvas->back_opacity != CD_OPAQUE) ? ctxcanvas->fgColor : ctxcanvas->bgColor, ctxcanvas->linewidth, ctxcanvas->linecap, ctxcanvas->linejoin, ctxcanvas->backopacity);
    break;
  }

  fprintf(ctxcanvas->file, "</pattern>\n");

  return style;
}

static void make_pattern(cdCtxCanvas *ctxcanvas, int n, int m, void* data, int (*data2rgb)(cdCtxCanvas *ctxcanvas, int n, int i, int j, void* data, unsigned char*r, unsigned char*g, unsigned char*b))
{
  int i, j;
  unsigned char r, g, b;
  char color[20];

  sprintf(ctxcanvas->pattern, "url(#pattern%d)", ++ctxcanvas->last_fill_mode);
  fprintf(ctxcanvas->file, "<pattern id=\"pattern%d\" patternUnits=\"userSpaceOnUse\" x=\"0\" y=\"0\" width=\"%d\" height=\"%d\">\n", ctxcanvas->last_fill_mode, n, m);

  for (j = 0; j < m; j++)
  {
    for (i = 0; i < n; i++)
    {
      int ret = data2rgb(ctxcanvas, n, i, j, data, &r, &g, &b);
      if (ret == -1) continue;

      sprintf(color, "rgb(%d,%d,%d)", (int)r, (int)g, (int)b);

      fprintf(ctxcanvas->file, "<rect x=\"%g\" y=\"%g\" width=\"%g\" height=\"%g\" style=\"fill:%s; opacity:%d\" />\n",
        (double)i, (double)j, 1.0, 1.0, color, ctxcanvas->backopacity);
    }
  }

  fprintf(ctxcanvas->file, "</pattern>\n");
}

static int long2rgb(cdCtxCanvas *ctxcanvas, int n, int i, int j, void* data, unsigned char*r, unsigned char*g, unsigned char*b)
{
  long* long_data = (long*)data;
  (void)ctxcanvas;
  cdDecodeColor(long_data[j*n+i], r, g, b);
  return 1;
}

static void cdpattern(cdCtxCanvas *ctxcanvas, int n, int m, const long int *pattern)
{
  ctxcanvas->canvas->interior_style = CD_PATTERN;
  make_pattern(ctxcanvas, n, m, (void*)pattern, long2rgb);
}

static int uchar2rgb(cdCtxCanvas *ctxcanvas, int n, int i, int j, void* data, unsigned char*r, unsigned char*g, unsigned char*b)
{
  int ret = 1;
  unsigned char* uchar_data = (unsigned char*)data;
  if (uchar_data[j*n+i])
  {
    cdDecodeColor(ctxcanvas->canvas->foreground, r, g, b);
    ret = 1;
  }
  else
  {
    cdDecodeColor(ctxcanvas->canvas->background, r, g, b);
    if (ctxcanvas->canvas->back_opacity==CD_TRANSPARENT)
      ret = -1;
  }

  return ret;
}

static void cdstipple(cdCtxCanvas *ctxcanvas, int n, int m, const unsigned char *stipple)
{
  ctxcanvas->canvas->interior_style = CD_STIPPLE;
  make_pattern(ctxcanvas, n, m, (void*)stipple, uchar2rgb);
}

static int cdfont(cdCtxCanvas *ctxcanvas, const char* type_face, int style, int size)
{
  /* Define type_face and size */
  if(type_face != NULL)
    sprintf(ctxcanvas->font_family, "%s", type_face);
  
  if(size > 0)
    sprintf(ctxcanvas->font_size, "%dpt", size);
  else
    sprintf(ctxcanvas->font_size, "%dpx", (-1)*size);

  if(style != -1)
  {
    /* Default: CD_PLAIN */
    sprintf(ctxcanvas->font_weight,     "%s", "normal");
    sprintf(ctxcanvas->font_style,      "%s", "normal");
    sprintf(ctxcanvas->font_decoration, "%s", "none");

    /* Define styles and decorations */
    if (style & CD_BOLD)
      sprintf(ctxcanvas->font_weight, "%s", "bold");

    if (style & CD_ITALIC)
      sprintf(ctxcanvas->font_style, "%s", "italic");

    if (style & CD_STRIKEOUT)
      sprintf(ctxcanvas->font_decoration, "%s", "line-through");

    if (style & CD_UNDERLINE)
      sprintf(ctxcanvas->font_decoration, "%s", "underline");
  }

  return 1;
}

static long cdbackground(cdCtxCanvas *ctxcanvas, long int color)
{
  unsigned char r, g, b;
  cdDecodeColor(color, &r, &g, &b);
  sprintf(ctxcanvas->bgColor, "rgb(%d,%d,%d)", (int)r, (int)g, (int)b);

  return color;
}

static long cdforeground(cdCtxCanvas *ctxcanvas, long int color)
{
  unsigned char r, g, b;
  cdDecodeColor(color, &r, &g, &b);
  sprintf(ctxcanvas->fgColor, "rgb(%d,%d,%d)", (int)r, (int)g, (int)b);

  return color;
}

static void cdputimagerectrgb(cdCtxCanvas *ctxcanvas, int iw, int ih, const unsigned char *r, const unsigned char *g, const unsigned char *b, int x, int y, int w, int h, int xmin, int xmax, int ymin, int ymax)
{
  int i, j, d, rw, rh, rgb_size, target_size;
  unsigned char* rgb_data, *rgb_buffer;
  size_t buffer_size;
  LodePNG_Encoder encoder;
  char* rgb_target;

  if (xmin<0 || ymin<0 || xmax-xmin+1>iw || ymax-ymin+1>ih) return;

  rw = xmax-xmin+1;
  rh = ymax-ymin+1;

  rgb_size = 4*rw*rh;
  rgb_data = (unsigned char*)malloc(rgb_size);
  if (!rgb_data) return;

  d = 0;
  for (i=ymax; i>=ymin; i--)
  {
    for (j=xmin; j<=xmax; j++)
    {
      rgb_data[d] = r[i*iw+j]; d++;
      rgb_data[d] = g[i*iw+j]; d++;
      rgb_data[d] = b[i*iw+j]; d++;
      rgb_data[d] = (unsigned char)0xFF; d++;
    }
  }

  LodePNG_Encoder_init(&encoder);
  LodePNG_encode(&encoder, &rgb_buffer, &buffer_size, rgb_data, rw, rh);

  target_size = (buffer_size+2)/3*4+1;
  rgb_target = (char*)malloc(target_size);
  base64_encode(rgb_buffer, buffer_size, rgb_target, target_size);
 
  if (ctxcanvas->canvas->use_matrix)  /* Transformation active */
    fprintf(ctxcanvas->file, "<image transform=\"matrix(%d %d %d %d %d %d)\" width=\"%d\" height=\"%d\" xlink:href=\"data:image/png;base64,%s\"/>\n", 
    1, 0, 0, -1, x, y+h, w, h, rgb_target);
  else
    fprintf(ctxcanvas->file, "<image transform=\"matrix(%d %d %d %d %d %d)\" width=\"%d\" height=\"%d\" xlink:href=\"data:image/png;base64,%s\"/>\n", 
    1, 0, 0, 1, x, y-h, w, h, rgb_target);

  free(rgb_data);
  free(rgb_buffer);
  free(rgb_target);
  LodePNG_Encoder_cleanup(&encoder);
}

static void cdputimagerectrgba(cdCtxCanvas *ctxcanvas, int iw, int ih, const unsigned char *r, const unsigned char *g, const unsigned char *b, const unsigned char *a, int x, int y, int w, int h, int xmin, int xmax, int ymin, int ymax)
{
  int i, j, d, rw, rh, rgb_size, target_size;
  size_t buffer_size;
  unsigned char* rgb_data, *rgb_buffer;
  LodePNG_Encoder encoder;
  char* rgb_target;

  if (xmin<0 || ymin<0 || xmax-xmin+1>iw || ymax-ymin+1>ih) return;

  rw = xmax-xmin+1;
  rh = ymax-ymin+1;

  rgb_size = 4*rw*rh;
  rgb_data = (unsigned char*)malloc(rgb_size);
  if (!rgb_data) return;

  d = 0;
  for (i=ymax; i>=ymin; i--)
  {
    for (j=xmin; j<=xmax; j++)
    {
      rgb_data[d] = r[i*iw+j]; d++;
      rgb_data[d] = g[i*iw+j]; d++;
      rgb_data[d] = b[i*iw+j]; d++;
      rgb_data[d] = a[i*iw+j]; d++;
    }
  }

  LodePNG_Encoder_init(&encoder);
  LodePNG_encode(&encoder, &rgb_buffer, &buffer_size, rgb_data, rw, rh);

  target_size = (buffer_size+2)/3*4+1;
  rgb_target = (char*)malloc(target_size);
  base64_encode(rgb_buffer, buffer_size, rgb_target, target_size);

  if (ctxcanvas->canvas->use_matrix)  /* Transformation active */
    fprintf(ctxcanvas->file, "<image transform=\"matrix(%d %d %d %d %d %d)\" width=\"%d\" height=\"%d\" xlink:href=\"data:image/png;base64,%s\"/>\n", 
    1, 0, 0, -1, x, y+h, w, h, rgb_target);
  else
    fprintf(ctxcanvas->file, "<image transform=\"matrix(%d %d %d %d %d %d)\" width=\"%d\" height=\"%d\" xlink:href=\"data:image/png;base64,%s\"/>\n", 
    1, 0, 0, 1, x, y-h, w, h, rgb_target);

  free(rgb_data);
  free(rgb_buffer);
  free(rgb_target);
  LodePNG_Encoder_cleanup(&encoder);
}

static void cdputimagerectmap(cdCtxCanvas *ctxcanvas, int iw, int ih, const unsigned char *index, const long int *colors, int x, int y, int w, int h, int xmin, int xmax, int ymin, int ymax)
{
  int i, j, d, rw, rh, rgb_size, target_size;
  unsigned char* rgb_data, *rgb_buffer;
  size_t buffer_size;
  LodePNG_Encoder encoder;
  char* rgb_target;

  if (xmin<0 || ymin<0 || xmax-xmin+1>iw || ymax-ymin+1>ih) return;

  rw = xmax-xmin+1;
  rh = ymax-ymin+1;

  rgb_size = 4*rw*rh;
  rgb_data = (unsigned char*)malloc(rgb_size);
  if (!rgb_data) return;

  d = 0;
  for (i=ymax; i>=ymin; i--)
  {
    for (j=xmin; j<=xmax; j++)
    {
      unsigned char r, g, b;
      cdDecodeColor(colors[index[i*iw+j]], &r, &g, &b);
      rgb_data[d] = r; d++;
      rgb_data[d] = g; d++;
      rgb_data[d] = b; d++;
      rgb_data[d] = (unsigned char)0xFF; d++;
    }
  }

  LodePNG_Encoder_init(&encoder);
  LodePNG_encode(&encoder, &rgb_buffer, &buffer_size, rgb_data, rw, rh);

  target_size = (buffer_size+2)/3*4+1;
  rgb_target = (char*)malloc(target_size);
  base64_encode(rgb_buffer, buffer_size, rgb_target, target_size);

  if (ctxcanvas->canvas->use_matrix)  /* Transformation active */
    fprintf(ctxcanvas->file, "<image transform=\"matrix(%d %d %d %d %d %d)\" width=\"%d\" height=\"%d\" xlink:href=\"data:image/png;base64,%s\"/>\n", 
    1, 0, 0, -1, x, y+h, w, h, rgb_target);
  else
    fprintf(ctxcanvas->file, "<image transform=\"matrix(%d %d %d %d %d %d)\" width=\"%d\" height=\"%d\" xlink:href=\"data:image/png;base64,%s\"/>\n", 
    1, 0, 0, 1, x, y-h, w, h, rgb_target);

  free(rgb_data);
  free(rgb_buffer);
  free(rgb_target);
  LodePNG_Encoder_cleanup(&encoder);
}

static void cdpixel(cdCtxCanvas *ctxcanvas, int x, int y, long int color)
{
  unsigned char r, g, b;
  cdDecodeColor(color, &r, &g, &b);

  fprintf(ctxcanvas->file, "<circle cx=\"%d\" cy=\"%d\" r=\"0.1\" style=\"fill:rgb(%d,%d,%d); stroke:rgb(%d,%d,%d); stroke-width:%d; stroke-linecap:%s; stroke-linejoin:%s; stroke-dasharray:%s; opacity:%d\" />\n",
    x, y, r, g, b, r, g, b, ctxcanvas->linewidth, ctxcanvas->linecap, ctxcanvas->linejoin, ctxcanvas->linestyle, ctxcanvas->backopacity);
}

static void cddeactivate (cdCtxCanvas* ctxcanvas)
{
  fflush(ctxcanvas->file);
}

static void cdflush (cdCtxCanvas* ctxcanvas)
{
  fflush(ctxcanvas->file);
}

static void cdcreatecanvas(cdCanvas *canvas, void *data)
{
  char filename[10240] = "";
  char* strdata = (char*)data;
  double w_mm = INT_MAX*3.78, h_mm = INT_MAX*3.78, res = 3.78;
  cdCtxCanvas* ctxcanvas;
  int size;

  strdata += cdGetFileName(strdata, filename);
  if (filename[0] == 0)
    return;

  sscanf(strdata, "%lgx%lg %lg", &w_mm, &h_mm, &res);

  ctxcanvas = (cdCtxCanvas *)malloc(sizeof(cdCtxCanvas));
  memset(ctxcanvas, 0, sizeof(cdCtxCanvas));

  ctxcanvas->file = fopen(filename, "w");
  if (!ctxcanvas->file)
  {
    free(ctxcanvas);
    return;
  }

  size = strlen(filename);
  ctxcanvas->filename = malloc(size+1);
  memcpy(ctxcanvas->filename, filename, size+1);

  /* store the base canvas */
  ctxcanvas->canvas = canvas;

  /* update canvas context */
  canvas->w = (int)(w_mm * res);
  canvas->h = (int)(h_mm * res);
  canvas->w_mm = w_mm;
  canvas->h_mm = h_mm;
  canvas->bpp = 24;
  canvas->xres = res;
  canvas->yres = res;
  canvas->invert_yaxis = 1;

  /* update canvas context */
  canvas->ctxcanvas = ctxcanvas;

  ctxcanvas->last_fill_mode = -1;
  ctxcanvas->last_clip_path = -1;
  
  ctxcanvas->clip_off_control  = 0;
  ctxcanvas->transform_control = 0;

  ctxcanvas->clip_polygon = 0;
  ctxcanvas->hatchboxsize = 8;

  fprintf(ctxcanvas->file, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
  fprintf(ctxcanvas->file, "<svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" width=\"%dpt\" height=\"%dpt\" viewBox=\"0 0 %d %d\" version=\"1.1\">\n", canvas->w, canvas->h, canvas->w, canvas->h);
  fprintf(ctxcanvas->file, "<g>\n");
}

static void cdinittable(cdCanvas* canvas)
{
  canvas->cxPixel = cdpixel;
  canvas->cxLine = cdline;
  canvas->cxPoly = cdpoly;
  canvas->cxRect = cdrect;
  canvas->cxBox = cdbox;
  canvas->cxArc = cdarc;
  canvas->cxSector = cdsector;
  canvas->cxChord = cdchord;
  canvas->cxText = cdtext;
  canvas->cxClip = cdclip;
  canvas->cxClipArea = cdcliparea;
  canvas->cxFont = cdfont;
  
  canvas->cxPutImageRectRGB = cdputimagerectrgb;
  canvas->cxPutImageRectRGBA = cdputimagerectrgba;
  canvas->cxPutImageRectMap = cdputimagerectmap;
  
  canvas->cxFLine = cdfline;
  canvas->cxFPoly = cdfpoly;
  canvas->cxFRect = cdfrect;
  canvas->cxFBox = cdfbox;
  canvas->cxFArc = cdfarc;
  canvas->cxFSector = cdfsector;
  canvas->cxFChord = cdfchord;
  canvas->cxFText = cdftext;
  canvas->cxFClipArea = cdfcliparea;
  
  canvas->cxBackOpacity = cdbackopacity;
  canvas->cxLineStyle = cdlinestyle;
  canvas->cxLineWidth = cdlinewidth;
  canvas->cxLineCap = cdlinecap;
  canvas->cxLineJoin = cdlinejoin;

  canvas->cxHatch = cdhatch;
  canvas->cxStipple = cdstipple;
  canvas->cxPattern = cdpattern;

  canvas->cxBackground = cdbackground;
  canvas->cxForeground = cdforeground;

  canvas->cxTransform = cdtransform;

  canvas->cxFlush = cdflush;
  canvas->cxDeactivate = cddeactivate;
  canvas->cxKillCanvas = (void (*)(cdCtxCanvas*))cdkillcanvas;
}

static cdContext cdSVGContext =
{
  CD_CAP_ALL & ~(CD_CAP_CLEAR | CD_CAP_PLAY | CD_CAP_PALETTE | 
                 CD_CAP_REGION | CD_CAP_IMAGESRV | CD_CAP_WRITEMODE | 
                 CD_CAP_FONTDIM | CD_CAP_TEXTSIZE | 
                 CD_CAP_GETIMAGERGB),
  0,
  cdcreatecanvas,
  cdinittable,
  NULL,
  NULL,
};

cdContext* cdContextSVG(void)
{
  return &cdSVGContext;
}
