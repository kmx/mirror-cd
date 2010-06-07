PROJNAME = cd
LIBNAME = cdluaim

OPT = YES

DEF_FILE = cdluaim5.def
SRCDIR = lua5
SRC = cdluaim5.c

ifdef USE_LUA52
  LIBNAME := $(LIBNAME)52
else
  USE_LUA51 = Yes
  LIBNAME := $(LIBNAME)51
endif

USE_CD = YES
USE_CDLUA = YES
USE_IM = YES
USE_IMLUA = YES
NO_LUALINK = Yes
CD = ..
