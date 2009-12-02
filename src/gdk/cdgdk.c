/** \file
 * \brief Gdk Cairo Base Driver
 *
 * See Copyright Notice in cd.h
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <math.h>

#include "cdgdk.h"

#include <gdk/gdk.h>


GdkColor (*cdgdkGetPixel)(cdCtxCanvas *ctxcanvas, unsigned long rgb); /* access to the color table */
void (*cdgdkGetRGB)(cdCtxCanvas *ctxcanvas, unsigned long pixel, 
                                          unsigned char* red, 
                                          unsigned char* green, 
                                          unsigned char* blue); /* access to the color table */
static int cdgdkDirectColorTable[256];    /* used with directColor visuals */

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

/******************************************************/

static void update_colors(cdCtxCanvas *ctxcanvas)
{
  gboolean success;
  gdk_colormap_alloc_colors(ctxcanvas->colormap, ctxcanvas->color_table, ctxcanvas->num_colors, FALSE, TRUE, &success);
}

static int find_color(cdCtxCanvas *ctxcanvas, GdkColor xc1)
{
  int pos = 0, i;
  unsigned long min_dist = ULONG_MAX, this_dist;
  int dr, dg, db;
  GdkColor xc2;

  for (i = 0; i < ctxcanvas->num_colors; i++)
  {
    xc2 = ctxcanvas->color_table[i];

    dr =   (xc1.red   - xc2.red) /  850;    /* 0.30 / 255 */
    dg = (xc1.green - xc2.green) /  432;    /* 0.59 / 255 */
    db =  (xc1.blue  - xc2.blue) / 2318;    /* 0.11 / 255 */

    this_dist = dr*dr + dg*dg + db*db;

    if (this_dist < min_dist)
    {
      min_dist = this_dist;            
      pos = i;                          
    }
  }

  return pos;
}

/* Search the nearest RGB in the color table */
static unsigned long nearest_rgb(cdCtxCanvas *ctxcanvas, GdkColor clr)
{
  static int nearest_try = 0;

  int pos = find_color(ctxcanvas, clr);

  /* Is the color allocated? */
  if (!gdk_colormap_alloc_color(ctxcanvas->colormap, &(ctxcanvas->color_table[pos]), FALSE, TRUE))
  {
    /* Not allocated = update the table and search again */
    /* This is because the color can be found in an application that no longer exists */
    /* Once updated, the problem doesn't occur during a new search */
    /* or the cell is read write */

    if (nearest_try == 1)
    {
      nearest_try = 0;
      return ctxcanvas->color_table[pos].pixel;
    }

    /* What is slower?
       Make a query colors across the nearest (probably, this should be slower)
       or execute the previous search, firstly, and risking a repeat? */

    update_colors(ctxcanvas);

    nearest_try = 1;  /* ensures that this will be done only once */
    return nearest_rgb(ctxcanvas, clr);
  }

  return ctxcanvas->color_table[pos].pixel;
}

/* GetPixel using the conversion table. Used when the system is not True color. */
static GdkColor not_truecolor_get_pixel(cdCtxCanvas *ctxcanvas, unsigned long rgb)
{
  GdkColor clr;

  clr.red   = cdCOLOR8TO16(cdRed(rgb));
  clr.green = cdCOLOR8TO16(cdGreen(rgb));
  clr.blue  = cdCOLOR8TO16(cdBlue(rgb));

  /* 	Is there a new color available? */
  if (!gdk_colormap_alloc_color(ctxcanvas->colormap, &clr, FALSE, TRUE))
  {
    /* Not available: search the nearest rgb in color table */
    clr.pixel = nearest_rgb(ctxcanvas, clr); 
  }
  else
  {
    /* Available: update the color table */
    ctxcanvas->color_table[clr.pixel] = clr;
  }

  return clr;
}

/* GetRGB using the conversion table. Used when the system is not True color. */
static void not_truecolor_get_rgb(cdCtxCanvas *ctxcanvas, unsigned long pixel, unsigned char* red, unsigned char* green, unsigned char* blue)
{
  GdkColor clr;

  gdk_colormap_query_color(ctxcanvas->colormap, pixel, &clr);

  *red = cdCOLOR16TO8(clr.red);
  *green = cdCOLOR16TO8(clr.green);
  *blue = cdCOLOR16TO8(clr.blue);
}

/* GetRGB function used in True color system. */
static void truecolor_get_rgb(cdCtxCanvas *ctxcanvas, unsigned long pixel, unsigned char* red, unsigned char* green, unsigned char* blue)
{
  unsigned long r = pixel & ctxcanvas->vis->red_mask;
  unsigned long g = pixel & ctxcanvas->vis->green_mask;
  unsigned long b = pixel & ctxcanvas->vis->blue_mask;

  if (ctxcanvas->rshift < 0)
    r = r >> (-ctxcanvas->rshift);
  else
    r = r << ctxcanvas->rshift;

  if (ctxcanvas->gshift < 0)
    g = g >> (-ctxcanvas->gshift);
  else
    g = g << ctxcanvas->gshift;

  if (ctxcanvas->bshift < 0)
    b = b >> (-ctxcanvas->bshift);
  else
    b = b << ctxcanvas->bshift;

  *red   = cdCOLOR16TO8(r);
  *green = cdCOLOR16TO8(g);
  *blue  = cdCOLOR16TO8(b);
}

/* GetPixel function used in TrueColor mode. */
static GdkColor truecolor_get_pixel(cdCtxCanvas *ctxcanvas, unsigned long rgb)
{
  GdkColor clrRGB;

  clrRGB.red   = cdCOLOR8TO16(cdRed(rgb));
  clrRGB.green = cdCOLOR8TO16(cdGreen(rgb));
  clrRGB.blue  = cdCOLOR8TO16(cdBlue(rgb));

  (void)ctxcanvas;

  //if (ctxcanvas->rshift < 0) 
  //  clrRGB.red = clrRGB.red << (-ctxcanvas->rshift);
  //else 
  //  clrRGB.red = clrRGB.red >> ctxcanvas->rshift;
 
  //if (ctxcanvas->gshift < 0) 
  //  clrRGB.green = clrRGB.green << (-ctxcanvas->gshift);
  //else 
  //  clrRGB.green = clrRGB.green >> ctxcanvas->gshift;
 
  //if (ctxcanvas->bshift < 0) 
  //  clrRGB.blue = clrRGB.blue << (-ctxcanvas->bshift);
  //else 
  //  clrRGB.blue = clrRGB.blue >> ctxcanvas->bshift;
 
  //clrRGB.red   = clrRGB.red   & ctxcanvas->vis->red_mask;
  //clrRGB.green = clrRGB.green & ctxcanvas->vis->green_mask;
  //clrRGB.blue  = clrRGB.blue  & ctxcanvas->vis->blue_mask;

  return clrRGB;
}

/* Returns position of highest set bit in 'ul' as an integer (0-31), or -1 if none */
static int highbit(unsigned long ul)
{
  int i;
  unsigned long hb;
  
  hb = 0x80;
  hb = hb << 24;   /* hb = 0x80000000UL */
  
  for(i = 31; ((ul & hb) == 0) && i >= 0;  i--, ul <<= 1);
  return i;
}

static void makeDirectCmap(cdCtxCanvas *ctxcanvas, GdkColormap* cmap)
{
  int    i, cmaplen, numgot;
  unsigned char   origgot[256];
  GdkColor c;
  unsigned long rmask, gmask, bmask;
  int    rshift, gshift, bshift;
  
  rmask = ctxcanvas->vis->red_mask;
  gmask = ctxcanvas->vis->green_mask;
  bmask = ctxcanvas->vis->blue_mask;
  
  rshift = highbit(rmask) - 15;
  gshift = highbit(gmask) - 15;
  bshift = highbit(bmask) - 15;
  
  if (rshift < 0) rmask = rmask << (-rshift);
  else rmask = rmask >> rshift;
  
  if (gshift < 0) gmask = gmask << (-gshift);
  else gmask = gmask >> gshift;
  
  if (bshift < 0) bmask = bmask << (-bshift);
  else bmask = bmask >> bshift;
  
  cmaplen = ctxcanvas->vis->colormap_size;

  if (cmaplen > 256)
    cmaplen = 256;
  
  /* try to alloc a 'cmaplen' long grayscale colormap.  May not get all
  entries for whatever reason.  Build table 'cdgdkDirectColorTable[]' that
  maps range [0..(cmaplen-1)] into set of colors we did get */
  
  for (i = 0; i < 256; i++)
  {
    origgot[i] = 0;
    cdgdkDirectColorTable[i] = i;
  }
  
  for (i = numgot = 0; i < cmaplen; i++) 
  {
    c.red = c.green = c.blue = (unsigned short)((i * 0xffff) / (cmaplen - 1));
    c.red   = (unsigned short)(c.red   & rmask);
    c.green = (unsigned short)(c.green & gmask);
    c.blue  = (unsigned short)(c.blue  & bmask);
    
    if (gdk_colormap_alloc_color(cmap, &c, FALSE, TRUE)) 
    {
      origgot[i] = 1;
      numgot++;
    }
  }
  
  if (numgot == 0) 
    return;
  
  /* cdgdkDirectColorTable may or may not have holes in it. */
  for (i = 0; i < cmaplen; i++) 
  {
    if (!origgot[i]) 
    {
      int numbak, numfwd;
      numbak = numfwd = 0;
      while ((i - numbak) >= 0       && !origgot[i-numbak]) numbak++;
      while ((i + numfwd) <  cmaplen && !origgot[i+numfwd]) numfwd++;
      
      if (i-numbak < 0        || !origgot[i-numbak]) numbak = 999;
      if (i+numfwd >= cmaplen || !origgot[i+numfwd]) numfwd = 999;
      
      if      (numbak < numfwd) cdgdkDirectColorTable[i] = cdgdkDirectColorTable[i-numbak];
      else if (numfwd < 999)    cdgdkDirectColorTable[i] = cdgdkDirectColorTable[i+numfwd];
    }
  }
}

/******************************************************/

void cdgdkKillCanvas(cdCtxCanvas *ctxcanvas)
{
  if (ctxcanvas->canvas->bpp <= 8)
  {
    /* release all colors used in palette */
    g_object_unref(ctxcanvas->color_table);

    if (ctxcanvas->colormap != gdk_colormap_get_system())
      g_object_unref(ctxcanvas->colormap);
  }
 
  if (ctxcanvas->xidata) free(ctxcanvas->xidata);
  if (ctxcanvas->font) cairo_scaled_font_destroy(ctxcanvas->font);
  if (ctxcanvas->last_hatch) g_object_unref(ctxcanvas->last_hatch);
  if (ctxcanvas->clip_polygon) g_object_unref(ctxcanvas->clip_polygon);

  if (ctxcanvas->new_region) 
  {
    g_object_unref(ctxcanvas->region_aux_gc); 
    g_object_unref(ctxcanvas->region_aux);
    g_object_unref(ctxcanvas->new_region);
  }

  if (ctxcanvas->last_pattern)
  {
    g_object_unref(ctxcanvas->last_pattern_gc); 
    g_object_unref(ctxcanvas->last_pattern);
  }

  if (ctxcanvas->last_stipple)
  {
    g_object_unref(ctxcanvas->last_stipple_gc); 
    g_object_unref(ctxcanvas->last_stipple);
  }

  g_object_unref(ctxcanvas->gc); 

  free(ctxcanvas);
}

/******************************************************/

