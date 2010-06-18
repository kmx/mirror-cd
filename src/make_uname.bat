@echo off
REM This builds all the libraries of the folder for 1 uname

call tecmake %1 "MF=cd_freetype" %2 %3 %4 %5 %6 %7 %8
call tecmake %1 %2 %3 %4 %5 %6 %7 %8
call tecmake %1 "USE_GDK=Yes" %2 %3 %4 %5 %6 %7 %8
call tecmake %1 "MF=cd_pdflib" %2 %3 %4 %5 %6 %7 %8
call tecmake %1 "MF=cdpdf" %2 %3 %4 %5 %6 %7 %8
REM call tecmake %1 "MF=cdlua3" %2 %3 %4 %5 %6 %7 %8
REM call tecmake %1 "MF=cdluapdf3" %2 %3 %4 %5 %6 %7 %8
call tecmake %1 "MF=cdlua5" %2 %3 %4 %5 %6 %7 %8
call tecmake %1 "MF=cdluapdf5" %2 %3 %4 %5 %6 %7 %8
call tecmake %1 "MF=cdluaim5" %2 %3 %4 %5 %6 %7 %8

REM GDI+ and Cairo are NOT available in some compilers
REM so this may result in errors, just ignore them
call tecmake %1 "MF=cdcontextplus" %2 %3 %4 %5 %6
call tecmake %1 "MF=cdluacontextplus5" %2 %3 %4 %5 %6 %7 %8
call tecmake %1 "MF=cdcairo" %2 %3 %4 %5 %6
call tecmake %1 "MF=cdluacairo5" %2 %3 %4 %5 %6 %7 %8
