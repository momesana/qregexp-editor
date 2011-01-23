; NSIS installer script for qregexp-editor
; --------------- Headers --------------
!include MUI2.nsh

; --------------- General --------------
CRCCheck force
XPStyle on
SetCompressor /FINAL /SOLID lzma

!define P "QRegExp-Editor"                    ; Program name
!define P_NORM "qregexp-editor"               ; Program name (normalized)
!define V "0.2.0.1"                           ; Program version
!define ARCH "win32"                          ; Architecture
!define ROOT_DIR "..\.."                      ; Program root directory
!define BUILD_DIR "${ROOT_DIR}\build\src"     ; Build dir
!define QT_DIR "C:\Qt\4.7.1"                  ; Qt Installation directory
!define MINGW_DIR "C:\MinGW"                  ; MinGW Installation directory

InstallDir "$PROGRAMFILES"               ; Default installation directory
Name "${P}"                                   ; Name displayed on installer
OutFile "setup-${P_NORM}-${V}-${ARCH}.exe"    ; Resulting installer filename
BrandingText /TRIMLEFT "${P_NORM}-${V}"
RequestExecutionLevel admin

; ----------- Icon and Bitmap ---------
;!define MUI_ICON install.ico                 ; TODO: find suitable icon
;!define MUI_UNICON uninstall.ico             ; TODO: find suitable icon

; -------------------------------------
!define MUI_ABORTWARNING

;------------- Language Selection Dialog Settings --------------
!define MUI_LANGDLL_REGISTRY_ROOT "HKCU" 
!define MUI_LANGDLL_REGISTRY_KEY "Software\${P}\${V}" 
!define MUI_LANGDLL_REGISTRY_VALUENAME "Installer Language"

;-------------- Install Pages -------------
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE ${ROOT_DIR}\COPYING
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
    ; These indented statements modify settings for MUI_PAGE_FINISH
    !define MUI_FINISHPAGE_NOAUTOCLOSE
    !define MUI_FINISHPAGE_RUN "$INSTDIR\${P}\${V}\bin\${P_NORM}.exe"
    !define MUI_FINISHPAGE_RUN_CHECKED
    !define MUI_FINISHPAGE_RUN_TEXT "Launch ${P}"
    !define MUI_FINISHPAGE_SHOWREADME_CHECKED
	!define MUI_FINISHPAGE_SHOWREADME "$INSTDIR\${P}\${V}\README.txt"
!insertmacro MUI_PAGE_FINISH

;-------------- Uninstall Pages -------------
!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
	; These indented statements modify settings for MUI_UNPAGE_FINISH
	!define MUI_UNFINISHPAGE_NOAUTOCLOSE
!insertmacro MUI_UNPAGE_FINISH

;--------------- Languages ---------------
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "French"
!insertmacro MUI_LANGUAGE "German"
!insertmacro MUI_LANGUAGE "Spanish"
!insertmacro MUI_LANGUAGE "SpanishInternational"
!insertmacro MUI_LANGUAGE "SimpChinese"
!insertmacro MUI_LANGUAGE "TradChinese"
!insertmacro MUI_LANGUAGE "Japanese"
!insertmacro MUI_LANGUAGE "Korean"
!insertmacro MUI_LANGUAGE "Italian"
!insertmacro MUI_LANGUAGE "Dutch"
!insertmacro MUI_LANGUAGE "Danish"
!insertmacro MUI_LANGUAGE "Swedish"
!insertmacro MUI_LANGUAGE "Norwegian"
!insertmacro MUI_LANGUAGE "NorwegianNynorsk"
!insertmacro MUI_LANGUAGE "Finnish"
!insertmacro MUI_LANGUAGE "Greek"
!insertmacro MUI_LANGUAGE "Russian"
!insertmacro MUI_LANGUAGE "Portuguese"
!insertmacro MUI_LANGUAGE "PortugueseBR"
!insertmacro MUI_LANGUAGE "Polish"
!insertmacro MUI_LANGUAGE "Ukrainian"
!insertmacro MUI_LANGUAGE "Czech"
!insertmacro MUI_LANGUAGE "Slovak"
!insertmacro MUI_LANGUAGE "Croatian"
!insertmacro MUI_LANGUAGE "Bulgarian"
!insertmacro MUI_LANGUAGE "Hungarian"
!insertmacro MUI_LANGUAGE "Thai"
!insertmacro MUI_LANGUAGE "Romanian"
!insertmacro MUI_LANGUAGE "Latvian"
!insertmacro MUI_LANGUAGE "Macedonian"
!insertmacro MUI_LANGUAGE "Estonian"
!insertmacro MUI_LANGUAGE "Turkish"
!insertmacro MUI_LANGUAGE "Lithuanian"
!insertmacro MUI_LANGUAGE "Slovenian"
!insertmacro MUI_LANGUAGE "Serbian"
!insertmacro MUI_LANGUAGE "SerbianLatin"
!insertmacro MUI_LANGUAGE "Arabic"
!insertmacro MUI_LANGUAGE "Farsi"
!insertmacro MUI_LANGUAGE "Hebrew"
!insertmacro MUI_LANGUAGE "Indonesian"
!insertmacro MUI_LANGUAGE "Mongolian"
!insertmacro MUI_LANGUAGE "Luxembourgish"
!insertmacro MUI_LANGUAGE "Albanian"
!insertmacro MUI_LANGUAGE "Breton"
!insertmacro MUI_LANGUAGE "Belarusian"
!insertmacro MUI_LANGUAGE "Icelandic"
!insertmacro MUI_LANGUAGE "Malay"
!insertmacro MUI_LANGUAGE "Bosnian"
!insertmacro MUI_LANGUAGE "Kurdish"
!insertmacro MUI_LANGUAGE "Irish"
!insertmacro MUI_LANGUAGE "Uzbek"
!insertmacro MUI_LANGUAGE "Galician"
!insertmacro MUI_LANGUAGE "Afrikaans"
!insertmacro MUI_LANGUAGE "Catalan"
!insertmacro MUI_LANGUAGE "Esperanto"

