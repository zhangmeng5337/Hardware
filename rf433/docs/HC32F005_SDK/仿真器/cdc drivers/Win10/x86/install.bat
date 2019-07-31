@echo off

:: BatchGotAdmin
:-------------------------------------
REM  --> Check for permissions
>nul 2>&1 "%SYSTEMROOT%\system32\cacls.exe" "%SYSTEMROOT%\system32\config\system"

REM --> If error flag set, we do not have admin.
if '%errorlevel%' NEQ '0' (
    echo Requesting administrative privileges...
    goto UACPrompt
) else ( goto gotAdmin )

:UACPrompt
    echo Set UAC = CreateObject^("Shell.Application"^) > "%temp%\getadmin.vbs"
    set params = %*:"=""
    echo UAC.ShellExecute "cmd.exe", "/c %~s0 %params%", "", "runas", 1 >> "%temp%\getadmin.vbs"

    "%temp%\getadmin.vbs"
    del "%temp%\getadmin.vbs"
    exit /B

:gotAdmin
    pushd "%CD%"
    CD /D "%~dp0"
:--------------------------------------
Set RegQry=HKLM\Hardware\Description\System\CentralProcessor\0
REG.exe Query %RegQry% > CheckOS.txt 
Find /i "x86" < CheckOS.txt > nul
If %ERRORLEVEL% == 0 (
    cls
    echo Please disconnect any Virtual Com Port 
    pause
    cls
    echo Deinstalling, please wait...
    dpinst_x86.exe /U hdscusbvcomm.inf /D /S > nul
    dpinst_x86.exe /LM /SE /SW /F > nul
    cls
    echo Please connect Virtual Com Port 
    pause
    cls
    echo Installing, please wait...
    dpinst_x86.exe /LM /SE /SW /F > nul
) ELSE (
    cls
    echo Please disconnect any Virtual Com Port 
    pause
    cls
    echo Deinstalling, please wait...
    dpinst_x64.exe /U hdscusbvcomm.inf /D /S > nul
    dpinst_x64.exe /LM /SE /SW /F > nul
    cls
    echo Please connect Virtual Com Port 
    pause
    cls
    echo Installing, please wait...
    dpinst_x64.exe /LM /SE /SW /F > nul
)
del CheckOS.txt > nul
pause
