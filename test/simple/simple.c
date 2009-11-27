/*

  Simple Draw Application

  Shows the same picture on several canvas. Used to quick test the CD library and
  to demonstrate the use of CD library functions.

  This module uses only the CD library, there is another module to initialize the Window and its menus.

*/


#include "cd.h"
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
#include "cdpdf.h"
#include "cdwmf.h"
#include "cdiup.h"
#include "cddbuf.h"
#include "cddebug.h"
#include "wd.h"
#include "cdgdiplus.h"

#include "simple.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

/* Global variables */

cdCanvas *winCanvas = NULL;        /* The window drawing canvas */
char* winData = NULL;
cdCanvas *dbCanvas = NULL;        /* The double buffer canvas */
cdCanvas *curCanvas = NULL;        /* The current canvas */

int clipping = CD_CLIPOFF;                     /* Clipping flag, same as the CD */
int write_mode = CD_REPLACE;                   /* Write Mode flag, same as the CD */
int gdpiplus = 0;
int simple_draw = 0;
int use_transform = 0;
int simulate = 0;

enum {DRAW_ALL, DRAW_TEXTFONTS, DRAW_TEXTALIGN, DRAW_TEST};

#define STYLE_SIZE 10 /* A small pattern and stipple size */
long pattern[STYLE_SIZE*STYLE_SIZE];          /* Pattern buffer */
unsigned char stipple[STYLE_SIZE*STYLE_SIZE]; /* Stipple buffer */

#define IMAGE_SIZE 100
unsigned char red[IMAGE_SIZE*IMAGE_SIZE];       /* Red image buffer */
unsigned char green[IMAGE_SIZE*IMAGE_SIZE];     /* Green image buffer */
unsigned char blue[IMAGE_SIZE*IMAGE_SIZE];      /* Blue image buffer */
unsigned char alpha[IMAGE_SIZE*IMAGE_SIZE];     /* Alpha image buffer */


/* Prototype of the function that makes the drawing independent of canvas. */
void SimpleDraw(void);

void SimpleInitAlpha(int width, int height, unsigned char* _alpha)
{
  int c, l;
  /* initialize the alpha image buffer with a degrade from transparent to opaque */
  for (l = 0; l < height; l++)
    for (c = 0; c < width; c++)
      _alpha[l*width + c] = (unsigned char)((c*255)/(width-1));
}

void SimpleCreateCanvasWindow(void)
{
  /* creates the canvas based in an existing window */
  if (gdpiplus) cdUseContextPlus(1);
  winCanvas = cdCreateCanvas(CD_IUP, winData);
  if (gdpiplus) cdUseContextPlus(0);
  curCanvas = winCanvas;
}

void SimpleCreateCanvas(char* data)
{
  int c, l;

  memset(pattern, 0xFF, STYLE_SIZE*STYLE_SIZE*4);

  pattern[11] = CD_RED;   /*------------*/
  pattern[21] = CD_RED;   /*  0123456789*/               
  pattern[31] = CD_RED;   /*            */               
  pattern[41] = CD_RED;   /*9 WWWWWWWWWW*/               
  pattern[51] = CD_RED;   /*8 WWWWGGGGGW*/               
  pattern[12] = CD_RED;   /*7 WWWGGGGGBW*/                                         
  pattern[22] = CD_RED;   /*6 WWGGGGGBBW*/                                         
  pattern[32] = CD_RED;   /*5 WrrrrrBBBW*/                                         
  pattern[42] = CD_RED;   /*4 WrrrrrBBBW*/                                         
  pattern[52] = CD_RED;   /*3 WrrrrrBBWW*/                                         
  pattern[13] = CD_RED;   /*2 WrrrrrBWWW*/                                         
  pattern[23] = CD_RED;   /*1 WrrrrrWWWW*/                                         
  pattern[33] = CD_RED;   /*0 WWWWWWWWWW*/                                         
  pattern[43] = CD_RED;   /*------------*/                                         
  pattern[53] = CD_RED;                                            
  pattern[14] = CD_RED;   pattern[15] = CD_RED;
  pattern[24] = CD_RED;   pattern[25] = CD_RED;
  pattern[34] = CD_RED;   pattern[35] = CD_RED;
  pattern[44] = CD_RED;   pattern[45] = CD_RED;
  pattern[54] = CD_RED;   pattern[55] = CD_RED;
  
  pattern[26] = CD_BLUE;  pattern[37] = CD_BLUE;
  pattern[36] = CD_BLUE;  pattern[47] = CD_BLUE;
  pattern[46] = CD_BLUE;  pattern[57] = CD_BLUE;
  pattern[56] = CD_BLUE;  pattern[67] = CD_BLUE;
  
  pattern[48] = CD_BLUE;  pattern[62] = CD_GREEN;
  pattern[58] = CD_BLUE;  pattern[63] = CD_GREEN;
  pattern[68] = CD_BLUE;  pattern[64] = CD_GREEN;
  pattern[78] = CD_BLUE;  pattern[65] = CD_GREEN;
                               pattern[66] = CD_GREEN;

  pattern[73] = CD_GREEN; pattern[84] = CD_GREEN;
  pattern[74] = CD_GREEN; pattern[85] = CD_GREEN;
  pattern[75] = CD_GREEN; pattern[86] = CD_GREEN;
  pattern[76] = CD_GREEN; pattern[87] = CD_GREEN;
  pattern[77] = CD_GREEN; pattern[88] = CD_GREEN;

  /* initialize the stipple buffer with cross pattern */
  for (l = 0; l < STYLE_SIZE; l++)
    for (c = 0; c < STYLE_SIZE; c++)
      stipple[l*STYLE_SIZE + c] = (c % 4) == 0? 1: 0;

  SimpleInitAlpha(IMAGE_SIZE, IMAGE_SIZE, alpha);

  winData = data;
  SimpleCreateCanvasWindow();
  SimpleDrawWindow();
}

