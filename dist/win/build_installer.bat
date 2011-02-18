@echo off

setlocal EnableDelayedExpansion

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
		echo %FOUND% MinGW!
		goto check_qt
	)
	
	if exist "!MINGW!\bin\make.exe" (
		echo %FOUND% MinGW!
		goto check_qt
	)
	
	echo %NOT_FOUND% MinGW!
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
		echo %FOUND% Qt!
		goto check_arch
	)

	echo %NOT_FOUND% Qt!
	goto get_qt_path
)

:check_arch
if not defined ARCH (
	set /P ARCH="Please either enter Architecture (32 or 64) or press enter for default (32)": 
	if not defined ARCH (
		set ARCH=32
		echo Set to default: 32
	)
)

if not defined VERSION (
	if exist ..\..\build\VERSION_FILE (
		echo "file exists"
		set /P VERSION=<..\..\build\VERSION_FILE
	)
	
	if not defined VERSION (
		:get_version_number
		set /P VERSION=Please enter the version number to be used: 
		if not defined VERSION (
			echo You didn't enter a valid version number
			goto get_version_number
		)
	)
)

:: Starting the compilation process
echo "BUILDING INSTALLER"
makensis.exe qregexp-editor.nsi

endlocal
