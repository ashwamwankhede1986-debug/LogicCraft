# ==============================================================================
# LOGICCRAFT — COMPLETE REVIEW-2 DEMONSTRATION HARNESS
# Executes the full 12-step academic review demonstration sequence across
# C++, Computer Graphics (OpenGL), Java (OOP), and COA (64-Bit Assembly).
# ==============================================================================

$baseDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$env:PATH = "C:\msys64\mingw64\bin;" + $env:PATH

Write-Host "=================================================================" -ForegroundColor Cyan
Write-Host "  LOGICCRAFT: VISUAL LOGIC GATE SIMULATOR - REVIEW-2 SUITE       " -ForegroundColor Yellow
Write-Host "=================================================================" -ForegroundColor Cyan
Write-Host 'Subjects Covered (CO1 and CO2 for each):'
Write-Host '  1. Programming Lab - C++ (CO1 Arrays, CO2 Linked List)'
Write-Host '  2. Computer Graphics - OpenGL (CO1 Primitives, CO2 Transformations)'
Write-Host '  3. Problem Solving Using OOP - Java (CO1 Fundamentals, CO2 Advanced OOP)'
Write-Host '  4. COA / ALP - 64-bit Assembly (CO1 Processor Modeling, CO2 Assembly)'
Write-Host "=================================================================`n"

# ------------------------------------------------------------------------------
# SECTION 1: C++ / PROGRAMMING LAB (CO1 & CO2)
# ------------------------------------------------------------------------------
Write-Host '>>> [PART 1/4] EXECUTING C++ MODULE (CO1 Arrays and CO2 Linked List) ...' -ForegroundColor Green
Set-Location "$baseDir\Cpp"
g++ -std=c++14 main.cpp Gates/Gate.cpp DataStructures/ArrayOperations.cpp DataStructures/LinkedList.cpp CircuitEngine/TruthTable.cpp CircuitEngine/Circuit.cpp -o logiccraft_cpp.exe
if ($LASTEXITCODE -eq 0) {
    & .\logiccraft_cpp.exe
} else {
    Write-Host '[ERROR] C++ build failed' -ForegroundColor Red
}

# ------------------------------------------------------------------------------
# SECTION 2: COMPUTER GRAPHICS / OPENGL (CO1 & CO2)
# ------------------------------------------------------------------------------
Write-Host "`n>>> [PART 2/4] EXECUTING COMPUTER GRAPHICS MODULE (CO1 Primitives and CO2 Transformations) ..." -ForegroundColor Green
Set-Location "$baseDir\Computer Graphics"
g++ -std=c++14 main.cpp OpenGL/GLWindow.cpp Rendering/GateRenderer.cpp Canvas/Canvas.cpp Transformations/Camera.cpp -lopengl32 -lgdi32 -o logiccraft_cg.exe
if ($LASTEXITCODE -eq 0) {
    Write-Host 'Running automated verification of OpenGL transformations and clipping...'
    & .\logiccraft_cg.exe --test
    Write-Host 'To launch interactive visual window, run: .\run_graphics.bat' -ForegroundColor Yellow
} else {
    Write-Host '[ERROR] OpenGL build failed' -ForegroundColor Red
}

# ------------------------------------------------------------------------------
# SECTION 3: JAVA / OOP (CO1 & CO2)
# ------------------------------------------------------------------------------
Write-Host "`n>>> [PART 3/4] EXECUTING JAVA MODULE (CO1 Fundamentals and CO2 Advanced OOP) ..." -ForegroundColor Green
Set-Location "$baseDir\java"
if (-not (Test-Path "bin")) { New-Item -ItemType Directory -Path "bin" | Out-Null }
javac -d bin gates/*.java circuit/*.java project/*.java reports/*.java simulation/*.java Main.java
if ($LASTEXITCODE -eq 0) {
    java -cp bin Main
} else {
    Write-Host '[ERROR] Java build failed' -ForegroundColor Red
}

# ------------------------------------------------------------------------------
# SECTION 4: COA / 64-BIT ASSEMBLY (CO1 & CO2)
# ------------------------------------------------------------------------------
Write-Host "`n>>> [PART 4/4] EXECUTING COA / ALP MODULE (CO1 Processor Model and CO2 64-Bit Assembly) ..." -ForegroundColor Green
Set-Location "$baseDir\COA"
g++ -std=c++14 main.cpp ProcessorSimulation/ProcessorSimulator.cpp BooleanOperations/BooleanALU.cpp ALP/logic_ops_64.s -o logiccraft_coa.exe
if ($LASTEXITCODE -eq 0) {
    & .\logiccraft_coa.exe
} else {
    Write-Host '[ERROR] COA build failed' -ForegroundColor Red
}

Set-Location "$baseDir"
Write-Host "`n=================================================================" -ForegroundColor Cyan
Write-Host "  LOGICCRAFT REVIEW-2 DEMONSTRATION COMPLETE FOR ALL 4 SUBJECTS!  " -ForegroundColor Green
Write-Host "=================================================================" -ForegroundColor Cyan
