@echo off
cls
:menu
echo ---------------------
echo CyanogenMod - PSP - C Compiler
echo ---------------------
echo [1] Compile
echo.
echo [2] Close
echo ---------------------

echo.
set INPUT=
Set /P INPUT= Action:


if "%input%" =="" goto menu
if "%input%" =="1" goto compile
if "%input%" =="2" goto close

:compile
PATH=c:\pspsdk\bin;%cd%;
make clean
make
pause

:close
ECHO Delete objects not needed...
DEL *.o
DEL *.sfo
cls
