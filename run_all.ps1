# ==============================================================================
# LOGICCRAFT - MASTER TASK 4 & COMPREHENSIVE ACADEMIC DEMONSTRATION HARNESS
# Executes the full academic review sequence across:
#   1. C++ Programming Lab (CO1 Arrays, CO2 Linked Lists, CO3 Stack & Queue, CO4 Searching/Sorting, CO5 Hashing)
#   2. Computer Graphics / OpenGL (CO1 Primitives, CO2 Transforms, CO4 Coordinate Precision, CO6 Texture & Lighting)
#   3. Problem Solving Using OOP - Java (CO1 Core, CO2 Advanced OOP, CO3 Exceptions/Collections, CO4 Files & Generics)
#   4. Computer Organization & Architecture Lab (CO1 Model, CO2 64-Bit Logic, CO3 Arithmetic & Strings, CO4 Integration)
# ==============================================================================

$baseDir = Split-Path -Parent $MyInvocation.MyCommand.Path

Write-Host "=================================================================" -ForegroundColor Cyan
Write-Host "  LOGICCRAFT: VISUAL LOGIC GATE SIMULATOR - TASK 4 SUITE        " -ForegroundColor Yellow
Write-Host "=================================================================" -ForegroundColor Cyan
Write-Host "Course Outcomes Covered:"
Write-Host "  1. Programming Lab - C++:            CO1 (Arrays), CO2 (Linked Lists), CO3 (Stack & Queue), CO4 (Searching & Sorting), CO5 (Hashing)"
Write-Host "  2. Computer Graphics - OpenGL:       CO1 (Primitives), CO2 (Transforms/Clip), CO4 (Precision Alignment), CO6 (Texture & Lighting)"
Write-Host "  3. Problem Solving Using OOP - Java: CO1 (Core OOP), CO2 (Advanced OOP), CO3 (Exceptions/Collections), CO4 (Files & Generics)"
Write-Host "  4. COA / 64-Bit Assembly:            CO1 (Model), CO2 (64-Bit Logic), CO3 (Successive Addition & Strings), CO4 (Hardware Integration)"
Write-Host "=================================================================`n"

# ------------------------------------------------------------------------------
# SECTION 1: C++ / PROGRAMMING LAB (CO1, CO2, CO3, CO4 & CO5)
# ------------------------------------------------------------------------------
Write-Host ">>> [PART 1/4] EXECUTING C++ MODULE (CO1 Arrays, CO2 Linked Lists, CO3 Stack & Queue, CO4 Search/Sort, CO5 Hashing) ..." -ForegroundColor Green
Set-Location "$baseDir\Cpp"
$env:PATH = "C:\msys64\ucrt64\bin;C:\msys64\mingw64\bin;" + $env:PATH
g++ -std=c++14 main.cpp CircuitEngine/Circuit.cpp Gates/Gate.cpp DataStructures/ArrayOperations.cpp DataStructures/LinkedList.cpp CircuitEngine/TruthTable.cpp DataStructures/StackOperations.cpp DataStructures/QueueOperations.cpp Searching/Searching.cpp Sorting/Sorting.cpp Hashing/HashTable.cpp Algorithms/PerformanceComparison.cpp CircuitEngine/CircuitValidator.cpp -o logiccraft_cpp.exe
if ($LASTEXITCODE -eq 0) {
    Write-Host "[1.1] Running C++ Interactive Console Demonstration ..." -ForegroundColor Cyan
    & .\logiccraft_cpp.exe 0
    Write-Host "`n[1.2] Running C++ Automated Test Suite (CO1, CO2, CO3, CO4, CO5) ..." -ForegroundColor Cyan
    g++ -std=c++14 Tests/TestRunner.cpp Tests/Task4Tests.cpp CircuitEngine/Circuit.cpp Gates/Gate.cpp DataStructures/ArrayOperations.cpp DataStructures/LinkedList.cpp CircuitEngine/TruthTable.cpp DataStructures/StackOperations.cpp DataStructures/QueueOperations.cpp Searching/Searching.cpp Sorting/Sorting.cpp Hashing/HashTable.cpp Algorithms/PerformanceComparison.cpp CircuitEngine/CircuitValidator.cpp -o Tests/test_runner.exe
    & .\Tests\test_runner.exe
}
else {
    Write-Host "[ERROR] C++ build failed" -ForegroundColor Red
}

