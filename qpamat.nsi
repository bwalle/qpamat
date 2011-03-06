;NSIS Modern User Interface QPaMaT installer
;Written by Bernhard Walle

;--------------------------------
;Include Modern UI

  !include "MUI.nsh"

;--------------------------------
;General

  ;Name and file
  Name "QPaMaT"
  OutFile "qpamat-setup.exe"

  ;Default installation folder
  InstallDir "$PROGRAMFILES\qpamat"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\Modern UI Test" ""

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "COPYING"
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH
  
  !insertmacro MUI_UNPAGE_WELCOME
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  !insertmacro MUI_UNPAGE_FINISH
  
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"
  !insertmacro MUI_LANGUAGE "German"

;--------------------------------
;Installer Sections

Section "Dummy Section" SecDummy

  SetOutPath "$INSTDIR\share\qpamat\doc"
  File /r build\doc\user\*
  
  SetOutPath "$INSTDIR\bin"
  File build\Release\*.exe

  SetOutPath "$INSTDIR\bin"
  File build\Release\*.dll

  
  SetOutPath "$INSTDIR\share\qpamat\dicts"
  File share\dicts\*
  
  SetOutPath "$INSTDIR\share\qpamat\translations"
  File build\*.qm
  
  ;Store installation folder
  WriteRegStr HKCU "Software\QPaMaT" "" $INSTDIR
  
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"

  ;create links
  CreateDirectory "$SMPROGRAMS\QPaMaT"
  CreateShortCut "$SMPROGRAMS\QPaMaT\QPaMaT.lnk" "$INSTDIR\bin\qpamat.exe"
  CreateShortCut "$SMPROGRAMS\QPaMaT\Uninstall QPaMaT.lnk" "$INSTDIR\uninstall.exe"

SectionEnd

;--------------------------------
;Uninstaller Section

Section "Uninstall"

  RMDir /r "$SMPROGRAMS\QPaMaT"
  RMDir /r "$INSTDIR"
  DeleteRegKey /ifempty HKCU "Software\QPaMaT"

SectionEnd
