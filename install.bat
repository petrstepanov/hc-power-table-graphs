mkdir _vsbuild
cd _vsbuild
cmake -DPORTABLE_INSTALL:BOOL=ON ../
msbuild PowerGraphs.sln /p:Configuration=Release
msbuild INSTALL.vcxproj /p:Configuration=Release

@echo off
:: Define shortcut properties
set "ShortCutName=PowerGraphs"
set "TargetFile=%USERPROFILE%\AppData\Local\%ShortCutName%\%ShortCutName%.exe"
set "ShortcutPath=%USERPROFILE%\Desktop\%ShortCutName%.lnk"
set "IconLocation=%USERPROFILE%\AppData\Local\%ShortCutName%\%ShortCutName%.ico"

:: Create and execute temporary VBScript
echo Set oWS = WScript.CreateObject("WScript.Shell") > CreateShortcut.vbs
echo Set oLink = oWS.CreateShortcut("%ShortcutPath%") >> CreateShortcut.vbs
echo oLink.TargetPath = "%TargetFile%" >> CreateShortcut.vbs
echo oLink.IconLocation = "%IconLocation%"  >> CreateShortcut.vbs
echo oLink.Save >> CreateShortcut.vbs
cscript /nologo CreateShortcut.vbs
echo Shortcut created: %ShortcutPath%
del CreateShortcut.vbs

set "ShortcutPath=%OneDrive%\Desktop\%ShortCutName%.lnk"

:: Create and execute temporary VBScript
echo Set oWS = WScript.CreateObject("WScript.Shell") > CreateShortcut2.vbs
echo Set oLink = oWS.CreateShortcut("%ShortcutPath%") >> CreateShortcut2.vbs
echo oLink.TargetPath = "%TargetFile%" >> CreateShortcut2.vbs
echo oLink.IconLocation = "%IconLocation%"  >> CreateShortcut2.vbs
echo oLink.Save >> CreateShortcut2.vbs
cscript /nologo CreateShortcut2.vbs
echo Shortcut created: %ShortcutPath%
del CreateShortcut2.vbs
::pause

cd ..