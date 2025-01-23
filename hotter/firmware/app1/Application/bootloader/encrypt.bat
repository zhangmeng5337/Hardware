ECHO ON
ECHO Encrypting firmware image

cd ..\application\src\bootloader
.\firmwarecrypto\encrypt.exe ER_IROM1 .\src\key.enc
del /Q ER_IROM1