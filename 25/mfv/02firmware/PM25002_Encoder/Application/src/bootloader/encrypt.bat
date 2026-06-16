ECHO ON
ECHO Encrypting firmware image

del ..\Images\encrypted_rhr126.bin
cd ..\application\src\bootloader
.\firmwarecrypto\encrypt.exe ER_IROM1 .\src\key.enc
copy encrypted ..\..\..\Images
ren ..\..\..\Images\encrypted encrypted_rhr126.bin

del /Q ER_IROM1