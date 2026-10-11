@echo off
setlocal
echo ========================================================
echo       BUILDING AND COMPILING HOTELSTAY COMPILER
echo ========================================================

:: Close any running instance of HotelStay.exe to prevent Windows "Permission Denied" lock
taskkill /f /im HotelStay.exe >nul 2>nul

:: Add portable tools folder to PATH
set "PATH=%~dp0tools\w64devkit\bin;C:\Windows\System32;%PATH%"

echo [Step 1] Running Bison parser generator (bison)...
bison -d HotelStay.y
if %errorlevel% neq 0 (
    echo [ERROR] Bison compilation failed!
    pause
    exit /b 1
)

echo [Step 2] Running Flex lexical analyzer generator (flex)...
flex HotelStay.l
if %errorlevel% neq 0 (
    echo [ERROR] Flex compilation failed!
    pause
    exit /b 1
)

echo [Step 3] Compiling C binaries with GCC...
gcc HotelStay.tab.c lex.yy.c -o HotelStay.exe
if %errorlevel% neq 0 (
    echo.
    echo [ERROR] GCC compilation failed!
    echo If you see "Permission denied", close any open command windows running HotelStay.exe.
    pause
    exit /b 1
)

echo.
echo [SUCCESS] HotelStay.exe compiled successfully!
echo.
echo ========================================================
echo       RUNNING HOTELSTAY ON DEMO SCRIPT: booking.stay
echo ========================================================
echo.
HotelStay.exe booking.stay

echo.
echo ========================================================
echo       RUNNING HOTELSTAY ON ERROR TEST: error_demo.stay
echo ========================================================
echo.
HotelStay.exe error_demo.stay

echo.
echo Build and Execution complete!
pause
