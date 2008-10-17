# Microsoft Developer Studio Project File - Name="cdtest" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=cdtest - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "cdtest.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cdtest.mak" CFG="cdtest - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cdtest - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "cdtest - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\bin"
# PROP BASE Intermediate_Dir "..\obj\cdtest"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\bin"
# PROP Intermediate_Dir "..\obj\cdtest"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /I "..\..\include" /I "..\..\..\iup\include" /Z7 /W3 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "CDTEST_WIN32" /D "_CRT_SECURE_NO_DEPRECATE" /D "_MBCS" /Fp"..\obj/cdtest.pch" /Fo"$(IntDir)/" /Fd"..\obj/cdtest" /GZ /c /GX 
# ADD CPP /nologo /MTd /I "..\..\include" /I "..\..\..\iup\include" /Z7 /W3 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "CDTEST_WIN32" /D "_CRT_SECURE_NO_DEPRECATE" /D "_MBCS" /Fp"..\obj/cdtest.pch" /Fo"$(IntDir)/" /Fd"..\obj/cdtest" /GZ /c /GX 
# ADD BASE MTL /nologo /D"_DEBUG" /mktyplib203 /tlb"..\bin\cdtest.tlb" /win32 
# ADD MTL /nologo /D"_DEBUG" /mktyplib203 /tlb"..\bin\cdtest.tlb" /win32 
# ADD BASE RSC /l 1046 /d "_DEBUG" 
# ADD RSC /l 1046 /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib cd.lib cdiup.lib cdgdiplus.lib iup.lib gdiplus.lib cdpdflib.lib /nologo /out:"..\bin\cdtest.exe" /incremental:yes /libpath:"..\..\lib" /libpath:"..\..\..\iup\lib" /debug /pdb:"..\bin\cdtest.pdb" /pdbtype:sept /subsystem:windows /MACHINE:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib cd.lib cdiup.lib cdgdiplus.lib iup.lib gdiplus.lib cdpdflib.lib /nologo /out:"..\bin\cdtest.exe" /incremental:yes /libpath:"..\..\lib" /libpath:"..\..\..\iup\lib" /debug /pdb:"..\bin\cdtest.pdb" /pdbtype:sept /subsystem:windows /MACHINE:I386

!ENDIF

# Begin Target

# Name "cdtest - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=Cdtest.c
# End Source File
# Begin Source File

SOURCE=.\cdtest.led
# End Source File
# Begin Source File

SOURCE=cdtest.rc
# End Source File
# Begin Source File

SOURCE=cdtest_led.c
# End Source File
# Begin Source File

SOURCE=Colorbar.c
# End Source File
# Begin Source File

SOURCE=Drivers.c
# End Source File
# Begin Source File

SOURCE=List.c
# End Source File
# Begin Source File

SOURCE=Rubber.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=Cdtest.h
# End Source File
# End Group
# End Target
# End Project