static void cdflush(cdCtxCanvas *ctxcanvas)
{
  (void)ctxcanvas;
  gdk_flush();
}

/******************************************************/

static GdkPixmap* build_clip_polygon(cdCtxCanvas *ctxcanvas, GdkPoint* pnt, int n)
{
  GdkPixmap *pix = gdk_pixmap_new(ctxcanvas->wnd, ctxcanvas->canvas->w, ctxcanvas->canvas->h, 1);
  GdkGC *gc = gdk_gc_new((GdkDrawable*)pix);
  GdkColor clr;

  clr.pixel = 0;
  gdk_gc_set_foreground(gc, &clr);
  gdk_draw_rectangle(pix, gc, TRUE, 0, 0, ctxcanvas->canvas->w, ctxcanvas->canvas->h);

  clr.pixel = 1;
  gdk_gc_set_foreground(gc, &clr);
  gdk_region_polygon(pnt, n, ctxcanvas->canvas->fill_mode == CD_EVENODD ? GDK_EVEN_ODD_RULE : GDK_WINDING_RULE);
  gdk_draw_polygon(pix, gc, TRUE, pnt, n);

  g_object_unref(gc);

  return pix;
}

static void gdksetclip_area(cdCtxCanvas *ctxcanvas)
{
  cdRect* clip_rect = &ctxcanvas->canvas->clip_rect;
  if (ctxcanvas->canvas->use_matrix)
  {
    cdPoint poly[4];
    poly[0].x = clip_rect->xmin; poly[0].y = clip_rect->ymin;
    poly[1].x = clip_rect->xmin; poly[1].y = clip_rect->ymax;
    poly[2].x = clip_rect->xmax; poly[2].y = clip_rect->ymax;
    poly[3].x = clip_rect->xmax; poly[3].y = clip_rect->ymin;
    ctxcanvas->canvas->cxPoly(ctxcanvas, CD_CLIP, poly, 4);
  }
  else
  {
    GdkRectangle rect;
    rect.x      = (short)clip_rect->xmin;
    rect.y      = (short)clip_rect->ymin;
    rect.width  = (unsigned short)(clip_rect->xmax - clip_rect->xmin + 1);
    rect.height = (unsigned short)(clip_rect->ymax - clip_rect->ymin + 1);
    gdk_gc_set_clip_rectangle(ctxcanvas->gc, &rect);
  }
}

int cdgdkClip(cdCtxCanvas *ctxcanvas, int clip_mode)
{
  switch (clip_mode)
  {
  case CD_CLIPOFF:
    gdk_gc_set_clip_mask(ctxcanvas->gc, NULL);
    break;
  case CD_CLIPAREA:
    gdksetclip_area(ctxcanvas);
    break;
  case CD_CLIPPOLYGON:
    if (ctxcanvas->clip_polygon)
      gdk_gc_set_clip_mask(ctxcanvas->gc, (GdkBitmap*)ctxcanvas->clip_polygon);
    break;
  case CD_CLIPREGION:
    if (ctxcanvas->new_region)
      gdk_gc_set_clip_mask(ctxcanvas->gc, (GdkBitmap*)ctxcanvas->new_region);
    break;
  }
  return clip_mode;
}

static void cdcliparea(cdCtxCanvas *ctxcanvas, int xmin, int xmax, int ymin, int ymax)
{
  if (ctxcanvas->canvas->clip_mode == CD_CLIPAREA) 
  {
    ctxcanvas->canvas->clip_rect.xmin = xmin;
    ctxcanvas->canvas->clip_rect.ymin = ymin;
    ctxcanvas->canvas->clip_rect.xmax = xmax;
    ctxcanvas->canvas->clip_rect.ymax = ymax;
    cdgdkClip(ctxcanvas, CD_CLIPAREA);
  }
}

static void cdnewregion(cdCtxCanvas *ctxcanvas)
{
  GdkColor clr;

  if (ctxcanvas->new_region) 
  {
    g_object_unref(ctxcanvas->region_aux_gc);
    g_object_unref(ctxcanvas->region_aux);
    g_object_unref(ctxcanvas->new_region);
  }
   
  ctxcanvas->new_region = gdk_pixmap_new(ctxcanvas->wnd, ctxcanvas->canvas->w, ctxcanvas->canvas->h, 1);

  {
    GdkGC* gc = gdk_gc_new((GdkDrawable*)ctxcanvas->new_region);

    clr.pixel = 0;
    gdk_gc_set_foreground(gc, &clr);

    gdk_draw_rectangle(ctxcanvas->new_region, gc, TRUE, 0, 0, ctxcanvas->canvas->w, ctxcanvas->canvas->h);

    g_object_unref(gc);
  }

  ctxcanvas->region_aux = gdk_pixmap_new(ctxcanvas->wnd, ctxcanvas->canvas->w, ctxcanvas->canvas->h, 1);
  ctxcanvas->region_aux_gc = gdk_gc_new((GdkDrawable*)ctxcanvas->region_aux);

  clr.pixel = 0;
  gdk_gc_set_background(ctxcanvas->region_aux_gc, &clr);
}

static int cdispointinregion(cdCtxCanvas *ctxcanvas, int x, int y)
{
  if (!ctxcanvas->new_region)
    return 0;

  if (x >= 0  && y >= 0 && x < ctxcanvas->canvas->w && y < ctxcanvas->canvas->h)
  {
    long p;
    GdkImage* img = gdk_drawable_get_image((GdkDrawable*)ctxcanvas->new_region, 0, 0, ctxcanvas->canvas->w, ctxcanvas->canvas->h);

    p = gdk_image_get_pixel(img, x, y);
    g_object_unref(img);
    
    if (p)
      return 1;
  }

  return 0;
}

static void cdgetregionbox(cdCtxCanvas *ctxcanvas, int *xmin, int *xmax, int *ymin, int *ymax)
{
  if (!ctxcanvas->new_region)
    return;
    
  *xmin = ctxcanvas->canvas->w-1;
  *xmax = 0;
  *ymin = ctxcanvas->canvas->h-1;
  *ymax = 0;

  {
    int x, y;
    long p;
    GdkImage* img = gdk_drawable_get_image((GdkDrawable*)ctxcanvas->new_region, 0, 0, ctxcanvas->canvas->w, ctxcanvas->canvas->h);

    for (y = 0; y < ctxcanvas->canvas->h; y++)
    {
      for (x = 0; x < ctxcanvas->canvas->w; x++)
      {
        p = gdk_image_get_pixel(img, x, y);
        
        if (p)
        {
          if (x < *xmin) *xmin = x; 
          if (x > *xmax) *xmax = x; 
          if (y < *ymin) *ymin = y; 
          if (y > *ymax) *ymax = y; 
          break;
        }
      }
      
      if (x != ctxcanvas->canvas->w-1)
      {
        for (x = ctxcanvas->canvas->w-1; x >= 0; x--)
        {
          p = gdk_image_get_pixel(img, x, y);
        
          if (p)
          {
            if (x < *xmin) *xmin = x; 
            if (x > *xmax) *xmax = x; 
            if (y < *ymin) *ymin = y; 
            if (y > *ymax) *ymax = y; 
            break;
          }
        }
      }
    }

    g_object_unref(img);
  }
}

static void sPrepareRegion(cdCtxCanvas *ctxcanvas)
{
  GdkColor clr;

  if (!ctxcanvas->new_region)
    return;

  gdk_gc_set_function(ctxcanvas->region_aux_gc, GDK_COPY);

  clr.pixel = 0;
  gdk_gc_set_foreground(ctxcanvas->region_aux_gc, &clr);

  gdk_draw_rectangle(ctxcanvas->region_aux, ctxcanvas->region_aux_gc, TRUE, 0, 0, ctxcanvas->canvas->w, ctxcanvas->canvas->h);

  clr.pixel = 1;
  gdk_gc_set_foreground(ctxcanvas->region_aux_gc, &clr);
}

static void sCombineRegion(cdCtxCanvas *ctxcanvas)
{
  switch(ctxcanvas->canvas->combine_mode)
  {                          
  case CD_UNION:
    gdk_gc_set_function(ctxcanvas->region_aux_gc, GDK_OR);
    break;
  case CD_INTERSECT:   
    gdk_gc_set_function(ctxcanvas->region_aux_gc, GDK_AND);
    break;
  case CD_DIFFERENCE:           
    gdk_gc_set_function(ctxcanvas->region_aux_gc, GDK_AND_INVERT);
    break;
  case CD_NOTINTERSECT:
    gdk_gc_set_function(ctxcanvas->region_aux_gc, GDK_XOR);
    break;
  }

  gdk_draw_drawable((GdkDrawable*)ctxcanvas->new_region, ctxcanvas->region_aux_gc,
                    (GdkDrawable*)ctxcanvas->region_aux, 0, 0, 0, 0,
                    ctxcanvas->canvas->w, ctxcanvas->canvas->h);
}

static void cdoffsetregion(cdCtxCanvas *ctxcanvas, int x, int y)
{
  if (!ctxcanvas->new_region)
    return;
    
  sPrepareRegion(ctxcanvas);    

  gdk_draw_drawable((GdkDrawable*)ctxcanvas->new_region, ctxcanvas->region_aux_gc,
                    (GdkDrawable*)ctxcanvas->region_aux, 0, 0, x, y,
                    ctxcanvas->canvas->w-x, ctxcanvas->canvas->h-y);

  gdk_draw_drawable((GdkDrawable*)ctxcanvas->region_aux, ctxcanvas->region_aux_gc,
                    (GdkDrawable*)ctxcanvas->new_region, 0, 0, 0, 0,
                    ctxcanvas->canvas->w, ctxcanvas->canvas->h);
}

/******************************************************/

static int cdwritemode(cdCtxCanvas *ctxcanvas, int write_mode)
{
  switch (write_mode)
  {
  case CD_REPLACE:
    gdk_gc_set_function(ctxcanvas->gc, GDK_COPY);
    break;
  case CD_XOR:
    gdk_gc_set_function(ctxcanvas->gc, GDK_XOR);
    break;
  case CD_NOT_XOR:
    gdk_gc_set_function(ctxcanvas->gc, GDK_EQUIV);
    break;
  }

  return write_mode;
}

static int cdinteriorstyle(cdCtxCanvas *ctxcanvas, int style)
{
  GdkFill sty = GDK_SOLID;

  switch (style)
  {
    case CD_SOLID:
      sty = GDK_SOLID;
      break;

    case CD_HATCH :
      if (!ctxcanvas->last_hatch) 
        return ctxcanvas->canvas->interior_style;

      gdk_gc_set_stipple(ctxcanvas->gc, ctxcanvas->last_hatch);

      if (ctxcanvas->canvas->back_opacity == CD_OPAQUE)
        sty = GDK_OPAQUE_STIPPLED;
      else
        sty = GDK_STIPPLED;
      break;

    case CD_STIPPLE:
      gdk_gc_set_stipple(ctxcanvas->gc, ctxcanvas->last_stipple);

      if (ctxcanvas->canvas->back_opacity == CD_OPAQUE)
        sty = GDK_OPAQUE_STIPPLED;
      else
        sty = GDK_STIPPLED;
      break;

    case CD_PATTERN:
      gdk_gc_set_tile(ctxcanvas->gc, ctxcanvas->last_pattern);
      sty = GDK_TILED;
      break;
  }

  gdk_gc_set_fill(ctxcanvas->gc, sty);

  return style;
}

