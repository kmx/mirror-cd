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

  ctx_list[CD_CTX_NATIVEWINDOW] = cdContextCairoNativeWindow();
  ctx_list[CD_CTX_IMAGE] = cdContextCairoImage();
  ctx_list[CD_CTX_DBUFFER] = cdContextCairoDBuffer();
#ifdef WIN32
//  ctx_list[CD_CTX_PRINTER] = cdContextCairoPrinter();
#endif

  cdInitContextPlusList(ctx_list);
}
