PROJNAME = cd
LIBNAME = cdgl
OPT = YES

DEFINES = CD_NO_OLD_INTERFACE
SRC = drv/cdgl.c

INCLUDES = . sim ftgl freetype2
LIBS = ftgl
ifeq ($(findstring Win, $(TEC_SYSNAME)), )
  LIBS += iconv
endif

USE_OPENGL = YES
USE_CD = YES
CD = ..

ifneq ($(findstring MacOS, $(TEC_UNAME)), )
  ifeq ($(TEC_SYSMINOR), 5)
    #Darwin9 Only - OpenGL bug fix for Fink, when the message bellow appears
    #   ld: cycle in dylib re-exports with /usr/X11R6/lib/libGL.dylib
    LFLAGS=-dylib_file /System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib:/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib
  endif
  
  BUILD_DYLIB=Yes
endif
