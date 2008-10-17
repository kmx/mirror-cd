# Microsoft Developer Studio Project File - Name="cdgdiplus" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=cdgdiplus - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "cdgdiplus.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cdgdiplus.mak" CFG="cdgdiplus - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cdgdiplus - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "cdgdiplus - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "..\obj\cdgdiplus"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "..\obj\cdgdiplus"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /I "..\include" /I "..\src" /I "..\src\gdiplus" /I "..\src\drv" /Z7 /W4 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "CD_NO_OLD_INTERFACE" /D "_CRT_SECURE_NO_DEPRECATE" /Fp"..\obj\gdiplus/cdgdiplus.pch" /Fo"$(IntDir)/" /Fd"..\obj\cdgdiplus/" /GZ /c /GX 
# ADD CPP /nologo /MT /I "..\include" /I "..\src" /I "..\src\gdiplus" /I "..\src\drv" /Z7 /W4 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "CD_NO_OLD_INTERFACE" /D "_CRT_SECURE_NO_DEPRECATE" /Fp"..\obj\gdiplus/cdgdiplus.pch" /Fo"$(IntDir)/" /Fd"..\obj\cdgdiplus/" /GZ /c /GX 
# ADD BASE MTL /nologo /win32 
# ADD MTL /nologo /win32 
# ADD BASE RSC /l 1046 
# ADD RSC /l 1046 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\cdgdiplus.lib" 
# ADD LIB32 /nologo /out:"..\lib\cdgdiplus.lib" 

!ENDIF

# Begin Target

# Name "cdgdiplus - Win32 Debug"
# Begin Source File

SOURCE=..\include\cdgdiplus.h
# End Source File
# Begin Source File

SOURCE=..\src\gdiplus\cdwclpp.cpp
# End Source File
# Begin Source File

SOURCE=..\src\gdiplus\cdwdbufp.cpp
# End Source File
# Begin Source File

SOURCE=..\src\gdiplus\cdwemfp.cpp
# End Source File
# Begin Source File

SOURCE=..\src\gdiplus\cdwgdiplus.c
# End Source File
# Begin Source File

SOURCE=..\src\gdiplus\cdwimgp.cpp
# End Source File
# Begin Source File

SOURCE=..\src\gdiplus\cdwinp.cpp
# End Source File
# Begin Source File

SOURCE=..\src\gdiplus\cdwinp.h
# End Source File
# Begin Source File

SOURCE=..\src\gdiplus\cdwnativep.cpp
# End Source File
# Begin Source File

SOURCE=..\src\gdiplus\cdwprnp.cpp
# End Source File
# End Target
# End Project

