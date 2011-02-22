@echo off

setlocal EnableDelayedExpansion

:: Variables
set MAKE=

:: Error Messages
set INVALID_PATH=The directory you entered does not exist!
set NOT_FOUND=Failed to detect:
set FOUND=Successfully detected:

:: Check for MinGW
if not defined MINGW (
	:get_mingw_path
	set /P MINGW=Please enter path to MinGW: 
	if not exist "!MINGW!" (
		echo %INVALID_PATH%
		goto get_mingw_path
	)

	if exist "!MINGW!\bin\mingw32-make.exe" (
		set MAKE="mingw32-make"
		echo %FOUND% MinGW!
		goto check_qt
	)

	if exist "!MINGW!\bin\make.exe" (
		set MAKE="make"
		echo %FOUND% MinGW!
		goto check_qt
	)

	echo %NOT_FOUND% MinGW
	goto get_mingw_path
)

:: Check for Qt
:check_qt
if not defined QTDIR (
	:get_qt_path
	set /P QTDIR=Please enter path to Qt:
	if not exist "!QTDIR!" (
		echo %INVALID_PATH%
		goto get_qt_path
	)

	if exist "!QTDIR!\bin\qmake.exe" (
		echo %NOT_FOUND% Qt!
		goto set_path
	)

	echo %NOT_FOUND% Qt!
	goto get_qt_path
)

:set_path
set PATH=%MINGW%\bin;%QTDIR%\bin;%PATH%

:: Starting the compilation process
echo "COMPILING QREGEXP-EDITOR"
md build
cd build && cmake -G "MinGW Makefiles" .. && %MAKE% && %Make% translations

endlocal
