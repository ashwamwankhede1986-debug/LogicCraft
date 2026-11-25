@echo off
setlocal
echo ========================================================
echo   LogicCraft: C++ / Programming Lab (CO1, CO2, CO3)
echo ========================================================
if exist "C:\msys64\ucrt64\bin" set PATH=C:\msys64\ucrt64\bin;%PATH%
if exist "C:\msys64\mingw64\bin" set PATH=C:\msys64\mingw64\bin;%PATH%

cd /d "%~dp0Cpp"
echo [1/2] Compiling C++ module...
g++ -std=c++14 main.cpp Gates/Gate.cpp DataStructures/ArrayOperations.cpp DataStructures/LinkedList.cpp DataStructures/StackOperations.cpp DataStructures/QueueOperations.cpp CircuitEngine/TruthTable.cpp CircuitEngine/Circuit.cpp Searching/Searching.cpp Sorting/Sorting.cpp Hashing/HashTable.cpp Algorithms/PerformanceComparison.cpp CircuitEngine/CircuitValidator.cpp -o logiccraft_cpp.exe
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] C++ Compilation failed!
    pause
    exit /b %ERRORLEVEL%
)

echo [2/2] Running C++ Demonstration...
logiccraft_cpp.exe
pause

