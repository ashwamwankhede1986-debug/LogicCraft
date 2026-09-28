@echo off
setlocal
echo ========================================================
echo   LogicCraft: Computer Graphics / OpenGL (CO1 & CO2)
echo ========================================================
set PATH=C:\msys64\mingw64\bin;%PATH%

cd /d "%~dp0Computer Graphics"
echo [1/2] Compiling OpenGL module...
g++ -std=c++14 main.cpp OpenGL/GLWindow.cpp Rendering/GateRenderer.cpp Canvas/Canvas.cpp Transformations/Camera.cpp -lopengl32 -lgdi32 -o logiccraft_cg.exe
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] OpenGL Compilation failed!
    pause
    exit /b %ERRORLEVEL%
)

echo [2/2] Launching OpenGL Canvas...
logiccraft_cg.exe %*
pause
