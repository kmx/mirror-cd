# Microsoft Developer Studio Project File - Name="cdpdf" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=cdpdf - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "cdpdf.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cdpdf.mak" CFG="cdpdf - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cdpdf - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "cdpdf - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "..\obj\cdpdf"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "..\obj\cdpdf"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /I "..\include" /I "..\src" /I "..\src\sim" /I "..\src\pdflib\pdflib" /Z7 /W4 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "CD_NO_OLD_INTERFACE" /D "_CRT_SECURE_NO_DEPRECATE" /Fp"..\obj\cdpdf/cdpdf.pch" /Fo"$(IntDir)/" /Fd"..\obj\cdpdf/" /GZ /c /GX 
# ADD CPP /nologo /MT /I "..\include" /I "..\src" /I "..\src\sim" /I "..\src\pdflib\pdflib" /Z7 /W4 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "CD_NO_OLD_INTERFACE" /D "_CRT_SECURE_NO_DEPRECATE" /Fp"..\obj\cdpdf/cdpdf.pch" /Fo"$(IntDir)/" /Fd"..\obj\cdpdf/" /GZ /c /GX 
# ADD BASE MTL /nologo /win32 
# ADD MTL /nologo /win32 
# ADD BASE RSC /l 1046 
# ADD RSC /l 1046 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\cdpdflib.lib" 
# ADD LIB32 /nologo /out:"..\lib\cdpdflib.lib" 

!ENDIF

# Begin Target

# Name "cdpdf - Win32 Debug"
# Begin Source File

SOURCE=..\src\drv\cdpdf.c
# End Source File
# Begin Source File

SOURCE=..\src\cdpdf.def
# End Source File
# Begin Source File

SOURCE=..\include\cdpdf.h
# End Source File
# End Target
# End Project

