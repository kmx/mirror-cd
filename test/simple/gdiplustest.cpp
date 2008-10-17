#include <windows.h>
#include <gdiplus.h>
using namespace Gdiplus;

/* Visual C++ 7.1 + SP1
   GDI+ 1.0 File Version 5.1.3102.2180
*/

void DrawLineMarks(Graphics* graphics, Pen* greenPen, int x, int y, int w, int h)
{
  graphics->DrawLine(greenPen, x+w-1, y-5, x+w-1, y+5); // end markers
  graphics->DrawLine(greenPen, x-5, y+h-1, x+5, y+h-1);
}

void SimpleImageTest(HWND hWnd)
{
  int x, y;
  Graphics* graphics = new Graphics(hWnd);
  graphics->Clear(Color(255, 255, 255)); // white background

  Bitmap image(16, 16, PixelFormat24bppRGB);
  image.SetResolution(graphics->GetDpiX(), graphics->GetDpiX());

  /* black pixel border */
  for (y = 0; y < 16; y++)
    image.SetPixel(0, y, Color(0, 0, 0));
  for (y = 0; y < 16; y++)
    image.SetPixel(15, y, Color(0, 0, 0));
  for (x = 1; x < 15; x++)
    image.SetPixel(x, 0, Color(0, 0, 0));
  for (x = 1; x < 15; x++)
    image.SetPixel(x, 15, Color(0, 0, 0));

  /* light yellow contents */
  for (y = 1; y < 15; y++)
    for (x = 1; x < 15; x++)
      image.SetPixel(x, y, Color(192, 192, 0));

  Pen redPen(Color(255, 0, 0), 1);
  redPen.SetDashStyle(DashStyleDash);
  Pen greenPen(Color(0, 255, 0), 1);
  greenPen.SetDashStyle(DashStyleDash);

  // I add {} to avoid reusing some Rect in the next test

  graphics->SetPixelOffsetMode(PixelOffsetModeHalf); // pixel center is (.5,.5) instead of (0, 0)

  // NO zoom
  {
  RectF actualRect(10, 10, 16, 16);
  graphics->DrawImage(&image, 10, 10);  
  graphics->DrawRectangle(&redPen, actualRect);
  DrawLineMarks(graphics, &greenPen, 10, 10, 16, 16);
  }

  // zoom using Bilinear Interpolation
  {
  RectF zoomRect(50, 10, 160, 160);
  graphics->SetInterpolationMode(InterpolationModeBilinear);
  graphics->DrawImage(&image, zoomRect);
  graphics->DrawRectangle(&redPen, zoomRect);
  DrawLineMarks(graphics, &greenPen, 50, 10, 160, 160);
  }

  // zoom using Nearest Neighborhood
  {
  RectF zoomRect2(250, 10, 160, 160);
  graphics->SetInterpolationMode(InterpolationModeNearestNeighbor);
  graphics->DrawImage(&image, zoomRect2);
  graphics->DrawRectangle(&redPen, zoomRect2);
  DrawLineMarks(graphics, &greenPen, 250, 10, 160, 160);
  }


  // Using a source image size, smaller than actual


  // NO zoom
  {
  RectF actualRect3(10, 200, 16, 16);
  graphics->DrawImage(&image, actualRect3, 0, 0, 16-1, 16-1, UnitPixel, NULL, NULL);
  graphics->DrawRectangle(&redPen, actualRect3);
  DrawLineMarks(graphics, &greenPen, 10, 400, 16, 16);
  }

  // zoom using Bilinear Interpolation
  {
  RectF zoomRect6(50, 200, 160, 160);
  graphics->SetInterpolationMode(InterpolationModeBilinear);
  graphics->DrawImage(&image, zoomRect6, 0, 0, 16-1, 16-1, UnitPixel, NULL, NULL);
  graphics->DrawRectangle(&redPen, zoomRect6);
  DrawLineMarks(graphics, &greenPen, 50, 400, 160, 160);
  }

  // zoom using Nearest Neighborhood
  {
  RectF zoomRect7(250, 200, 160, 160);
  graphics->SetInterpolationMode(InterpolationModeNearestNeighbor);
  graphics->DrawImage(&image, zoomRect7, 0, 0, 16-1, 16-1, UnitPixel, NULL, NULL);
  graphics->DrawRectangle(&redPen, zoomRect7);
  DrawLineMarks(graphics, &greenPen, 250, 400, 160, 160);
  }

  delete graphics;
}

#include <iup.h>

extern "C" char* winData;
extern "C" void SimpleDrawTest(void);

void SimpleDrawTest(void)
{
  HWND hWnd = (HWND)IupGetAttribute((Ihandle*)winData, "HWND");
  SimpleImageTest(hWnd);
}
