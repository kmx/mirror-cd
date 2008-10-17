/*

  Simple Draw Application

  Shows the same picture on several canvas. Used to quick test the CD library and
  to demonstrate the use of CD library functions.

  This module uses only the CD library, there is another module to initialize the Window and its menus.

*/


#include "cd.h"
#include "cdnative.h"
#include "cdcgm.h"
#include "cddgn.h"
#include "cddxf.h"
#include "cdclipbd.h"
#include "cdemf.h"
#include "cdimage.h"
#include "cdirgb.h"
#include "cdmf.h"
#include "cdprint.h"
#include "cdps.h"
#include "cdwmf.h"

#include "simple.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

/* A small pattern and stipple size */
#define SIZE 20

/* Global variables */

cdCanvas *myCanvas = NULL;        /* The window drawing canvas */
cdContext* simple_drv = NULL;     /* The window driver */
char* simple_data = NULL;         /* The window driver data */

int clipping;                     /* Clipping flag, same as the CD */
int write_mode;                   /* Write Mode flag, same as the CD */

long pattern[SIZE*SIZE];          /* Pattern buffer */
unsigned char stipple[SIZE*SIZE]; /* Stipple buffer */
unsigned char red[100*100];       /* Red image buffer */
unsigned char green[100*100];     /* Green image buffer */
unsigned char blue[100*100];      /* Blue image buffer */
unsigned char alpha[100*100];     /* Alpha image buffer */


/* Prototype of the function that makes the drawing independent of canvas. */
void SimpleDraw(void);


void SimpleCreateCanvas(cdContext* drv, char* data)
{
  int c, l;

  simple_drv = drv;
  simple_data = data;

  /* creates the canvas based in an existing window */
  myCanvas = cdCreateCanvas(drv, data);
  clipping = CD_CLIPOFF;
  write_mode = CD_REPLACE;

  /* initialize the pattern buffer with a grey degrade */
  for (l = 0; l < SIZE; l++)
    for (c = 0; c < SIZE; c++)
    {
      unsigned char gray = (unsigned char)((l * 255) / (SIZE - 1));
      pattern[l*20 + c] = cdEncodeColor(gray, gray, gray);
    }

  /* initialize the stipple buffer with cross pattern */
  for (l = 0; l < SIZE; l++)
    for (c = 0; c < SIZE; c++)
      stipple[l*SIZE + c] = (c % 4) == 0? 1: 0;

  /* initialize the alpha image buffer with a degrade from transparent to opaque */
  for (l = 0; l < 100; l++)
    for (c = 0; c < 100; c++)
      alpha[l*100 + c] = (unsigned char)c;
}

int SimplePlayClipboard(void)
{
  /* interprets the contents of the clipboard */
  int w, h;
  cdActivate(myCanvas);
  cdBackground(CD_WHITE);
  cdClear();
  cdGetCanvasSize(&w, &h, 0, 0);
  cdPlay(CD_CLIPBOARD, 100, w-100, 100, h-100, NULL);
  return 0;
}

int SimplePlayCGMBin(void)
{
  /* interprets the contents of the CGM file */
  int w, h;
  cdActivate(myCanvas);
  cdBackground(CD_WHITE);
  cdClear();
  cdGetCanvasSize(&w, &h, 0, 0);
  cdPlay(CD_CGM, 100, w-100, 100, h-100, "simple_b.cgm");
  return 0;
}


int SimplePlayCGMText(void)
{
  /* interprets the contents of the CGM file */
  int w, h;
  cdActivate(myCanvas);
  cdBackground(CD_WHITE);
  cdClear();
  cdGetCanvasSize(&w, &h, 0, 0);
  cdPlay(CD_CGM, 100, w-100, 100, h-100, "simple_t.cgm");
  return 0;
}


int SimplePlayMetafile(void)
{
  int w, h;
  /* interprets the contents of the CD Metafile */
  cdActivate(myCanvas);
  cdBackground(CD_WHITE);
  cdClear();
  cdGetCanvasSize(&w, &h, 0, 0);
  cdPlay(CD_METAFILE, 100, w-100, 100, h-100, "simple.cdmf");
  return 0;
}

int SimplePlayWMF(void)
{
  int w, h;
  /* interprets the contents of the WMF file */
  cdActivate(myCanvas);
  cdBackground(CD_WHITE);
  cdClear();
  cdGetCanvasSize(&w, &h, 0, 0);
  cdPlay(CD_WMF, 100, w-100, 100, h-100, "simple.wmf");
  return 0;
}

