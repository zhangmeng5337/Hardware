@echo off
setlocal enabledelayedexpansion
set "out=find.txt"
set "today=%date%"

:: 获取系统日期格式中的年月日（假设为 yyyy/mm/dd 或 yyyy-mm-dd）
:: 如果日期格式不同，请根据你电脑实际显示调整
for /f "tokens=1-3 delims=/-. " %%a in ("%today%") do (
    set "yy=%%a"
    set "mm=%%b"
    set "dd=%%c"
)
:: 去掉可能的前导空格（某些系统日期是 “dd/mm/yyyy”）
set "yy=!yy: =!"
set "mm=!mm: =!"
set "dd=!dd: =!"

:: 生成当天修改过的文件及目录列表
> "%out%" (
    for /f "delims=" %%F in ('dir /b /a') do (
        set "name=%%F"
        set "ftime=%%~tF"
        :: 提取日期部分（通常格式为 yyyy/mm/dd 或 yyyy-mm-dd）
        set "fdate=!ftime:~0,10!"
        :: 标准化日期分隔符，统一转为 /
        set "fdate=!fdate:-=/!"
        if "!fdate!"=="!yy!/!mm!/!dd!" echo !name!
    )
)

sort "%out%" /o "%out%"
echo 完成，当天文件列表已保存至 %out%