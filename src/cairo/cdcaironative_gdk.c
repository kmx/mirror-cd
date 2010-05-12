/** \file
* \brief Cairo Native Window Driver
*
* See Copyright Notice in cd.h
*/

#include <stdlib.h>
#include <stdio.h>

#include <gdk/gdk.h>

#include "cdcairoctx.h"
#include "cdnative.h"


static void cdkillcanvas(cdCtxCanvas *ctxcanvas)
{
  cdcairoKillCanvas(ctxcanvas);
}

int cdactivate(cdCtxCanvas *ctxcanvas)
{
  gdk_drawable_get_size(ctxcanvas->drawable, &ctxcanvas->canvas->w, &ctxcanvas->canvas->h);

  ctxcanvas->canvas->w_mm = ((double)ctxcanvas->canvas->w) / ctxcanvas->canvas->xres;
  ctxcanvas->canvas->h_mm = ((double)ctxcanvas->canvas->h) / ctxcanvas->canvas->yres;

  return CD_OK;
}

static void cdcreatecanvas(cdCanvas* canvas, void *data)
{
  cdCtxCanvas *ctxcanvas;
	cairo_t* cr;
  GdkScreen* screen;
  GdkDrawable* drawable = (GdkDrawable*)data;

  cr = gdk_cairo_create(drawable);
  if (!cr) 
    return;

  screen = gdk_drawable_get_screen(drawable);
  canvas->bpp = gdk_drawable_get_depth(drawable);
  canvas->xres = ((double)gdk_screen_get_width(screen)  / (double)gdk_screen_get_width_mm(screen));
  canvas->yres = ((double)gdk_screen_get_height(screen) / (double)gdk_screen_get_height_mm(screen));
  gdk_drawable_get_size(drawable, &canvas->w, &canvas->h);

  canvas->w_mm = ((double)canvas->w) / canvas->xres;
  canvas->h_mm = ((double)canvas->h) / canvas->yres;

  ctxcanvas = cdcairoCreateCanvas(canvas, cr);

  ctxcanvas->drawable = drawable;
}

static void cdinittable(cdCanvas* canvas)
{
  cdcairoInitTable(canvas);

  canvas->cxKillCanvas = cdkillcanvas;
  canvas->cxActivate = cdactivate;
}

/******************************************************/

static cdContext cdNativeWindowContext =
{
  CD_CAP_ALL & ~(CD_CAP_PLAY | CD_CAP_YAXIS | CD_CAP_REGION | CD_CAP_WRITEMODE | CD_CAP_PALETTE | CD_CAP_FPRIMTIVES),
  1,
  cdcreatecanvas,
  cdinittable,
  NULL,
  NULL,
};


cdContext* cdContextCairoNativeWindow(void)
{
  return &cdNativeWindowContext;
}