static int cdhatch(cdCtxCanvas *ctxcanvas, int hatch_style)
{
  GdkColor fg, bg;

  if (ctxcanvas->last_hatch)
    g_object_unref(ctxcanvas->last_hatch);

  fg.pixel = 1;
  bg.pixel = 0;

  ctxcanvas->last_hatch = gdk_pixmap_create_from_data(ctxcanvas->wnd, hatches[hatch_style],
                                                      HATCH_WIDTH, HATCH_HEIGHT, 1, &fg, &bg);

  cdinteriorstyle(ctxcanvas, CD_HATCH);

  return hatch_style;
}

static void cdstipple(cdCtxCanvas *ctxcanvas, int w, int h, const unsigned char *data)
{
  int x, y;

  if (ctxcanvas->last_stipple == 0 || (ctxcanvas->last_stipple_w != w || ctxcanvas->last_stipple_h != h))
  {
    if (ctxcanvas->last_stipple != 0)
    {
      g_object_unref(ctxcanvas->last_stipple);
      g_object_unref(ctxcanvas->last_stipple_gc);
    }

    ctxcanvas->last_stipple = gdk_pixmap_new(ctxcanvas->wnd, w, h, 1);
    if (!ctxcanvas->last_stipple)
      return;
    
    ctxcanvas->last_stipple_gc = gdk_gc_new((GdkDrawable*)ctxcanvas->last_stipple);
    ctxcanvas->last_stipple_w = w;
    ctxcanvas->last_stipple_h = h;
  }

  for (y = 0; y < h; y++)
  {
    for (x = 0; x < w; x++)
    {
      GdkColor clr;

      if(data[y*w+x])
        clr.pixel = 1;
      else
        clr.pixel = 0;

      gdk_gc_set_foreground(ctxcanvas->last_stipple_gc, &clr);
      gdk_draw_point(ctxcanvas->last_stipple, ctxcanvas->last_stipple_gc, x, h-y-1);
    }
  }

  cdinteriorstyle(ctxcanvas, CD_STIPPLE);
}

static int find_match(unsigned long* palette, int pal_size, unsigned long color, unsigned char *match)
{
  int i;

  for (i = 0; i < pal_size; i++)
  {
    if (palette[i] == color)
    {
      *match = (unsigned char)i;
      return 1;
    }
  }

  return 0;
}

static void cdpattern(cdCtxCanvas *ctxcanvas, int w, int h, const long int *colors)
{
  int x, y, i;
  int size = w*h;
  GdkColor *pixels = g_new (GdkColor, size);

  if (ctxcanvas->last_pattern == 0 || (ctxcanvas->last_pattern_w != w || ctxcanvas->last_pattern_h != h))
  {
    if (ctxcanvas->last_pattern != 0)
    {
      g_object_unref(ctxcanvas->last_pattern);
      g_object_unref(ctxcanvas->last_pattern_gc);
    }

    ctxcanvas->last_pattern = gdk_pixmap_new(ctxcanvas->wnd, w, h, ctxcanvas->depth);
    if (!ctxcanvas->last_pattern)
      return;

    ctxcanvas->last_pattern_gc = gdk_gc_new((GdkDrawable*)ctxcanvas->last_pattern);
    ctxcanvas->last_pattern_w = w;
    ctxcanvas->last_pattern_h = h;
  }

  if (ctxcanvas->canvas->bpp <= 8)
  {
    GdkColor* match_table;        /* GDK colors */
    unsigned long palette[256];   /* CD  colors */
    unsigned char *index = (unsigned char*)malloc(size), match;
    int pal_size = 1;
    palette[0] = colors[0];

    /* find the "n" first different colors of the image (to 256) */
    for(i = 0; i < size; i++)
    {
      if (!find_match(palette, pal_size, colors[i], &match))
      {
         palette[pal_size] = colors[i];
         index[i] = (unsigned char)pal_size;
         pal_size++;

         if (pal_size == 256)
           break;
      }
      else
        index[i] = match;
    }

    /* CD colors to GDK colors */
    match_table = g_new (GdkColor, pal_size);
    for (i = 0; i < pal_size; i++)
      match_table[i] = cdgdkGetPixel(ctxcanvas, palette[i]);

    /* CD image to GDK image */
    for (i = 0; i < size; i++)
      pixels[i] = match_table[index[i]];

    g_free(match_table);
    free(index);
  }
  else
  {
    for(i=0;i<size;i++)
      pixels[i] = cdgdkGetPixel(ctxcanvas, colors[i]);
  }

  for (y = 0; y < h; y++)
  {
    for (x = 0; x < w; x++)
    {
      gdk_gc_set_rgb_fg_color(ctxcanvas->last_pattern_gc, &pixels[y*w+x]);
      gdk_draw_point(ctxcanvas->last_pattern, ctxcanvas->last_pattern_gc, x, h-y-1);
    }
  }

  cdinteriorstyle(ctxcanvas, CD_PATTERN);
  g_free(pixels);
}

static int cdlinestyle(cdCtxCanvas *ctxcanvas, int style)
{
  switch (style)
  {
  case CD_CONTINUOUS:
    ctxcanvas->gcval.line_style = GDK_LINE_SOLID;
    break;
  case CD_DASHED:
  case CD_DOTTED:
  case CD_DASH_DOT:
  case CD_DASH_DOT_DOT:
    {
      static struct {
        int size;
        signed char list[6];
      } dashes[4] = {
        { 2, { 6, 2 } },
        { 2, { 2, 2 } },
        { 4, { 6, 2, 2, 2 } },
        { 6, { 6, 2, 2, 2, 2, 2 } }
      };

      if (ctxcanvas->canvas->back_opacity == CD_OPAQUE)
        ctxcanvas->gcval.line_style = GDK_LINE_DOUBLE_DASH;
      else
        ctxcanvas->gcval.line_style = GDK_LINE_ON_OFF_DASH;
        
      gdk_gc_set_dashes(ctxcanvas->gc, 0, dashes[style-CD_DASHED].list, dashes[style-CD_DASHED].size);
      break;
    }
  case CD_CUSTOM:        
    {
      int i;
      signed char* dash_style = (signed char*)malloc(ctxcanvas->canvas->line_dashes_count);
      for (i = 0; i < ctxcanvas->canvas->line_dashes_count; i++)
        dash_style[i] = (char)ctxcanvas->canvas->line_dashes[i];

      if (ctxcanvas->canvas->back_opacity == CD_OPAQUE)
        ctxcanvas->gcval.line_style = GDK_LINE_DOUBLE_DASH;
      else
        ctxcanvas->gcval.line_style = GDK_LINE_ON_OFF_DASH;

      gdk_gc_set_dashes(ctxcanvas->gc, 0, dash_style, ctxcanvas->canvas->line_dashes_count);
      free(dash_style);
      break;
    }
  }

  gdk_gc_set_values(ctxcanvas->gc, &ctxcanvas->gcval, GDK_GC_LINE_STYLE);

  return style;
}

static int cdlinewidth(cdCtxCanvas *ctxcanvas, int width)
{
  if (width == 1) 
    ctxcanvas->gcval.line_width = 0;
  else
    ctxcanvas->gcval.line_width = width;

  gdk_gc_set_values(ctxcanvas->gc, &ctxcanvas->gcval, GDK_GC_LINE_WIDTH);

  return width;
}

static int cdlinecap(cdCtxCanvas *ctxcanvas, int cap)
{
  int cd2x_cap[] =  {GDK_CAP_BUTT, GDK_CAP_PROJECTING, GDK_CAP_ROUND};

  ctxcanvas->gcval.cap_style = cd2x_cap[cap];
  gdk_gc_set_values(ctxcanvas->gc, &ctxcanvas->gcval, GDK_GC_CAP_STYLE);

  return cap;
}

static int cdlinejoin(cdCtxCanvas *ctxcanvas, int join)
{
  int cd2x_join[] = {GDK_JOIN_MITER, GDK_JOIN_BEVEL, GDK_JOIN_ROUND};

  ctxcanvas->gcval.join_style = cd2x_join[join];
  gdk_gc_set_values(ctxcanvas->gc, &ctxcanvas->gcval, GDK_GC_JOIN_STYLE);

  return join;
}

static int cdbackopacity(cdCtxCanvas *ctxcanvas, int opaque)
{
  ctxcanvas->canvas->back_opacity = opaque;
  cdinteriorstyle(ctxcanvas, ctxcanvas->canvas->interior_style);
  cdlinestyle(ctxcanvas, ctxcanvas->canvas->line_style);
  return opaque;
}

static int cdfont(cdCtxCanvas *ctxcanvas, const char *type_face, int style, int size)
{
  cairo_t* crWnd = gdk_cairo_create(ctxcanvas->wnd);
  cairo_scaled_font_t *font;
  cairo_font_slant_t  slant  = CAIRO_FONT_SLANT_NORMAL;    /* default is */
  cairo_font_weight_t weight = CAIRO_FONT_WEIGHT_NORMAL;   /*  CD_PLAIN  */

  if (cdStrEqualNoCase(type_face, "System"))
    type_face = "fixed";
  else if (cdStrEqualNoCase(type_face, "Monospace") || cdStrEqualNoCase(type_face, "Courier New"))
    type_face = "courier";
  else if (cdStrEqualNoCase(type_face, "Serif") || cdStrEqualNoCase(type_face, "Times New Roman"))
    type_face = "times";
  else if (cdStrEqualNoCase(type_face, "Sans") || cdStrEqualNoCase(type_face, "Arial"))
    type_face = "helvetica";

  /* no underline or strikeout support */

  if (style & CD_BOLD)
    weight = CAIRO_FONT_WEIGHT_BOLD;

  if (style & CD_ITALIC)
    slant = CAIRO_FONT_SLANT_ITALIC;

  cairo_select_font_face(crWnd, type_face, slant, weight);
  cairo_set_font_size(crWnd, (double)cdGetFontSizePixels(ctxcanvas->canvas, size));

  font = cairo_get_scaled_font(crWnd);

  if (!font)
    return 0;

  if (ctxcanvas->font) 
    cairo_scaled_font_destroy(ctxcanvas->font);

  font = cairo_scaled_font_reference(font);

  ctxcanvas->font = font;

  cairo_destroy(crWnd);

  return 1;
}

