/** \file
 * \brief Gdk Cairo Base Driver
 *
 * See Copyright Notice in cd.h
 */

#ifndef __CDGDK_H
#define __CDGDK_H

#include <gdk/gdk.h>

#include "cd.h"
#include "cd_private.h"


struct _cdCtxImage {
  unsigned int w, h, depth;
  GdkPixmap* img;
  GdkScreen* scr;
  GdkVisual* vis;
};

struct _cdCtxCanvas {
  cdCanvas* canvas;
  GdkVisual* vis;           /* visual usado pela aplicacao */
  GdkScreen *scr;
  GdkGC* gc;                 /* graphic context */
  GdkDrawable* wnd;          /* drawable */
  GdkColor fg;

  GdkPixmap* last_hatch;     /* ultimo hatch setado pelo usuario */
  GdkPixmap* last_stipple;   /* ultimo stipple setado pelo usuario */
  GdkPixmap* last_pattern;   /* ultimo pattern setado pelo usuario */
  GdkGC* last_stipple_gc;
  int last_stipple_w;
  int last_stipple_h;

  GdkGC* last_pattern_gc;
  int last_pattern_w;
  int last_pattern_h;

  unsigned int depth;    /* depth do canvas */
  GdkPixmap* clip_polygon;   /* poligono de clipping */
  GdkPixmap* new_region;
  GdkPixmap* region_aux;
  GdkGC* region_aux_gc;
  void *data;            /* informacoes especificas do driver */
  long int *xidata;      /* ximage cache */
  int xisize;
  GdkColormap* colormap;      /* colormap para todos os canvas */
  GdkColor* color_table;      /* tabela de cores do colormap */
  int num_colors;             /* tamanho maximo da tabela de cores  */
  int rshift;                 /* constante red para calculo truecolor */
  int gshift;                 /* constante green para calculo truecolor */
  int bshift;                 /* constante blue para calculo truecolor */
  double xmatrix[6];          /* transformation matrix that includes axis inversion */
  float  rotate_angle;
  int    rotate_center_x,
         rotate_center_y;

  cairo_font_face_t* font;     /* fonte de caracteres no X */

  cdImage* image_dbuffer; /* utilizado pelo driver de Double buffer */
  cdCanvas* canvas_dbuffer;
};

#define cdCOLOR8TO16(_x) (_x*257)  /* 65535/255 = 257 */
#define cdCOLOR16TO8(_x) ((unsigned char)(_x/257))

extern GdkColor (*cdgdkGetPixel)(cdCtxCanvas *ctxcanvas, unsigned long rgb);
extern void (*cdxGetRGB)(cdCtxCanvas *ctxcanvas, unsigned long pixel, 
                                                 unsigned char* red, 
                                                 unsigned char* green, 
                                                 unsigned char* blue);

cdCtxCanvas *cdgdkCreateCanvas(cdCanvas* canvas, GdkDrawable* wnd, GdkScreen* scr, GdkVisual* vis);
void cdxInitTable(cdCanvas* canvas);
void cdxKillCanvas(cdCtxCanvas *ctxcanvas);
int cdgdkClip(cdCtxCanvas *ctxcanvas, int clip_mode);
void cdgdkPoly(cdCtxCanvas *ctxcanvas, int mode, cdPoint* poly, int n);

#endif