int SimpleTransform(void)
{
  use_transform = !use_transform;
  SimpleDrawRepaint();
  return 0;
}

int SimpleContextPlus(void)
{
#ifndef USE_GDK
  gdpiplus = !gdpiplus;
  SimpleKillCanvas();
  SimpleCreateCanvasWindow();
  SimpleDrawRepaint();
#endif
  return 0;
}

void PlayCanvasDriver(cdContext* ctx, char* StrData)
{
  int w, h;
  cdActivate(curCanvas);
  cdBackground(CD_WHITE);
  cdClear();
  cdGetCanvasSize(&w, &h, 0, 0);
  cdPlay(ctx, 100, w-100, 100, h-100, StrData);
//  cdPlay(ctx, 0, 0, 0, 0, StrData);
}

int SimplePlayClipboard(void)
{
  PlayCanvasDriver(CD_CLIPBOARD, NULL);
  return 0;
}

int SimplePlayCGMBin(void)
{
  PlayCanvasDriver(CD_CGM, "simple_b.cgm");
  return 0;
}

int SimplePlayCGMText(void)
{
  PlayCanvasDriver(CD_CGM, "simple_t.cgm");
  return 0;
}

int SimplePlayMetafile(void)
{
  PlayCanvasDriver(CD_METAFILE, "simple.mf");
  return 0;
}

int SimplePlayWMF(void)
{
  PlayCanvasDriver(CD_WMF, "simple.wmf");
  return 0;
}

int SimplePlayEMF(void)
{
  PlayCanvasDriver(CD_EMF, "simple.emf");
  return 0;
}

int SimpleDrawRepaint(void)
{
  cdActivate(curCanvas);
  SimpleDraw();
  cdFlush();
  return 0;
}

int SimpleDrawWindow(void)
{
  curCanvas = winCanvas;
  return SimpleDrawRepaint();
}

void DrawCanvasDriver(cdContext* ctx, char* StrData)
{
  cdCanvas* tmpCanvas = cdCreateCanvas(ctx, StrData);
  if (tmpCanvas == NULL) return;
  cdActivate(tmpCanvas);
  SimpleDraw();
  cdKillCanvas(tmpCanvas);
  cdActivate(curCanvas);
}

void DrawCanvasDriverSize(cdContext* ctx, char* name, int pixels)
{
  char StrData[100];
  int w, h;
  double w_mm, h_mm;
  cdActivate(curCanvas);
  cdGetCanvasSize(&w, &h, &w_mm, &h_mm);
  if (pixels)
    sprintf(StrData, "%s %dx%d", name, w, h);
  else
    sprintf(StrData, "%s %gx%g", name, w_mm, h_mm);
  DrawCanvasDriver(ctx, StrData);
}

void DrawCanvasDriverSizeParam(cdContext* ctx, char* param)
{
  char StrData[100];
  int w, h;
  cdActivate(curCanvas);
  cdGetCanvasSize(&w, &h, 0, 0);
  sprintf(StrData, "%dx%d %s", w, h, param);
  DrawCanvasDriver(ctx, StrData);
}

int SimpleDrawDebug(void)
{
  DrawCanvasDriverSize(CD_DEBUG, "simple_debug.txt", 0);
  return 0;
}

int SimpleDrawCGMText(void)
{
  DrawCanvasDriverSize(CD_CGM, "simple_t.cgm - t", 0);
  return 0;
}

int SimpleDrawCGMBin(void)
{
  DrawCanvasDriverSize(CD_CGM, "simple_b.cgm", 0);
  return 0;
}

int SimpleDrawDXF(void)
{
  DrawCanvasDriverSize(CD_DXF, "simple.dxf", 0);
  return 0;
}

int SimpleDrawDGN(void)
{
  DrawCanvasDriverSize(CD_DGN, "simple.dgn", 0);
  return 0;
}

int SimpleDrawEMF(void)
{
  if (gdpiplus) cdUseContextPlus(1);
  DrawCanvasDriverSize(CD_EMF, "simple.emf", 1);
  if (gdpiplus) cdUseContextPlus(0);
  return 0;
}

int SimpleDrawMetafile(void)
{
  DrawCanvasDriverSize(CD_METAFILE, "simple.mf", 0);
  return 0;
}

int SimpleDrawPS(void)
{
  DrawCanvasDriver(CD_PS, "simple.ps");
  return 0;
}

int SimpleDrawPDF(void)
{
  DrawCanvasDriver(CD_PDF, "simple.pdf");
  return 0;
}

int SimpleDrawEPS(void)
{
  DrawCanvasDriver(CD_PS, "simple.eps -e");
  return 0;
}

int SimpleDrawWMF(void)
{
  DrawCanvasDriverSize(CD_WMF, "simple.wmf", 1);
  return 0;
}

