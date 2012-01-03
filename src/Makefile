ifeq ($(OS), Windows_NT)
  TECMAKE_CMD = $(MAKE) --no-print-directory -f ../tecmakewin.mak TEC_UNAME=$(TEC_UNAME)
else
  TECMAKE_CMD = $(MAKE) --no-print-directory -f ../tecmake.mak
endif

.PHONY: do_all cd_zlib cd_freetype cd_ftgl cd cdx11 cdgdk cd_pdflib cdpdf cdgl cdcontextplus cdcairo cdlua3 cdluapdf3 cdluagl3 cdlua5 cdluapdf5 cdluagl5 cdluacontextplus5 cdluacairo5 cdluaim5
do_all: cd_zlib cd_freetype cd_ftgl cd cd_pdflib cdpdf cdgl cdlua5 cdluapdf5 cdluagl5 cdluaim5

cd_zlib:
	@$(TECMAKE_CMD) MF=cd_zlib
cd_freetype:
	@$(TECMAKE_CMD) MF=cd_freetype
cd_ftgl:
	@$(TECMAKE_CMD) MF=cd_ftgl
cd_pdflib:
	@$(TECMAKE_CMD) MF=cd_pdflib
  
cd:
	@$(TECMAKE_CMD)
cdx11:
	@$(TECMAKE_CMD) USE_X11=Yes
cdgdk:
	@$(TECMAKE_CMD) USE_GDK=Yes
  
cdpdf:
	@$(TECMAKE_CMD) MF=cdpdf
cdgl:
	@$(TECMAKE_CMD) MF=cdgl
  
cdcontextplus:
	@$(TECMAKE_CMD) MF=cdcontextplus
cdcairo:
	@$(TECMAKE_CMD) MF=cdcairo
  
cdlua3:
	@$(TECMAKE_CMD) MF=cdlua3
cdluapdf3:
	@$(TECMAKE_CMD) MF=cdluapdf3
cdluagl3:
	@$(TECMAKE_CMD) MF=cdluagl3
  
cdlua5:
	@$(TECMAKE_CMD) MF=cdlua5 USE_LUA51=Yes
	@$(TECMAKE_CMD) MF=cdlua5 USE_LUA52=Yes
cdluapdf5:
	@$(TECMAKE_CMD) MF=cdluapdf5 USE_LUA51=Yes
	@$(TECMAKE_CMD) MF=cdluapdf5 USE_LUA52=Yes
cdluagl5:
	@$(TECMAKE_CMD) MF=cdluagl5 USE_LUA51=Yes
	@$(TECMAKE_CMD) MF=cdluagl5 USE_LUA52=Yes
cdluacontextplus5:
	@$(TECMAKE_CMD) MF=cdluacontextplus5 USE_LUA51=Yes
	@$(TECMAKE_CMD) MF=cdluacontextplus5 USE_LUA52=Yes
cdluacairo5:
	@$(TECMAKE_CMD) MF=cdluacairo5 USE_LUA51=Yes
	@$(TECMAKE_CMD) MF=cdluacairo5 USE_LUA52=Yes
cdluaim5:
	@$(TECMAKE_CMD) MF=cdluaim5 USE_LUA51=Yes
	@$(TECMAKE_CMD) MF=cdluaim5 USE_LUA52=Yes
