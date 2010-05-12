/** \file
 * \brief Cairo extra drivers.
 *        Rendering PDF, PS, SVG and IMAGERGB.
 *
 * See Copyright Notice in cd.h
 */

#ifndef __CD_CAIRO_H
#define __CD_CAIRO_H

#ifdef __cplusplus
extern "C" {
#endif

/* Some of these context can be used directly or by cdInitContextPlus,
   as  CD_NATIVEWINDOW, CD_IMAGE and CD_DBUFFER.
   The other only directly. 
*/

cdContext* cdContextCairoPS(void);
cdContext* cdContextCairoPDF(void);
cdContext* cdContextCairoSVG(void);
cdContext* cdContextCairoImageRGB(void);
cdContext* cdContextCairoDBuffer(void);
cdContext* cdContextCairoImage(void);
cdContext* cdContextCairoNativeWindow(void);

#define CD_CAIRO_NATIVEWINDOW cdContextCairoNativeWindow()
#define CD_CAIRO_IMAGE cdContextCairoImage()
#define CD_CAIRO_DBUFFER cdContextCairoDBuffer()
#define CD_CAIRO_PS cdContextCairoPS()
#define CD_CAIRO_PDF cdContextCairoPDF()
#define CD_CAIRO_SVG cdContextCairoSVG()
#define CD_CAIRO_IMAGERGB cdContextCairoImageRGB()


#ifdef __cplusplus
}
#endif

#endif /* ifndef __CD_CAIRO_ */
