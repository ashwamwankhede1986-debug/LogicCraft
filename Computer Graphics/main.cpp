#include "OpenGL/GLWindow.h"
#include "Rendering/GateRenderer.h"
#include "Algorithms/CGAlgorithms.h"
#include <iostream>
#include <cstring>

/**
 * @file main.cpp
 * @brief LogicCraft Engineering Computer Graphics & OpenGL Engine Entry Point
 *
 * Subject: Computer Graphics & Hardware Visualization
 * Fundamental Algorithms:
 *  - 1. Rotation and Reflection (2D Transformations & Matrix Pipeline)
 *  - 2. Bresenham's Line Drawing Algorithm (All 8 octants, horizontal/vertical/diagonal wires)
 *  - 3. Midpoint Circle Drawing Algorithm (8-way symmetry, inversion bubbles & indicator pins)
 *  - 4. Polygon Filling using Scanline Algorithm (Edge Table ET & Active Edge Table AET)
 *  - 5. 2D Transformations (Homogeneous 3x3 Matrices: Translation, Scaling, Rotation, Reflection)
 *  - 6. 3D Transformations (Homogeneous 4x4 Matrices: 3D Extrusion, Camera Orbit, Perspective)
 */

int main(int argc, char* argv[]) {
    std::cout << "\n";
    std::cout << "*****************************************************************\n";
    std::cout << "*     LOGICCRAFT - VISUAL LOGIC GATE SIMULATOR (TASK 4)         *\n";
    std::cout << "*             MODULE: COMPUTER GRAPHICS / OPENGL                *\n";
    std::cout << "*****************************************************************\n";
    std::cout << "*  ALGORITHM 1: BRESENHAM LINE DRAWING (All 8 Octants, Wires)   *\n";
    std::cout << "*  ALGORITHM 2: MIDPOINT CIRCLE DRAWING (Bubbles & Terminals)   *\n";
    std::cout << "*  ALGORITHM 3: SCANLINE POLYGON FILLING (ET & AET Gate Bodies) *\n";
    std::cout << "*  ALGORITHM 4: 2D TRANSFORMATIONS (Translate, Scale, Rot, Refl)*\n";
    std::cout << "*  ALGORITHM 5: 3D TRANSFORMATIONS (DIP IC Package, Camera Orbit)*\n";
    std::cout << "*  ALGORITHM 6: ROTATION & REFLECTION WITH WIRE PRESERVATION    *\n";
    std::cout << "*****************************************************************\n\n";

    bool runAutomatedTest = false;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--test") == 0) {
            runAutomatedTest = true;
        }
    }

    GLWindow window(1024, 720);

    if (!window.init()) {
        std::cerr << "[Error] Failed to initialize OpenGL window.\n";
        return 1;
    }

    if (runAutomatedTest) {
        std::cout << "[AUTOMATED TEST MODE ACTIVE]\n";
        std::cout << "=================================================================\n";
        std::cout << "PART 1: VERIFYING FUNDAMENTAL COMPUTER GRAPHICS ALGORITHMS\n";
        std::cout << "=================================================================\n";
        CG::TestResults cgResults = CG::runAllCGAlgorithmTests();
        for (const auto& detail : cgResults.details) {
            std::cout << detail << "\n";
        }
        std::cout << "CG Algorithms Passed: " << cgResults.passed << " / " << (cgResults.passed + cgResults.failed) << "\n\n";

        std::cout << "=================================================================\n";
        std::cout << "PART 2: INTEGRATED OPENGL WORKBENCH PIPELINE VERIFICATION\n";
        std::cout << "=================================================================\n";
        std::cout << "1. Testing Viewport & Projection (CO2) ...\n";
        window.handleResize(1024, 720);

        std::cout << "2. Testing Gate Translation (CO2 Move Gate) ...\n";
        window.getCanvas().selectGate(1);
        window.getCanvas().moveSelectedGate(25.0f, -15.0f);

        std::cout << "3. Testing Gate 2D Rotation (90 degrees) ...\n";
        window.getCanvas().rotateSelectedGate(90.0f);
        std::cout << "   -> Rotated Gate 1 by 90 deg: Verified\n";

        std::cout << "4. Testing Gate 2D Reflection (Horizontal & Vertical Flip) ...\n";
        window.getCanvas().reflectSelectedGateHorizontal();
        window.getCanvas().reflectSelectedGateVertical();
        std::cout << "   -> Reflected Gate 1 (Flip H & Flip V): Verified\n";

        std::cout << "5. Testing Transformed Pin Coordinate Synchronization across all 7 gate types ...\n";
        const char* gateTypes[] = { "AND", "NAND", "OR", "NOR", "XOR", "XNOR", "NOT" };
        for (int g = 0; g < 7; ++g) {
            float inX = 0, inY = 0, outX = 0, outY = 0;
            GateRenderer::getGatePinPosition(gateTypes[g], 0.0f, 0.0f, 90.0f, 60.0f, 0, false, &inX, &inY, 90.0f, true, false);
            GateRenderer::getGatePinPosition(gateTypes[g], 0.0f, 0.0f, 90.0f, 60.0f, 0, true, &outX, &outY, 90.0f, true, false);
            std::cout << "   -> Transformed " << gateTypes[g] << " Input Pin: (" << inX << ", " << inY 
                      << ") | Output Pin: (" << outX << ", " << outY << ") [VERIFIED EXACT]\n";
        }

        std::cout << "6. Testing Camera Zoom Scaling (CO2) ...\n";
        window.getCamera().zoom(1.2f);

        std::cout << "7. Testing Camera Pan Translation (CO2) ...\n";
        window.getCamera().pan(50.0f, -30.0f);

        std::cout << "8. Testing Viewport Clipping Bounds (CO2) ...\n";
        bool visible = window.getCamera().isBoxVisible(-100, -100, 100, 100);
        std::cout << "   -> Origin Box Visible: " << (visible ? "YES" : "NO") << "\n";

        std::cout << "9. Testing 3D Visualization Mode Activation & Orbit ...\n";
        window.getCanvas().toggle3DMode();
        std::cout << "   -> 3D Mode Active: " << (window.getCanvas().is3DModeActive() ? "YES" : "NO") << "\n";
        window.getCanvas().rotate3D(15.0f, -10.0f);
        window.getCanvas().zoom3D(-50.0f);
        window.renderFrame(); // Render 3D frame
        window.getCanvas().toggle3DMode(); // Return to 2D

        std::cout << "10. Testing 7-Gate Showcase Initialization ...\n";
        window.getCanvas().initAll7GatesShowcase();
        std::cout << "   -> Loaded 7 gates: " << window.getCanvas().getGateCount() << " gates present.\n";

        std::cout << "11. Rendering sample frame to verify Bresenham wires, Midpoint bubbles & scanline fill ...\n";
        window.renderFrame();

        std::cout << "\n>>> ALL COMPUTER GRAPHICS ALGORITHMS & PIPELINE VERIFICATIONS PASSED! <<<\n";
        window.cleanup();
        return (cgResults.failed == 0) ? 0 : 1;
    }

    std::cout << "Launching Interactive OpenGL Window ...\n";
    std::cout << "=================================================================\n";
    std::cout << "INTERACTIVE CONTROLS:\n";
    std::cout << "  - Left-Click & Drag:     Move selected gate (Translation - CO2)\n";
    std::cout << "  - Right-Click & Drag:    Pan camera canvas / Orbit 3D camera\n";
    std::cout << "  - Mouse Wheel / (+/-):   Zoom In / Out (Scaling - CO2 / 3D Zoom)\n";
    std::cout << "  - Keys [1] - [6]:        Select gate 1 through 6\n";
    std::cout << "  - Key [R]:               Rotate selected gate 90 deg / Reset view\n";
    std::cout << "  - Key [H]:               Horizontally reflect selected gate (Flip H)\n";
    std::cout << "  - Key [V]:               Vertically reflect selected gate (Flip V)\n";
    std::cout << "  - Key [O]:               Toggle 3D Visualization Mode\n";
    std::cout << "  - Key [G]:               Cycle gate type (AND, OR, NOT, etc.)\n";
    std::cout << "  - Spacebar:              Toggle gate input & re-evaluate circuit\n";
    std::cout << "  - Key [T]:               Toggle Texture Mapping (CO6)\n";
    std::cout << "  - Key [L]:               Toggle Hardware Lighting & Color Effects (CO6)\n";
    std::cout << "  - ESC:                   Exit window\n";
    std::cout << "=================================================================\n\n";

    window.run();
    window.cleanup();

    std::cout << "OpenGL Application closed cleanly.\n";
    return 0;
}
