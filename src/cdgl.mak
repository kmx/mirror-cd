PROJNAME = cd
LIBNAME = cdgl
OPT = YES

DEFINES = CD_NO_OLD_INTERFACE
SRC = drv/cdgl.c

INCLUDES = . sim ftgl freetype2
LIBS = ftgl
ifeq ($(findstring Win, $(TEC_SYSNAME)), )
#  LIBS += iconv
endif

USE_OPENGL = YES
USE_CD = YES
CD = ..

ifneq ($(findstring AIX, $(TEC_UNAME)), )
  DEFINES += NO_FONTCONFIG
endif

ifneq ($(findstring IRIX, $(TEC_UNAME)), )
  DEFINES += NO_FONTCONFIG
endif

ifneq ($(findstring SunOS, $(TEC_UNAME)), )
  DEFINES += NO_FONTCONFIG
endif

ifneq ($(findstring MacOS, $(TEC_UNAME)), )
  ifneq ($(TEC_SYSMINOR), 4)
    BUILD_DYLIB=Yes
  endif
endif
