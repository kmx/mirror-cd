
#include <iup.h>
#include <cd.h>

#include "simple.h"

int cmdExit(void)
{
  return IUP_CLOSE;
}

void simple_loadled (void);

int main(void)
{
  IupOpen();

  cdInitContextPlus();

  simple_loadled();

  IupSetAttribute(IupGetHandle("SimpleDialog"), "PLACEMENT", "MAXIMIZED");
  IupShow(IupGetHandle("SimpleDialog"));

  SimpleCreateCanvas((char*)IupGetHandle("SimpleCanvas"));

  IupSetFunction("cmdExit", (Icallback) cmdExit);

  IupSetFunction("SimplePlayClipboard", (Icallback) SimplePlayClipboard);
  IupSetFunction("SimplePlayCGMText", (Icallback) SimplePlayCGMText);
  IupSetFunction("SimplePlayCGMBin", (Icallback) SimplePlayCGMBin);
  IupSetFunction("SimplePlayMetafile", (Icallback) SimplePlayMetafile);
  IupSetFunction("SimplePlayWMF", (Icallback) SimplePlayWMF);
  IupSetFunction("SimplePlayEMF", (Icallback) SimplePlayEMF);

  IupSetFunction("SimpleDrawDebug", (Icallback) SimpleDrawDebug);
  IupSetFunction("SimpleDrawWindow", (Icallback) SimpleDrawWindow);
  IupSetFunction("SimpleDrawCGMText", (Icallback) SimpleDrawCGMText);
  IupSetFunction("SimpleDrawCGMBin", (Icallback) SimpleDrawCGMBin);
  IupSetFunction("SimpleDrawDXF", (Icallback) SimpleDrawDXF);
  IupSetFunction("SimpleDrawDGN", (Icallback) SimpleDrawDGN);
  IupSetFunction("SimpleDrawEMF", (Icallback) SimpleDrawEMF);
  IupSetFunction("SimpleDrawMetafile", (Icallback) SimpleDrawMetafile);
  IupSetFunction("SimpleDrawPDF", (Icallback) SimpleDrawPDF);
  IupSetFunction("SimpleDrawPS", (Icallback) SimpleDrawPS);
  IupSetFunction("SimpleDrawEPS", (Icallback) SimpleDrawEPS);
  IupSetFunction("SimpleDrawWMF", (Icallback) SimpleDrawWMF);
  IupSetFunction("SimpleDrawPrint", (Icallback) SimpleDrawPrint);
  IupSetFunction("SimpleDrawPrintDialog", (Icallback) SimpleDrawPrintDialog);
  IupSetFunction("SimpleDrawClipboardBitmap", (Icallback) SimpleDrawClipboardBitmap);
  IupSetFunction("SimpleDrawClipboardMetafile", (Icallback) SimpleDrawClipboardMetafile);
  IupSetFunction("SimpleDrawClipboardEMF", (Icallback) SimpleDrawClipboardEMF);
  IupSetFunction("SimpleDrawImage", (Icallback) SimpleDrawImage);
  IupSetFunction("SimpleDrawImageRGB", (Icallback) SimpleDrawImageRGB);
  IupSetFunction("SimpleDrawSimulate", (Icallback) SimpleDrawSimulate);

  IupSetFunction("SimpleNotXor", (Icallback) SimpleNotXor);
  IupSetFunction("SimpleXor", (Icallback) SimpleXor);
  IupSetFunction("SimpleReplace", (Icallback) SimpleReplace);
  IupSetFunction("SimpleClippingOff", (Icallback) SimpleClippingOff);
  IupSetFunction("SimpleClippingArea", (Icallback) SimpleClippingArea);
  IupSetFunction("SimpleClippingPolygon", (Icallback) SimpleClippingPolygon);
  IupSetFunction("SimpleClippingRegion", (Icallback) SimpleClippingRegion);
  IupSetFunction("SimpleContextPlus", (Icallback) SimpleContextPlus);
  IupSetFunction("SimpleTransform", (Icallback) SimpleTransform);

  IupSetFunction("SimpleDrawAll", (Icallback) SimpleDrawAll);
  IupSetFunction("SimpleDrawTextAlign", (Icallback) SimpleDrawTextAlign);
  IupSetFunction("SimpleDrawTextFonts", (Icallback) SimpleDrawTextFonts);
  IupSetFunction("SimpleDrawTest", (Icallback) SimpleDrawTest);

  IupSetFunction("SimpleRepaint", (Icallback) SimpleDrawRepaint);

  SimpleDrawWindow();

  IupMainLoop();

  SimpleKillCanvas();

  IupClose();

  return 1;
}
