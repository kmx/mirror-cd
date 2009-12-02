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
  GdkVisual* vis;            /* visual of the application */
  GdkScreen *scr;
  GdkGC* gc;                 /* graphic context */
  GdkDrawable* wnd;          /* drawable */
  GdkColor fg, bg;

  GdkGCValues gcval;

  GdkPixmap* last_hatch;     /* last hatch   set by user */
  GdkPixmap* last_stipple;   /* last stipple set by user */
  GdkPixmap* last_pattern;   /* last pattern set by user */
  GdkGC* last_stipple_gc;
  int last_stipple_w;
  int last_stipple_h;

  GdkGC* last_pattern_gc;
  int last_pattern_w;
  int last_pattern_h;

  unsigned int depth;        /* canvas depth */
  GdkPixmap* clip_polygon;   /* clipping polygon */
  GdkPixmap* new_region;
  GdkPixmap* region_aux;
  GdkGC* region_aux_gc;
  void *data;                /* specific information about the driver */
  long int *xidata;          /* Image cache */
  int xisize;
  GdkColormap* colormap;     /* Color map default */
  GdkColor* color_table;     /* Color table of the color map */
  int num_colors;            /* Size of the color table  */
  int rshift;                /* Red constant to calculate the true color */
  int gshift;                /* Green constant to calculate the true color */
  int bshift;                /* Blue constant to calculate the true color */
  double xmatrix[6];         /* Transformation matrix that includes axis inversion */
  float  rotate_angle;
  int    rotate_center_x,
         rotate_center_y;

  cairo_scaled_font_t* font;    /* Scaled font (character font + size font */

  cdImage* image_dbuffer;       /* Used by double buffer driver */
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
void cdgdkInitTable(cdCanvas* canvas);
void cdgdkKillCanvas(cdCtxCanvas *ctxcanvas);
int cdgdkClip(cdCtxCanvas *ctxcanvas, int clip_mode);
void cdgdkPoly(cdCtxCanvas *ctxcanvas, int mode, cdPoint* poly, int n);

#endif
