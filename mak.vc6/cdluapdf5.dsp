# Microsoft Developer Studio Project File - Name="cdluapdf5" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=cdluapdf5 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "cdluapdf5.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cdluapdf5.mak" CFG="cdluapdf5 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cdluapdf5 - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "cdluapdf5 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "..\obj\cdluapdf5"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "..\obj\cdluapdf5"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /I "..\include" /I "..\..\lua5.1\include" /Z7 /W4 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "CD_NO_OLD_INTERFACE" /D "_CRT_SECURE_NO_DEPRECATE" /Fo"..\obj\cdluapdf5/" /Fd"..\obj\cdluapdf5/" /GZ /c /GX 
# ADD CPP /nologo /MT /I "..\include" /I "..\..\lua5.1\include" /Z7 /W4 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "CD_NO_OLD_INTERFACE" /D "_CRT_SECURE_NO_DEPRECATE" /Fo"..\obj\cdluapdf5/" /Fd"..\obj\cdluapdf5/" /GZ /c /GX 
# ADD BASE MTL /nologo /win32 
# ADD MTL /nologo /win32 
# ADD BASE RSC /l 1033 
# ADD RSC /l 1033 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\lib\cdluapdf51.lib" 
# ADD LIB32 /nologo /out:"..\lib\cdluapdf51.lib" 

!ENDIF

# Begin Target

# Name "cdluapdf5 - Win32 Debug"
# Begin Source File

SOURCE=..\include\cdluapdf.h
# End Source File
# Begin Source File

SOURCE=..\src\lua5\cdluapdf5.c
# End Source File
# Begin Source File

SOURCE=..\src\lua5\cdluapdf5.def
# End Source File
# End Target
# End Project