int SimpleDrawPrint(void)
{
  if (gdpiplus) cdUseContextPlus(1);
  DrawCanvasDriver(CD_PRINTER, "simple print");
  if (gdpiplus) cdUseContextPlus(0);
  return 0;
}

int SimpleDrawPrintDialog(void)
{
  if (gdpiplus) cdUseContextPlus(1);
  DrawCanvasDriver(CD_PRINTER, "simple -d");
  if (gdpiplus) cdUseContextPlus(0);
  return 0;
}

int SimpleDrawClipboardBitmap(void)
{
  if (gdpiplus) cdUseContextPlus(1);
  DrawCanvasDriverSizeParam(CD_CLIPBOARD, "-b");
  if (gdpiplus) cdUseContextPlus(0);
  return 0;
}

int SimpleDrawClipboardMetafile(void)
{
  if (gdpiplus) cdUseContextPlus(1);
  DrawCanvasDriverSizeParam(CD_CLIPBOARD, "-m");
  if (gdpiplus) cdUseContextPlus(0);
  return 0;
}

int SimpleDrawClipboardEMF(void)
{
  if (gdpiplus) cdUseContextPlus(1);
  DrawCanvasDriverSizeParam(CD_CLIPBOARD, "");
  if (gdpiplus) cdUseContextPlus(0);
  return 0;
}

int SimpleReplace(void)
{
  write_mode = CD_REPLACE;
  cdActivate(curCanvas);
  SimpleDrawAll();
  return 0;
}

int SimpleXor(void)
{
  write_mode = CD_XOR;
  cdActivate(curCanvas);
  SimpleDrawAll();
  return 0;
}

int SimpleNotXor(void)
{
  write_mode = CD_NOT_XOR;
  cdActivate(curCanvas);
  SimpleDrawAll();
  return 0;
}

int SimpleClippingOff(void)
{
  clipping = CD_CLIPOFF;
  cdActivate(curCanvas);
  SimpleDrawAll();
  return 0;
}

int SimpleClippingArea(void)
{
  clipping = CD_CLIPAREA;
  cdActivate(curCanvas);
  SimpleDrawAll();
  return 0;
}

int SimpleClippingPolygon(void)
{
  clipping = CD_CLIPPOLYGON;
  cdActivate(curCanvas);
  SimpleDrawAll();
  return 0;
}

int SimpleClippingRegion(void)
{
  clipping = CD_CLIPREGION;
  cdActivate(curCanvas);
  SimpleDrawAll();
  return 0;
}

void* CreateImageRGBA(int w, int h)
{
  void* myImage;
  unsigned char * _alpha = malloc(w * h);
  SimpleInitAlpha(w, h, _alpha);
  cdSetAttribute("IMAGEALPHA", (char*)_alpha);    
  cdSetAttribute("IMAGEFORMAT", "32");    // afetara� o proximo cdCreateImage
  myImage = cdCreateImage(w, h);
  cdSetAttribute("IMAGEFORMAT", NULL);    // remove o atributo para nao afetar outros cdCreateImage
  return myImage;
}

int SimpleDrawImage(void)
{
  if (dbCanvas) cdKillCanvas(dbCanvas);

  if (gdpiplus) cdUseContextPlus(1);
  dbCanvas = cdCreateCanvas(CD_DBUFFER, winCanvas);
  if (gdpiplus) cdUseContextPlus(0);

  curCanvas = dbCanvas;
  SimpleDrawRepaint();

  return 0;
}

int SimpleDrawImageRGB(void)
{
  if (dbCanvas) cdKillCanvas(dbCanvas);

  if (gdpiplus) cdUseContextPlus(1);
  dbCanvas = cdCreateCanvas(CD_DBUFFERRGB, winCanvas);
  if (gdpiplus) cdUseContextPlus(0);

  curCanvas = dbCanvas;
  SimpleDrawRepaint();

  return 0;
}

int SimpleDrawSimulate(void)
{
  cdActivate(curCanvas);

  simulate = !simulate;

  if (simulate)
    cdSimulate(CD_SIM_ALL);
  else
    cdSimulate(CD_SIM_NONE);

  SimpleDrawRepaint();

  return 0;
}

void SimpleKillCanvas(void)
{
  if (dbCanvas)
  {
    cdKillCanvas(dbCanvas);
    dbCanvas = NULL;
  }
  if (winCanvas)
  {
    cdKillCanvas(winCanvas);
    winCanvas = NULL;
  }
}

void SimpleDraw(void)
{
  if (simple_draw == DRAW_TEXTFONTS)
    SimpleDrawTextFonts();
  else if (simple_draw == DRAW_TEXTALIGN)
    SimpleDrawTextAlign();
  else if (simple_draw == DRAW_TEST)
    SimpleDrawTest();
  else
    SimpleDrawAll();
}

