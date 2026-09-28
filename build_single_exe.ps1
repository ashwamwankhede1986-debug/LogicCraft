# ========================================================
# LogicCraft: Compiling Unified Master Executable (Single EXE)
# ========================================================
$env:PATH = "C:\msys64\mingw64\bin;" + $env:PATH
$baseDir = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $baseDir

Write-Host "[1/3] Compiling source components to object files..." -ForegroundColor Cyan
g++ -std=c++14 -c LogicCraft_Master.cpp -o LogicCraft_Master.o
g++ -std=c++14 -c Cpp/Gates/Gate.cpp -o Gate.o
g++ -std=c++14 -c Cpp/DataStructures/ArrayOperations.cpp -o ArrayOperations.o
g++ -std=c++14 -c Cpp/DataStructures/LinkedList.cpp -o LinkedList.o
g++ -std=c++14 -c Cpp/CircuitEngine/TruthTable.cpp -o TruthTable.o
g++ -std=c++14 -c Cpp/CircuitEngine/Circuit.cpp -o Circuit.o

g++ -std=c++14 -c "Computer Graphics/OpenGL/GLWindow.cpp" -o GLWindow.o
g++ -std=c++14 -c "Computer Graphics/Rendering/GateRenderer.cpp" -o GateRenderer.o
g++ -std=c++14 -c "Computer Graphics/Canvas/Canvas.cpp" -o Canvas.o
g++ -std=c++14 -c "Computer Graphics/Transformations/Camera.cpp" -o Camera.o

g++ -std=c++14 -c COA/BooleanOperations/BooleanALU.cpp -o BooleanALU.o
g++ -std=c++14 -c COA/ProcessorSimulation/ProcessorSimulator.cpp -o ProcessorSimulator.o
gcc -c COA/ALP/logic_ops_64.s -o logic_ops_64.o

Write-Host "[2/3] Linking all 4 subjects into single LogicCraft.exe..." -ForegroundColor Cyan
g++ LogicCraft_Master.o Gate.o ArrayOperations.o LinkedList.o TruthTable.o Circuit.o GLWindow.o GateRenderer.o Canvas.o Camera.o BooleanALU.o ProcessorSimulator.o logic_ops_64.o -lopengl32 -lgdi32 -o LogicCraft.exe

if ($LASTEXITCODE -eq 0) {
    Write-Host "[3/3] Copying runtime DLLs for complete portability..." -ForegroundColor Cyan
    Copy-Item "C:\msys64\mingw64\bin\libgcc_s_seh-1.dll" -Destination . -Force -ErrorAction SilentlyContinue
    Copy-Item "C:\msys64\mingw64\bin\libstdc++-6.dll" -Destination . -Force -ErrorAction SilentlyContinue
    Copy-Item "C:\msys64\mingw64\bin\libwinpthread-1.dll" -Destination . -Force -ErrorAction SilentlyContinue

    Remove-Item *.o -ErrorAction SilentlyContinue

    # Sync to nested directory if it exists
    if (Test-Path "LogicCraft") {
        Copy-Item "LogicCraft.exe" -Destination "LogicCraft\" -Force
        Copy-Item "Computer Graphics\Rendering\GateRenderer.cpp" -Destination "LogicCraft\Computer Graphics\Rendering\" -Force -ErrorAction SilentlyContinue
        Copy-Item "Computer Graphics\OpenGL\GLWindow.cpp" -Destination "LogicCraft\Computer Graphics\OpenGL\" -Force -ErrorAction SilentlyContinue
    }

    Write-Host "========================================================" -ForegroundColor Green
    Write-Host " BUILD SUCCESSFUL! LogicCraft.exe is ready to run." -ForegroundColor Green
    Write-Host "========================================================" -ForegroundColor Green
} else {
    Write-Host "[ERROR] Build failed! Check compiler output above." -ForegroundColor Red
}
