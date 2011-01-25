@echo off

setlocal EnableDelayedExpansion

REM Setting environment variables
if not defined MINGW     (set MINGW=C:\MinGW)
if not defined QTDIR     (set QTDIR=C:\Qt\4.7.1)
set PATH=%MINGW%\bin;%QTDIR%\bin;%PATH%

REM Starting the compilation process
echo "COMPILING QREGEXP-EDITOR"
md build
cd build && cmake -G "MinGW Makefiles" .. && make

endlocal