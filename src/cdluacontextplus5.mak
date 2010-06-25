PROJNAME = cd
LIBNAME = cdluacontextplus

OPT = YES

DEFINES = CD_NO_OLD_INTERFACE
SRCDIR = lua5
SRC = cdluacontextplus5.c
DEF_FILE = cdluacontextplus5.def

ifeq ($(findstring Win, $(TEC_SYSNAME)), )
  # Check for XRender availability if NOT Windows
  CHECK_XRENDER = Yes
endif

LIBS = cdcontextplus

ifdef USE_LUA52
  LIBNAME := $(LIBNAME)52
else
  USE_LUA51 = Yes
  LIBNAME := $(LIBNAME)51
endif

NO_LUALINK = Yes
USE_CD = YES
USE_CDLUA = YES
CD = ..