int SimplePlayEMF(void)
{
  int w, h;
  /* interprets the contents of the EMF file */
  cdActivate(myCanvas);
  cdBackground(CD_WHITE);
  cdClear();
  cdGetCanvasSize(&w, &h, 0, 0);
  cdPlay(CD_EMF, 100, w-100, 100, h-100, "simple.emf");
  return 0;
}

int SimpleDrawWindow(void)
{
  /* draws in the window */
  cdActivate(myCanvas);
  SimpleDraw();
  return 0;
}

int SimpleDrawCGMText(void)
{
  char StrData[100];
  int w, h;
  cdCanvas* tmpCanvas;
  cdActivate(myCanvas);
  cdGetCanvasSize(&w, &h, 0, 0);
  sprintf(StrData, "simple_t.cgm %gx%g 1 -t", (double)w, (double)h);

  /* draws in the CGM file */
  tmpCanvas = cdCreateCanvas(CD_CGM, StrData);
  if (tmpCanvas == NULL) return 0;
  cdActivate(tmpCanvas);
  SimpleDraw();
  cdKillCanvas(tmpCanvas);
  return 0;
}

int SimpleDrawCGMBin(void)
{
  char StrData[100];
  int w, h;
  cdCanvas* tmpCanvas;
  cdActivate(myCanvas);
  cdGetCanvasSize(&w, &h, 0, 0);
  sprintf(StrData, "simple_b.cgm %gx%g", (double)w, (double)h);

  /* draws in the CGM file */
  tmpCanvas = cdCreateCanvas(CD_CGM, StrData);
  if (tmpCanvas == NULL) return 0;
  cdActivate(tmpCanvas);
  SimpleDraw();
  cdKillCanvas(tmpCanvas);
  return 0;
}

int SimpleDrawDXF(void)
{
  char StrData[100];
  int w, h;
  cdCanvas* tmpCanvas;
  cdActivate(myCanvas);
  cdGetCanvasSize(&w, &h, 0, 0);
  sprintf(StrData, "simple.dxf %gx%g", (double)w, (double)h);

  /* draws in the DXF file */
  tmpCanvas = cdCreateCanvas(CD_DXF, StrData);
  if (tmpCanvas == NULL) return 0;
  cdActivate(tmpCanvas);
  SimpleDraw();
  cdKillCanvas(tmpCanvas);
  return 0;
}

int SimpleDrawDGN(void)
{
  char StrData[100];
  int w, h;
  cdCanvas* tmpCanvas;
  cdActivate(myCanvas);
  cdGetCanvasSize(&w, &h, 0, 0);
  sprintf(StrData, "simple.dgn %gx%g", (double)w, (double)h);

  /* draws in the DGN file */
  tmpCanvas = cdCreateCanvas(CD_DGN, StrData);
  if (tmpCanvas == NULL) return 0;
  cdActivate(tmpCanvas);
  SimpleDraw();
  cdKillCanvas(tmpCanvas);
  return 0;
}

int SimpleDrawEMF(void)
{
  char StrData[100];
  int w, h;
  cdCanvas* tmpCanvas;
  cdActivate(myCanvas);
  cdGetCanvasSize(&w, &h, 0, 0);
  sprintf(StrData, "simple.emf %dx%d", w, h);

  /* draws in the EMF file */
  tmpCanvas = cdCreateCanvas(CD_EMF, StrData);
  if (tmpCanvas == NULL) return 0;
  cdActivate(tmpCanvas);
  SimpleDraw();
  cdKillCanvas(tmpCanvas);
  return 0;
}

int SimpleDrawMetafile(void)
{
  char StrData[100];
  int w, h;
  cdCanvas* tmpCanvas;
  cdActivate(myCanvas);
  cdGetCanvasSize(&w, &h, 0, 0);
  sprintf(StrData, "simple.cdmf %gx%g", (double)w, (double)h);

  /* draws in the CD Metafile */
  tmpCanvas = cdCreateCanvas(CD_METAFILE, StrData);
  if (tmpCanvas == NULL) return 0;
  cdActivate(tmpCanvas);
  SimpleDraw();
  cdKillCanvas(tmpCanvas);
  return 0;
}

