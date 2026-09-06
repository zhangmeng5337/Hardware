ECHO OFF
ECHO Generating keys for bootloader and release builds

cd ..\Utilities
.\firmwarekeygen\keygen.exe $eb@sT1anAl3x628 j0yB1lG6$k@10(*#
copy key.c .\src
copy key.enc .\src
del key.c
del key.enc