; ------------ Reserve Files ------------
!insertmacro MUI_RESERVEFILE_LANGDLL

; ------------- Installer Functions ----------
Function .onInit
  !insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

;-------------- Uninstaller Functions ------------
Function un.onInit
  !insertmacro MUI_UNGETLANGUAGE
FunctionEnd

;------------ Installer -------------
Section "" ; No components page, name is not important
SetOutPath $INSTDIR\${P}\${V}\ ; Set output path to the installation directory.
WriteUninstaller $INSTDIR\${P}\${V}\uninstall.exe ; Tell it where to put the uninstaller

; Readme, License etc.
File ${ROOT_DIR}\COPYING
File ${ROOT_DIR}\COPYING.html
File ${ROOT_DIR}\README.txt

; Bin
SetOutPath $INSTDIR\${P}\${V}\bin
File ${BUILD_DIR}\${P_NORM}.exe
File ${QT_DIR}\bin\mingwm10.dll
File ${QT_DIR}\bin\libgcc_s_dw2-1.dll
File ${MINGW_DIR}\bin\libstdc++-6.dll
File ${QT_DIR}\bin\QtCore4.dll
File ${QT_DIR}\bin\QtGui4.dll

; Shortcuts 
CreateDirectory "$SMPROGRAMS\${P}\${V}"
CreateShortCut "$SMPROGRAMS\${P}\${V}\${P_NORM}.lnk" "$INSTDIR\${P}\${V}\bin\${P_NORM}.exe"
CreateShortCut "$SMPROGRAMS\${P}\${V}\uninstall.lnk" "$INSTDIR\${P}\${V}\uninstall.exe"
SectionEnd

;------------ Uninstaller -------------
Section "uninstall"

; Bin
Delete $INSTDIR\bin\${P_NORM}.exe
Delete $INSTDIR\bin\mingwm10.dll
Delete $INSTDIR\bin\libgcc_s_dw2-1.dll
Delete $INSTDIR\bin\libstdc++-6.dll
Delete $INSTDIR\bin\QtCore4.dll
Delete $INSTDIR\bin\QtGui4.dll

RMDir  $INSTDIR\bin

; Readme, License etc.
Delete $INSTDIR\COPYING
Delete $INSTDIR\COPYING.html
Delete $INSTDIR\README.txt
Delete $INSTDIR\uninstall.exe

RMDir  $INSTDIR
RMDir  $INSTDIR\..

; Removing shortcuts
Delete "$SMPROGRAMS\${P}\${V}\${P_NORM}.lnk"
Delete "$SMPROGRAMS\${P}\${V}\uninstall.lnk"
RMDir "$SMPROGRAMS\${P}\${V}"
RMDir "$SMPROGRAMS\${P}"
SectionEnd
