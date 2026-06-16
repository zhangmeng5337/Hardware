ECHO ON
ECHO Converting AXF File to bin File.........

cd ..\Application\src\bootloader
copy ..\..\..\MDK-ARM\Release\gfc23REL.axf .
C:\Keil_v5\ARM\ARMCC\bin\fromelf.exe --bin gfc23REL.axf --output bin/
del /Q *.axf
copy bin\ER_IROM1 .
rmdir /Q /S .\bin





