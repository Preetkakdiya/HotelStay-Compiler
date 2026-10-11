@echo off
setlocal

:: Close any open instance to prevent file-lock
taskkill /f /im HotelStay.exe >nul 2>nul

:: Set local compiler toolchain
set "PATH=%~dp0tools\w64devkit\bin;C:\Windows\System32;%PATH%"

echo ========================================================
echo       COMPILING HOTELSTAY COMPILER (LEX ^& YACC)
echo ========================================================
echo [1/3] Running Bison...
bison -d HotelStay.y
if %errorlevel% neq 0 (
    echo [ERROR] Bison failed!
    pause
    exit /b 1
)

echo [2/3] Running Flex...
flex HotelStay.l
if %errorlevel% neq 0 (
    echo [ERROR] Flex failed!
    pause
    exit /b 1
)

echo [3/3] Compiling with GCC...
gcc HotelStay.tab.c lex.yy.c -o HotelStay.exe
if %errorlevel% neq 0 (
    echo [ERROR] GCC failed!
    pause
    exit /b 1
)

echo.
echo [SUCCESS] Compiled successfully into HotelStay.exe!
echo.
echo ========================================================
echo              RUNNING PROGRAM: booking.stay
echo ========================================================
echo.
HotelStay.exe booking.stay

echo.
echo ========================================================
echo Program Finished! Press any key to exit.
echo ========================================================
pause