int SimpleDrawAll(void)
{
  int w, h;
  cdGetCanvasSize(&w, &h, 0, 0);
  
  simple_draw = DRAW_ALL;

  wdViewport(0,w-1,0,h-1);
  if (w>h)
    wdWindow(0,(double)w/(double)h,0,1);
  else
    wdWindow(0,1,0,(double)h/(double)w);

  /* Clear the background to be white */
  cdBackground(CD_WHITE);
//  cdBackground(CD_GREEN);
  cdClear();

  cdLineWidth(3);
  cdForeground(cdEncodeAlpha(CD_DARK_MAGENTA, 128));
  cdRect(100, 200, 100, 200);

  cdBegin(CD_OPEN_LINES);
  cdVertex(300, 250);
  cdVertex(320, 270);
  cdVertex(350, 260);
  cdVertex(340, 200);
  cdVertex(310, 210);
  cdEnd();
  
  cdInteriorStyle(CD_SOLID);

  cdForeground(CD_RED);
  cdLineWidth(3);
  {
    int dashes[] = {20, 15, 5, 5};
    cdLineStyleDashes(dashes, 4);
  }
  cdLineStyle(CD_CUSTOM);
  cdLine(0, 0, w-1, h-1);

  cdForeground(CD_BLUE);
  cdLineWidth(10);
  cdLineStyle(CD_DOTTED);
  //cdLine(0, 0, 500, 500);
//  wdLine(0, 1, 1, 0);
  cdLine(0, h-1, w-1, 0);

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
  case CD_CLIPREGION:
    cdTextAlignment(CD_CENTER);
    cdFont(CD_TIMES_ROMAN, CD_BOLD, 50);

    cdBegin(CD_REGION);
    cdRegionCombineMode(CD_UNION);
    cdBox(100, 200, 100, 200);
    cdSector(w/2-50, h/2+50, 150, 150, 0, 360);
    cdSector(w/2-50, h/2-50, 150, 150, 0, 360);
    cdSector(w/2+50, h/2+50, 150, 150, 0, 360);
    cdSector(w/2+50, h/2-50, 150, 150, 0, 360);
    cdRegionCombineMode(CD_DIFFERENCE); 
    cdText(w/2, h/2, "TEXT");
    cdEnd();
//    cdOffsetRegion(-50, 50);
    cdClip(CD_CLIPREGION);

    cdForeground(CD_DARK_RED);
    cdBox(0,w,0,h);
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

  if (use_transform)
  {
    cdCanvasTransform(cdActiveCanvas(), NULL);
    cdCanvasTransformTranslate(cdActiveCanvas(), w/2, h/2);
    cdCanvasTransformRotate(cdActiveCanvas(), 30);
    cdCanvasTransformScale(cdActiveCanvas(), 0.5, 0.5);
    cdCanvasTransformTranslate(cdActiveCanvas(), -w/2, -h/2);
  }

//  cdSetfAttribute("ROTATE", "15 %d %d", w/2, h/2);

  cdLineStyle(CD_CONTINUOUS);
  cdLineWidth(1);
  cdBackOpacity(CD_TRANSPARENT); 
                            
  cdForeground(CD_MAGENTA);
  cdSector(w-100, 100, 100, 100, 50, 180);
  cdForeground(CD_RED);
  cdArc(100, 100, 100, 100, 50, 180);

  cdForeground(CD_YELLOW);
  cdBox(w/2 - 100, w/2 + 100, h/2 - 100, h/2 + 100); 

  cdTextAlignment(CD_CENTER);
  cdTextOrientation(70);
  cdFont(CD_TIMES_ROMAN, CD_BOLD, 24);

  {
    int rect[8];
    cdTextBounds(w/2, h/2, "cdMin Draw (���)", rect);
    cdForeground(CD_RED);
    cdBegin(CD_CLOSED_LINES);
    cdVertex(rect[0], rect[1]);
    cdVertex(rect[2], rect[3]);
    cdVertex(rect[4], rect[5]);
    cdVertex(rect[6], rect[7]);
    cdEnd();
  }
  cdForeground(CD_BLUE);
  cdText(w/2, h/2, "cdMin Draw (���)");
  cdTextOrientation(0);

  wdBox(0.20, 0.30, 0.40, 0.50);
  cdForeground(CD_RED);
  wdLine(0.20, 0.40, 0.30, 0.50);

//  wdVectorTextDirection(0, 0, 1, 1);
  wdVectorCharSize(0.07);

//  wdVectorText(0.1, 0.4, "�� ����� ����� ����� �����");
//  wdVectorText(0.1, 0.2, "�� ����� ����� ����� �����");
  //{
  //  int i;
  //  char t[2];
  //  char s[10];
  //  int x = 20;
  //  int y = 0;
  //  t[1] = 0;
  //  for (i = 0; i < 256; i++)
  //  {
  //    int dx = 90;
  //    t[0] = (char)i;
  //    sprintf(s, "%d", i);
  //    cdText(x, y, s);
  //    cdText(x+dx, y, t);
  //    cdVectorText(x+2*dx, y, t);
  //    
  //    x += 3*dx + 2*dx/3;
  //    if ((i+1) % 7 == 0)
  //    {
  //      x = 20;
  //      y += 90;
  //    }

  //  }
  //}

  {
    double rect[8];
    cdForeground(CD_RED);
    if (gdpiplus)
      wdGetVectorTextBounds("WDj-Plus", 0.25, 0.35, rect);
    else
      wdGetVectorTextBounds("WDj", 0.25, 0.35, rect);
    cdBegin(CD_CLOSED_LINES);
    wdVertex(rect[0], rect[1]);
    wdVertex(rect[2], rect[3]);
    wdVertex(rect[4], rect[5]);
    wdVertex(rect[6], rect[7]);
    cdEnd();
  }

  cdPixel(10, h/2+0, CD_RED);
  cdPixel(11, h/2+1, CD_GREEN);
  cdPixel(12, h/2+2, CD_BLUE);

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
  cdHatch(CD_DIAGCROSS); 
  cdBegin(CD_FILL);
  cdVertex(100, h/2); 
  cdVertex(150, h/2 + 50); 
  cdVertex(200, h/2); 
  cdVertex(150, h/2 - 50); 
  cdEnd();

  /* pattern filled polygon */
  cdPattern(STYLE_SIZE, STYLE_SIZE, pattern);
  cdBegin(CD_FILL);
  cdVertex(w - 100, h/2); 
  cdVertex(w - 150, h/2 + 50); 
  cdVertex(w - 200, h/2); 
  cdVertex(w - 150, h/2 - 50); 
  cdEnd();
  
  /* stipple filled polygon */
  cdStipple(STYLE_SIZE, STYLE_SIZE, stipple);
  cdBegin(CD_FILL);
  cdVertex(w/2, 100); 
  cdVertex(w/2 + 50, 150); 
  cdVertex(w/2, 200); 
  cdVertex(w/2 - 50, 150); 
  cdEnd();

  cdBegin(CD_BEZIER);
  cdVertex(100, 100); 
  cdVertex(150, 200); 
  cdVertex(180, 250); 
  cdVertex(180, 200); 
  cdVertex(180, 150); 
  cdVertex(150, 100); 
  cdVertex(300, 100); 
  cdEnd();

  cdLineWidth(2);
  cdLineStyle(CD_CONTINUOUS);
  if (gdpiplus)
    wdVectorText(0.25, 0.35, "WDj-Plus");
  else
    wdVectorText(0.25, 0.35, "WDj");

  /* always clear the image buffer contents */
//#define IMAGE_SIZE 16
  memset(red, 0xFF, IMAGE_SIZE*IMAGE_SIZE/2);
  memset(green, 0x5F, IMAGE_SIZE*IMAGE_SIZE/2);
  memset(blue, 0x5F, IMAGE_SIZE*IMAGE_SIZE/2);
  memset(red+IMAGE_SIZE*IMAGE_SIZE/2, 0x5F, IMAGE_SIZE*IMAGE_SIZE/2);
  memset(green+IMAGE_SIZE*IMAGE_SIZE/2, 0x8F, IMAGE_SIZE*IMAGE_SIZE/2);
  memset(blue+IMAGE_SIZE*IMAGE_SIZE/2, 0x5F, IMAGE_SIZE*IMAGE_SIZE/2);
  memset(red+IMAGE_SIZE*(IMAGE_SIZE-1), 0, IMAGE_SIZE);
  memset(green+IMAGE_SIZE*(IMAGE_SIZE-1), 0, IMAGE_SIZE);
  memset(blue+IMAGE_SIZE*(IMAGE_SIZE-1), 0, IMAGE_SIZE);
  memset(red, 0, IMAGE_SIZE);
  memset(green, 0, IMAGE_SIZE);
  memset(blue, 0, IMAGE_SIZE);
  {
    int i, offset;
    for (i = 0; i < IMAGE_SIZE; i++)
    {
      offset = i*IMAGE_SIZE;
      red[offset] = 0;
      green[offset] = 0;
      blue[offset] = 0;
      red[offset+IMAGE_SIZE-1] = 0;
      green[offset+IMAGE_SIZE-1] = 0;
      blue[offset+IMAGE_SIZE-1] = 0;
    }
  }

  //cdSetAttribute("ANTIALIAS", "0");
//  cdGetImageRGB(red, green, blue, w/2 - 50, h/2-50, 100, 100);
//  cdPutImageRectRGB(14, 13, red, green, blue, -20, -15, 649, 603, 0, 13, 0, 12);
//  cdPutImageRectRGB(16, 16, red, green, blue, 10, 10, 608, 608, 5, 10, 5, 10);
//  cdPutImageRectRGB(16, 16, red, green, blue, 10, 10, 64, 64, 5, 10, 5, 10);

//  cdPutImageRGB(IMAGE_SIZE, IMAGE_SIZE, red, green, blue, 100, h - 200, IMAGE_SIZE, IMAGE_SIZE);
//  cdPutImageRGBA(IMAGE_SIZE, IMAGE_SIZE, red, green, blue, alpha, 100, h - 200, IMAGE_SIZE, IMAGE_SIZE);
  cdPutImageRGB(IMAGE_SIZE, IMAGE_SIZE, red, green, blue, w - 400, h - 310, 3*IMAGE_SIZE, 3*IMAGE_SIZE);
//  cdPutImageRGBA(IMAGE_SIZE, IMAGE_SIZE, red, green, blue, alpha, w - 400, h - 310, 3*IMAGE_SIZE, 3*IMAGE_SIZE);

  cdSetAttribute("ROTATE", NULL);
  if (use_transform)
    cdCanvasTransform(cdActiveCanvas(), NULL);
  cdClip(CD_CLIPOFF);
  cdFlush();

  return 0;
}