static int cdnativefont(cdCtxCanvas *ctxcanvas, const char* nativefont)
{
  int size = 12, style = CD_PLAIN;
  char type_face[1024];
  cairo_t* crWnd = gdk_cairo_create(ctxcanvas->wnd);

  if (nativefont[0] == '-')
  {
    cairo_scaled_font_t *font;

    cairo_select_font_face(crWnd, nativefont, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(crWnd, (double)cdGetFontSizePixels(ctxcanvas->canvas, size));

    font = cairo_get_scaled_font(crWnd);

    if (!font)
      return 0;

    if (!cdParseXWinFont(nativefont, type_face, &style, &size))
    {
      cairo_scaled_font_destroy(font);
      return 0;
    }

    if (ctxcanvas->font)
      cairo_scaled_font_destroy(ctxcanvas->font);
    
    ctxcanvas->font = font;

    cairo_set_scaled_font(crWnd, ctxcanvas->font);
  }
  else
  {
    if (!cdParsePangoFont(nativefont, type_face, &style, &size))
      return 0;

    if (!cdfont(ctxcanvas, type_face, style, size))
      return 0;
  }

  /* update cdfont parameters */
  ctxcanvas->canvas->font_style = style;
  ctxcanvas->canvas->font_size = size;
  strcpy(ctxcanvas->canvas->font_type_face, type_face);

  cairo_destroy(crWnd);

  return 1;
}

static void cdgetfontdim(cdCtxCanvas *ctxcanvas, int *max_width, int *height, int *ascent, int *descent)
{
  cairo_t* crWnd = gdk_cairo_create(ctxcanvas->wnd);
  cairo_font_extents_t extents;

  if (!ctxcanvas->font)
    return;

  cairo_set_scaled_font(crWnd, ctxcanvas->font);
  cairo_font_extents(crWnd, &extents);

  if (max_width) *max_width = (int)extents.max_x_advance;
  if (height)    *height    = (int)extents.height;
  if (ascent)    *ascent    = (int)extents.ascent;
  if (descent)   *descent   = (int)extents.descent;

  cairo_destroy(crWnd);
}

static long int cdbackground(cdCtxCanvas *ctxcanvas, long int color)
{
  ctxcanvas->bg = cdgdkGetPixel(ctxcanvas, color);
  gdk_gc_set_rgb_bg_color(ctxcanvas->gc, &ctxcanvas->bg);
  return color;
}

static long int cdforeground(cdCtxCanvas *ctxcanvas, long int color)
{          
  ctxcanvas->fg = cdgdkGetPixel(ctxcanvas, color);
  gdk_gc_set_rgb_fg_color(ctxcanvas->gc, &ctxcanvas->fg);
  return color;
}

static void cdpalette(cdCtxCanvas *ctxcanvas, int n, const long int *palette, int mode)
{
  int i;
  GdkColor *pixels = g_new (GdkColor, 256);

  for(i = 0; i < ctxcanvas->num_colors; i++)
    pixels[i] = ctxcanvas->color_table[i];

  gdk_colormap_free_colors(ctxcanvas->colormap, ctxcanvas->color_table, ctxcanvas->num_colors);

  if (mode == CD_FORCE)
  {
    GdkColor clr;
    int tokeep;

    /* if POLITE then allocates own palette */
    if (ctxcanvas->colormap == gdk_colormap_get_system())
      ctxcanvas->colormap = gdk_colormap_new(ctxcanvas->vis, FALSE);

    /* if FORCE then it will allocate all colors...
       but if the number of colors is less than the maximum
       then the difference is used to preserve the first allocated colors in the default colormap. */
    tokeep = ctxcanvas->num_colors - n;
    if (tokeep)
    {
      gboolean success;

      for (i=0; i<tokeep; i++) 
        ctxcanvas->color_table[i].pixel = i;

      gdk_colormap_alloc_colors(gdk_colormap_get_system(), ctxcanvas->color_table, tokeep, FALSE, TRUE, &success);

      /* reserve these colors to use in CD */
      for (i=0; i<tokeep; i++)
        gdk_colormap_alloc_color(ctxcanvas->colormap, &(ctxcanvas->color_table[i]), FALSE, TRUE);
    }

    /* allocate all the palette colors to the CD */
    for (i = 0; i < n; i++)
    {
      clr.red   = cdCOLOR8TO16(cdRed(palette[i]));
      clr.green = cdCOLOR8TO16(cdGreen(palette[i]));
      clr.blue  = cdCOLOR8TO16(cdBlue(palette[i]));
      gdk_colormap_alloc_color(ctxcanvas->colormap, &clr, FALSE, TRUE);
    }

    /* update the entire color table */
    gdk_drawable_set_colormap(ctxcanvas->wnd, ctxcanvas->colormap);
    update_colors(ctxcanvas);
  }
  else
  {
    /* if was FORCE, remove the own palette */
    if (ctxcanvas->colormap != gdk_colormap_get_system())
    {
      g_object_unref(ctxcanvas->colormap);
      ctxcanvas->colormap = gdk_colormap_get_system();
    }

    /* Update the color table before adding new colors.
       After all, all released before that we could. */
    update_colors(ctxcanvas);

    /* if POLITE then just try to allocate all the colors of the palette */
    for (i = 0; i < n; i++)
      cdgdkGetPixel(ctxcanvas, palette[i]);
  }

  g_free(pixels);
}

/******************************************************/

static void cdgdkCheckSolidStyle(cdCtxCanvas *ctxcanvas, int set)
{
  if (ctxcanvas->canvas->interior_style == CD_SOLID)
    return;

  if (set)
    gdk_gc_set_fill(ctxcanvas->gc, GDK_SOLID);
  else
    cdinteriorstyle(ctxcanvas, ctxcanvas->canvas->interior_style);
}

static void cdclear(cdCtxCanvas* ctxcanvas)
{
  GdkColor clr;

  cdgdkCheckSolidStyle(ctxcanvas, 1);
  
  clr = cdgdkGetPixel(ctxcanvas, ctxcanvas->canvas->background);
  gdk_gc_set_rgb_fg_color(ctxcanvas->gc, &clr);

  gdk_draw_rectangle(ctxcanvas->wnd, ctxcanvas->gc, TRUE, 0, 0, ctxcanvas->canvas->w, ctxcanvas->canvas->h);

  clr = cdgdkGetPixel(ctxcanvas, ctxcanvas->canvas->foreground);
  gdk_gc_set_rgb_fg_color(ctxcanvas->gc, &clr);

  cdgdkCheckSolidStyle(ctxcanvas, 0);
}

static void cdline(cdCtxCanvas *ctxcanvas, int x1, int y1, int x2, int y2)
{ 
  if (ctxcanvas->canvas->use_matrix)
  {
    cdMatrixTransformPoint(ctxcanvas->xmatrix, x1, y1, &x1, &y1);
    cdMatrixTransformPoint(ctxcanvas->xmatrix, x2, y2, &x2, &y2);
  }

  cdgdkCheckSolidStyle(ctxcanvas, 1);
  gdk_draw_line(ctxcanvas->wnd, ctxcanvas->gc, x1, y1, x2, y2);
  cdgdkCheckSolidStyle(ctxcanvas, 0);
}

static void cdarc(cdCtxCanvas *ctxcanvas, int xc, int yc, int w, int h, double a1, double a2)
{
  if (ctxcanvas->canvas->use_matrix)
  {
    cdarcSIM(ctxcanvas, xc, yc, w, h, a1, a2);
    return;
  }

  cdgdkCheckSolidStyle(ctxcanvas, 1);
  gdk_draw_arc(ctxcanvas->wnd, ctxcanvas->gc, FALSE, xc-w/2, yc-h/2, w, h, cdRound(a1*64), cdRound((a2 - a1)*64));
  cdgdkCheckSolidStyle(ctxcanvas, 0);
}

static void cdsector(cdCtxCanvas *ctxcanvas, int xc, int yc, int w, int h, double a1, double a2)
{
  if (ctxcanvas->canvas->use_matrix)
  {
    cdsectorSIM(ctxcanvas, xc, yc, w, h, a1, a2);
    return;
  }

  if (ctxcanvas->canvas->new_region)
  {
    sPrepareRegion(ctxcanvas);
    /* "filled parameter = TRUE" produces an 'pie slice' */
    gdk_draw_arc(ctxcanvas->region_aux, ctxcanvas->region_aux_gc, TRUE, xc-w/2, yc-h/2, w, h, cdRound(a1*64), cdRound((a2 - a1)*64));
    sCombineRegion(ctxcanvas);
  }
  else
  {
    /* "filled parameter = TRUE" produces an 'pie slice' */
    gdk_draw_arc(ctxcanvas->wnd, ctxcanvas->gc, TRUE, xc-w/2, yc-h/2, w, h, cdRound(a1*64), cdRound((a2 - a1)*64));
  }
}

static void cdchord(cdCtxCanvas *ctxcanvas, int xc, int yc, int w, int h, double a1, double a2)
{
  if (ctxcanvas->canvas->new_region)
  {
    sPrepareRegion(ctxcanvas);
    cdchordSIM(ctxcanvas, xc, yc, w, h, a1, a2);
    sCombineRegion(ctxcanvas);
  }
  else
  {
    cdchordSIM(ctxcanvas, xc, yc, w, h, a1, a2);
  }
}

static void cdrect(cdCtxCanvas *ctxcanvas, int xmin, int xmax, int ymin, int ymax)
{
  if (ctxcanvas->canvas->use_matrix)
  {
    cdrectSIM(ctxcanvas, xmin, xmax, ymin, ymax);
    return;
  }

  cdgdkCheckSolidStyle(ctxcanvas, 1);
  gdk_draw_rectangle(ctxcanvas->wnd, ctxcanvas->gc, FALSE, xmin, ymin, xmax-xmin, ymax-ymin);
  cdgdkCheckSolidStyle(ctxcanvas, 0);
}

static void cdbox(cdCtxCanvas *ctxcanvas, int xmin, int xmax, int ymin, int ymax)
{
  if (ctxcanvas->canvas->use_matrix)
  {
    cdboxSIM(ctxcanvas, xmin, xmax, ymin, ymax);
    return;
  }

  if (ctxcanvas->canvas->new_region)
  {
    sPrepareRegion(ctxcanvas);
    gdk_draw_rectangle(ctxcanvas->region_aux, ctxcanvas->region_aux_gc, TRUE, xmin, ymin, xmax-xmin+1, ymax-ymin+1);
    sCombineRegion(ctxcanvas);
  }
  else
    gdk_draw_rectangle(ctxcanvas->wnd, ctxcanvas->gc, TRUE, xmin, ymin, xmax-xmin+1, ymax-ymin+1);
}

static void cdtext(cdCtxCanvas *ctxcanvas, int x, int y, const char *s, int len)
{
  cairo_t *crWnd;
  cairo_text_extents_t extents;
  cairo_font_extents_t font_extents;
  const char *utf8_text;
  
  int dir = -1;
  int x_origin = x;
  int y_origin = y;

  if (!ctxcanvas->canvas->new_region)
    crWnd = gdk_cairo_create(ctxcanvas->wnd);
  else
    crWnd = gdk_cairo_create((GdkDrawable*)ctxcanvas->region_aux);

  utf8_text = g_locale_to_utf8(s, len, NULL, 0, NULL);

  cairo_set_scaled_font(crWnd, ctxcanvas->font);

  cairo_font_extents(crWnd, &font_extents);
  cairo_text_extents(crWnd, utf8_text, &extents);
  
  switch (ctxcanvas->canvas->text_alignment)
  {
  case CD_BASE_RIGHT:
  case CD_NORTH_EAST:
  case CD_EAST:
  case CD_SOUTH_EAST:
    x = x - (int)extents.width; 
    break;
  case CD_BASE_CENTER:
  case CD_CENTER:
  case CD_NORTH:
  case CD_SOUTH:
    x = x - (int)(extents.width/2);  
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
    y = y - dir*(int)font_extents.descent;
    break;
  case CD_NORTH_EAST:
  case CD_NORTH:
  case CD_NORTH_WEST:
    y = y + dir*(int)(extents.height - font_extents.descent);
    break;
  case CD_CENTER:
  case CD_EAST:
  case CD_WEST:
    y = y + dir*(int)(extents.height/2 - font_extents.descent);
    break;
  }

  if (ctxcanvas->canvas->text_orientation != 0)
  {
    cairo_matrix_t matrix;

    cairo_translate(crWnd, (double)x_origin, (double)y_origin);
    cairo_get_matrix(crWnd, &matrix);

    x -= ctxcanvas->canvas->w/2;
    y -= ctxcanvas->canvas->h/2;

    cairo_matrix_rotate(&matrix, -ctxcanvas->canvas->text_orientation*CD_DEG2RAD);
    cairo_matrix_translate(&matrix, (double)x, (double)y);

    cairo_set_matrix(crWnd, &matrix);
  }
  else
  {
    cairo_move_to(crWnd, (double)x, (double)y);
  }

  cdgdkCheckSolidStyle(ctxcanvas, 1);

  if (ctxcanvas->canvas->use_matrix)
    cdMatrixTransformPoint(ctxcanvas->xmatrix, x, y, &x, &y);

  if (ctxcanvas->canvas->new_region)
  {
    sPrepareRegion(ctxcanvas);

    cairo_set_source_rgb(crWnd, 1.0, 1.0, 1.0);
    cairo_show_text(crWnd, utf8_text);

    sCombineRegion(ctxcanvas);
  }
  else
  {
    cairo_set_source_rgb(crWnd, ctxcanvas->fg.red/255, ctxcanvas->fg.green/255, ctxcanvas->fg.blue/255); 
    cairo_show_text(crWnd, utf8_text);
  }

  cdgdkCheckSolidStyle(ctxcanvas, 0);

  cairo_destroy(crWnd);
}

static void cdgettextsize(cdCtxCanvas *ctxcanvas, const char *s, int len, int *width, int *height)
{
  cairo_t* crWnd = gdk_cairo_create(ctxcanvas->wnd);
  cairo_text_extents_t extents;
  const char *utf8_text;

  if (!ctxcanvas->font)
    return;

  utf8_text = g_locale_to_utf8(s, len, NULL, 0, NULL);

  cairo_set_scaled_font(crWnd, ctxcanvas->font);
  cairo_text_extents(crWnd, utf8_text, &extents);

  if (width)
    *width = (int)extents.width;

  if (height)
    *height = (int)extents.height;

  cairo_destroy(crWnd);
}

void cdgdkPoly(cdCtxCanvas *ctxcanvas, int mode, cdPoint* poly, int n)
{
   int i;
   GdkPoint* pnt;

   pnt = g_new (GdkPoint, n);

   if(!pnt)
     return;

   if (mode != CD_BEZIER)
   {
     for (i = 0; i < n; i++)
     {
       int x = poly[i].x, 
           y = poly[i].y;
 
       if (ctxcanvas->canvas->use_matrix)
         cdMatrixTransformPoint(ctxcanvas->xmatrix, x, y, &x, &y);
 
       pnt[i].x = (short)x;
       pnt[i].y = (short)y;
     }
   }
 
   switch( mode )
   {
     case CD_FILL:
       if (ctxcanvas->canvas->new_region)
       {
         sPrepareRegion(ctxcanvas);
         gdk_region_polygon(pnt, n, ctxcanvas->canvas->fill_mode == CD_EVENODD ? GDK_EVEN_ODD_RULE : GDK_WINDING_RULE);
         gdk_draw_polygon(ctxcanvas->region_aux, ctxcanvas->region_aux_gc, TRUE, pnt, n);
         sCombineRegion(ctxcanvas);
       }
       else
       {
         gdk_region_polygon(pnt, n, ctxcanvas->canvas->fill_mode == CD_EVENODD ? GDK_EVEN_ODD_RULE : GDK_WINDING_RULE);
         gdk_draw_polygon(ctxcanvas->wnd, ctxcanvas->gc, TRUE, pnt, n);
       }
       break;
     
     case CD_CLOSED_LINES:
       cdgdkCheckSolidStyle(ctxcanvas, 1);
       gdk_draw_polygon(ctxcanvas->wnd, ctxcanvas->gc, FALSE, pnt, n);
       cdgdkCheckSolidStyle(ctxcanvas, 0);
       break;
   
     case CD_OPEN_LINES:
       cdgdkCheckSolidStyle(ctxcanvas, 1);
       gdk_draw_lines(ctxcanvas->wnd, ctxcanvas->gc, pnt, n);
       cdgdkCheckSolidStyle(ctxcanvas, 0);
       break;
          
     case CD_CLIP:
       if (ctxcanvas->clip_polygon)
         g_object_unref(ctxcanvas->clip_polygon);
       
       ctxcanvas->clip_polygon = build_clip_polygon(ctxcanvas, pnt, n);
       
       if (ctxcanvas->canvas->clip_mode == CD_CLIPPOLYGON)
         cdgdkClip(ctxcanvas, CD_CLIPPOLYGON);
       
       break;
     
     case CD_BEZIER:
       cdSimPolyBezier(ctxcanvas->canvas, poly, n);
       break;
   }
 
  g_free(pnt);
}

/******************************************************/

static void cdgetimagergb(cdCtxCanvas *ctxcanvas, unsigned char *r, unsigned char *g, unsigned char *b, int x, int y, int w, int h)
{
  int col, lin, pos;
  GdkImage* xi = gdk_drawable_get_image(ctxcanvas->wnd, x, y-h+1, w, h);
  if (!xi)
  {
    fprintf(stderr, "CanvasDraw: error getting image\n");
    return;
  }
  
  for (lin = 0; lin < h; lin++)
  {
    for (col = 0; col < w; col++)
    {
      pos = (h-lin-1) * w + col;
      cdgdkGetRGB(ctxcanvas, gdk_image_get_pixel(xi, col, lin), r+pos, g+pos, b+pos);
    }
  }

  g_object_unref(xi);
}

static long int* get_data_buffer(cdCtxCanvas *ctxcanvas, int size)
{
  if (!ctxcanvas->xidata)
  {
    ctxcanvas->xisize = size;
    ctxcanvas->xidata = (long int *)malloc(ctxcanvas->xisize);
  }
  else if (ctxcanvas->xisize < size)
  {
    ctxcanvas->xisize = size;
    ctxcanvas->xidata = (long int *)realloc(ctxcanvas->xidata, ctxcanvas->xisize);
  }

  if (!ctxcanvas->xidata)
    ctxcanvas->xisize = 0;

  return ctxcanvas->xidata;
}
  
static GdkImage *map2gdkImage(cdCtxCanvas *ctxcanvas, int ew, int eh, const unsigned char *index, const long int * colors, int by, int bx, int bw, int bh, int iw)
{
  GdkColor** match_table = NULL;
  int i, j, pal_size;
  unsigned long xcol;
  GdkImage *xim;
  int *fx, *fy, src, dst;
  unsigned char idx;
  GdkColor tmpClr;
  
  xim = (GdkImage*) NULL;
  
  /* The size of the palette is unknown, a priori...
     So, it is necessary to find the highest index used in image */
  pal_size = 0;
  
  for (i = 0; i < bh; i++) 
  {
    for (j = 0; j < bw; j++) 
    {
      src = (i+by)*iw + j+bx;
      idx = index[src];
      if (idx > pal_size)
        pal_size = idx;
    }
  }
  
  pal_size++;

  for (i = 0; i < pal_size; i++)
  {
    tmpClr = cdgdkGetPixel(ctxcanvas, colors[i]);
    match_table[i] = &tmpClr;
  }

  fx = cdGetZoomTable(ew, bw, bx);
  fy = cdGetZoomTable(eh, bh, by);

  switch (ctxcanvas->depth) 
  {
  case 8: 
    {
      unsigned char  *imagedata, *ip;
      int imew, nullCount;
    
      nullCount = (4 - (ew % 4)) & 0x03;  /* # of padding bytes per line */
      imew = ew + nullCount;
    
      /* Now get the image data - pad each scan line as necessary */
      imagedata = (unsigned char*)get_data_buffer(ctxcanvas, eh * imew);
      if (!imagedata) 
      {
        fprintf(stderr, "CanvasDraw: not enough memory putting image\n");
        return NULL;
      }
    
      for (i = 0; i < eh; i++) 
      {
        ip = imagedata + (eh-1-i)*imew;

        for (j = 0; j < ew; j++, ip++) 
        {
          src = (fy[i])*iw + fx[j];
          ip = (unsigned char*) match_table[index[src]]->pixel;
        }
      }

      xim = gdk_image_new(GDK_IMAGE_FASTEST, ctxcanvas->vis, ew, eh);

      if (!xim) 
      {
        fprintf(stderr, "CanvasDraw: not enough memory putting image\n");
        return NULL;
      }

      if(xim)
      {
        xim->depth = (short)ctxcanvas->depth;
        xim->bpp = 32;
        xim->bpl = (short)imew;
      }
    }
    break;

  case 12:
  case 15:
  case 16: 
    {
      unsigned char *imagedata;
      unsigned short *ip, *tip;
    
      /* Now get the image data - pad each scan line as necessary */
      imagedata = (unsigned char*)get_data_buffer(ctxcanvas, 2*ew*eh);
      if (!imagedata) 
      {
        fprintf(stderr, "CanvasDraw: not enough memory putting image\n");
        return NULL;
      }

      xim = gdk_image_new(GDK_IMAGE_FASTEST, ctxcanvas->vis, ew, eh);

      if (!xim) 
      {
        fprintf(stderr, "CanvasDraw: not enough memory putting image\n");
        return NULL;
      }

      if(xim)
      {
        xim->depth = (short)ctxcanvas->depth;
        xim->bpp = 16;
        xim->bpl = 0;
      }
    
      if (ctxcanvas->depth == 12 && xim->bits_per_pixel != 16) 
      {
        g_object_unref(xim);
        fprintf(stderr,"No code for this type of display (depth=%d, bperpix=%d)", ctxcanvas->depth, xim->bits_per_pixel);
        return NULL;
      }
    
      ip = (unsigned short*)(imagedata + (eh-1)*xim->bpl);

      for (i = 0; i < eh; i++) 
      {
        for (j = 0, tip = ip; j < ew; j++) 
        {
          src = (fy[i])*iw + fx[j];
          xcol = match_table[index[src]]->pixel;
          
          if (xim->byte_order == GDK_MSB_FIRST) 
          {
            *tip++ = (unsigned short)(xcol & 0xffff);
          }
          else
          {
            /*  WAS *tip++ = ((xcol>>8) & 0xff) | ((xcol&0xff) << 8);  */
            *tip++ = (unsigned short)(xcol);
          }
        }

        ip -= ew;
      }
    }
    break;

  case 24:
  case 32: 
    {
      unsigned char  *imagedata, *ip, *tip;
      int do32;
    
      /* Now get the image data - pad each scan line as necessary */
      imagedata = (unsigned char*)get_data_buffer(ctxcanvas, 4*ew*eh);
      if (!imagedata) 
      {
        fprintf(stderr, "CanvasDraw: not enough memory putting image\n");
        return NULL;
      }

      xim = gdk_image_new(GDK_IMAGE_FASTEST, ctxcanvas->vis, ew, eh);

      if (!xim) 
      {
        fprintf(stderr, "CanvasDraw: not enough memory putting image\n");
        return NULL;
      }

      if(xim)
      {
        xim->depth = (short)ctxcanvas->depth;
        xim->bpp = 32;
        xim->bpl = 0;
      }
    
      do32 = (xim->bits_per_pixel == 32 ? 1 : 0);
    
      ip = imagedata + (eh-1)*xim->bpl;

      for (i = 0; i < eh; i++) 
      {
        for (j = 0, tip = ip; j < ew; j++) 
        {
          src = (fy[i])*iw + fx[j];
          xcol = match_table[index[src]]->pixel;
        
          if (xim->byte_order == GDK_MSB_FIRST) 
          {
            if (do32) *tip++ = 0;
            *tip++ = (unsigned char)((xcol>>16) & 0xff);
            *tip++ = (unsigned char)((xcol>>8)  & 0xff);
            *tip++ = (unsigned char)( xcol      & 0xff);
          }
          else 
          {  /* GDK_LSB_FIRST */
            *tip++ = (unsigned char)( xcol      & 0xff);
            *tip++ = (unsigned char)((xcol>>8)  & 0xff);
            *tip++ = (unsigned char)((xcol>>16) & 0xff);
            if (do32) *tip++ = 0;
          }
        }

        ip -= xim->bpl;
      }
    }
    break;
  default: 
    {
      /* Now get the image data - pad each scan line as necessary */
      unsigned long* imagedata = (unsigned long*)get_data_buffer(ctxcanvas, 4*ew*eh);
      if (!imagedata) 
      {
        fprintf(stderr, "CanvasDraw: not enough memory putting image\n");
        return NULL;
      }
    
      xim = gdk_image_new(GDK_IMAGE_FASTEST, ctxcanvas->vis, ew, eh);

      if (!xim) 
      {
        fprintf(stderr, "CanvasDraw: not enough memory putting image\n");
        return NULL;
      }

      if(xim)
      {
        xim->depth = (short)ctxcanvas->depth;
        xim->bpp = 32;
        xim->bpl = (short)ew*4;
      }

      xim->bits_per_pixel = 32;
      xim->bpl = 4 * (short)iw;
      xim->byte_order = GDK_MSB_FIRST;

      for (i = 0; i < eh; i++) 
      {
        for (j = 0; j < ew; j++) 
        {
          src = (fy[i])*iw + fx[j];
          dst = (eh-1 - i)*ew + j;
          imagedata[dst] = match_table[index[src]]->pixel;
        }
      }
    }
    break;
  }
  
  free(fx);
  free(fy);

  return(xim);
}

static GdkImage *rgb2gdkImage(cdCtxCanvas *ctxcanvas, int ew, int eh, 
                            const unsigned char *red, const unsigned char *green, const unsigned char *blue, 
                            const unsigned char *alpha, GdkImage *oxi, 
                            int by, int bx, int bw, int bh, int iw)
{
/* if we're displaying on a TrueColor or DirectColor display, we've got all the colors we're going to need,
 * and 'all we have to do' is convert 24-bit RGB pixels into whatever variation of RGB the X device in
 * question wants.  No color allocation is involved.
 */
  GdkImage *xim;
  int *fx, *fy;
  int i, j, maplen, src;
  int rshift, gshift, bshift, bperpix, bperline, byte_order, cshift;
  unsigned long r, g, b, rmask, gmask, bmask, xcol;
  unsigned char *lip, *ip, *imagedata, or, ob, og, al;

  /* compute various shifting constants that we'll need... */
  rmask = ctxcanvas->vis->red_mask;
  gmask = ctxcanvas->vis->green_mask;
  bmask = ctxcanvas->vis->blue_mask;
  rshift = 7 - highbit(rmask);
  gshift = 7 - highbit(gmask);
  bshift = 7 - highbit(bmask);

  maplen = ctxcanvas->vis->colormap_size;
  
  if (maplen > 256)
    maplen = 256;
  
  cshift = 7 - highbit((unsigned long) (maplen-1));
  
  xim = gdk_image_new(GDK_IMAGE_FASTEST, ctxcanvas->vis, ew, eh);

  if (!xim) 
  {
    fprintf(stderr, "CanvasDraw: not enough memory putting image\n");
    return NULL;
  }

  bperline   = xim->bpl;
  bperpix    = xim->bits_per_pixel;
  byte_order = xim->byte_order;

  if (bperpix != 8 && bperpix != 16 && bperpix != 24 && bperpix != 32) 
  {
    g_object_unref(xim);
    fprintf(stderr, "CanvasDraw: bpp=%d not supported!\n", bperpix);
    return NULL;
  }
  
  imagedata = (unsigned char*)get_data_buffer(ctxcanvas, eh * bperline);
  if (!imagedata)
  {
    g_object_unref(xim);
    fprintf(stderr, "CanvasDraw: not enough memory putting image\n");
    return NULL;
  }

  fx = cdGetZoomTable(ew, bw, bx);
  fy = cdGetZoomTable(eh, bh, by);

  lip = imagedata + (eh - 1) * bperline;

  for (i = 0; i < eh; i++, lip -= bperline) 
  {
    for (j = 0, ip = lip; j < ew; j++) 
    {
      src = fy[i]*iw + fx[j];

      if (alpha)
      {
        cdgdkGetRGB(ctxcanvas, gdk_image_get_pixel(oxi, j, eh-i-1), &or, &og, &ob);
        al = alpha[src];
        r = CD_ALPHA_BLEND(red[src], or, al);
        g = CD_ALPHA_BLEND(green[src], og, al);
        b = CD_ALPHA_BLEND(blue[src], ob, al);
      }
      else
      {
        r = red[src];  
        g = green[src];  
        b = blue[src];
      }
      
      /* shift r, g, b so that high bit of 8-bit color specification is aligned with
      *  high bit of r, g, b-mask in visual, AND each component with its mask, and
      *  OR the three components together */
      if(gdk_visual_get_best_type() == GDK_VISUAL_DIRECT_COLOR)
      {
        r = (unsigned long) cdgdkDirectColorTable[(r>>cshift) & 0xff] << cshift;
        g = (unsigned long) cdgdkDirectColorTable[(g>>cshift) & 0xff] << cshift;
        b = (unsigned long) cdgdkDirectColorTable[(b>>cshift) & 0xff] << cshift;
      }
      
      /* shift the bits around */
      if (rshift < 0)
        r = r << (-rshift);
      else
        r = r >> rshift;
      
      if (gshift < 0)
        g = g << (-gshift);
      else
        g = g >> gshift;
      
      if (bshift < 0)
        b = b << (-bshift);
      else
        b = b >> bshift;
      
      r = r & rmask;
      g = g & gmask;
      b = b & bmask;
      
      xcol = r | g | b;
      
      if (bperpix == 32) 
      {
        if (byte_order == GDK_MSB_FIRST)
        {
          *ip++ = (unsigned char)((xcol>>24) & 0xff);
          *ip++ = (unsigned char)((xcol>>16) & 0xff);
          *ip++ = (unsigned char)((xcol>>8)  & 0xff);
          *ip++ = (unsigned char)( xcol      & 0xff);
        }
        else 
        {  /* GDK_LSB_FIRST */
          *ip++ = (unsigned char)( xcol      & 0xff);
          *ip++ = (unsigned char)((xcol>>8)  & 0xff);
          *ip++ = (unsigned char)((xcol>>16) & 0xff);
          *ip++ = (unsigned char)((xcol>>24) & 0xff);
        }
      }
      else if (bperpix == 24) 
      {
        if (byte_order == GDK_MSB_FIRST) 
        {
          *ip++ = (unsigned char)((xcol>>16) & 0xff);
          *ip++ = (unsigned char)((xcol>>8)  & 0xff);
          *ip++ = (unsigned char)( xcol      & 0xff);
        }
        else 
        {  /* GDK_LSB_FIRST */
          *ip++ = (unsigned char)( xcol      & 0xff);
          *ip++ = (unsigned char)((xcol>>8)  & 0xff);
          *ip++ = (unsigned char)((xcol>>16) & 0xff);
        }
      }
      else if (bperpix == 16) 
      {
        if (byte_order == GDK_MSB_FIRST) 
        {
          *ip++ = (unsigned char)((xcol>>8)  & 0xff);
          *ip++ = (unsigned char)( xcol      & 0xff);
        }
        else {  /* GDK_LSB_FIRST */
          *ip++ = (unsigned char)( xcol      & 0xff);
          *ip++ = (unsigned char)((xcol>>8)  & 0xff);
        }
      }
      else if (bperpix == 8) 
      {
        *ip++ =  (unsigned char)(xcol      & 0xff);
      }
    }
  }

  /* Copy imagedata to GdkImage */
  memcpy (xim->mem, imagedata, eh * bperline);

  free(fx);
  free(fy);

  return xim;
}

static void cdputimagerectrgba_matrix(cdCtxCanvas* ctxcanvas, int iw, int ih, const unsigned char *r, const unsigned char *g, const unsigned char *b, const unsigned char *a, int x, int y, int w, int h, int xmin, int xmax, int ymin, int ymax)
{
  int t_xmin, t_xmax, t_ymin, t_ymax, ew, eh,
      t_x, t_y, dst_offset, size, nc, doff, rect[8];
  float i_x, i_y, xfactor, yfactor;
  unsigned char *dst_r, *dst_g, *dst_b, *dst_a = NULL;
  double inv_matrix[6];

  /* calculate the destination limits */
  cdImageRGBCalcDstLimits(ctxcanvas->canvas, x, y, w, h, &t_xmin, &t_xmax, &t_ymin, &t_ymax, rect);

  /* Setup inverse transform (use the original transform here, NOT ctxcanvas->xmatrix) */
  cdImageRGBInitInverseTransform(w, h, xmin, xmax, ymin, ymax, &xfactor, &yfactor, ctxcanvas->canvas->matrix, inv_matrix);

  /* create an image for the destination area */
  ew = (t_xmax-t_xmin+1);
  eh = (t_ymax-t_ymin+1); 
  size = ew*eh;
  nc = 3;
  if (a) nc = 4;
  dst_r = malloc(nc*size);
  if (!dst_r)
  {
    fprintf(stderr, "CanvasDraw: no enough memory\n");
    return;
  }
  dst_g = dst_r + size;
  dst_b = dst_g + size;
  if (a) dst_a = dst_b + size;
  memset(dst_r, 0, nc*size);

  /* for all pixels in the destiny area */
  for(t_y = t_ymin; t_y <= t_ymax; t_y++)
  {
    dst_offset = (t_y-t_ymin) * ew;

    for(t_x = t_xmin; t_x <= t_xmax; t_x++)
    {
      cdImageRGBInverseTransform(t_x, t_y, &i_x, &i_y, xfactor, yfactor, xmin, ymin, x, y, inv_matrix);

      if (i_x > xmin && i_y > ymin && i_x < xmax+1 && i_y < ymax+1)
      {
        doff = (t_x-t_xmin) + dst_offset;
        *(dst_r+doff) = cdBilinearInterpolation(iw, ih, r, i_x, i_y);
        *(dst_g+doff) = cdBilinearInterpolation(iw, ih, g, i_x, i_y);
        *(dst_b+doff) = cdBilinearInterpolation(iw, ih, b, i_x, i_y);
        if (a) *(dst_a+doff) = cdBilinearInterpolation(iw, ih, a, i_x, i_y);
      }
    }
  }

  {
    int ex = t_xmin, 
        ey = t_ymin + eh-1;  /* gdkImage origin is at top-left */
    GdkImage *xi, *oxi = NULL;
    GdkPixmap *clip_polygon, *clip_mask = NULL;
    GdkPoint* pnt = g_malloc(64);

    /* Since the transformation used was the original transformation, */
    /* must invert the Y axis here. */
    ey = _cdInvertYAxis(ctxcanvas->canvas, ey);

    /* use clipping to select only the transformed rectangle */
    pnt[0].x = (short)rect[0]; pnt[0].y = (short)_cdInvertYAxis(ctxcanvas->canvas, rect[1]);
    pnt[1].x = (short)rect[2]; pnt[1].y = (short)_cdInvertYAxis(ctxcanvas->canvas, rect[3]);
    pnt[2].x = (short)rect[4]; pnt[2].y = (short)_cdInvertYAxis(ctxcanvas->canvas, rect[5]);
    pnt[3].x = (short)rect[6]; pnt[3].y = (short)_cdInvertYAxis(ctxcanvas->canvas, rect[7]);
    clip_polygon = build_clip_polygon(ctxcanvas, pnt, 4);

    /* combine with the existing clipping */
    if (ctxcanvas->canvas->clip_mode == CD_CLIPAREA || ctxcanvas->canvas->clip_mode == CD_CLIPPOLYGON)
      clip_mask = ctxcanvas->clip_polygon;
    else if (ctxcanvas->canvas->clip_mode == CD_CLIPREGION)
      clip_mask = ctxcanvas->new_region;

    gdk_gc_set_function(ctxcanvas->gc, GDK_AND);

    gdk_draw_drawable((GdkDrawable*)clip_polygon, ctxcanvas->gc,
                      (GdkDrawable*)clip_mask, 0, 0, 0, 0,
                       ctxcanvas->canvas->w, ctxcanvas->canvas->h);

    gdk_gc_set_clip_mask(ctxcanvas->gc, (GdkBitmap*)clip_polygon);

    cdwritemode(ctxcanvas, ctxcanvas->canvas->write_mode);  /* reset gdk_gc_set_function */

    if (a)
    {
      oxi = gdk_drawable_get_image(ctxcanvas->wnd, ex, ey, ew, eh);

      if (!oxi)
      {
        fprintf(stderr, "CanvasDraw: error getting image\n");
        free(dst_r);
        return;
      }
    }

    xi = rgb2gdkImage(ctxcanvas, ew, eh, dst_r, dst_g, dst_b, dst_a, oxi, 0, 0, ew, eh, ew);
    if (!xi)
      return;

    gdk_draw_image(ctxcanvas->wnd, ctxcanvas->gc, xi, 0, 0, ex, ey, ew, eh);

    /* reset clipping */
    g_object_unref(clip_polygon);
    cdgdkClip(ctxcanvas, ctxcanvas->canvas->clip_mode);

    g_object_unref(xi);
    if (oxi)
      g_object_unref(oxi);

    g_free(pnt);
  }

  free(dst_r);
}

static void cdputimagerectmap_matrix(cdCtxCanvas* ctxcanvas, int iw, int ih, const unsigned char *index, const long int *colors, int x, int y, int w, int h, int xmin, int xmax, int ymin, int ymax)
{
  int t_xmin, t_xmax, t_ymin, t_ymax, ew, eh,
      t_x, t_y, dst_offset, size, doff, rect[8];
  float i_x, i_y, xfactor, yfactor;
  unsigned char *dst_index;
  double inv_matrix[6];

  /* calculate the destination limits */
  cdImageRGBCalcDstLimits(ctxcanvas->canvas, x, y, w, h, &t_xmin, &t_xmax, &t_ymin, &t_ymax, rect);

  /* Setup inverse transform (use the original transform here, NOT ctxcanvas->xmatrix) */
  cdImageRGBInitInverseTransform(w, h, xmin, xmax, ymin, ymax, &xfactor, &yfactor, ctxcanvas->canvas->matrix, inv_matrix);

  /* create an image for the destination area */
  ew = (t_xmax-t_xmin+1);
  eh = (t_ymax-t_ymin+1); 
  size = ew*eh;
  dst_index = malloc(size);
  if (!dst_index)
  {
    fprintf(stderr, "CanvasDraw: no enough memory\n");
    return;
  }
  memset(dst_index, 0, size);

  /* for all pixels in the destiny area */
  for(t_y = t_ymin; t_y <= t_ymax; t_y++)
  {
    dst_offset = (t_y-t_ymin) * ew;

    for(t_x = t_xmin; t_x <= t_xmax; t_x++)
    {
      cdImageRGBInverseTransform(t_x, t_y, &i_x, &i_y, xfactor, yfactor, xmin, ymin, x, y, inv_matrix);

      if (i_x > xmin && i_y > ymin && i_x < xmax+1 && i_y < ymax+1)
      {
        doff = (t_x-t_xmin) + dst_offset;
        *(dst_index+doff) = cdZeroOrderInterpolation(iw, ih, index, i_x, i_y);
      }
    }
  }

  {
    int ex = t_xmin, 
        ey = t_ymin + eh-1;  /* GdkImage* origin is at top-left */
    
    GdkImage *xi;
    GdkPixmap *clip_polygon, *clip_mask = NULL;
    GdkPoint pnt[4];

    /* Since the transformation used was the original transformation, */
    /* must invert the Y axis here. */
    ey = _cdInvertYAxis(ctxcanvas->canvas, ey);

    /* use clipping to select only the transformed rectangle */
    pnt[0].x = (short)rect[0]; pnt[0].y = (short)_cdInvertYAxis(ctxcanvas->canvas, rect[1]);
    pnt[1].x = (short)rect[2]; pnt[1].y = (short)_cdInvertYAxis(ctxcanvas->canvas, rect[3]);
    pnt[2].x = (short)rect[4]; pnt[2].y = (short)_cdInvertYAxis(ctxcanvas->canvas, rect[5]);
    pnt[3].x = (short)rect[6]; pnt[3].y = (short)_cdInvertYAxis(ctxcanvas->canvas, rect[7]);
    clip_polygon = build_clip_polygon(ctxcanvas, pnt, 4);

    /* combine with the existing clipping */
    if (ctxcanvas->canvas->clip_mode == CD_CLIPAREA || ctxcanvas->canvas->clip_mode == CD_CLIPPOLYGON)
      clip_mask = ctxcanvas->clip_polygon;
    else if (ctxcanvas->canvas->clip_mode == CD_CLIPREGION)
      clip_mask = ctxcanvas->new_region;

    gdk_gc_set_function(ctxcanvas->gc, GDK_AND);

    gdk_draw_drawable((GdkDrawable*)clip_polygon, ctxcanvas->gc,
                      (GdkDrawable*)clip_mask, 0, 0, 0, 0,
                      ctxcanvas->canvas->w, ctxcanvas->canvas->h);

    gdk_gc_set_clip_mask(ctxcanvas->gc, (GdkBitmap*)clip_polygon);

    cdwritemode(ctxcanvas, ctxcanvas->canvas->write_mode);  /* reset gdk_gc_set_function */

    xi = map2gdkImage(ctxcanvas, ew, eh, dst_index, colors, 0, 0, ew, eh, ew);
    if (!xi)
      return;

    gdk_draw_image(ctxcanvas->wnd, ctxcanvas->gc, xi, 0, 0, ex, ey, ew, eh);

    /* reset clipping */
    g_object_unref(clip_polygon);
    cdgdkClip(ctxcanvas, ctxcanvas->canvas->clip_mode);

    g_object_unref(xi);
  }

  free(dst_index);
}

static void cdputimagerectrgb(cdCtxCanvas *ctxcanvas, int iw, int ih, const unsigned char *r, const unsigned char *g, const unsigned char *b, int x, int y, int w, int h, int xmin, int xmax, int ymin, int ymax)
{
  int ew = w, eh = h, ex = x, ey = y;
  int bw = iw, bh = ih, bx = 0, by = 0;
  int rw, rh;
  GdkImage* xi;

  if (ctxcanvas->canvas->use_matrix)
  {
    cdputimagerectrgba_matrix(ctxcanvas, iw, ih, r, g, b, NULL, x, y, w, h, xmin, xmax, ymin, ymax);
    return;
  }

  rw = xmax-xmin+1;
  rh = ymax-ymin+1;
  y -= (h - 1);        /* gdkImage origin is at top-left */

  if (!cdCalcZoom(ctxcanvas->canvas->w, x, w, &ex, &ew, xmin, rw, &bx, &bw, 1))
    return;
  
  if (!cdCalcZoom(ctxcanvas->canvas->h, y, h, &ey, &eh, ymin, rh, &by, &bh, 0))
    return;

  xi = rgb2gdkImage(ctxcanvas, ew, eh, r, g, b, NULL, NULL, by, bx, bw, bh, iw);
  if (!xi)
    return;

  gdk_draw_image(ctxcanvas->wnd, ctxcanvas->gc, xi, 0, 0, ex, ey, ew, eh);

  g_object_unref(xi);
}

static void cdputimagerectrgba(cdCtxCanvas *ctxcanvas, int iw, int ih, const unsigned char *r, const unsigned char *g, const unsigned char *b, const unsigned char *a, int x, int y, int w, int h, int xmin, int xmax, int ymin, int ymax)
{
  GdkImage *xi, *oxi;
  int ew = w, eh = h, ex = x, ey = y;
  int bw = iw, bh = ih, bx = 0, by = 0;
  int rw, rh;

  if (ctxcanvas->canvas->use_matrix)
  {
    cdputimagerectrgba_matrix(ctxcanvas, iw, ih, r, g, b, a, x, y, w, h, xmin, xmax, ymin, ymax);
    return;
  }

  rw = xmax-xmin+1;
  rh = ymax-ymin+1;
  y -= (h - 1);        /* gdkImage origin is at top-left */

  if (!cdCalcZoom(ctxcanvas->canvas->w, x, w, &ex, &ew, xmin, rw, &bx, &bw, 1))
    return;
  
  if (!cdCalcZoom(ctxcanvas->canvas->h, y, h, &ey, &eh, ymin, rh, &by, &bh, 0))
    return;

  oxi = gdk_drawable_get_image(ctxcanvas->wnd, ex, ey, ew, eh);

  if (!oxi)
  {
    fprintf(stderr, "CanvasDraw: error getting image\n");
    return;
  }

  xi = rgb2gdkImage(ctxcanvas, ew, eh, r, g, b, a, oxi, by, bx, bw, bh, iw);
  if (!xi)
    return;

  gdk_draw_image(ctxcanvas->wnd, ctxcanvas->gc, xi, 0, 0, ex, ey, ew, eh);

  g_object_unref(xi);
  g_object_unref(oxi);
}

static void cdputimagerectmap(cdCtxCanvas *ctxcanvas, int iw, int ih, const unsigned char *index, const long int *colors, int x, int y, int w, int h, int xmin, int xmax, int ymin, int ymax)
{
  int ew = w, eh = h, ex = x, ey = y;
  int bw = iw, bh = ih, bx = 0, by = 0;
  int rw, rh;
  GdkImage* xi;

  if (ctxcanvas->canvas->use_matrix)
  {
    cdputimagerectmap_matrix(ctxcanvas, iw, ih, index, colors, x, y, w, h, xmin, xmax, ymin, ymax);
    return;
  }

  rw = xmax-xmin+1;
  rh = ymax-ymin+1;
  y -= (h - 1);        /* gdkImage origin is at top-left */

  if (!cdCalcZoom(ctxcanvas->canvas->w, x, w, &ex, &ew, xmin, rw, &bx, &bw, 1))
    return;
  
  if (!cdCalcZoom(ctxcanvas->canvas->h, y, h, &ey, &eh, ymin, rh, &by, &bh, 0))
    return;

  xi = map2gdkImage(ctxcanvas, ew, eh, index, colors, by, bx, bw, bh, iw);
  if (!xi)
    return;

  gdk_draw_image(ctxcanvas->wnd, ctxcanvas->gc, xi, 0, 0, ex, ey, ew, eh);

  g_object_unref(xi);
}

static void cdpixel(cdCtxCanvas *ctxcanvas, int x, int y, long int color)
{
  if (ctxcanvas->canvas->foreground != color)
  {
    GdkColor clr = cdgdkGetPixel(ctxcanvas, color);
    gdk_gc_set_rgb_fg_color(ctxcanvas->gc, &clr);
  }

  if (ctxcanvas->canvas->use_matrix)
    cdMatrixTransformPoint(ctxcanvas->xmatrix, x, y, &x, &y);

  /* Draw pixel */
  gdk_draw_point(ctxcanvas->wnd, ctxcanvas->gc, x, y);

  if (ctxcanvas->canvas->foreground != color)
    gdk_gc_set_rgb_fg_color(ctxcanvas->gc, &ctxcanvas->fg);
}

static cdCtxImage *cdcreateimage (cdCtxCanvas *ctxcanvas, int w, int h)
{
  GdkGC* gc;
  cdCtxImage *ctximage = (cdCtxImage *)malloc(sizeof(cdCtxImage));
  GdkColor clr;

  ctximage->w = w;
  ctximage->h = h;
  ctximage->depth = ctxcanvas->depth;
  ctximage->scr   = ctxcanvas->scr;
  ctximage->vis   = ctxcanvas->vis;

  ctximage->img = gdk_pixmap_new(ctxcanvas->wnd, w, h, ctxcanvas->depth);

  if (!ctximage->img)
  {
    free(ctximage);
    return (void *)0;
  }

  gc = gdk_gc_new(ctximage->img);

  clr = cdgdkGetPixel(ctxcanvas, CD_WHITE);

  gdk_gc_set_rgb_fg_color(gc, &clr);
  gdk_draw_rectangle(ctximage->img, gc, TRUE, 0, 0, ctximage->w, ctxcanvas->canvas->h);

  g_object_unref(gc);

  return (void*)ctximage;
}

static void cdgetimage (cdCtxCanvas *ctxcanvas, cdCtxImage *ctximage, int x, int y)
{
  gdk_draw_drawable(ctximage->img, ctxcanvas->gc,
                    ctxcanvas->wnd, x, y - ctximage->h+1, 0, 0,
                    ctximage->w, ctximage->h);
}

static void cdputimagerect (cdCtxCanvas *ctxcanvas, cdCtxImage *ctximage, int x, int y, int xmin, int xmax, int ymin, int ymax)
{
  gdk_draw_drawable(ctxcanvas->wnd, ctxcanvas->gc,
                    ctximage->img, xmin, ctximage->h-ymax-1, x, y-(ymax-ymin+1)+1,
                    xmax-xmin+1, ymax-ymin+1);
}

static void cdkillimage (cdCtxImage *ctximage)
{
  g_object_unref(ctximage->img);
  free(ctximage);
}

static void cdscrollarea (cdCtxCanvas *ctxcanvas, int xmin, int xmax, int ymin, int ymax, int dx, int dy)
{
  gdk_draw_drawable(ctxcanvas->wnd, ctxcanvas->gc,
                    ctxcanvas->wnd, xmin, ymin, xmin+dx, ymin+dy,
                    xmax-xmin+1, ymax-ymin+1);
}

static void cdtransform(cdCtxCanvas *ctxcanvas, const double* matrix)
{
  if (matrix)
  {
    /* configure a bottom-up coordinate system */
    ctxcanvas->xmatrix[0] = 1; 
    ctxcanvas->xmatrix[1] = 0;
    ctxcanvas->xmatrix[2] = 0; 
    ctxcanvas->xmatrix[3] = -1; 
    ctxcanvas->xmatrix[4] = 0; 
    ctxcanvas->xmatrix[5] = (ctxcanvas->canvas->h-1); 
    cdMatrixMultiply(matrix, ctxcanvas->xmatrix);

    ctxcanvas->canvas->invert_yaxis = 0;
  }
  else
  {
    ctxcanvas->canvas->invert_yaxis = 1;
  }
}

/******************************************************************/

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

static char* get_gc_attrib(cdCtxCanvas *ctxcanvas)
{
  return (char*)ctxcanvas->gc;
}

static cdAttribute gc_attrib =
{
  "GC",
  NULL,
  get_gc_attrib
}; 

static void set_aa_attrib(cdCtxCanvas* ctxcanvas, char* data)
{
  cairo_t* crWnd = gdk_cairo_create(ctxcanvas->wnd);
  cairo_font_options_t *crOptions = cairo_font_options_create();

  cairo_scaled_font_get_font_options  (ctxcanvas->font, crOptions);

  if (!data || data[0] == '0')
  {
    cairo_set_antialias(crWnd, CAIRO_ANTIALIAS_NONE);
    cairo_font_options_set_antialias(crOptions, CAIRO_ANTIALIAS_NONE);
  }
  else
  {
    cairo_set_antialias(crWnd, CAIRO_ANTIALIAS_DEFAULT);
    cairo_font_options_set_antialias(crOptions, CAIRO_ANTIALIAS_DEFAULT);
  }

  cairo_font_options_destroy(crOptions);
  cairo_destroy(crWnd);
}

static char* get_aa_attrib(cdCtxCanvas* ctxcanvas)
{
  cairo_t* crWnd = gdk_cairo_create(ctxcanvas->wnd);

  if(cairo_get_antialias(crWnd) == CAIRO_ANTIALIAS_DEFAULT)
    return "1";
  else
    return "0";
}

static cdAttribute aa_attrib =
{
  "ANTIALIAS",
  set_aa_attrib,
  get_aa_attrib
}; 

static char* get_version_attrib(cdCtxCanvas* ctxcanvas)
{
  (void)ctxcanvas;
  return (char*)cairo_version_string();
}

static cdAttribute version_attrib =
{
  "CAIROVERSION",
  NULL,
  get_version_attrib
}; 

cdCtxCanvas *cdgdkCreateCanvas(cdCanvas* canvas, GdkDrawable* wnd, GdkScreen* scr, GdkVisual* vis)
{
  static int first = 1;

  cdCtxCanvas *ctxcanvas = (cdCtxCanvas *)malloc(sizeof(cdCtxCanvas));
  memset(ctxcanvas, 0, sizeof(cdCtxCanvas));

  ctxcanvas->scr = scr;
  ctxcanvas->vis = vis;
  ctxcanvas->wnd = wnd;

  ctxcanvas->gc = gdk_gc_new(wnd);
  
  if (!ctxcanvas->gc) 
  {
    free(canvas);
    return NULL;
  }

  gdk_gc_set_colormap(ctxcanvas->gc, gdk_screen_get_default_colormap(ctxcanvas->scr));
  
  ctxcanvas->canvas = canvas;
  canvas->ctxcanvas = ctxcanvas;
  
  gdk_drawable_get_size(wnd, &ctxcanvas->canvas->w, &ctxcanvas->canvas->h);
  ctxcanvas->depth = gdk_drawable_get_depth(wnd);

  canvas->bpp = ctxcanvas->depth;
  canvas->xres = ((double)gdk_screen_get_width(scr)  / (double)gdk_screen_get_width_mm(scr));
  canvas->yres = ((double)gdk_screen_get_height(scr) / (double)gdk_screen_get_height_mm(scr));
  canvas->w_mm = ((double)canvas->w) / canvas->xres;
  canvas->h_mm = ((double)canvas->h) / canvas->yres;
  canvas->invert_yaxis = 1;

  if (first)
  {
    if (canvas->bpp > 8)
    {
      cdgdkGetRGB   = truecolor_get_rgb;
      cdgdkGetPixel = truecolor_get_pixel;

      /* make linear colormap for DirectColor visual */
      if(gdk_visual_get_best_type() == GDK_VISUAL_DIRECT_COLOR)
        makeDirectCmap(ctxcanvas, gdk_screen_get_default_colormap(ctxcanvas->scr));
    }
    else
    {
      cdgdkGetRGB   = not_truecolor_get_rgb;
      cdgdkGetPixel = not_truecolor_get_pixel;
    }
  }
  
  if (canvas->bpp > 8)
  {
    ctxcanvas->rshift = 15 - highbit(ctxcanvas->vis->red_mask);
    ctxcanvas->gshift = 15 - highbit(ctxcanvas->vis->green_mask);
    ctxcanvas->bshift = 15 - highbit(ctxcanvas->vis->blue_mask);
  
    ctxcanvas->num_colors = 0;
    ctxcanvas->colormap = NULL;

    /* For canvas bpp <= 8 RGBA is simulated with cdGetImageRGB */
    canvas->cxPutImageRectRGBA = cdputimagerectrgba;
  }
  else
  {
    int i;

    ctxcanvas->colormap = gdk_screen_get_default_colormap(scr);
    ctxcanvas->num_colors = 1L << canvas->bpp;

    for (i = 0; i < ctxcanvas->num_colors; i++)
      ctxcanvas->color_table[i].pixel = i;

    update_colors(ctxcanvas);
  }

  cdRegisterAttribute(canvas, &gc_attrib);
  cdRegisterAttribute(canvas, &rotate_attrib);
  cdRegisterAttribute(canvas, &aa_attrib);
  cdRegisterAttribute(canvas, &version_attrib);

  first = 0;

  return ctxcanvas;
}

void cdgdkInitTable(cdCanvas* canvas)
{
  canvas->cxFlush = cdflush;
  canvas->cxClear = cdclear;
  
  canvas->cxPixel  = cdpixel;
  canvas->cxLine   = cdline;
  canvas->cxPoly   = cdgdkPoly;
  canvas->cxRect   = cdrect;
  canvas->cxBox    = cdbox;
  canvas->cxArc    = cdarc;
  canvas->cxSector = cdsector;
  canvas->cxChord  = cdchord;
  canvas->cxText   = cdtext;

  canvas->cxNewRegion = cdnewregion;
  canvas->cxIsPointInRegion = cdispointinregion;
  canvas->cxOffsetRegion = cdoffsetregion;
  canvas->cxGetRegionBox = cdgetregionbox;
  canvas->cxClip = cdgdkClip;
  canvas->cxClipArea = cdcliparea;
  canvas->cxWriteMode = cdwritemode;
  canvas->cxLineStyle = cdlinestyle;
  canvas->cxLineWidth = cdlinewidth;
  canvas->cxLineCap = cdlinecap;
  canvas->cxLineJoin = cdlinejoin;
  canvas->cxBackOpacity = cdbackopacity;
  canvas->cxInteriorStyle = cdinteriorstyle;
  canvas->cxHatch = cdhatch;
  canvas->cxStipple = cdstipple;
  canvas->cxPattern = cdpattern;
  canvas->cxFont = cdfont;
  canvas->cxNativeFont = cdnativefont;
  canvas->cxGetFontDim = cdgetfontdim;
  canvas->cxGetTextSize = cdgettextsize;
  canvas->cxPalette = cdpalette;
  canvas->cxBackground = cdbackground;
  canvas->cxForeground = cdforeground;
  canvas->cxTransform = cdtransform;
 
  canvas->cxGetImageRGB = cdgetimagergb;
  canvas->cxScrollArea = cdscrollarea;

  canvas->cxCreateImage = cdcreateimage;
  canvas->cxGetImage = cdgetimage;
  canvas->cxPutImageRect = cdputimagerect;
  canvas->cxKillImage = cdkillimage;

  canvas->cxPutImageRectRGB = cdputimagerectrgb;
  canvas->cxPutImageRectMap = cdputimagerectmap;

  if (canvas->bpp > 8)
    canvas->cxPutImageRectRGBA = cdputimagerectrgba;
}
