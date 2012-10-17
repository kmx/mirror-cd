PROJNAME = cd
LIBNAME = freetype
OPT = YES   

# Changes to freetype-2.4.8: search for CDLIB (2).

SRC  := \
  autofit/autofit.c bdf/bdf.c cff/cff.c \
  base/ftbase.c base/ftcid.c base/ftfstype.c cache/ftcache.c \
  gzip/ftgzip.c lzw/ftlzw.c gxvalid/gxvalid.c otvalid/otvalid.c pcf/pcf.c \
  pfr/pfr.c psaux/psaux.c pshinter/pshinter.c psnames/psnames.c raster/raster.c \
  sfnt/sfnt.c smooth/smooth.c truetype/truetype.c type1/type1.c cid/type1cid.c \
  type42/type42.c winfonts/winfnt.c \
  base/ftapi.c base/ftbbox.c base/ftbdf.c base/ftbitmap.c base/ftdebug.c \
  base/ftgasp.c base/ftglyph.c base/ftgxval.c base/ftinit.c base/ftlcdfil.c \
  base/ftmm.c base/ftotval.c base/ftpatent.c base/ftpfr.c base/ftstroke.c \
  base/ftsynth.c base/ftsystem.c base/fttype1.c base/ftwinfnt.c base/ftxf86.c
SRC := $(addprefix freetype2/, $(SRC))

DEFINES += FT2_BUILD_LIBRARY FT_CONFIG_OPTION_SYSTEM_ZLIB
INCLUDES = freetype2 zlib
LDIR = ../lib/$(TEC_UNAME)
LINK_ZLIB = Yes

ifneq ($(findstring dll, $(TEC_UNAME)), )
  SRC += freetype2/freetype.rc
  DEF_FILE = freetype2/freetype.def
endif

ifneq ($(findstring Win, $(TEC_SYSNAME)), )
  # To be compatible with the existing DLLs of gnuwin32
  LIBNAME = freetype6
endif
ifneq ($(findstring cygw, $(TEC_UNAME)), )
  # To be compatible with the existing DLLs of cygwin
  LIBNAME = freetype-6
endif

ifneq ($(findstring bc5, $(TEC_UNAME)), )
  FLAGS = -w-8004
endif

ifneq ($(findstring MacOS, $(TEC_UNAME)), )
  ifneq ($(TEC_SYSMINOR), 4)
    BUILD_DYLIB=Yes
  endif
  DEFINES += DARWIN_NO_CARBON
endif
