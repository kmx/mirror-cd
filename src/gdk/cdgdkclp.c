/** \file
 * \brief Gdk Clipboard Driver
 *
 * See Copyright Notice in cd.h
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <gtk/gtk.h>

#include "cd.h"
#include "cd_private.h"
#include "cdclipbd.h"
#include "cdmf.h"
#include "cdmf_private.h"


static void cdkillcanvas(cdCtxCanvas *ctxcanvas)
{
  char* buffer;
  long dwSize;
  FILE* file;
  char filename[10240];
  cdCanvasMF* mfcanvas = (cdCanvasMF*)ctxcanvas;
  GtkClipboard *clipboard = (GtkClipboard*)mfcanvas->data;

  /* guardar antes de remover o canvas */
  strcpy(filename, mfcanvas->filename);

  cdkillcanvasMF(mfcanvas);

  file = fopen(filename, "r");
  fseek(file, 0, SEEK_END);
  dwSize = ftell(file); 
  fseek(file, 0, SEEK_SET);

  buffer = (char*)malloc(dwSize);
  fread(buffer, dwSize, 1, file);

  fclose(file);

  remove(filename);

  gtk_clipboard_store(clipboard);
}

static int cdplay(cdCanvas* canvas, int xmin, int xmax, int ymin, int ymax, void *data)
{
  char filename[1024]; 
  int dwSize;
  FILE* file;
  GdkAtom* buffer;

  gtk_clipboard_wait_for_targets((GtkClipboard*)data, &buffer, &dwSize);
  if(!buffer)
    return CD_ERROR;

  tmpnam(filename);
  file = fopen(filename, "w");
  fwrite(buffer, dwSize, 1, file);
  fclose(file);

  cdCanvasPlay(canvas, CD_METAFILE, xmin, xmax, ymin, ymax, filename);

  remove(filename);

  g_free(buffer);

  return CD_OK;
}

static void cdcreatecanvas(cdCanvas* canvas, void *data)
{
  char tmpPath[512];
  char* str = (char*)data;
  GtkClipboard* clp = NULL;

  /* Init parameters */
  if (str == NULL) 
    return;

  sscanf(str, "%p", &clp); 

  if (!clp)
    return;

  str = strstr(str, " ");
  if (!str)
    return;

  str++;
  tmpnam(tmpPath);

  strcat(tmpPath, " ");
  strcat(tmpPath, str);

  cdcreatecanvasMF(canvas, str);
  if (!canvas->ctxcanvas)
    return;

  {
    cdCanvasMF* mfcanvas = (cdCanvasMF*)canvas->ctxcanvas;
    mfcanvas->data = clp;
  }
}

static void cdinittable(cdCanvas* canvas)
{
  cdinittableMF(canvas);
  canvas->cxKillCanvas = cdkillcanvas;
}


static cdContext cdClipboardContext =
{
  CD_CAP_ALL & ~(CD_CAP_GETIMAGERGB | CD_CAP_IMAGESRV | CD_CAP_FONTDIM | CD_CAP_TEXTSIZE),
  0,
  cdcreatecanvas,  
  cdinittable,
  cdplay,          
  NULL
};

cdContext* cdContextClipboard(void)
{
  return &cdClipboardContext;
}