void DrawVectorTextBox(int x, int y, char* text)
{
  int rect[8], draw_box;

  cdLineWidth(1);
  cdLineStyle(CD_CONTINUOUS);

  draw_box = 0;
  if (draw_box)
  {
    int xmin, xmax, ymin, ymax;
    cdCanvasGetVectorTextBox(cdActiveCanvas(), x, y, text, &xmin, &xmax, &ymin, &ymax);
    cdForeground(CD_GREEN);
    cdRect(xmin, xmax, ymin, ymax);

    if (cdTextOrientation(CD_QUERY) == 0)
    {
      cdForeground(CD_RED);
      cdLine(xmin, y, xmax, y);
    }
  }
  else
  {
    /* bounding box */
    cdGetVectorTextBounds(text, x, y, rect);
    cdForeground(CD_GREEN);
    cdBegin(CD_CLOSED_LINES);
    cdVertex(rect[0], rect[1]);
    cdVertex(rect[2], rect[3]);
    cdVertex(rect[4], rect[5]);
    cdVertex(rect[6], rect[7]);
    cdEnd();
  }

  /* reference point */
  cdForeground(CD_BLUE);
  cdMarkType(CD_PLUS);
  cdMarkSize(30);
  cdMark(x, y);

  cdForeground(CD_BLACK);
  cdVectorText(x, y, text);
}

