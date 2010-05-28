APPNAME = simple

ifdef USE_GDK
  APPNAME = simplegdk
  USE_GTK = Yes
else
  DEFINES = USE_CONTEXTPLUS
endif

SRC = simple.c simple_led.c iupmain.c
            
#DBG = Yes
USE_CD=Yes
USE_IUP3=Yes

ifeq "$(TEC_SYSNAME)" "Win32"
  LEDC = $(IUP)/bin/$(TEC_SYSNAME)/ledc
else  
  LEDC = $(IUP)/bin/$(TEC_UNAME)/ledc
endif

simple_led.c: simple.led
	$(LEDC) -f simple_loadled -o simple_led.c simple.led

USE_STATIC = Yes

#IUP = ../../../iup
#CD = ../..

#USE_IM = Yes

ifneq ($(findstring Win, $(TEC_SYSNAME)), )
  LIBS = cdpdf pdflib
  ifndef USE_GDK
    LIBS += cdcontextplus gdiplus
  endif
else
  ifdef DBG_DIR
    CDLIB = $(CD)/lib/$(TEC_UNAME)d
  else
    CDLIB = $(CD)/lib/$(TEC_UNAME)
  endif  

  SLIB = $(CDLIB)/libcdpdf.a $(CDLIB)/libpdflib.a 
  ifndef USE_GDK
    SLIB += $(CDLIB)/libcdcontextplus.a
    LIBS = Xrender Xft
  else  
  endif
endif
