# Microsoft Developer Studio Project File - Name="cd" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=cd - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "cd.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cd.mak" CFG="cd - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cd - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "cd - Win32 Debug"

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
# ADD BASE CPP /nologo /MT /I "..\include" /I "..\src" /I "..\src\drv" /I "..\src\freetype2" /I "..\src\intcgm" /I "..\src\iup" /I "..\src\sim" /Z7 /W4 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "CD_NO_OLD_INTERFACE" /D "FT2_BUILD_LIBRARY" /D "_CRT_SECURE_NO_DEPRECATE" /Fp".\..\obj/cd.pch" /Fo"$(IntDir)/" /Fd"..\obj\" /GZ /c /GX 
# ADD CPP /nologo /MT /I "..\include" /I "..\src" /I "..\src\drv" /I "..\src\freetype2" /I "..\src\intcgm" /I "..\src\iup" /I "..\src\sim" /Z7 /W4 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "CD_NO_OLD_INTERFACE" /D "FT2_BUILD_LIBRARY" /D "_CRT_SECURE_NO_DEPRECATE" /Fp".\..\obj/cd.pch" /Fo"$(IntDir)/" /Fd"..\obj\" /GZ /c /GX 
# ADD BASE MTL /nologo /win32 
# ADD MTL /nologo /win32 
# ADD BASE RSC /l 1046 
# ADD RSC /l 1046 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\cd.lib" 
# ADD LIB32 /nologo /out:"..\lib\cd.lib" 

!ENDIF

# Begin Target

# Name "cd - Win32 Debug"
# Begin Group "INTCGM"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\intcgm\bparse.c
# End Source File
# Begin Source File

SOURCE=..\src\intcgm\circle.c
# End Source File
# Begin Source File

SOURCE=..\src\intcgm\ellipse.c
# End Source File
# Begin Source File

SOURCE=..\src\intcgm\intcgm1.c
# End Source File
# Begin Source File

SOURCE=..\src\intcgm\intcgm2.c
# End Source File
# Begin Source File

SOURCE=..\src\intcgm\intcgm4.c
# End Source File
# Begin Source File

SOURCE=..\src\intcgm\intcgm6.c
# End Source File
# Begin Source File

SOURCE=..\src\intcgm\list.c
# End Source File
# Begin Source File

SOURCE=..\src\intcgm\sism.c
# End Source File
# Begin Source File

SOURCE=..\src\intcgm\tparse.c
# End Source File
# Begin Group "INC"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\intcgm\bparse.h
# End Source File
# Begin Source File

SOURCE=..\src\intcgm\circle.h
# End Source File
# Begin Source File

SOURCE=..\src\intcgm\ellipse.h
# End Source File
# Begin Source File

SOURCE=..\src\intcgm\intcgm.h
# End Source File
# Begin Source File

SOURCE=..\src\intcgm\intcgm2.h
# End Source File
# Begin Source File

SOURCE=..\src\intcgm\intcgm4.h
# End Source File
# Begin Source File

SOURCE=..\src\intcgm\intcgm6.h
# End Source File
# Begin Source File

SOURCE=..\src\intcgm\list.h
# End Source File
# Begin Source File

SOURCE=..\src\intcgm\sism.h
# End Source File
# Begin Source File

SOURCE=..\src\intcgm\tparse.h
# End Source File
# Begin Source File

SOURCE=..\src\intcgm\types.h
# End Source File
# End Group
# End Group
# Begin Group "WIN32"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\win32\cdwclp.c
# End Source File
# Begin Source File

SOURCE=..\src\win32\cdwdbuf.c
# End Source File
# Begin Source File

SOURCE=..\src\win32\cdwdib.c
# End Source File
# Begin Source File

SOURCE=..\src\win32\cdwemf.c
# End Source File
# Begin Source File

SOURCE=..\src\win32\cdwimg.c
# End Source File
# Begin Source File

SOURCE=..\src\win32\cdwin.c
# End Source File
# Begin Source File

SOURCE=..\src\win32\cdwin.h
# End Source File
# Begin Source File

SOURCE=..\src\win32\cdwnative.c
# End Source File
# Begin Source File

SOURCE=..\src\win32\cdwprn.c
# End Source File
# Begin Source File

SOURCE=..\src\win32\cdwwmf.c
# End Source File
# Begin Source File

SOURCE=..\src\win32\wmf_emf.c
# End Source File
# End Group
# Begin Group "SIM"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\sim\cd_truetype.c
# End Source File
# Begin Source File

SOURCE=..\src\sim\cdfontex.c

!IF  "$(CFG)" == "cd - Win32 Debug"

# ADD CPP /nologo /W4 /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\sim\sim.c

!IF  "$(CFG)" == "cd - Win32 Debug"

# ADD CPP /nologo /W4 /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\sim\sim.h
# End Source File
# Begin Source File

