@echo off

REM exit code 0 : success
REM exit code 1 : steam not found in reg
REM exit code 2 : steamVR vrpathreg not found
REM exit code 3 : hvr driver file holder not found
REM exit code 4 : vrpathreg failed

setlocal EnableDelayedExpansion

set ErrorCode=

:prepare

set OHVR_STEAM_INSTALL_PATH=

for /f "tokens=1,2,* delims= " %%i in ('%SystemRoot%\System32\reg.exe QUERY "HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Valve\Steam" /v "InstallPath"') do (
	if not '%%k'=='' set OHVR_STEAM_INSTALL_PATH=%%k && goto init
)

if '%OHVR_STEAM_INSTALL_PATH%'=='' (
	for /f "tokens=1,2,* delims= " %%i in ('%SystemRoot%\System32\reg.exe QUERY "HKEY_LOCAL_MACHINE\SOFTWARE\Valve\Steam" /v "InstallPath"') do if not '%%k'=='' set OHVR_STEAM_INSTALL_PATH=%%k && goto init
)

echo steam not found in reg
REM exit /b 1
set ErrorCode=1
goto err

:init
if "%OHVR_STEAM_INSTALL_PATH:~-1%"==" " set "OHVR_STEAM_INSTALL_PATH=%OHVR_STEAM_INSTALL_PATH:~0,-1%" && goto init

:process
echo Registering...
set OHVR_Driver_Name=sample
cd /d %~dp0
set SteamPath=%OHVR_STEAM_INSTALL_PATH:"=%\steamapps\common\SteamVR\bin\win32
set Driver_Output=%cd%\%OHVR_Driver_Name%
echo Driver_Output = %Driver_Output%

if not exist "%Driver_Output%" (
	echo hvr driver file holder not found
	REM exit /b 3
	set ErrorCode=3
	goto err
)

echo Looking for SteamVR vrpathreg.exe from '%SteamPath%'
if exist "%SteamPath%\vrpathreg.exe" (
	goto handle
)
echo Not Found for SteamVR vrpathreg.exe from '%SteamPath%', try other steam library folder

REM check all steam libraryfolders to find steamVR
set libraryfoldersVdfPath=%OHVR_STEAM_INSTALL_PATH:"=%\steamapps\libraryfolders.vdf
REM echo %libraryfoldersVdfPath%
for /f "eol=} skip=4 tokens=1,2* usebackq" %%i in ("%libraryfoldersVdfPath%") do (
	set steamLibrary=%%j
	set steamLibrary=!steamLibrary:~1,-1!
	echo check steam library : !steamLibrary!\steamapps\common\SteamVR\bin\win32\vrpathreg.exe
	if exist "!steamLibrary!\steamapps\common\SteamVR\bin\win32\vrpathreg.exe" (
		set SteamPath=!steamLibrary!\steamapps\common\SteamVR\bin\win32
		goto handle
	)
)
echo steamVR vrpathreg not found
REM exit /b 2
set ErrorCode=2
goto err

:handle
echo steamVR vrpathreg found
pushd "%SteamPath%"
echo %SteamPath%\vrpathreg.exe
vrpathreg.exe show

echo unregister exist hvr driver
for /f "skip=4 delims=	" %%i in ('vrpathreg.exe show') do (
	echo exist register path "%%i"
	set removePath=%%i
	REM echo "!removePath!"
:removeSpaceBefore
	REM echo "!removePath:~0,1!"
	if "!removePath:~0,1!" EQU " " set "removePath=!removePath:~1!" && goto removeSpaceBefore
	if "!removePath:~0,1!" EQU "	" set "removePath=!removePath:~1!" && goto removeSpaceBefore
	echo "!removePath!"
:removeSpaceAfter
	REM echo "!removePath:~-1!"
	if "!removePath:~-1!" EQU " " set "removePath=!removePath:~0,-1!" && goto removeSpaceAfter
	if "!removePath:~-1!" EQU "	" set "removePath=!removePath:~0,-1!" && goto removeSpaceAfter
	echo removePath = "!removePath!"
	
	if "!removePath:~-15,15!" EQU "\%OHVR_Driver_Name%" echo remove "!removePath!" & vrpathreg removedriver "!removePath!"
	REM clear hvr too
	if "!removePath:~-4,4!" EQU "\hvr" echo remove "!removePath!" & vrpathreg removedriver "!removePath!"
)

vrpathreg adddriver "%Driver_Output%"
echo run vrpathreg return %ERRORLEVEL%
if %ERRORLEVEL% EQU 0 goto succ
popd
echo vrpathreg failed
set ErrorCode=4
REM exit /b 4
goto err 

:err
echo Failed!
if '%1'=='PauseWhenError' pause
REM Wait 2 senconds.
if '%1'=='Wait2sToShowResult' ping -n 3 127.0.0.1>nul
goto end

:succ
vrpathreg show
popd
echo Success!
set ErrorCode=0
REM Wait 2 senconds.
if '%1'=='Wait2sToShowResult' ping -n 3 127.0.0.1>nul
goto end

:end
echo %ErrorCode%
endlocal
exit /b %ErrorCode%
echo on
