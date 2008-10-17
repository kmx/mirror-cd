#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <io.h>

#include "iup.h"
#include "cd.h"
#include "cdiup.h"
#include "cdirgb.h"

/***************************************************************************
*  callbacks do IUP                                                        *
****************************************************************************/
int Draw( Ihandle *dial )
{
	cdCanvas *cnv = (cdCanvas *) IupGetAttribute(dial, "CDCANVAS");
	cdCanvas *imgCnv = (cdCanvas *) IupGetAttribute(dial, "IMGCNV");

	if (cnv==NULL)
		return IUP_DEFAULT;

	int width, height;
	cdCanvasGetSize(cnv, &width, &height, NULL, NULL);

	cdCanvasInteriorStyle(cnv, CD_SOLID);
	cdCanvasForeground(cnv, CD_WHITE);
	cdCanvasBox(cnv, 0, width, 0, height);

	if (imgCnv==NULL)
		return IUP_DEFAULT;

	int w, h;
	cdCanvasGetSize(imgCnv, &w, &h, NULL, NULL);

	unsigned char* red = cdRedImage(imgCnv);
	unsigned char* green = cdGreenImage(imgCnv);
	unsigned char* blue = cdBlueImage(imgCnv);

	int dx = width - w;
	int dy = height - h;

	cdCanvasPutImageRectRGB(cnv, w, h, red, green, blue, (int)(dx / 2.0), (int)(dy / 2.0), 0, 0, 0, 0, 0, 0);

	return IUP_DEFAULT;
}

int repaint (Ihandle *self, float x, float y)
{
 Ihandle *dial = (Ihandle *) IupGetDialog(self);

 cdCanvas *cnv = (cdCanvas *) IupGetAttribute(dial, "CDCANVAS");

 Draw(dial);

 return IUP_DEFAULT;
}

static int Resize (Ihandle *self, int larg, int alt)
{
 Ihandle *dial = IupGetDialog(self);

 if (dial==NULL) return IUP_DEFAULT;

 cdCanvas *cnv = (cdCanvas *) IupGetAttribute(dial, "CDCANVAS");

 if (cnv!=NULL)
	cdCanvasActivate(cnv);

 Draw(dial);

 return IUP_DEFAULT;
}

static int doexit(Ihandle *self)
{
 return IUP_CLOSE;
}

cdCanvas* buildBackImage(Ihandle* dial)
{
	unsigned char* r = (unsigned char*) IupGetAttribute(dial, "BUFFRED");
	unsigned char* g = (unsigned char*) IupGetAttribute(dial, "BUFFGREEN");
	unsigned char* b = (unsigned char*) IupGetAttribute(dial, "BUFFBLUE");

	cdCanvas* imgCnv = (cdCanvas*) IupGetAttribute(dial, "IMGCNV");

	if (imgCnv!=NULL)
		delete imgCnv;

	if (r!=NULL)
		delete r;
	if (g!=NULL)
		delete g;
	if (b!=NULL)
		delete b;

	int w = 200;
	int h = 200;

	int size = w * h;

	r = new unsigned char[size];
	g = new unsigned char[size];
	b = new unsigned char[size];

	char data[100];
	sprintf(data, "%dx%d %p %p %p", w, h, r, g, b);
	imgCnv = cdCreateCanvas(CD_IMAGERGB, data);

	IupSetAttribute(dial, "BUFFRED", (char*) r);
	IupSetAttribute(dial, "BUFFGREEN", (char*) g);
	IupSetAttribute(dial, "BUFFBLUE", (char*) b);

	IupSetAttribute(dial, "IMGCNV", (char*) imgCnv);

	return imgCnv;
}

void drawInImage(cdCanvas* cnv)
{
	int w, h;
	cdCanvasGetSize(cnv, &w, &h, NULL, NULL);

	cdCanvasBegin(cnv, CD_CLIP);
#if 0
	cdCanvasVertex(cnv, 0, 0);
	cdCanvasVertex(cnv, w, 0);
	cdCanvasVertex(cnv, w, h);
	cdCanvasVertex(cnv, 0, h);
#else
	cdCanvasVertex(cnv, -100, -100);
	cdCanvasVertex(cnv, w+100, -100);
	cdCanvasVertex(cnv, w+100, h+100);
	cdCanvasVertex(cnv, -100, h+100);
#endif
	cdCanvasEnd(cnv);

	cdCanvasClip(cnv, CD_CLIPPOLYGON);

	cdCanvasInteriorStyle(cnv, CD_SOLID);
	cdCanvasForeground(cnv, CD_WHITE);
	cdCanvasBox(cnv, 0, w, 0, h);

	cdCanvasForeground(cnv, CD_RED);
	cdCanvasTextAlignment(cnv, CD_CENTER);
	cdCanvasText(cnv, w/2, h/2, "SCURI É BOIOLA!");
}

static int tofly(Ihandle *self)
{
 Ihandle *dial = IupGetDialog(self);

 cdCanvas* back =  buildBackImage(dial);

 if (back!=NULL)
	drawInImage(back);

 Draw(dial);

 return IUP_DEFAULT;
}

Ihandle *BuildDialog(void)
{
 Ihandle *saida = IupItem ("Saida", "acao_sai");
 Ihandle *voe = IupItem ("Voe", "acao_voar");
 Ihandle *main_menu = IupMenu ( saida, voe, NULL );

 Ihandle *tela = IupCanvas("rpaint");
 IupSetAttribute(tela, IUP_BUTTON_CB, "buttonCb");
 IupSetAttribute(tela, IUP_MOTION_CB, "motionCb");

 Ihandle *hbox = IupHbox ( tela, NULL );

 Ihandle *dial = IupDialog ( hbox );

 IupSetHandle( "main_menu", main_menu );
 IupSetHandle( "tela", tela );
 IupSetAttribute( dial, IUP_MENU, "main_menu" );
 IupSetAttribute( dial, IUP_SIZE, "HALFxHALF" );
 IupSetAttribute( tela, IUP_SIZE, "300x200" );
 IupSetAttribute( tela, IUP_RESIZE_CB, "resize_cb");

 IupSetFunction( "rpaint", (Icallback) repaint );
 IupSetFunction( "acao_sai", (Icallback) doexit );
 IupSetFunction( "acao_voar", (Icallback) tofly );
 IupSetFunction( "resize_cb", (Icallback) Resize );

 IupMap(dial);

 return dial;
}

int main (void)
{
 Ihandle *dial;

 /* Abre IUP */
 IupOpen();   

 dial = BuildDialog();

 cdCanvas *cnv = cdCreateCanvas( CD_IUP, IupGetHandle("tela") );

 IupSetAttribute(dial, "CDCANVAS", (char *) cnv);

 /* Exibe dialogo */
 IupShow(dial);

 /* Interage com o usuario */
 IupMainLoop();

 cdKillCanvas( cnv );

 /* Fecha IUP */
 IupClose();

 return 1;
}
