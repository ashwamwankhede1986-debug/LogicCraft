@echo off
setlocal EnableDelayedExpansion
echo ========================================================
echo   LogicCraft: Java / OOP (CO1, CO2, CO3, and CO4)
echo ========================================================

cd /d "%~dp0java"
if not exist "bin" mkdir bin

echo [1/2] Compiling Java classes in packages...
(for /r src %%i in (*.java) do (
    set "f=%%i"
    set "f=!f:%CD%\=!"
    set "f=!f:\=/!"
    echo !f!
)) > sources_list.txt
javac -d bin -sourcepath src @sources_list.txt
set COMPILE_STATUS=%ERRORLEVEL%
if exist sources_list.txt del sources_list.txt
if %COMPILE_STATUS% NEQ 0 (
    echo [ERROR] Java Compilation failed!
    pause
    exit /b %COMPILE_STATUS%
)

echo [2/2] Running Java CO1, CO2, CO3, and CO4 Demonstration...
if "%1"=="--gui" (
    echo Launching Java Swing UI...
    java -cp bin logiccraft.Main --gui
) else (
    java -cp bin logiccraft.Main
)
echo.
echo To launch the interactive Swing GUI, run: run_java.bat --gui
pause

