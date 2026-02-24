ECHO ON
ECHO Converting AXF File to bin File.........

cd ..\Application\src\bootloader
copy  ..\..\MDK-ARM\code\app.axf
C:\Keil_v5\ARM\ARMCC\bin\fromelf.exe --bin app.axf --output bin/
del /Q *.axf
copy bin\ER_IROM1 .
rmdir /Q /S .\bin





