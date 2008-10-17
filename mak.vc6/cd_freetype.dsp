# Microsoft Developer Studio Project File - Name="freetype6" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=freetype6 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "freetype6.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "freetype6.mak" CFG="freetype6 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "freetype6 - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "freetype6 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "..\obj"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "..\obj"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /I "..\src\freetype2" /Z7 /W4 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "FT2_BUILD_LIBRARY" /D "_CRT_SECURE_NO_DEPRECATE" /Fp".\..\obj/freetype.pch" /Fo"$(IntDir)/" /Fd"..\obj\" /GZ /c /GX 
# ADD CPP /nologo /MT /I "..\src\freetype2" /Z7 /W4 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "FT2_BUILD_LIBRARY" /D "_CRT_SECURE_NO_DEPRECATE" /Fp".\..\obj/freetype.pch" /Fo"$(IntDir)/" /Fd"..\obj\" /GZ /c /GX 
# ADD BASE MTL /nologo /win32 
# ADD MTL /nologo /win32 
# ADD BASE RSC /l 1046 
# ADD RSC /l 1046 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\freetype6.lib" 
# ADD LIB32 /nologo /out:"..\lib\freetype6.lib" 

!ENDIF

# Begin Target

# Name "freetype6 - Win32 Debug"
# Begin Group "FreeType2"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\freetype2\autofit\autofit.c
# End Source File
# Begin Source File

SOURCE=..\src\freetype2\bdf\bdf.c
# End Source File
# Begin Source File

SOURCE=..\src\freetype2\cff\cff.c
# End Source File
# Begin Source File

SOURCE=..\src\freetype2\base\ftbase.c
# End Source File
# Begin Source File

SOURCE=..\src\freetype2\cache\ftcache.c
# End Source File
# Begin Source File

SOURCE=..\src\freetype2\gzip\ftgzip.c
# End Source File
# Begin Source File

SOURCE=..\src\freetype2\lzw\ftlzw.c
# End Source File
# Begin Source File

SOURCE=..\src\freetype2\gxvalid\gxvalid.c
# End Source File
# Begin Source File

SOURCE=..\src\freetype2\otvalid\otvalid.c
# End Source File
# Begin Source File

SOURCE=..\src\freetype2\pcf\pcf.c
# End Source File
# Begin Source File

SOURCE=..\src\freetype2\pfr\pfr.c
# End Source File
# Begin Source File

SOURCE=..\src\freetype2\psaux\psaux.c
# End Source File
# Begin Source File

SOURCE=..\src\freetype2\pshinter\pshinter.c
# End Source File
# Begin Source File

SOURCE=..\src\freetype2\psnames\psnames.c
# End Source File
# Begin Source File

SOURCE=..\src\freetype2\raster\raster.c
# End Source File
# Begin Source File

SOURCE=..\src\freetype2\sfnt\sfnt.c
# End Source File
# Begin Source File

SOURCE=..\src\freetype2\smooth\smooth.c
# End Source File
# Begin Source File

SOURCE=..\src\freetype2\truetype\truetype.c
# End Source File
# Begin Source File

SOURCE=..\src\freetype2\type1\type1.c
# End Source File
# Begin Source File

SOURCE=..\src\freetype2\cid\type1cid.c
# End Source File
# Begin Source File

SOURCE=..\src\freetype2\type42\type42.c
# End Source File
# Begin Source File

SOURCE=..\src\freetype2\winfonts\winfnt.c
# End Source File
# Begin Group "base"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\freetype2\base\ftapi.c
# End Source File
# Begin Source File

SOURCE=..\src\freetype2\base\ftbbox.c
# End Source File
# Begin Source File

SOURCE=..\src\freetype2\base\ftbdf.c
# End Source File
# Begin Source File

SOURCE=..\src\freetype2\base\ftbitmap.c
# End Source File
# Begin Source File

SOURCE=..\src\freetype2\base\ftdebug.c
# End Source File
# Begin Source File

SOURCE=..\src\freetype2\base\ftgasp.c
# End Source File
# Begin Source File

SOURCE=..\src\freetype2\base\ftglyph.c
# End Source File
# Begin Source File

SOURCE=..\src\freetype2\base\ftgxval.c
# End Source File
# Begin Source File

SOURCE=..\src\freetype2\base\ftinit.c
# End Source File
# Begin Source File

SOURCE=..\src\freetype2\base\ftlcdfil.c
# End Source File
# Begin Source File

SOURCE=..\src\freetype2\base\ftmm.c
# End Source File
# Begin Source File

SOURCE=..\src\freetype2\base\ftotval.c
# End Source File
# Begin Source File

SOURCE=..\src\freetype2\base\ftpatent.c
# End Source File
# Begin Source File

SOURCE=..\src\freetype2\base\ftpfr.c
# End Source File
# Begin Source File

SOURCE=..\src\freetype2\base\ftstroke.c
# End Source File
# Begin Source File

SOURCE=..\src\freetype2\base\ftsynth.c
# End Source File
# Begin Source File

SOURCE=..\src\freetype2\base\ftsystem.c
# End Source File
# Begin Source File

SOURCE=..\src\freetype2\base\fttype1.c
# End Source File
# Begin Source File

SOURCE=..\src\freetype2\base\ftwinfnt.c
# End Source File
# Begin Source File

SOURCE=..\src\freetype2\base\ftxf86.c
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=..\src\cd_freetype.def
# End Source File
# End Target
# End Project

