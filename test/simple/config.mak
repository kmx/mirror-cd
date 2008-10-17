APPNAME = simple

#SRC = simple.c simple_led.c iupmain.c
SRC = teste.cpp
            
#DBG = Yes
USE_CD=Yes
USE_IUP=Yes

simple_led.c: simple.led
	ledc -f simple_loadled -o simple_led.c simple.led

USE_STATIC = Yes

#IUP = ../../../iup
#CD = ../..

USE_IM = Yes

#ifneq ($(findstring Win, $(TEC_SYSNAME)), )
#  LIBS = cdpdflib cdgdiplus gdiplus
#else
#  SLIB = $(CD)/lib/$(TEC_UNAME)/libcdpdflib.a $(CD)/lib/$(TEC_UNAME)/libcdxrender.a
#  LIBS = Xrender Xft
#endif
