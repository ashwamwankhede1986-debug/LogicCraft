@echo off
setlocal
echo ========================================================
echo   LogicCraft: Compiling Unified Master Executable (Single EXE)
echo ========================================================
set PATH=C:\msys64\mingw64\bin;%PATH%

cd /d "%~dp0"

echo [1/3] Compiling source components to object files...
g++ -std=c++14 -c LogicCraft_Master.cpp -o LogicCraft_Master.o
if %ERRORLEVEL% NEQ 0 goto error

g++ -std=c++14 -c Cpp/Gates/Gate.cpp -o Gate.o
g++ -std=c++14 -c Cpp/DataStructures/ArrayOperations.cpp -o ArrayOperations.o
g++ -std=c++14 -c Cpp/DataStructures/LinkedList.cpp -o LinkedList.o
g++ -std=c++14 -c Cpp/CircuitEngine/TruthTable.cpp -o TruthTable.o
g++ -std=c++14 -c Cpp/CircuitEngine/Circuit.cpp -o Circuit.o
if %ERRORLEVEL% NEQ 0 goto error

g++ -std=c++14 -c "Computer Graphics/OpenGL/GLWindow.cpp" -o GLWindow.o
g++ -std=c++14 -c "Computer Graphics/Rendering/GateRenderer.cpp" -o GateRenderer.o
g++ -std=c++14 -c "Computer Graphics/Canvas/Canvas.cpp" -o Canvas.o
g++ -std=c++14 -c "Computer Graphics/Transformations/Camera.cpp" -o Camera.o
if %ERRORLEVEL% NEQ 0 goto error

g++ -std=c++14 -c COA/BooleanOperations/BooleanALU.cpp -o BooleanALU.o
g++ -std=c++14 -c COA/ProcessorSimulation/ProcessorSimulator.cpp -o ProcessorSimulator.o
gcc -c COA/ALP/logic_ops_64.s -o logic_ops_64.o
if %ERRORLEVEL% NEQ 0 goto error

echo [2/3] Linking all 4 subjects into single LogicCraft.exe...
g++ LogicCraft_Master.o Gate.o ArrayOperations.o LinkedList.o TruthTable.o Circuit.o GLWindow.o GateRenderer.o Canvas.o Camera.o BooleanALU.o ProcessorSimulator.o logic_ops_64.o -lopengl32 -lgdi32 -o LogicCraft.exe
if %ERRORLEVEL% NEQ 0 goto error

echo [3/3] Copying runtime DLLs for complete portability...
copy /y "C:\msys64\mingw64\bin\libgcc_s_seh-1.dll" . >nul 2>&1
copy /y "C:\msys64\mingw64\bin\libstdc++-6.dll" . >nul 2>&1
copy /y "C:\msys64\mingw64\bin\libwinpthread-1.dll" . >nul 2>&1

del /q *.o >nul 2>&1

echo ========================================================
echo  BUILD SUCCESSFUL! LogicCraft.exe is ready to run.
echo ========================================================
pause
exit /b 0

:error
echo [ERROR] Build failed! Check compiler output above.
pause
exit /b 1
