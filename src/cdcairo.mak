PROJNAME = cd
LIBNAME = cdcairo
OPT = YES

DEFINES = CD_NO_OLD_INTERFACE

USE_CAIRO = Yes

INCLUDES = . cairo drv
SRCDIR = cairo
SRC = cdcairodbuf.c cdcairopdf.c cdcairosvg.c cdcairo.c cdcairoimg.c cdcairoplus.c cdcairoirgb.c cdcairops.c

# Unused here, goes inside cdgdk
# cdcaironative_gdk.c  

ifneq ($(findstring Win, $(TEC_SYSNAME)), )
  SRC += cdcaironative_win32.c
else
  SRC += cdcaironative_x11.c    
endif

USE_CD = YES
CD = ..
