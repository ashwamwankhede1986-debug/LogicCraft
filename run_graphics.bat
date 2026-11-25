@echo off
setlocal
echo ========================================================
echo   LogicCraft: Computer Graphics & Hardware Visualization
echo   Fundamental Algorithms & OpenGL Engine (Review-2)
echo ========================================================
set PATH=C:\msys64\ucrt64\bin;C:\msys64\mingw64\bin;C:\MinGW\bin;%PATH%

cd /d "%~dp0Computer Graphics"
echo [1/2] Compiling OpenGL module with Fundamental Algorithms...
g++ -std=c++14 main.cpp OpenGL/GLWindow.cpp Rendering/GateRenderer.cpp Canvas/Canvas.cpp Transformations/Camera.cpp Algorithms/CGAlgorithms.cpp -lopengl32 -lgdi32 -o logiccraft_cg.exe
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] OpenGL Compilation failed!
    pause
    exit /b %ERRORLEVEL%
)

echo [2/2] Launching OpenGL Canvas...
logiccraft_cg.exe %*
pause
