SET C51INC=C:\Keil_v5\c51\C51\Inc;C:\Keil_v5\c51\C51\Inc\DWIN
SET C51LIB=C:\Keil_v5\c51\C51\Lib
SET CPU_TYPE=T5
SET CPU_VENDOR=DWIN
SET UV2_TARGET=Target 1
SET CPU_XTAL=0x016E3600
"C:\Keil_v5\c51\C51\BIN\A51.EXE" @".\obj\STARTUP_M5._ia"
"C:\Keil_v5\c51\C51\BIN\C51.EXE" @".\obj\main.__i"
"C:\Keil_v5\c51\C51\BIN\C51.EXE" @".\obj\sys.__i"
"C:\Keil_v5\c51\C51\BIN\C51.EXE" @".\obj\uart.__i"
"C:\Keil_v5\c51\C51\BIN\C51.EXE" @".\obj\Led.__i"
"C:\Keil_v5\c51\C51\BIN\C51.EXE" @".\obj\i2c.__i"
"C:\Keil_v5\c51\C51\BIN\C51.EXE" @".\obj\save_data_dgus.__i"
"C:\Keil_v5\c51\C51\BIN\C51.EXE" @".\obj\crc16.__i"
"C:\Keil_v5\c51\C51\BIN\LX51.EXE" @".\obj\T5L51.lnp"
"C:\Keil_v5\c51\C51\BIN\OHX51.EXE" ".\obj\T5L51" HEX
.\1.bat