int SimpleDrawPS(void)
{
  /* draws in the PS file */
  cdCanvas* tmpCanvas = cdCreateCanvas(CD_PS, "simple.ps");
  if (tmpCanvas == NULL) return 0;
  cdActivate(tmpCanvas);
  SimpleDraw();
  cdKillCanvas(tmpCanvas);
  return 0;
}

int SimpleDrawEPS(void)
{
  /* draws in the EPS file */
  cdCanvas* tmpCanvas = cdCreateCanvas(CD_PS, "simple.eps -e");
  if (tmpCanvas == NULL) return 0;
  cdActivate(tmpCanvas);
  SimpleDraw();
  cdKillCanvas(tmpCanvas);
  return 0;
}

int SimpleDrawWMF(void)
{
  char StrData[100];
  int w, h;
  cdCanvas* tmpCanvas;
  cdActivate(myCanvas);
  cdGetCanvasSize(&w, &h, 0, 0);
  sprintf(StrData, "simple.wmf %dx%d", w, h);

  /* draws in the WMF file */
  tmpCanvas = cdCreateCanvas(CD_WMF, StrData);
  if (tmpCanvas == NULL) return 0;
  cdActivate(tmpCanvas);
  SimpleDraw();
  cdKillCanvas(tmpCanvas);
  return 0;
}

int SimpleDrawPrint(void)
{
  /* draws in the Printer canvas */
  cdCanvas* tmpCanvas = cdCreateCanvas(CD_PRINTER, "simple");
  if (tmpCanvas == NULL) return 0;
  cdActivate(tmpCanvas);
  SimpleDraw();
  cdKillCanvas(tmpCanvas);
  return 0;
}

int SimpleDrawPrintDialog(void)
{
  /* draws in the Printer canvas, but first shows the printer dialog */
  cdCanvas* tmpCanvas = cdCreateCanvas(CD_PRINTER, "simple -d");
  if (tmpCanvas == NULL) return 0;
  cdActivate(tmpCanvas);
  SimpleDraw();
  cdKillCanvas(tmpCanvas);
  return 0;
}

int SimpleDrawClipboardBitmap(void)
{
  char StrData[100];
  int w, h;
  cdCanvas* tmpCanvas;
  cdActivate(myCanvas);
  cdGetCanvasSize(&w, &h, 0, 0);
  sprintf(StrData, "%dx%d -b", w, h);

  /* draws in the CLipboard canvas as a bitmap */
  tmpCanvas = cdCreateCanvas(CD_CLIPBOARD, StrData);
  if (tmpCanvas == NULL) return 0;
  cdActivate(tmpCanvas);
  SimpleDraw();
  cdKillCanvas(tmpCanvas);
  return 0;
}

int SimpleDrawClipboardMetafile(void)
{
  char StrData[100];
  int w, h;
  cdCanvas* tmpCanvas;
  cdActivate(myCanvas);
  cdGetCanvasSize(&w, &h, 0, 0);
  sprintf(StrData, "%dx%d -m", w, h);

  /* draws in the CLipboard canvas as a metafile */
  tmpCanvas = cdCreateCanvas(CD_CLIPBOARD, StrData);
  if (tmpCanvas == NULL) return 0;
  cdActivate(tmpCanvas);
  SimpleDraw();
  cdKillCanvas(tmpCanvas);
  return 0;
}

int SimpleDrawClipboardWMFEMF(void)
{
  char StrData[100];
  int w, h;
  cdCanvas* tmpCanvas;
  cdActivate(myCanvas);
  cdGetCanvasSize(&w, &h, 0, 0);
  sprintf(StrData, "%dx%d", w, h);

  /* draws in the CLipboard canvas as a metafile */
  tmpCanvas = cdCreateCanvas(CD_CLIPBOARD, StrData);
  if (tmpCanvas == NULL) return 0;
  cdActivate(tmpCanvas);
  SimpleDraw();
  cdKillCanvas(tmpCanvas);
  return 0;
}

int SimpleReplace(void)
{
  write_mode = CD_REPLACE;
  cdActivate(myCanvas);
  SimpleDraw();
  return 0;
}

int SimpleXor(void)
{
  write_mode = CD_XOR;
  cdActivate(myCanvas);
  SimpleDraw();
  return 0;
}

int SimpleNotXor(void)
{
  write_mode = CD_NOT_XOR;
  cdActivate(myCanvas);
  SimpleDraw();
  return 0;
}

int SimpleClippingOff(void)
{
  clipping = CD_CLIPOFF;
  cdActivate(myCanvas);
  SimpleDraw();
  return 0;
}

