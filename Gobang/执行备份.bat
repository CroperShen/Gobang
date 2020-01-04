@echo off
set LibDir=C:\Users\Administrator\source\repos\MyLib\
set NumofBackups=10

call:update "cprlib.h"
call:update "linkedlist.h"
call:update "ChessAI.h"
call:update "ChessAI2.h"

pause
goto:eof

:update
call:isnew "%~1" , " %LibDir%%~1"
if %ans% equ 2  copy  %LibDir%%~1 %~1 >nul
if %ans% equ 2  echo 已从库中更新%~1
if %ans% equ 1 call:backupfile %LibDir%%~1 %~1
if %ans% equ 1 copy  %~1 %LibDir%%~1 >nul
if %ans% equ 1 echo 已将%~1更新入库
if %ans% equ 0 echo 未对%~1进行更新
goto:eof

:backupfile
set i=%NumofBackups%

:backupfile_cyclebegin
set /a j=%i%-1
copy %~1.oldsave%j%  %~1.oldsave%i% >nul
set i=%j%
if %i% lss 2 goto:backupfile_cycleend
goto:backupfile_cyclebegin

:backupfile_cycleend
if %NumofBackups% gtr 0 copy %~1  %~1.oldsave1 >nul
goto:eof

:isnew1
set ans=1
goto:eof

:isnew2
set ans=2
goto:eof

:isnew
if not exist %~1 goto:isnew2
for %%i in (%~1) do set time1=%%~ti
if not exist %~2 goto:isnew1
for %%i in (%~2) do set time2=%%~ti

set t1=%time1:~0,4%
set t2=%time2:~0,4%
if %t1% gtr %t2% goto:isnew1
if %t1% lss %t2% goto:isnew2
set t1=%time1:~5,2%
set t2=%time2:~5,2%
if %t1% gtr %t2% goto:isnew1
if %t1% lss %t2% goto:isnew2
set t1=%time1:~8,2%
set t2=%time2:~8,2%
if %t1% gtr %t2% goto:isnew1
if %t1% lss %t2% goto:isnew2
set t1=%time1:~11,2%
set t2=%time2:~11,2%
if %t1% gtr %t2% goto:isnew1
if %t1% lss %t2% goto:isnew2
set t1=%time1:~14,2%
set t2=%time2:~14,2%
if %t1% gtr %t2% goto:isnew1
if %t1% lss %t2% goto:isnew2
set ans=0
goto:eof


