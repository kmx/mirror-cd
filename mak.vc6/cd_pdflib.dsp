# Microsoft Developer Studio Project File - Name="pdflib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=pdflib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "pdflib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "pdflib.mak" CFG="pdflib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "pdflib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "pdflib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "..\obj\pdflib"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "..\obj\pdflib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /I "..\src\pdflib\font" /I "..\src\pdflib\flate" /I "..\src\pdflib\pdcore" /I "..\src\pdflib\pdflib" /Z7 /W3 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "NO_vsnprintf" /Fp"..\obj\pdflib/pdflib.pch" /Fo"$(IntDir)/" /Fd"..\obj\pdflib/" /GZ /c /GX 
# ADD CPP /nologo /MT /I "..\src\pdflib\font" /I "..\src\pdflib\flate" /I "..\src\pdflib\pdcore" /I "..\src\pdflib\pdflib" /Z7 /W3 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "NO_vsnprintf" /Fp"..\obj\pdflib/pdflib.pch" /Fo"$(IntDir)/" /Fd"..\obj\pdflib/" /GZ /c /GX 
# ADD BASE MTL /nologo /win32 
# ADD MTL /nologo /win32 
# ADD BASE RSC /l 1046 
# ADD RSC /l 1046 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\pdflib.lib" 
# ADD LIB32 /nologo /out:"..\lib\pdflib.lib" 

!ENDIF

# Begin Target

# Name "pdflib - Win32 Debug"
# Begin Group "pdflib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\pdflib\pdflib\pdflib.c
# End Source File
# Begin Group "flate"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\pdflib\flate\adler32.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\flate\compress.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\flate\crc32.c

!IF  "$(CFG)" == "pdflib - Win32 Debug"

# ADD CPP /nologo /Z7 /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\pdflib\flate\deflate.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\flate\inffast.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\flate\inflate.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\flate\inftrees.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\flate\trees.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\flate\uncompr.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\flate\zutil.c
# End Source File
# Begin Group "inc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\pdflib\flate\crc32.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\flate\deflate.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\flate\inffast.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\flate\inffixed.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\flate\inflate.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\flate\inftrees.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\flate\trees.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\flate\zconf.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\flate\zlib.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\flate\zprefix.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\flate\zutil.h
# End Source File
# End Group
# End Group
# Begin Group "font"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\pdflib\font\ft_cid.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\font\ft_corefont.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\font\ft_font.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\font\ft_hostfont.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\font\ft_pdffont.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\font\ft_truetype.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\font\ft_type1.c
# End Source File
# Begin Group "inc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\pdflib\font\ft_cid.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\font\ft_corefont.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\font\ft_font.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\font\ft_generr.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\font\ft_pdffont.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\font\ft_truetype.h
# End Source File
# End Group
# End Group
# Begin Group "pdcore"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_aes.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_aescbc.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_arc4.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_chartabs.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_contain.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_core.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_crypt.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_ctype.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_digsig.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_ebcdic.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_encoding.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_file.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_geom.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_md5.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_optparse.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_output.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_resource.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_scan.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_scope.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_string.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_unicode.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_util.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_xmp.c
# End Source File
# Begin Group "inc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_aes.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_aeslocal.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_arc4.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_chartabs.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_classic.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_config.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_contain.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_core.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_crypt.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_ctype.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_digsig.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_ebcdic.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_encoding.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_exports.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_file.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_generr.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_geom.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_md5.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_optparse.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_output.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_prefix.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_pstok.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_resource.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_scan.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_scantok.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_scope.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_strconst.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_string.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_unicode.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_util.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdcore\pc_xmp.h
# End Source File
# End Group
# End Group
# Begin Group "pdflib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_actions.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_afm.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_annots.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_bmp.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_ccitt.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_cid.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_color.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_document.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_draw.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_encoding.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_filter.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_font.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_gif.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_gstate.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_hyper.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_icclib.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_image.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_jpeg.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_jpx.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_kerning.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_mbox.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_object.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_opi.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_page.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_params.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_pattern.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_pfm.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_photoshp.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_png.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_shading.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_subsett.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_tagged.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_template.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_text.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_tiff.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_truetype.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_type1.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_type3.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_util.c
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_xgstate.c
# End Source File
# Begin Group "inc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_color.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_defopt.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_font.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_generr.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_hkscmyk.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_hkslab.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_icc.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_icc9809.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_icclib.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_image.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_intern.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_keyconn.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_layer.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_page.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_pantlab.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_params.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\p_tagged.h
# End Source File
# Begin Source File

SOURCE=..\src\pdflib\pdflib\pdflib.h
# End Source File
# End Group
# End Group
# End Group
# Begin Source File

SOURCE=..\src\cd_pdflib.def
# End Source File
# End Target
# End Project

