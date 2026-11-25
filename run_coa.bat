@echo off
setlocal
echo ========================================================
echo   LogicCraft: COA / 64-bit Assembly (CO1, CO2, CO3, CO4)
echo ========================================================
if exist "C:\msys64\ucrt64\bin" set PATH=C:\msys64\ucrt64\bin;%PATH%
if exist "C:\msys64\mingw64\bin" set PATH=C:\msys64\mingw64\bin;%PATH%

cd /d "%~dp0COA"
echo [1/2] Compiling COA Simulator and 64-bit Assembly...
g++ -std=c++14 main.cpp ProcessorSimulation/ProcessorSimulator.cpp BooleanOperations/BooleanALU.cpp ControlUnit/ControlUnit.cpp Pipeline/PipelineSimulator.cpp ALP/logic_ops_64.s -o logiccraft_coa.exe
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] COA Compilation failed!
    pause
    exit /b %ERRORLEVEL%
)

echo [2/2] Running COA / Assembly Demonstration...
logiccraft_coa.exe
pause

