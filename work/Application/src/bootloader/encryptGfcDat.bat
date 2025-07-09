ECHO ON
ECHO Encrypting firmware image

cd ..\bootloader\export
call  find.bat
cd ..\..\bootloader


for /f "skip=2 delims=[" %%l in ( .\export\find.txt) do (

mkdir encrypt\%%l\
)

for /f "skip=2 delims=[" %%l in ( .\export\find.txt) do (

cd ..\bootloader\export\%%l\
call  find.bat
cd ..\..\..\bootloader
)

for /f "skip=2 delims=[" %%l in ( .\export\find.txt) do (

for /f "skip=2 delims=[" %%k in ( .\export\%%l\find.txt) do (
.\firmwarecrypto\encryptGFC.exe .\export\%%l\%%k. .\src\key.enc
set "filename=%%~nk"
ren encrypted %%~nk
move %%~nk encrypt/%%l/%%~nk
)
)
pause


