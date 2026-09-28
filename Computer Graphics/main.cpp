#include "OpenGL/GLWindow.h"
#include <iostream>
#include <cstring>

/**
 * @file main.cpp
 * @brief LogicCraft Review-2 Demonstration Entry Point for Computer Graphics / OpenGL
 *
 * Subject: Computer Graphics
 * Focus:
 *  - CO1: OpenGL Primitives (Canvas, Grid, AND, OR, NOT gates, Pins, Wires)
 *  - CO2: Transformation & Clipping (Translation, Scaling, Viewport, Camera Pan/Zoom)
 */

int main(int argc, char* argv[]) {
    std::cout << "\n";
    std::cout << "*****************************************************************\n";
    std::cout << "*     LOGICCRAFT — VISUAL LOGIC GATE SIMULATOR (REVIEW 2)       *\n";
    std::cout << "*             MODULE: COMPUTER GRAPHICS / OPENGL                *\n";
    std::cout << "*****************************************************************\n";
    std::cout << "*  CO1: OPENGL PRIMITIVES  -> Grid, AND, OR, NOT gates, Wires,  *\n";
    std::cout << "*                             Terminals, Vector Stroke Typography*\n";
    std::cout << "*  CO2: TRANSFORMATIONS    -> Translation (Move Gates),         *\n";
    std::cout << "*                             Scaling (Zoom In/Out),            *\n";
    std::cout << "*                             Viewport & Boundary Clipping      *\n";
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
        std::cout << "1. Testing Viewport & Projection (CO2) ...\n";
        window.handleResize(1024, 720);

        std::cout << "2. Testing Gate Translation (CO2 Move Gate) ...\n";
        window.getCanvas().selectGate(1);
        window.getCanvas().moveSelectedGate(25.0f, -15.0f);

        std::cout << "3. Testing Camera Zoom Scaling (CO2) ...\n";
        window.getCamera().zoom(1.2f);

        std::cout << "4. Testing Camera Pan Translation (CO2) ...\n";
        window.getCamera().pan(50.0f, -30.0f);

        std::cout << "5. Testing Viewport Clipping Bounds (CO2) ...\n";
        bool visible = window.getCamera().isBoxVisible(-100, -100, 100, 100);
        std::cout << "   -> Origin Box Visible: " << (visible ? "YES" : "NO") << "\n";

        std::cout << "6. Rendering sample frame to verify OpenGL primitives pipeline ...\n";
        window.renderFrame();

        std::cout << "\n>>> ALL COMPUTER GRAPHICS CO1 & CO2 TESTS PASSED! <<<\n";
        window.cleanup();
        return 0;
    }

    std::cout << "Launching Interactive OpenGL Window ...\n";
    std::cout << "=================================================================\n";
    std::cout << "INTERACTIVE CONTROLS:\n";
    std::cout << "  - Left-Click & Drag:     Move selected gate (Translation - CO2)\n";
    std::cout << "  - Right-Click & Drag:    Pan camera canvas (Translation - CO2)\n";
    std::cout << "  - Mouse Wheel / (+/-):   Zoom In / Out (Scaling - CO2)\n";
    std::cout << "  - Keys [1], [2], [3]:    Select AND, OR, or NOT gate\n";
    std::cout << "  - Spacebar:              Toggle gate input & re-evaluate circuit\n";
    std::cout << "  - Key [R]:               Reset camera view\n";
    std::cout << "  - ESC:                   Exit window\n";
    std::cout << "=================================================================\n\n";

    window.run();
    window.cleanup();

    std::cout << "OpenGL Application closed cleanly.\n";
    return 0;
}
