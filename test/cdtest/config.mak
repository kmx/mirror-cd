APPNAME = cdtest

SRC = cdtest.c cdtest_led.c colorbar.c drivers.c list.c rubber.c

ifeq "$(TEC_SYSNAME)" "Win32"
  SRC += cdtest.rc
endif

USE_CD=Yes
USE_IUP=Yes

cdtest_led.c: cdtest.led
	ledc -f cdtest_loadled -o cdtest_led.c cdtest.led

USE_STATIC = Yes

#IUP = ../../../iup
CD = ../..

ifneq ($(findstring Win, $(TEC_SYSNAME)), )
  LIBS = cdpdflib
else
  SLIB = $(CD)/lib/$(TEC_UNAME)/libcdpdflib.a
endif
