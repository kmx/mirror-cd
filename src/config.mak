PROJNAME = cd
LIBNAME = cd
OPT = YES   

DEFINES = CD_NO_OLD_INTERFACE

SRCSVG = base64.c lodepng.c cdsvg.c
SRCSVG := $(addprefix svg/, $(SRCSVG))

SRCINTCGM = circle.c ellipse.c intcgm1.c \
  intcgm2.c intcgm4.c intcgm6.c list.c \
  sism.c tparse.c bparse.c 
SRCINTCGM  := $(addprefix intcgm/, $(SRCINTCGM))

SRCSIM := cdfontex.c sim.c cd_truetype.c sim_other.c sim_primitives.c sim_text.c sim_linepolyfill.c
SRCSIM  := $(addprefix sim/, $(SRCSIM))

SRCWIN32 = cdwclp.c cdwemf.c cdwimg.c cdwin.c cdwnative.c cdwprn.c cdwwmf.c wmf_emf.c cdwdbuf.c cdwdib.c
SRCWIN32  := $(addprefix win32/, $(SRCWIN32))

SRCX11 = cdx11.c cdxclp.c cdximg.c cdxnative.c cdxdbuf.c xvertex.c
SRCX11  := $(addprefix x11/, $(SRCX11))

SRCGDK = cdgdk.c cdgdkclp.c cdgdkdbuf.c cdgdkimg.c cdgdknative.c
SRCGDK  := $(addprefix gdk/, $(SRCGDK))

SRCCAIRO = cdcairodbuf.c cdcairopdf.c cdcairosvg.c cdcairo.c cdcairoimg.c cdcairoplus.c cdcairoirgb.c cdcairops.c cdcaironative_gdk.c
SRCCAIRO := $(addprefix cairo/, $(SRCCAIRO))

SRCDRV = cddgn.c cdcgm.c cgm.c cddxf.c cdirgb.c cdmf.c cdps.c cdpicture.c cddebug.c
SRCDRV  := $(addprefix drv/, $(SRCDRV))

SRCNULL = cd0prn.c cd0emf.c cd0wmf.c
SRCNULL  := $(addprefix drv/, $(SRCNULL))

SRCCOMM = cd.c wd.c wdhdcpy.c rgb2map.c cd_vectortext.c cd_active.c \
          cd_attributes.c cd_bitmap.c cd_image.c cd_primitives.c cd_text.c cd_util.c
      
SRC = $(SRCCOMM) $(SRCSVG) $(SRCINTCGM) $(SRCDRV) $(SRCSIM)

ifneq ($(findstring Win, $(TEC_SYSNAME)), )
  ifdef USE_GDK
    SRC += $(SRCGDK) $(SRCNULL) $(SRCCAIRO)
    LIBNAME := cdgdk
    USE_GTK = Yes
  else
    SRC += $(SRCWIN32)
  endif
  LIBS = freetype6
else
  ifdef USE_GDK
    SRC += $(SRCGDK) $(SRCCAIRO)
    LIBNAME := cdgdk
    USE_GTK = Yes
  else
    SRC += $(SRCX11)
    USE_X11 = Yes
  endif
  SRC += $(SRCNULL)
  LIBS = freetype
endif

ifneq ($(findstring dll, $(TEC_UNAME)), )
  SRC += cd.rc
endif

LDIR = ../lib/$(TEC_UNAME)

INCLUDES = . drv x11 win32 intcgm freetype2 sim cairo ../include
