@ECHO OFF
setlocal

:: Please set the name of the application.
set APPLICATION=backupGame

:: 1 for yes, any for no.
set MAKE3DSXBACKUP=1

:: 1 for yes, any for no.
set DELETEBACKUPSFIRST=1

:: Whatever or not you wanna update DSGE
set UPDATEDSGE=0

:::::::::::::::::::::::::::::::::::::::::::::::::::
set tdsx=3dsx_backups

cd output
if %DELETEBACKUPSFIRST% EQU 1 (
    rmdir /S /Q %tdsx%
)

set /a fileCount=1
for %%f in ("build\%tdsx%\*.*") do (
    set /a fileCount=fileCount+1
)

:t
cls
ECHO ======================================================================
ECHO Building...
ECHO == COMMAND OUTPUT ====================================================

if %UPDATEDSGE% == 1 (
	REM Please update to your path please.
	REM cd C:\devkitPro\examples\3dsge
	REM python amalgamate.py -c c.json -s ""
	REM xcopy dsge.hpp "C:\devkitPro\examples\Demo\source\dsge.hpp" /Y
)

cd /d %~dp0
make
attrib +h build /s /d
:: del .smdh
cd build
if not exist %tdsx% (
    mkdir %tdsx%
)
ECHO ======================================================================
if exist output.3dsx (
    ECHO Done! Launching...
    output.3dsx

    if %MAKE3DSXBACKUP% EQU 1 (
        move output.3dsx %tdsx%\%APPLICATION%-build%fileCount%.3dsx
        set /a fileCount=%fileCount%+1
    ) else (
        del output.3dsx
    )
) else (
    ECHO Failed to build! Check Errors.
    ECHO Or check if your APPLICATION variable is the same name as the .3dsx
)
ECHO ======================================================================

pause
goto t