void DrawTextBox(int x, int y, char* text)
{
  int rect[8], draw_box;

  cdLineWidth(1);
  cdLineStyle(CD_CONTINUOUS);

  draw_box = 0;
  if (draw_box)
  {
    int xmin, xmax, ymin, ymax;
    cdTextBox(x, y, text, &xmin, &xmax, &ymin, &ymax);
    cdRect(xmin, xmax, ymin, ymax);

    if (cdTextOrientation(CD_QUERY) == 0)
    {
      cdForeground(CD_RED);
      cdLine(xmin, y, xmax, y);
    }
  }
  else
  {
    /* bounding box */
    cdTextBounds(x, y, text, rect);
    cdForeground(CD_GREEN);
    cdBegin(CD_CLOSED_LINES);
    cdVertex(rect[0], rect[1]);
    cdVertex(rect[2], rect[3]);
    cdVertex(rect[4], rect[5]);
    cdVertex(rect[6], rect[7]);
    cdEnd();
  }

  /* reference point */
  cdForeground(CD_BLUE);
  cdMarkType(CD_PLUS);
  cdMarkSize(30);
  cdMark(x, y);

  cdForeground(CD_BLACK);
  cdText(x, y, text);
}

int SimpleDrawTextAlign(void)
{
  int w, h, i, xoff, yoff, use_vector;

  int text_aligment[] = {
  CD_NORTH,
  CD_SOUTH,
  CD_EAST,
  CD_WEST,
  CD_NORTH_EAST,
  CD_NORTH_WEST,
  CD_SOUTH_EAST,
  CD_SOUTH_WEST,
  CD_CENTER,
  CD_BASE_CENTER,
  CD_BASE_RIGHT,
  CD_BASE_LEFT
  };

#if 1
  char* text_aligment_str[] = {
  "North (�yj)\nSecond Line (�yj)",
  "South (�yj)\nSecond Line (�yj)",
  "East (�yj)\nSecond Line (�yj)",
  "West (�yj)\nSecond Line (�yj)",
  "North East (�yj)\nSecond Line (�yj)",
  "North West (�yj)\nSecond Line (�yj)",
  "South East (�yj)\nSecond Line (�yj)",
  "South West (�yj)\nSecond Line (�yj)",
  "Center (�yj)\nSecond Line (�yj)",
  "Base Center (�yj)\nSecond Line (�yj)",
  "Base Right (�yj)\nSecond Line (�yj)",
  "Base Left (�yj)\nSecond Line (�yj)"
  };
#else
  char* text_aligment_str[] = {
  "North (�yj)",
  "South (�yj)",
  "East (�yj)",
  "West (�yj)",
  "North East (�yj)",
  "North West (�yj)",
  "South East (�yj)",
  "South West (�yj)",
  "Center (�yj)",
  "Base Center (�yj)",
  "Base Right (�yj)",
  "Base Left (�yj)"
  };
#endif

  cdGetCanvasSize(&w, &h, 0, 0);

  cdBackground(CD_WHITE);
  cdClear();

  simple_draw = DRAW_TEXTALIGN;

  use_vector = 0;

#if 0
  if (use_vector)
    cdVectorTextDirection(0, 0, 1, 1);
  else
    cdTextOrientation(45);
#endif

  xoff = w/4;
  yoff = h/7;

  if (use_vector)
    cdVectorCharSize(30);
  else
  {
    //cdFont(CD_TIMES_ROMAN, CD_PLAIN, 14);
    cdFont(CD_HELVETICA, CD_PLAIN, 24);
  }

  for (i = 0; i < 12; i++)
  {
    cdTextAlignment(text_aligment[i]);
    if (i < 6)
    {
      if (use_vector)
        DrawVectorTextBox(xoff, yoff*(i+1), text_aligment_str[i]);
      else
        DrawTextBox(xoff, yoff*(i+1), text_aligment_str[i]);
    }
    else
    {
      if (use_vector)
        DrawVectorTextBox(3*xoff, yoff*(i-5), text_aligment_str[i]);
      else
        DrawTextBox(3*xoff, yoff*(i-5), text_aligment_str[i]);
    }
  }

  cdFlush();
  return 0;
}