# ------------------------------------------------------------------------------
# SECTION 2: COMPUTER GRAPHICS / OPENGL (CO1, CO2, CO4 & CO6)
# ------------------------------------------------------------------------------
Write-Host "`n>>> [PART 2/4] EXECUTING COMPUTER GRAPHICS MODULE (CO1 Primitives, CO2 Transforms, CO4 Coordinate Precision, CO6 Texture & Lighting) ..." -ForegroundColor Green
Set-Location "$baseDir\Computer Graphics"
$env:PATH = "C:\msys64\ucrt64\bin;C:\msys64\mingw64\bin;" + $env:PATH
g++ -std=c++14 main.cpp OpenGL/GLWindow.cpp Rendering/GateRenderer.cpp Canvas/Canvas.cpp Transformations/Camera.cpp -lopengl32 -lgdi32 -o logiccraft_cg.exe
if ($LASTEXITCODE -eq 0) {
    Write-Host "Running automated verification of OpenGL primitives, transformations, clipping, CO4 pin alignment, CO6 texture mapping & lighting..." -ForegroundColor Cyan
    & .\logiccraft_cg.exe --test
    Write-Host "To launch the full interactive OpenGL canvas window, run: .\run_graphics.bat" -ForegroundColor Yellow
}
else {
    Write-Host "[ERROR] OpenGL build failed" -ForegroundColor Red
}

# ------------------------------------------------------------------------------
# SECTION 3: JAVA / OOP (CO1, CO2, CO3 & CO4)
# ------------------------------------------------------------------------------
Write-Host "`n>>> [PART 3/4] EXECUTING JAVA MODULE (CO1 Core, CO2 Advanced OOP, CO3 Exceptions/Collections, CO4 Files & Generics) ..." -ForegroundColor Green
Set-Location "$baseDir\java"
$env:PATH = "C:\Program Files\Java\jdk-26.0.2.1\bin;" + $env:PATH
Get-ChildItem -Path "src" -Filter "*.java" -Recurse | ForEach-Object { $_.FullName } | Out-File -FilePath "sources_list.txt" -Encoding ascii
if (-not (Test-Path "bin")) { New-Item -ItemType Directory -Path "bin" | Out-Null }
javac -d bin "`@sources_list.txt"
if ($LASTEXITCODE -eq 0) {
    Write-Host "[3.1] Running Java Demonstration Harness (CO1, CO2, CO3 & CO4) ..." -ForegroundColor Cyan
    java -cp bin logiccraft.Main
    Write-Host "`n[3.2] Running Java Test Suites (12 Unit Tests) ..." -ForegroundColor Cyan
    java -cp bin test.ExceptionTest
    java -cp bin test.GenericTest
    java -cp bin test.Task4FileGenericTest
    java -cp bin test.FileTest
    Write-Host "To launch the Java Swing GUI, run: .\run_java.bat --gui" -ForegroundColor Yellow
}
else {
    Write-Host "[ERROR] Java build failed" -ForegroundColor Red
}

# ------------------------------------------------------------------------------
# SECTION 4: COA / 64-BIT ASSEMBLY (CO1, CO2, CO3 & CO4)
# ------------------------------------------------------------------------------
Write-Host "`n>>> [PART 4/4] EXECUTING COA / ALP MODULE (CO1 Model, CO2 64-Bit Logic, CO3 Arithmetic & Strings, CO4 Integration) ..." -ForegroundColor Green
Set-Location "$baseDir\COA"
$env:PATH = "C:\msys64\ucrt64\bin;C:\msys64\mingw64\bin;" + $env:PATH
g++ -std=c++14 main.cpp ProcessorSimulation/ProcessorSimulator.cpp BooleanOperations/BooleanALU.cpp ControlUnit/ControlUnit.cpp Pipeline/PipelineSimulator.cpp ALP/logic_ops_64.s -o logiccraft_coa.exe
if ($LASTEXITCODE -eq 0) {
    Write-Host "[4.1] Running COA Console Demonstration ..." -ForegroundColor Cyan
    & .\logiccraft_coa.exe 0
    Write-Host "`n[4.2] Running 64-bit Assembly Unit Tests (Task 1 to Task 4 Integration) ..." -ForegroundColor Cyan
    g++ -std=c++14 Tests/TestALP.cpp Tests/Task4IntegrationTests.cpp ProcessorSimulation/ProcessorSimulator.cpp BooleanOperations/BooleanALU.cpp ControlUnit/ControlUnit.cpp Pipeline/PipelineSimulator.cpp ALP/logic_ops_64.s -o Tests/test_alp.exe
    & .\Tests\test_alp.exe
}
else {
    Write-Host "[ERROR] COA build failed" -ForegroundColor Red
}

Set-Location "$baseDir"
Write-Host "`n=================================================================" -ForegroundColor Cyan
Write-Host "  LOGICCRAFT TASK 4 DEMONSTRATION COMPLETE FOR ALL 4 SUBJECTS!   " -ForegroundColor Green
Write-Host "=================================================================" -ForegroundColor Cyan
