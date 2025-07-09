ECHO OFF
ECHO Converting AXF File to bin File.........

cd ..\Utilities
copy ..\Proj\Release\vDrive.axf .
C:\Keil\ARM\ARMCC\bin\fromelf.exe --bin vDrive.axf --output bin/
del /Q *.axf
copy bin\ER_IROM1 .
rmdir /Q /S .\bin





