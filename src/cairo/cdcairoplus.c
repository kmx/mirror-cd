/** \file
 * \brief Cairo as Context Plus
 *
 * See Copyright Notice in cd.h
 */
 
#include "cd.h"
#include "cd_private.h"
#include "cdcairo.h"
#include <stdlib.h>
#include <memory.h>


void cdInitContextPlus(void)
{
  cdContext* ctx_list[NUM_CONTEXTPLUS];
  memset(ctx_list, 0, sizeof(ctx_list));

  ctx_list[CD_CTXPLUS_NATIVEWINDOW] = cdContextCairoNativeWindow();
  ctx_list[CD_CTXPLUS_IMAGE] = cdContextCairoImage();
  ctx_list[CD_CTXPLUS_DBUFFER] = cdContextCairoDBuffer();
#ifndef CAIRO_X11
  ctx_list[CD_CTXPLUS_PRINTER] = cdContextCairoPrinter();
#endif
#ifdef WIN32
  ctx_list[CD_CTXPLUS_EMF] = cdContextCairoEMF();
#endif

  cdInitContextPlusList(ctx_list);
}
