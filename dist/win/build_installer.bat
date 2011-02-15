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

if not defined ARCH (
	set /P ARCH="Please either enter Architecture (32 or 64) or press enter for default (32)": 
	if not defined ARCH (
		set ARCH=32
		echo Set to default: 32
	)
)

if not defined VERSION (
	if exist ..\..\build\VERSION-FILE (
		echo "file exists"
		set /P VERSION=<..\..\build\VERSION-FILE
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