void DrawTextFont(int font, int size, int xoff, int yoff, char* text)
{
  cdFont(font, CD_PLAIN, size);
  DrawTextBox(xoff, yoff, text);

  cdFont(font, CD_BOLD, size);
  DrawTextBox(2*xoff, yoff, text);

  cdFont(font, CD_ITALIC, size);
  DrawTextBox(3*xoff, yoff, text);

  cdFont(font, CD_BOLD_ITALIC, size);
  DrawTextBox(4*xoff, yoff, text);
}

int SimpleDrawTextFonts(void)
{
  int xoff, yoff, size;

  cdBackground(CD_WHITE);
  cdClear();

  simple_draw = DRAW_TEXTFONTS;

  xoff = 470;
  yoff = 150;
  size = -30;

  cdTextAlignment(CD_CENTER);

  DrawTextFont(CD_COURIER, size, xoff, yoff, "Courier");
  DrawTextFont(CD_TIMES_ROMAN, size, xoff, 2*yoff, "Times Roman");
  DrawTextFont(CD_HELVETICA, size, xoff, 3*yoff, "Helvetica");
  DrawTextFont(CD_SYSTEM, size, xoff, 4*yoff, "System");

  {
//    static char native[50] = "Tecmedia, -60";
//    static char native[50] = "-*-helvetica-medium-r-*-*-8-*";
//    static char native[50] = "Edwardian Script ITC, 24";
//    cdSetAttribute("ADDFONTMAP","Edwardian Script ITC=ITCEDSCR");

//    char native[50] = "Book Antiqua, 24";
//    cdSetAttribute("ADDFONTMAP", "Book Antiqua=BKANT");

//    cdNativeFont("-d");
//    cdNativeFont(native);
//    DrawTextBox(xoff, yoff, native);
//    DrawTextBox(xoff, yoff, "The quick brown fox.");
  }

  //cdNativeFont("Tecmedia, 36");

  //cdSetAttribute("ADDFONTMAP", "WingDings=WingDing");
  //cdNativeFont("WingDings, 36");

  //cdText(500, 50, "X");
  //cdText(500, 50, "abcdefghijklmnopqrstuvwxyz");
  //cdText(500, 150, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
  //cdText(500, 250, "1234567890");
  //cdText(500, 350, "'\"!@#$%�&*()_+-=[]^/;.,");

  //cdFont(CD_COURIER, 0, 22);
  //cdText(10, 60, "abcdefghijklmnopqrstuvwxyz");
  //cdText(10, 160, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
  //cdText(10, 260, "1234567890");
  //cdText(500, 360, "'\"!@#$%�&*()_+-=[]^/;.,");

  cdFlush();
  return 0;
}

//void SimpleDrawTest(void)
void SimpleDrawMainTest(void)
{
  long pattern[16];  /* 4x4 pattern */
  int w, h;
  int xmin, xmax, ymin, ymax;

  simple_draw = DRAW_TEST;

/* notice that if we are not using world coordinates 
   it is harder to position all the objetcs we want. */
  cdGetCanvasSize(&w, &h, 0, 0);

  cdBackground(CD_WHITE);
  cdClear();

/* pattern initialization */
  pattern[0]  = CD_RED;    pattern[1]  = CD_RED;    /* first line */
  pattern[2]  = CD_YELLOW; pattern[3]  = CD_YELLOW;
  pattern[4]  = CD_RED;    pattern[5]  = CD_RED;    /* second line */
  pattern[6]  = CD_YELLOW; pattern[7]  = CD_YELLOW;
  pattern[8]  = CD_YELLOW; pattern[9]  = CD_YELLOW; /* third line */
  pattern[10] = CD_YELLOW; pattern[11] = CD_YELLOW;
  pattern[12] = CD_YELLOW; pattern[13] = CD_YELLOW; /* fourth line */
  pattern[14] = CD_YELLOW; pattern[15] = CD_YELLOW;

/* set the line attributes */
  cdLineWidth(4);
  cdLineStyle(CD_CONTINUOUS);

/* in the center draw a pattern pizza 
   with a slice mising */
  cdPattern(4, 4, pattern);
  cdSector(w/2, h/2, w/2, h/2, 45, 0);
/* draws a dark red border */
  cdForeground(CD_DARK_RED);
  cdInteriorStyle(CD_HOLLOW);
  cdSector(w/2, h/2, w/2, h/2, 45, 0);

/* on the left a red hash diamond */
/* notice the the default back opacity is transparent
   and the pattern of the sector will still be visible
   inside the hatch where the two objects intersect */
  cdForeground(CD_RED);
  cdHatch(CD_DIAGCROSS); 
  cdBegin(CD_FILL);
  cdVertex(w/4, h/4); 
  cdVertex(w/2-w/8, h/2); 
  cdVertex(w/4, 3*h/4); 
  cdVertex(w/8, h/2); 
  cdEnd();

/* draws a blue roof.*/
  cdForeground(CD_BLUE);
  cdLine(w/8, h/2, w/4, 3*h/4);
  cdLine(w/4, 3*h/4, w/2-w/8, h/2);

/* draws a dashed ribbon on the right 
   with a custom color */
  cdForeground(cdEncodeColor(100, 25, 200));
  cdLineStyle(CD_DASH_DOT);
  cdBegin(CD_BEZIER);
  cdVertex(3*w/4-20, h/2-50); 
  cdVertex(3*w/4+150, 3*h/4-50); 
  cdVertex(3*w/4-150, 3*h/4-50); 
  cdVertex(3*w/4+20, h/2-50); 
  cdEnd();

  cdFont(CD_HELVETICA, CD_BOLD, 40);
  cdTextAlignment(CD_CENTER);
  cdText(w/2, h/4-50, "Canvas Draw");
  cdTextBox(w/2, h/4-50, "Canvas Draw", &xmin, &xmax, &ymin, &ymax);
  cdRect(xmin, xmax, ymin, ymax);
  cdFlush();
}

