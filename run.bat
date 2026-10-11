@echo off
setlocal
set "PATH=%~dp0tools\w64devkit\bin;C:\Windows\System32;%PATH%"

if not exist "%~dp0HotelStay.exe" (
    echo [INFO] HotelStay.exe not found. Building now...
    call "%~dp0build.bat"
    exit /b
)

if "%~1"=="" (
    echo Running default program: booking.stay
    "%~dp0HotelStay.exe" "%~dp0booking.stay"
) else (
    echo Running program: %1
    "%~dp0HotelStay.exe" "%~1"
)

pause
