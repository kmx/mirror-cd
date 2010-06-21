PROJNAME = cd
LIBNAME = cdgl
OPT = YES

DEFINES = CD_NO_OLD_INTERFACE
SRC = drv/cdgl.c

INCLUDES = . sim ftgl freetype2
LIBS = ftgl
ifneq ($(findstring Win, $(TEC_SYSNAME)), )
  LIBS += iconv
endif

USE_OPENGL = YES
USE_CD = YES
CD = ..
