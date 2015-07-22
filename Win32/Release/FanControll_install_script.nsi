# This installs two files, app.exe and logo.ico, creates a start menu shortcut, builds an uninstaller, and
# adds uninstall information to the registry for Add/Remove Programs
 
# To get started, put this script into a folder with the two files (app.exe, logo.ico, and license.rtf -
# You'll have to create these yourself) and run makensis on it
 
# If you change the names "app.exe", "logo.ico", or "license.rtf" you should do a search and replace - they
# show up in a few places.
# All the other settings can be tweaked by editing the !defines at the top of this script
!define APPNAME "FancontrollV2"
!define COMPANYNAME "Alia5"
!define DESCRIPTION "Fancontroller Software"
# These three must be integers
!define VERSIONMAJOR 1
!define VERSIONMINOR 1
!define VERSIONBUILD 1
# These will be displayed by the "Click here for support information" link in "Add/Remove Programs"
# It is possible to use "mailto:" links in here to open the email client
# This is the size (in kB) of all the files copied into "Program Files"
!define INSTALLSIZE 44100
 
RequestExecutionLevel admin ;Require admin rights on NT6+ (When UAC is turned on)
 
InstallDir "$PROGRAMFILES\${APPNAME}"
 
# rtf or txt file - remember if it is txt, it must be in the DOS text format (\r\n)
# This will be in the installer/uninstaller's title bar
Name "${COMPANYNAME} - ${APPNAME}"
outFile "Fancontroll-installer.exe"
 
!include LogicLib.nsh
 
# Just three pages - license agreement, install location, and installation
page directory
Page instfiles
 
!macro VerifyUserIsAdmin
UserInfo::GetAccountType
pop $0
${If} $0 != "admin" ;Require admin rights on NT4+
        messageBox mb_iconstop "Administrator rights required!"
        setErrorLevel 740 ;ERROR_ELEVATION_REQUIRED
        quit
${EndIf}
!macroend
 
function .onInit
	setShellVarContext all
	!insertmacro VerifyUserIsAdmin
functionEnd
 
section "install"
	# Files for the install directory - to build the installer, these should be in the same directory as the install script (this file)
	setOutPath $INSTDIR
	# Files added here should be removed by the uninstaller (see section "uninstall")
	file "FanControllerV2.exe"
	file "hidapi.dll"
	file "icudt54.dll"
	file "icuin54.dll"
	file "icuuc54.dll"
	file "Qt5Core.dll"
	file "Qt5Gui.dll"
	file "Qt5Widgets.dll"
	file "Qt5Network.dll"
	file "HWiNFO64.dll"
	file "readme.txt"
	file "license.txt"
	file "qt-license.txt"
	file "vc_redist_2013_x86.exe"
	file "vc_redist_2015_x86.exe"
	setOutPath $INSTDIR\platforms
	file "platforms\qwindows.dll"
	# Add any other files for the install directory (license files, app data, etc) here
 
	# Uninstaller - See function un.onInit and section "uninstall" for configuration
	writeUninstaller "$INSTDIR\uninstall.exe"
 
	# Start Menu
	createDirectory "$SMPROGRAMS\${APPNAME}"
	createShortCut "$SMPROGRAMS\${APPNAME}\${APPNAME}.lnk" "$INSTDIR\FanControllerV2.exe"
	createShortCut "$SMPROGRAMS\${APPNAME}\Uninstall.lnk" "$INSTDIR\uninstall.exe"
	
	WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Run" "FanControlV2" "$INSTDIR\FanControllerV2.exe"
 
	# Registry information for add/remove programs
	WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "DisplayName" "${APPNAME} - ${COMPANYNAME} - ${DESCRIPTION}"
	WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "UninstallString" "$\"$INSTDIR\uninstall.exe$\""
	WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "QuietUninstallString" "$\"$INSTDIR\uninstall.exe$\" /S"
	WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "InstallLocation" "$\"$INSTDIR$\""
	WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "Publisher" "$\"${COMPANYNAME}$\""
	WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "DisplayVersion" "$\"${VERSIONMAJOR}.${VERSIONMINOR}.${VERSIONBUILD}$\""
	WriteRegDWORD HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "VersionMajor" ${VERSIONMAJOR}
	WriteRegDWORD HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "VersionMinor" ${VERSIONMINOR}
	# There is no option for modifying or repairing the install
	WriteRegDWORD HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "NoModify" 1
	WriteRegDWORD HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "NoRepair" 1
	# Set the INSTALLSIZE constant (!defined at the top of this script) so Add/Remove Programs can accurately report the size
	WriteRegDWORD HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "EstimatedSize" ${INSTALLSIZE}
sectionEnd
 
# Uninstaller
 
function un.onInit
	SetShellVarContext all
 
	#Verify the uninstaller - last chance to back out
	MessageBox MB_OKCANCEL "Permanantly remove ${APPNAME}?" IDOK next
		Abort
	next:
	!insertmacro VerifyUserIsAdmin
functionEnd
 
section "uninstall"
 
	# Remove Start Menu launcher
	delete "$SMPROGRAMS\${APPNAME}\${APPNAME}.lnk"
	delete "$SMPROGRAMS\${APPNAME}\Uninstall.lnk"
	# Try to remove the Start Menu folder - this will only happen if it is empty
	rmDir "$SMPROGRAMS\${APPNAME}"
 
	# Remove files
	delete $INSTDIR\FanControllerV2.exe
	delete $INSTDIR\hidapi.dll
	delete $INSTDIR\icudt54.dll
	delete $INSTDIR\icuin54.dll
	delete $INSTDIR\icuuc54.dll
	delete $INSTDIR\Qt5Core.dll
	delete $INSTDIR\Qt5Gui.dll
	delete $INSTDIR\Qt5Widgets.dll
	delete $INSTDIR\Qt5Network.dll
	delete $INSTDIR\HWiNFO64.dll
	delete $INSTDIR\readme.txt
	delete $INSTDIR\license.txt
	delete $INSTDIR\qt-license.txt
	delete $INSTDIR\vc_redist_2013_x86.exe
	delete $INSTDIR\vc_redist_2015_x86.exe
	delete $INSTDIR\platforms\qwindows.dll
	
	delete $PROFILE\AppData\Roaming\FanControllerV2\automode.dat
	delete $PROFILE\AppData\Roaming\FanControllerV2\fansettings.dat
	delete $PROFILE\AppData\Roaming\FanControllerV2\general.dat
	
	rmDir $PROFILE\AppData\Roaming\FanControllerV2
 
	# Always delete uninstaller as the last action
	delete $INSTDIR\uninstall.exe
 
	# Try to remove the install directory - this will only happen if it is empty
	rmDir $INSTDIR\platforms
	rmDir $INSTDIR
 
	# Remove uninstaller information from the registry
	DeleteRegKey HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}"
	DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Run" "FanControlV2"
sectionEnd