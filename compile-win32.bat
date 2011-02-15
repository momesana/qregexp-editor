@echo off

setlocal EnableDelayedExpansion

:: Setting environment variables
if not defined MINGW (
	:get_mingw_path
	set /P MINGW=Please enter path to MinGW: 
	if not exist "!MINGW!" (
		echo The directory you entered does not exist!
		goto get_mingw_path
	)
)

if not defined QTDIR (
	:get_qt_path
	set /P QTDIR=Please enter path to QT: 
	if not exist "!QTDIR!" (
		echo The directory you entered does not exist!
		goto get_qt_path
	)
)

set PATH=%MINGW%\bin;%QTDIR%\bin;%PATH%

:: Starting the compilation process
echo "COMPILING QREGEXP-EDITOR"
md build
cd build && cmake -G "MinGW Makefiles" .. && make

endlocal