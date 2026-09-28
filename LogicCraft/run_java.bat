@echo off
setlocal
echo ========================================================
echo   LogicCraft: Java / OOP (CO1 & CO2)
echo ========================================================

cd /d "%~dp0java"
if not exist "bin" mkdir bin

echo [1/2] Compiling Java classes...
javac -d bin gates/*.java circuit/*.java project/*.java reports/*.java simulation/*.java Main.java
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Java Compilation failed!
    pause
    exit /b %ERRORLEVEL%
)

echo [2/2] Running Java Demonstration...
java -cp bin Main
pause
