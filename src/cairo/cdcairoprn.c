/** \file
 * \brief Cairo/GTK Printer Driver  (UNIX Only)
 *
 * See Copyright Notice in cd.h
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gtk/gtkprintunixdialog.h>

#include "cdcairoctx.h"
#include "cdprint.h"



static void cdkillcanvas(cdCtxCanvas *ctxcanvas)
{
  //if(ctxcanvas->settings)
  //  g_object_unref(ctxcanvas->settings);

  gtk_print_job_send(ctxcanvas->job, NULL, NULL, NULL);

  cdcairoKillCanvas(ctxcanvas);
}

static char* get_printername_attrib(cdCtxCanvas* ctxcanvas)
{
  return (char*)gtk_printer_get_name(gtk_print_job_get_printer(ctxcanvas->job));
}

static cdAttribute printername_attrib =
{
  "PRINTERNAME",
  NULL,
  get_printername_attrib
}; 

static void cdcreatecanvas(cdCanvas* canvas, void *data)
{
  cdCtxCanvas* ctxcanvas;
  char *data_str = (char*) data;
  char docname[256] = "CD - Canvas Draw Document";
  GtkPrintUnixDialog* dialog;
  GtkPrinter* printer;
  GtkPrintSettings* settings;
  GtkPageSetup* page_setup;
  GtkPrintJob* job;
  int show_dialog = 0;

  /* Starting parameters */
  if (data_str == NULL) 
    return;

  if (data_str[0] != 0)
  {
    char *ptr = strstr(data_str, "-d");

    if (ptr != NULL)
      show_dialog = 1;

    if (data_str[0] != '-')
    {
      strcpy(docname, data_str);

      if (show_dialog)
        docname[ptr - data_str - 1] = 0;
    }
  }

  dialog = (GtkPrintUnixDialog*)gtk_print_unix_dialog_new(NULL, NULL);

  //GTK_PRINT_CAPABILITY_PAGE_SET         = 1 << 0,
  //GTK_PRINT_CAPABILITY_COPIES           = 1 << 1,
  //GTK_PRINT_CAPABILITY_COLLATE          = 1 << 2,
  //GTK_PRINT_CAPABILITY_REVERSE          = 1 << 3,
  //GTK_PRINT_CAPABILITY_SCALE            = 1 << 4,
  //GTK_PRINT_CAPABILITY_GENERATE_PDF     = 1 << 5,
  //GTK_PRINT_CAPABILITY_GENERATE_PS      = 1 << 6,
  //GTK_PRINT_CAPABILITY_PREVIEW          = 1 << 7,
  //GTK_PRINT_CAPABILITY_NUMBER_UP        = 1 << 8,
  //GTK_PRINT_CAPABILITY_NUMBER_UP_LAYOUT = 1 << 9

  gtk_print_unix_dialog_set_manual_capabilities(dialog, GTK_PRINT_CAPABILITY_GENERATE_PDF|GTK_PRINT_CAPABILITY_GENERATE_PS);

  gtk_widget_realize(GTK_WIDGET(dialog));

  if (show_dialog)
  {
    int response = gtk_dialog_run(GTK_DIALOG(dialog));

    if (response == GTK_RESPONSE_CANCEL)
    {
      gtk_widget_destroy(GTK_WIDGET(dialog));  
      return;
    }
  }

  printer = gtk_print_unix_dialog_get_selected_printer(dialog);
  settings = gtk_print_unix_dialog_get_settings(dialog);
  page_setup = gtk_print_unix_dialog_get_page_setup(dialog);

  job = gtk_print_job_new(docname, printer, settings, page_setup);

  canvas->w = (int)gtk_page_setup_get_page_width(page_setup, GTK_UNIT_PIXEL);
  canvas->h = (int)gtk_page_setup_get_page_height(page_setup, GTK_UNIT_PIXEL);
  canvas->w_mm = (int)gtk_page_setup_get_page_width(page_setup, GTK_UNIT_MM);
  canvas->h_mm = (int)gtk_page_setup_get_page_height(page_setup, GTK_UNIT_MM);
  canvas->bpp  = 24;
  canvas->xres = (double)canvas->w / canvas->w_mm;
  canvas->yres = (double)canvas->h / canvas->h_mm;

  ctxcanvas = cdcairoCreateCanvas(canvas, cairo_create(gtk_print_job_get_surface(job, NULL)));
  ctxcanvas->job = job;

  cdRegisterAttribute(canvas, &printername_attrib);

//  gtk_widget_destroy(GTK_WIDGET(dialog)); 
  //g_object_unref(settings);
}

static void cdinittable(cdCanvas* canvas)
{
  cdcairoInitTable(canvas);
  
  canvas->cxKillCanvas = cdkillcanvas;
}

static cdContext cdPrinterCairoContext =
{
  CD_CAP_ALL & ~(CD_CAP_PLAY | CD_CAP_YAXIS | CD_CAP_REGION | CD_CAP_GETIMAGERGB |
                 CD_CAP_WRITEMODE | CD_CAP_PALETTE | CD_CAP_IMAGESRV),
  0,
  cdcreatecanvas,
  cdinittable,
  NULL,
  NULL,
};

cdContext* cdContextCairoPrinter(void)
{
  return &cdPrinterCairoContext;
}
