@echo off

rem presentacion
cscript //nologo maximize.vbs
chcp 65001 > nul
timeout /t 1 /nobreak > nul

rem compilar
gcc main_tablashash.c tablashash.c TADListaDL.c -o a

rem verificacion de errores en la compilacion
if %errorlevel% neq 0 (
    echo Hubo errores durante la compilacion.
    pause
    exit /b %errorlevel%
)

rem ejecutar
a.exe
pause