int SimpleClippingArea(void)
{
  clipping = CD_CLIPAREA;
  cdActivate(myCanvas);
  SimpleDraw();
  return 0;
}

int SimpleClippingPolygon(void)
{
  clipping = CD_CLIPPOLYGON;
  cdActivate(myCanvas);
  SimpleDraw();
  return 0;
}

int SimpleDrawImage(void)
{
  void* myImage;
  int w, h;
  cdCanvas* tmpCanvas;
  cdActivate(myCanvas);
  cdGetCanvasSize(&w, &h, 0, 0);

  myImage = cdCreateImage(w, h);
  if (myImage == NULL) return 0;

  /* draws in the Server Image canvas */
  tmpCanvas = cdCreateCanvas(CD_IMAGE, myImage);
  if (tmpCanvas == NULL)
  {
    cdKillImage(myImage);
    return 0;
  }

  cdActivate(tmpCanvas);
  SimpleDraw();
  cdKillCanvas(tmpCanvas);

  /* transfer the contents of the image to the window canvas */
  cdActivate(myCanvas);
  cdBackground(CD_WHITE);
  cdClear();
  cdPutImage(myImage, 0, 0);

  cdKillImage(myImage);
  return 0;
}

int SimpleDrawImageRGB(void)
{
  char StrData[100];
  unsigned char *r, *g, *b;
  int size, w, h;
  cdCanvas* tmpCanvas;
  cdActivate(myCanvas);
  cdGetCanvasSize(&w, &h, 0, 0);

  size = w * h;
  r = (unsigned char*)malloc(size);
  if (r == NULL) return 0;
  
  g = (unsigned char*)malloc(size);
  if (g == NULL)
  {
    free(g);
    return 0;
  }

  b = (unsigned char*)malloc(size);
  if (b == NULL) 
  {
    free(r);
    free(g);
    return 0;
  }

  sprintf(StrData, "%dx%d %p %p %p", w, h, r, g, b);

  /* draws in the Client Image canvas */
  tmpCanvas = cdCreateCanvas(CD_IMAGERGB, StrData);
  if (tmpCanvas == NULL)
  {
    free(r);
    free(g);
    free(b);
    return 0;
  }

  cdActivate(tmpCanvas);
  SimpleDraw();
  cdKillCanvas(tmpCanvas);

  /* transfer the contents of the image to the window canvas */
  cdActivate(myCanvas);
  cdBackground(CD_WHITE);
  cdClear();
  cdPutImageRGB(w, h, r, g, b, 0, 0, w, h);

  free(r);
  free(g);
  free(b);
  return 0;
}

int SimpleDrawSimulate(void)
{
  cdActivate(myCanvas);
  cdBackground(CD_WHITE);
  cdClear();

  cdSimulate(CD_SIM_ALL);

  SimpleDraw();

  cdSimulate(CD_SIM_NONE);
  
  return 0;
}

void SimpleKillCanvas(void)
{
  if (myCanvas != NULL)
  {
    cdKillCanvas(myCanvas);
    myCanvas = NULL;
  }
}


