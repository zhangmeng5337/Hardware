ECHO ON
ECHO Encrypting firmware image

cd ..\application\src\bootloader
.\firmwarecrypto\encrypt.exe app.txt .\src\key.enc
del /Q ER_IROM1