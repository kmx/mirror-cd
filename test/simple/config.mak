APPNAME = simple

SRC = simple.c simple_led.c iupmain.c
            
DBG = Yes
USE_CD=Yes
USE_IUP3=Yes

simple_led.c: simple.led
	ledc -f simple_loadled -o simple_led.c simple.led

USE_STATIC = Yes

#IUP = ../../../iup
#CD = ../..

#USE_IM = Yes

ifneq ($(findstring Win, $(TEC_SYSNAME)), )
  LIBS = cdpdf pdflib cdcontextplus gdiplus
else
  SLIB = $(CD)/lib/$(TEC_UNAME)/libcdpdf.a $(CD)/lib/$(TEC_UNAME)/libpdflib.a $(CD)/lib/$(TEC_UNAME)/libcdcontextplus.a
  LIBS = Xrender Xft
endif
