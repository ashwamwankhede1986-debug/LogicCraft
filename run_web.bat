@echo off
setlocal
echo ========================================================
echo   LogicCraft: Full-Stack Web Application (Task 4)
echo ========================================================
cd /d "%~dp0"

echo [1/2] Checking Node.js environment...
where node >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Node.js is not found in PATH!
    pause
    exit /b 1
)

echo [2/2] Launching LogicCraft Server on http://localhost:3000 ...
start "" "http://localhost:3000"
node backend/server.js
pause
