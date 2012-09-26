/** \file
* \brief Cairo Native Window Driver
*
* See Copyright Notice in cd.h
*/

#include <stdlib.h>
#include <stdio.h>

#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include "cdcairoctx.h"
#include "cdnative.h"

#ifdef USE_GTK3
int cdGetScreenColorPlanes(void)
{
  static int first = 1;
  static int bpp;

  if (first)
  {
    GdkVisual* info = gdk_visual_get_system();

    if (info != NULL)
    {
#if GTK_CHECK_VERSION(2, 22, 0)
      bpp = gdk_visual_get_depth(info);
#else
			bpp = info->depth;
#endif
      return bpp;
    }

    bpp = 2;
    first = 0;
  }

  return bpp;
}

void cdGetScreenSize(int *width, int *height, double *width_mm, double *height_mm)
{
  static int first = 1;
  static int dpy_width, dpy_height, dpy_width_mm, dpy_height_mm;

  if (first)
  {
    GdkScreen* drv_screen = gdk_screen_get_default();

    dpy_width = gdk_screen_get_width(drv_screen);
    dpy_height = gdk_screen_get_height(drv_screen);
    dpy_width_mm = gdk_screen_get_width_mm(drv_screen);
    dpy_height_mm = gdk_screen_get_height_mm(drv_screen);

    first = 0;
  }

  if (width) *width = dpy_width;
  if (height) *height = dpy_height;
  if (width_mm) *width_mm = dpy_width_mm;
  if (height_mm) *height_mm = dpy_height_mm;
}
#endif

#if GTK_CHECK_VERSION(3, 0, 0)
#ifdef WIN32
#include <gdk/gdkwin32.h>
#include <cairo-win32.h>
#else
#include <gdk/gdkx.h>
#include <cairo-xlib.h>
#endif
#endif

static cairo_t* cdcairoNativeCreateContext(cdCanvas* canvas, GdkWindow* window)
{
#if GTK_CHECK_VERSION(3, 0, 0)
	cairo_t* cr;
	cairo_surface_t* surface;
#ifdef WIN32
  /* UNTESTED */
  HWND hWnd = GDK_WINDOW_HWND(window);
  HDC hDC = GetDC(hWnd);
  /* initial clip extents controls size */
  HRGN clip_hrgn = CreateRectRgn(0, 0, canvas->w, canvas->h);
  SelectClipRgn(hDC, clip_hrgn);
  DeleteObject(clip_hrgn);
  surface = cairo_win32_surface_create(hDC);
/* TODO: HDC cairo_win32_surface_get_dc(cairo_surface_t *surface);
   ReleaseDC(GDK_WINDOW_HWND(window), ctxcanvas->hDC);  */
#else
  XWindowAttributes wa;
  GdkDisplay* display = gdk_display_get_default();
  Display* dpy = GDK_DISPLAY_XDISPLAY(display);
  Window wnd = GDK_WINDOW_XID(window);
  XGetWindowAttributes(dpy, wnd, &wa);
  surface = cairo_xlib_surface_create(dpy, wnd, wa.visual, canvas->w, canvas->h);
#endif
  cr = cairo_create(surface);
  cairo_surface_destroy(surface);
  return cr;
#else
  (void)canvas;
  return gdk_cairo_create(window);  /* this does not works for GTK3 because of the new paint stack they implemented */
#endif
}

int cdactivate(cdCtxCanvas *ctxcanvas)
{
  cdCanvas* canvas = ctxcanvas->canvas;
  int old_w = canvas->w;
  int old_h = canvas->h;

#if GTK_CHECK_VERSION(3, 0, 0)
  canvas->w = gdk_window_get_width(ctxcanvas->window);
  canvas->h = gdk_window_get_height(ctxcanvas->window);
#else
  gdk_drawable_get_size(ctxcanvas->window, &canvas->w, &canvas->h);
#endif

  ctxcanvas->canvas->w_mm = ((double)canvas->w) / canvas->xres;
  ctxcanvas->canvas->h_mm = ((double)canvas->h) / canvas->yres;

 if (old_w != canvas->w || old_h != canvas->h)
  {
    /* Re-create the context so internal size is updated. */
    cairo_destroy(ctxcanvas->cr);
    ctxcanvas->cr = cdcairoNativeCreateContext(canvas, ctxcanvas->window);

    ctxcanvas->last_source = -1;

    cairo_save(ctxcanvas->cr);
    cairo_set_operator(ctxcanvas->cr, CAIRO_OPERATOR_OVER);

    /* update canvas attributes */
    canvas->cxForeground(ctxcanvas, canvas->foreground);
    canvas->cxLineStyle(ctxcanvas, canvas->line_style);
    canvas->cxLineWidth(ctxcanvas, canvas->line_width);
    canvas->cxLineCap(ctxcanvas, canvas->line_cap);
    canvas->cxLineJoin(ctxcanvas, canvas->line_join);
    canvas->cxInteriorStyle(ctxcanvas, canvas->interior_style);
    if (canvas->clip_mode != CD_CLIPOFF) canvas->cxClip(ctxcanvas, canvas->clip_mode);
    if (canvas->use_matrix) canvas->cxTransform(ctxcanvas, canvas->matrix);
  }

  return CD_OK;
}

static void cdcreatecanvas(cdCanvas* canvas, void *data)
{
  cdCtxCanvas *ctxcanvas;
	cairo_t* cr;
  GdkScreen* screen;
  GdkWindow* window = (GdkWindow*)data;

#if GTK_CHECK_VERSION(3, 0, 0)
  if (!GDK_IS_WINDOW(window))
#else
  if (!GDK_IS_DRAWABLE(window))
#endif
    return;

#if GTK_CHECK_VERSION(3, 0, 0)
  screen = gdk_window_get_screen(window);
  canvas->bpp = gdk_visual_get_depth(gdk_window_get_visual(window));
  canvas->w = gdk_window_get_width(window);
  canvas->h = gdk_window_get_height(window);
#else
  screen = gdk_drawable_get_screen(window);
  canvas->bpp = gdk_drawable_get_depth(window);
  gdk_drawable_get_size(window, &canvas->w, &canvas->h);
#endif

  cr = cdcairoNativeCreateContext(canvas, window);
  if (!cr) 
    return;

  canvas->xres = ((double)gdk_screen_get_width(screen)  / (double)gdk_screen_get_width_mm(screen));
  canvas->yres = ((double)gdk_screen_get_height(screen) / (double)gdk_screen_get_height_mm(screen));
  canvas->w_mm = ((double)canvas->w) / canvas->xres;
  canvas->h_mm = ((double)canvas->h) / canvas->yres;

  ctxcanvas = cdcairoCreateCanvas(canvas, cr);

  ctxcanvas->window = window;
}

static void cdinittable(cdCanvas* canvas)
{
  cdcairoInitTable(canvas);

  canvas->cxKillCanvas = cdcairoKillCanvas;
  canvas->cxActivate = cdactivate;
}

/******************************************************/

static cdContext cdNativeWindowContext =
{
  CD_CAP_ALL & ~(CD_CAP_PLAY | CD_CAP_YAXIS | CD_CAP_REGION | CD_CAP_WRITEMODE | CD_CAP_PALETTE),
  CD_CTX_WINDOW|CD_CTX_PLUS,
  cdcreatecanvas,
  cdinittable,
  NULL,
  NULL,
};


#ifdef USE_GTK3
cdContext* cdContextNativeWindow(void)
#else
cdContext* cdContextCairoNativeWindow(void)
#endif
{
  return &cdNativeWindowContext;
}