static void SimpleDraw(void)
{
  int w, h;

  cdGetCanvasSize(&w, &h, 0, 0);

  switch(clipping)
  {
  case CD_CLIPOFF:
    cdClip(CD_CLIPOFF);
    break;
  case CD_CLIPAREA:
    /* Defines the clipping area equals the canvas area minus a 100 pixels margin. */
    cdClipArea(100, w - 100, 100, h - 100);
    cdClip(CD_CLIPAREA);
    break;
  case CD_CLIPPOLYGON:
    cdBegin(CD_CLIP);
    cdVertex(100, 100);
    cdVertex(w - 100, 100);
    cdVertex(w / 2, h - 100);
    cdEnd();
    cdClip(CD_CLIPPOLYGON);
    break;
  }

  switch(write_mode)
  {
  case CD_REPLACE:
    cdWriteMode(CD_REPLACE);
    break;
  case CD_XOR:
    cdWriteMode(CD_XOR);
    break;
  case CD_NOT_XOR:
    cdWriteMode(CD_NOT_XOR);
    break;
  }

  /* Clear the background to be white */
  cdInteriorStyle(CD_SOLID);
  cdBackground(CD_WHITE);
  cdClear();
                            
  cdForeground(CD_RED);
  cdLineWidth(1);
  cdLineStyle(CD_DASHED);
  cdLine(0, 0, w, h);

  cdForeground(CD_BLUE);
  cdLineWidth(10);
  cdLineStyle(CD_DOTTED);
  cdLine(0, h, w, 0);

  cdLineStyle(CD_CONTINUOUS);
  cdLineWidth(1);

  cdForeground(CD_GREEN);
  cdArc(100, 100, 100, 100, 0, 180);
  cdSector(w - 100, 100, 100, 100, 0, 180);

  cdForeground(CD_YELLOW);
  cdBox(w/2 - 100, w/2 + 100, h/2 - 100, h/2 + 100); 

  /* transparent text */
  cdForeground(CD_BLUE);
  cdBackOpacity(CD_TRANSPARENT); 
  cdTextAlignment(CD_CENTER);
  cdFont(CD_TIMES_ROMAN, CD_BOLD, 24);
  cdText(w/2, h/2, "Simple Draw");

  /* draws all the mark type possibilities */
  cdForeground(CD_RED);
  cdMarkSize(30);
  cdMarkType(CD_PLUS);
  cdMark(200, 200);
  cdMarkType(CD_CIRCLE);
  cdMark(w - 200, 200);
  cdMarkType(CD_HOLLOW_CIRCLE);
  cdMark(200, h - 200);
  cdMarkType(CD_DIAMOND);
  cdMark(w - 200, h - 200);

  /* draws all the line style possibilities */
  cdLineWidth(1);
  cdLineStyle(CD_CONTINUOUS);
  cdLine(0, 10, w, 10);
  cdLineStyle(CD_DASHED);
  cdLine(0, 20, w, 20);
  cdLineStyle(CD_DASH_DOT);
  cdLine(0, 30, w, 30);
  cdLineStyle(CD_DASH_DOT_DOT);
  cdLine(0, 40, w, 40);

  /* draws all the hatch style possibilities */
  cdInteriorStyle(CD_HATCH);
  cdHatch(CD_VERTICAL); 
  cdBox(0, 50, h - 60, h);
  cdHatch(CD_FDIAGONAL); 
  cdBox(50, 100, h - 60, h);
  cdHatch(CD_BDIAGONAL); 
  cdBox(100, 150, h - 60, h);
  cdHatch(CD_CROSS); 
  cdBox(150, 200, h - 60, h);
  cdHatch(CD_HORIZONTAL); 
  cdBox(200, 250, h - 60, h);
  cdHatch(CD_DIAGCROSS); 
  cdBox(250, 300, h - 60, h);

  /* closed polygon */
  cdBegin(CD_CLOSED_LINES);
  cdVertex(w/2, h - 100); 
  cdVertex(w/2 + 50, h - 150); 
  cdVertex(w/2, h - 200); 
  cdVertex(w/2 - 50, h - 150); 
  cdEnd();

  /* hatch filled polygon */
  cdInteriorStyle(CD_HATCH);
  cdHatch(CD_DIAGCROSS); 

  cdBegin(CD_FILL);
  cdVertex(100, h/2); 
  cdVertex(150, h/2 + 50); 
  cdVertex(200, h/2); 
  cdVertex(150, h/2 - 50); 
  cdEnd();

  /* pattern filled polygon */
  cdInteriorStyle(CD_PATTERN);
  cdPattern(SIZE, SIZE, pattern);

  cdBegin(CD_FILL);
  cdVertex(w - 100, h/2); 
  cdVertex(w - 150, h/2 + 50); 
  cdVertex(w - 200, h/2); 
  cdVertex(w - 150, h/2 - 50); 
  cdEnd();
  
  /* stipple filled polygon */
  cdInteriorStyle(CD_STIPPLE);
  cdStipple(SIZE, SIZE, stipple);

  cdBegin(CD_FILL);
  cdVertex(w/2, 100); 
  cdVertex(w/2 + 50, 150); 
  cdVertex(w/2, 200); 
  cdVertex(w/2 - 50, 150); 
  cdEnd();


  /* always clear the image buffer contents */
  memset(red, 0xFF, 100*100);
  memset(green, 0xFF, 100*100);
  memset(blue, 0xFF, 100*100);

  cdGetImageRGB(red, green, blue, w - 200, 100, 100, 100);
  cdPutImageRGB(100, 100, red, green, blue, 100, h - 200, 100, 100);
  cdPutImageRGBA(100, 100, red, green, blue, alpha, w - 300, h - 300, 200, 200);
}

