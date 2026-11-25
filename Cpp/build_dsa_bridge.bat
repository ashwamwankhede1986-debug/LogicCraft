@echo off
setlocal
cd /d "%~dp0"
where g++ >nul 2>nul
if errorlevel 1 (
  echo ERROR: g++ is not installed or not on PATH.
  echo Install MinGW-w64 and add its bin folder to PATH.
  exit /b 1
)
g++ -std=c++14 -O2 DSABridge.cpp Searching/Searching.cpp Sorting/Sorting.cpp -o logiccraft_dsa_bridge.exe
if errorlevel 1 exit /b 1
echo LogicCraft native C++ DSA bridge built successfully.
