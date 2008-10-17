# Microsoft Developer Studio Project File - Name="simple" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=simple - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "simple.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "simple.mak" CFG="simple - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "simple - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "simple - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\bin"
# PROP BASE Intermediate_Dir "..\obj\simple"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\bin"
# PROP Intermediate_Dir "..\obj\simple"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /I "..\..\include" /I "..\..\..\iup\include" /Z7 /W4 /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "__CD__" /D "simple" /D "_CRT_SECURE_NO_DEPRECATE" /D "_MBCS" /Fp".\..\obj\simple/simple.pch" /Fo".\..\obj\simple/" /Fd".\..\obj\simple/" /GZ /c /GX 
# ADD CPP /nologo /MT /I "..\..\include" /I "..\..\..\iup\include" /Z7 /W4 /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "__CD__" /D "simple" /D "_CRT_SECURE_NO_DEPRECATE" /D "_MBCS" /Fp".\..\obj\simple/simple.pch" /Fo".\..\obj\simple/" /Fd".\..\obj\simple/" /GZ /c /GX 
# ADD BASE MTL /nologo /tlb".\..\bin\simple.tlb" /win32 
# ADD MTL /nologo /tlb".\..\bin\simple.tlb" /win32 
# ADD BASE RSC /l 1046 /d "_DEBUG" 
# ADD RSC /l 1046 /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib cd.lib cdiup.lib iup.lib comctl32.lib cdgdiplus.lib gdiplus.lib cdpdflib.lib iupcontrols.lib /nologo /out:"..\bin\simple.exe" /incremental:yes /libpath:"..\..\lib" /libpath:"..\..\..\iup\lib" /debug /pdb:".\..\bin\simple.pdb" /pdbtype:sept /subsystem:windows /MACHINE:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib cd.lib cdiup.lib iup.lib comctl32.lib cdgdiplus.lib gdiplus.lib cdpdflib.lib iupcontrols.lib /nologo /out:"..\bin\simple.exe" /incremental:yes /libpath:"..\..\lib" /libpath:"..\..\..\iup\lib" /debug /pdb:".\..\bin\simple.pdb" /pdbtype:sept /subsystem:windows /MACHINE:I386

!ENDIF

# Begin Target

# Name "simple - Win32 Debug"
# Begin Source File

SOURCE=iupmain.c
# End Source File
# Begin Source File

SOURCE=simple.c
# End Source File
# Begin Source File

SOURCE=simple.h
# End Source File
# Begin Source File

SOURCE=simple.led
# End Source File
# Begin Source File

SOURCE=simple_led.c
# End Source File
# End Target
# End Project