SOURCE=..\src\sim\sim_linepolyfill.c
# End Source File
# Begin Source File

SOURCE=..\src\sim\sim_other.c
# End Source File
# Begin Source File

SOURCE=..\src\sim\sim_primitives.c
# End Source File
# Begin Source File

SOURCE=..\src\sim\sim_text.c
# End Source File
# Begin Source File

SOURCE=..\src\sim\truetype.h
# End Source File
# End Group
# Begin Group "DRV"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\drv\cdcgm.c

!IF  "$(CFG)" == "cd - Win32 Debug"

# ADD CPP /nologo /W4 /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\drv\cddebug.c
# End Source File
# Begin Source File

SOURCE=..\src\drv\cddgn.c

!IF  "$(CFG)" == "cd - Win32 Debug"

# ADD CPP /nologo /W4 /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\drv\cddxf.c

!IF  "$(CFG)" == "cd - Win32 Debug"

# ADD CPP /nologo /W4 /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\drv\cdirgb.c

!IF  "$(CFG)" == "cd - Win32 Debug"

# ADD CPP /nologo /W4 /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\drv\cdmf.c

!IF  "$(CFG)" == "cd - Win32 Debug"

# ADD CPP /nologo /W4 /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\drv\cdpicture.c
# End Source File
# Begin Source File

SOURCE=..\src\drv\cdps.c

!IF  "$(CFG)" == "cd - Win32 Debug"

# ADD CPP /nologo /W4 /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\drv\cgm.c

!IF  "$(CFG)" == "cd - Win32 Debug"

# ADD CPP /nologo /W4 /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\drv\cgm.h
# End Source File
# End Group
# Begin Group "include"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\cd.h
# End Source File
# Begin Source File

SOURCE=..\include\cd_old.h
# End Source File
# Begin Source File

SOURCE=..\include\cd_private.h
# End Source File
# Begin Source File

SOURCE=..\include\cdcgm.h
# End Source File
# Begin Source File

SOURCE=..\include\cdclipbd.h
# End Source File
# Begin Source File

SOURCE=..\include\cddbuf.h
# End Source File
# Begin Source File

SOURCE=..\include\cddebug.h
# End Source File
# Begin Source File

SOURCE=..\include\cddgn.h
# End Source File
# Begin Source File

SOURCE=..\include\cddxf.h
# End Source File
# Begin Source File

SOURCE=..\include\cdemf.h
# End Source File
# Begin Source File

SOURCE=..\include\cdimage.h
# End Source File
# Begin Source File

SOURCE=..\include\cdirgb.h
# End Source File
# Begin Source File

SOURCE=..\include\cdmf.h
# End Source File
# Begin Source File

SOURCE=..\include\cdmf_private.h
# End Source File
# Begin Source File

SOURCE=..\include\cdnative.h
# End Source File
# Begin Source File

SOURCE=..\include\cdprint.h
# End Source File
# Begin Source File

SOURCE=..\include\cdps.h
# End Source File
# Begin Source File

SOURCE=..\include\cdwmf.h
# End Source File
# Begin Source File

SOURCE=..\include\wd.h
# End Source File
# Begin Source File

SOURCE=..\include\wd_old.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\src\cd.c

!IF  "$(CFG)" == "cd - Win32 Debug"

# ADD CPP /nologo /W4 /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\cd.def
# End Source File
# Begin Source File

SOURCE=..\src\cd_active.c

!IF  "$(CFG)" == "cd - Win32 Debug"

# ADD CPP /nologo /W4 /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\cd_attributes.c

!IF  "$(CFG)" == "cd - Win32 Debug"

# ADD CPP /nologo /W4 /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\cd_bitmap.c

!IF  "$(CFG)" == "cd - Win32 Debug"

# ADD CPP /nologo /W4 /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\cd_image.c

!IF  "$(CFG)" == "cd - Win32 Debug"

# ADD CPP /nologo /W4 /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\cd_primitives.c

!IF  "$(CFG)" == "cd - Win32 Debug"

# ADD CPP /nologo /W4 /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\cd_text.c

!IF  "$(CFG)" == "cd - Win32 Debug"

# ADD CPP /nologo /W4 /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\cd_util.c

!IF  "$(CFG)" == "cd - Win32 Debug"

# ADD CPP /nologo /W4 /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\cd_vectortext.c

!IF  "$(CFG)" == "cd - Win32 Debug"

# ADD CPP /nologo /W4 /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\rgb2map.c

!IF  "$(CFG)" == "cd - Win32 Debug"

# ADD CPP /nologo /W4 /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\wd.c

!IF  "$(CFG)" == "cd - Win32 Debug"

# ADD CPP /nologo /W4 /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=..\src\wdhdcpy.c

!IF  "$(CFG)" == "cd - Win32 Debug"

# ADD CPP /nologo /W4 /GZ /GX 
!ENDIF

# End Source File
# End Target
# End Project