void draw_wd(void)
{
  char* text;
  double x, y;
  double rect[8];

  cdBackground(CD_WHITE);
  cdClear();
  cdLineStyle(CD_CONTINUOUS);
  cdLineWidth(1);

//  wdVectorTextDirection(0, 0, 1, 1);
  cdTextAlignment(CD_NORTH_WEST);

//  text = "Vector Text";
  text = "Vector Text\nSecond Line\nThird Line";
  x = 0.25;
  y = 0.40;

  cdForeground(CD_BLACK);
  wdLine(0, 0, 1, 1);
  wdLine(0, 1, 1, 0);

  cdForeground(CD_GREEN);
  cdMarkType(CD_STAR);
  wdMark(x, y);

  cdForeground(CD_BLUE);
  wdVectorCharSize(0.1);
  wdVectorText(x, y, text);

  cdForeground(CD_RED);
  wdGetVectorTextBounds(text, x, y, rect);
  cdBegin(CD_CLOSED_LINES);
  wdVertex(rect[0], rect[1]);
  wdVertex(rect[2], rect[3]);
  wdVertex(rect[4], rect[5]);
  wdVertex(rect[6], rect[7]);
  cdEnd();

  cdFlush();
}

//void SimpleDrawTest(void)
void SimpleDrawTestHardCopy(void)
{
  int w, h;
  cdGetCanvasSize(&w, &h, 0, 0);
  
  simple_draw = DRAW_ALL;

  wdViewport(0,w-1,0,h-1);
  if (w>h)
    wdWindow(0,(double)w/(double)h,0,1);
  else
    wdWindow(0,1,0,(double)h/(double)w);

  draw_wd();

  //wdHardcopy(CD_CLIPBOARD, "800x600", cdActiveCanvas(), draw_wd );
  //cdFlush();
}

//void SimpleDrawTest(void)
void SimpleDrawTestImageRGB(void)
{
  int size = 2048*2048;
  unsigned char *red, *green, *blue;
  cdCanvas* canvas = cdCreateCanvas(CD_IMAGERGB, "2048x2048");
  cdActivate(canvas);

  simple_draw = DRAW_TEST;

  red = calloc(size, 1);
  green = calloc(size, 1);
  blue = calloc(size, 1);

  cdPutImageRectRGB(2048, 2048, red, green, blue, 0, 3, 2048, 2017, 0, 2047, 3, 2020);

  free(red);
  free(green);
  free(blue);

  cdKillCanvas(canvas);
  cdFlush();
}

//void SimpleDrawTest(void)
void SimpleDrawVectorFont(void)
{
  simple_draw = DRAW_TEST;

  cdBackground(CD_WHITE);
  cdClear();
  cdLineStyle(CD_CONTINUOUS);
  cdLineWidth(1);

//  wdVectorText(0.1, 0.4, "���� ����� ����� ����� �����");
//  wdVectorText(0.1, 0.2, "���� ����� ����� ����� ����� ");
  cdVectorFont("../etc/vectorfont26.txt"); /* original Simplex II */
  {
    int i;
    char t[2];
    char s[10];
    int x = 10;
    int y = 600;
    t[1] = 0;
  cdFont(CD_COURIER, CD_BOLD, 14);
  cdVectorCharSize(25);
    for (i = 128; i < 256; i++)
    {
      int dx = 30;
      t[0] = (char)i;
      sprintf(s, "%3d", i);
      cdForeground(CD_DARK_RED);
      cdText(x, y, s);
//      cdText(x+dx, y, t);
      cdForeground(CD_BLACK);
      cdVectorText(x+2*dx-10, y, t);
      
      x += 3*dx;
      if ((i+1) % 8 == 0)
      {
        x = 10;
        y -= 30;
      }
    }
  //cdFont(CD_TIMES_ROMAN, CD_PLAIN, 24);
  //cdVectorCharSize(24);
  //  for (i = 192; i < 256; i++)
  //  {
  //    int dx = 92;
  //    t[0] = (char)i;
  //    sprintf(s, "%d", i);
  //    cdText(x, y, s);
  //    cdText(x+dx, y, t);
  //    cdVectorText(x+2*dx, y, t);
  //    
  //    x += 3*dx + 2*dx/3;
  //    if ((i+1) % 4 == 0)
  //    {
  //      x = 30;
  //      y += 52;
  //    }
  //  }
  }
  cdFlush();
}

void SimpleDrawTest(void)
//void SimpleDrawPoly(void)
{
  int w, h;
  cdGetCanvasSize(&w, &h, 0, 0);

  simple_draw = DRAW_TEST;

  cdBackground(CD_WHITE);
  cdClear();

  cdInteriorStyle(CD_SOLID);
  cdBegin(CD_FILL);
  cdVertex(w/4, h/4); 
  cdVertex(w/2-w/8, h/4); 
  cdVertex(w/2, h/2); 
  cdVertex(w/2-w/8, h/2); 

  cdEnd();
}
