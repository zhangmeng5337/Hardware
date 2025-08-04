ECHO ON
ECHO Encrypting firmware image

cd ..\bootloader
@echo off
for /L %%I in (1,1,20) do (
@echo I
set strNum=%%I
.\firmwarecrypto\encrypt.exe .\export\gas%%I.txt .\src\key.enc
ren encrypted gas%%I
move gas%%I encrypt


)

for /L %%I in (1,1,3) do (
@echo I
set strNum=%%I
.\firmwarecrypto\encrypt.exe .\export\nozzle%%I.txt .\src\key.enc
ren encrypted nozzle%%I
move nozzle%%I encrypt


)

pause
