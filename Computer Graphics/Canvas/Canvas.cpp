#include "Canvas.h"
#include "../Rendering/GateRenderer.h"
#include <iostream>
#include <cmath>
#include <cstring>
#include <cstdio>

Canvas::Canvas() : gateCount(0), selectedGateId(-1), hoveredGateId(-1), isDraggingGate(false), 
                 dragStartGateX(0), dragStartGateY(0), mode3D(false), yaw3D(-25.0f), pitch3D(30.0f), distance3D(650.0f) {
    initDefaultCircuit();
}

void Canvas::initDefaultCircuit() {
    gateCount = 4;

    for (int i = 0; i < MAX_VISUAL_GATES; ++i) {
        gates[i].rotation = 0.0f;
        gates[i].flipH = false;
        gates[i].flipV = false;
        gates[i].scaleX = 1.0f;
        gates[i].scaleY = 1.0f;
    }

    // Gate 1: AND Gate
    gates[0].id = 1;
    strcpy(gates[0].type, "AND");
    strcpy(gates[0].label, "Primary_AND");
    gates[0].x = -320.0f;
    gates[0].y = 70.0f;
    gates[0].width = 90.0f;
    gates[0].height = 60.0f;
    gates[0].isSelected = false;
    gates[0].inputA = 1;
    gates[0].inputB = 1;
    gates[0].output = 1;

    // Gate 2: OR Gate
    gates[1].id = 2;
    strcpy(gates[1].type, "OR");
    strcpy(gates[1].label, "Mid_OR");
    gates[1].x = -90.0f;
    gates[1].y = -50.0f;
    gates[1].width = 90.0f;
    gates[1].height = 60.0f;
    gates[1].isSelected = false;
    gates[1].inputA = 1; // Driven by Gate 1 output
    gates[1].inputB = 0; // External input
    gates[1].output = 1;

    // Gate 3: XOR Gate
    gates[2].id = 3;
    strcpy(gates[2].type, "XOR");
    strcpy(gates[2].label, "Branch_XOR");
    gates[2].x = 140.0f;
    gates[2].y = 70.0f;
    gates[2].width = 90.0f;
    gates[2].height = 60.0f;
    gates[2].isSelected = false;
    gates[2].inputA = 1; // Driven by Gate 2 output
    gates[2].inputB = 0; // External input
    gates[2].output = 1;

    // Gate 4: NOT Gate
    gates[3].id = 4;
    strcpy(gates[3].type, "NOT");
    strcpy(gates[3].label, "Final_Inverter");
    gates[3].x = 360.0f;
    gates[3].y = -50.0f;
    gates[3].width = 80.0f;
    gates[3].height = 55.0f;
    gates[3].isSelected = false;
    gates[3].inputA = 1; // Driven by Gate 3 output
    gates[3].inputB = 0;
    gates[3].output = 0;

    evaluateCircuit();
}

void Canvas::initAll7GatesShowcase() {
    gateCount = 7;

    for (int i = 0; i < MAX_VISUAL_GATES; ++i) {
        gates[i].rotation = 0.0f;
        gates[i].flipH = false;
        gates[i].flipV = false;
        gates[i].scaleX = 1.0f;
        gates[i].scaleY = 1.0f;
    }

    // Row 1: AND, NAND, OR, NOR (Y = 110)
    // Gate 1: AND
    gates[0].id = 1;
    strcpy(gates[0].type, "AND");
    strcpy(gates[0].label, "AND_G1");
    gates[0].x = -330.0f;
    gates[0].y = 110.0f;
    gates[0].width = 90.0f;
    gates[0].height = 60.0f;
    gates[0].isSelected = false;
    gates[0].inputA = 1;
    gates[0].inputB = 1;
    gates[0].output = 1;

    // Gate 2: NAND
    gates[1].id = 2;
    strcpy(gates[1].type, "NAND");
    strcpy(gates[1].label, "NAND_G2");
    gates[1].x = -110.0f;
    gates[1].y = 110.0f;
    gates[1].width = 90.0f;
    gates[1].height = 60.0f;
    gates[1].isSelected = false;
    gates[1].inputA = 1;
    gates[1].inputB = 1;
    gates[1].output = 0;

    // Gate 3: OR
    gates[2].id = 3;
    strcpy(gates[2].type, "OR");
    strcpy(gates[2].label, "OR_G3");
    gates[2].x = 110.0f;
    gates[2].y = 110.0f;
    gates[2].width = 90.0f;
    gates[2].height = 60.0f;
    gates[2].isSelected = false;
    gates[2].inputA = 0;
    gates[2].inputB = 1;
    gates[2].output = 1;

    // Gate 4: NOR
    gates[3].id = 4;
    strcpy(gates[3].type, "NOR");
    strcpy(gates[3].label, "NOR_G4");
    gates[3].x = 330.0f;
    gates[3].y = 110.0f;
    gates[3].width = 90.0f;
    gates[3].height = 60.0f;
    gates[3].isSelected = false;
    gates[3].inputA = 0;
    gates[3].inputB = 1;
    gates[3].output = 0;

    // Row 2: XOR, XNOR, NOT (Y = -90)
    // Gate 5: XOR
    gates[4].id = 5;
    strcpy(gates[4].type, "XOR");
    strcpy(gates[4].label, "XOR_G5");
    gates[4].x = -220.0f;
    gates[4].y = -90.0f;
    gates[4].width = 90.0f;
    gates[4].height = 60.0f;
    gates[4].isSelected = false;
    gates[4].inputA = 1;
    gates[4].inputB = 0;
    gates[4].output = 1;

    // Gate 6: XNOR
    gates[5].id = 6;
    strcpy(gates[5].type, "XNOR");
    strcpy(gates[5].label, "XNOR_G6");
    gates[5].x = 0.0f;
    gates[5].y = -90.0f;
    gates[5].width = 90.0f;
    gates[5].height = 60.0f;
    gates[5].isSelected = false;
    gates[5].inputA = 1;
    gates[5].inputB = 1;
    gates[5].output = 1;

    // Gate 7: NOT
    gates[6].id = 7;
    strcpy(gates[6].type, "NOT");
    strcpy(gates[6].label, "NOT_G7");
    gates[6].x = 220.0f;
    gates[6].y = -90.0f;
    gates[6].width = 80.0f;
    gates[6].height = 55.0f;
    gates[6].isSelected = false;
    gates[6].inputA = 1;
    gates[6].inputB = 0;
    gates[6].output = 0;

    evaluateCircuit();
}

void Canvas::evaluateCircuit() {
    for (int i = 0; i < gateCount; ++i) {
        if (strcmp(gates[i].type, "AND") == 0) {
            gates[i].output = (gates[i].inputA && gates[i].inputB) ? 1 : 0;
        } else if (strcmp(gates[i].type, "NAND") == 0) {
            gates[i].output = (gates[i].inputA && gates[i].inputB) ? 0 : 1;
        } else if (strcmp(gates[i].type, "OR") == 0) {
            gates[i].output = (gates[i].inputA || gates[i].inputB) ? 1 : 0;
        } else if (strcmp(gates[i].type, "NOR") == 0) {
            gates[i].output = (gates[i].inputA || gates[i].inputB) ? 0 : 1;
        } else if (strcmp(gates[i].type, "XOR") == 0) {
            gates[i].output = (gates[i].inputA ^ gates[i].inputB) ? 1 : 0;
        } else if (strcmp(gates[i].type, "XNOR") == 0) {
            gates[i].output = (gates[i].inputA == gates[i].inputB) ? 1 : 0;
        } else if (strcmp(gates[i].type, "NOT") == 0) {
            gates[i].output = (gates[i].inputA == 0) ? 1 : 0;
        }
    }

    // If 4-gate cascade mode, propagate cascade signals
    if (gateCount == 4) {
        gates[1].inputA = gates[0].output;
        gates[1].output = (gates[1].inputA || gates[1].inputB) ? 1 : 0;

        gates[2].inputA = gates[1].output;
        gates[2].output = (gates[2].inputA ^ gates[2].inputB) ? 1 : 0;

        gates[3].inputA = gates[2].output;
        gates[3].output = (gates[3].inputA == 0) ? 1 : 0;
    }
}

void Canvas::renderGrid(const Camera& camera) {
    float zoom = camera.getZoom();
    float panX = camera.getPanX();
    float panY = camera.getPanY();

    float halfW = (camera.getWidth() * 0.5f) / zoom;
    float halfH = (camera.getHeight() * 0.5f) / zoom;

    float minX = -halfW - panX;
    float maxX = halfW - panX;
    float minY = -halfH - panY;
    float maxY = halfH - panY;

    float gridSize = 40.0f;

    // Minor Grid Lines (GL_LINES)
    glLineWidth(1.0f);
    glColor3f(0.12f, 0.16f, 0.22f); // Subtle slate blue grid
    glBegin(GL_LINES);

    float startX = floorf(minX / gridSize) * gridSize;
    for (float x = startX; x <= maxX; x += gridSize) {
        glVertex2f(x, minY);
        glVertex2f(x, maxY);
    }

    float startY = floorf(minY / gridSize) * gridSize;
    for (float y = startY; y <= maxY; y += gridSize) {
        glVertex2f(minX, y);
        glVertex2f(maxX, y);
    }
    glEnd();

    // Major Axes (X = 0, Y = 0)
    glColor3f(0.20f, 0.28f, 0.38f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(minX, 0.0f); glVertex2f(maxX, 0.0f);
    glVertex2f(0.0f, minY); glVertex2f(0.0f, maxY);
    glEnd();
    glLineWidth(1.0f);
}

void Canvas::renderWires() {
    if (gateCount < 4) return;
    // Exact sub-pixel coordinate alignment using GateRenderer::getGatePinPosition (CO4)
    // Preserves wire connectivity across gate rotations and reflections
    float x1, y1, x2, y2;

    // Wire 1: Gate 1 (AND) OUT -> Gate 2 (OR) Pin 0
    GateRenderer::getGatePinPosition(gates[0].type, gates[0].x, gates[0].y, gates[0].width, gates[0].height, 0, true, &x1, &y1,
                                    gates[0].rotation, gates[0].flipH, gates[0].flipV, gates[0].scaleX, gates[0].scaleY);
    GateRenderer::getGatePinPosition(gates[1].type, gates[1].x, gates[1].y, gates[1].width, gates[1].height, 0, false, &x2, &y2,
                                    gates[1].rotation, gates[1].flipH, gates[1].flipV, gates[1].scaleX, gates[1].scaleY);
    GateRenderer::drawWire(x1, y1, x2, y2, gates[0].output);

    // Wire 2: Gate 2 (OR) OUT -> Gate 3 (XOR) Pin 0
    GateRenderer::getGatePinPosition(gates[1].type, gates[1].x, gates[1].y, gates[1].width, gates[1].height, 0, true, &x1, &y1,
                                    gates[1].rotation, gates[1].flipH, gates[1].flipV, gates[1].scaleX, gates[1].scaleY);
    GateRenderer::getGatePinPosition(gates[2].type, gates[2].x, gates[2].y, gates[2].width, gates[2].height, 0, false, &x2, &y2,
                                    gates[2].rotation, gates[2].flipH, gates[2].flipV, gates[2].scaleX, gates[2].scaleY);
    GateRenderer::drawWire(x1, y1, x2, y2, gates[1].output);

    // Wire 3: Gate 3 (XOR) OUT -> Gate 4 (NOT) Pin 0
    GateRenderer::getGatePinPosition(gates[2].type, gates[2].x, gates[2].y, gates[2].width, gates[2].height, 0, true, &x1, &y1,
                                    gates[2].rotation, gates[2].flipH, gates[2].flipV, gates[2].scaleX, gates[2].scaleY);
    GateRenderer::getGatePinPosition(gates[3].type, gates[3].x, gates[3].y, gates[3].width, gates[3].height, 0, false, &x2, &y2,
                                    gates[3].rotation, gates[3].flipH, gates[3].flipV, gates[3].scaleX, gates[3].scaleY);
    GateRenderer::drawWire(x1, y1, x2, y2, gates[2].output);
}

void Canvas::render(const Camera& camera) {
    if (mode3D) {
        render3DView(camera.getWidth(), camera.getHeight());
        return;
    }

    // Clear canvas background
    glClearColor(0.07f, 0.09f, 0.13f, 1.0f); // Sleek modern dark IDE background
    glClear(GL_COLOR_BUFFER_BIT);

    float zoom = camera.getZoom();
    float panX = camera.getPanX();
    float panY = camera.getPanY();

    float halfW = (camera.getWidth() * 0.5f) / zoom;
    float halfH = (camera.getHeight() * 0.5f) / zoom;

    float minX = -halfW - panX;
    float maxX = halfW - panX;
    float minY = -halfH - panY;
    float maxY = halfH - panY;

    // 0. Render Textured Circuit Board Substrate (CO6 Texture Mapping)
    GateRenderer::drawTexturedSubstrate(minX, minY, maxX, maxY);

    // 1. Render Grid (CO1 Primitives)
    renderGrid(camera);

    // 2. Render Connecting Wires (Bresenham's Line Algorithm)
    renderWires();

    // 3. Render Gates with 2D Transformations (Rotation, Reflection, Scale, Translation)
    for (int i = 0; i < gateCount; ++i) {
        float gMinX = gates[i].x - gates[i].width * 0.5f - 35.0f;
        float gMaxX = gates[i].x + gates[i].width * 0.5f + 35.0f;
        float gMinY = gates[i].y - gates[i].height * 0.5f - 25.0f;
        float gMaxY = gates[i].y + gates[i].height * 0.5f + 15.0f;

        // Viewport Clipping check (CO2)
        if (!camera.isBoxVisible(gMinX, gMinY, gMaxX, gMaxY)) {
            continue; // Clipped out of current viewport
        }

        bool activeSelect = gates[i].isSelected || (gates[i].id == hoveredGateId);

        // Draw authentic logic gate symbol with full 2D transformation matrix
        GateRenderer::drawGate(gates[i].type, gates[i].x, gates[i].y, gates[i].width, gates[i].height, 
                               activeSelect, gates[i].inputA, gates[i].inputB, gates[i].output, gates[i].label,
                               gates[i].rotation, gates[i].flipH, gates[i].flipV, gates[i].scaleX, gates[i].scaleY);
    }
}

void Canvas::renderHUD(int windowWidth, int windowHeight, const Camera& camera) {
    // Switch to Screen-Space 2D Orthographic for overlay HUD
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, windowWidth, 0, windowHeight, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Top Header Banner
    glColor3f(0.10f, 0.14f, 0.20f);
    glBegin(GL_QUADS);
    glVertex2f(10.0f, windowHeight - 55.0f);
    glVertex2f(720.0f, windowHeight - 55.0f);
    glVertex2f(720.0f, windowHeight - 10.0f);
    glVertex2f(10.0f, windowHeight - 10.0f);
    glEnd();

    glColor3f(0.3f, 0.8f, 1.0f);
    GateRenderer::drawText("LOGICCRAFT: AUTHENTIC LOGIC GATE SYMBOLS (CO1, CO2, CO4, CO6)", 20.0f, windowHeight - 30.0f, 0.78f);

    glColor3f(0.2f, 0.9f, 0.4f);
    GateRenderer::drawText("STANDARD ANSI/IEEE 91-1984 SYMBOLS: AND | NAND | OR | NOR | XOR | XNOR | NOT", 20.0f, windowHeight - 48.0f, 0.62f);

    // Bottom Help Controls Box
    glColor3f(0.09f, 0.12f, 0.18f);
    glBegin(GL_QUADS);
    glVertex2f(10.0f, 10.0f);
    glVertex2f(820.0f, 10.0f);
    glVertex2f(820.0f, 140.0f);
    glVertex2f(10.0f, 140.0f);
    glEnd();

    glColor3f(0.8f, 0.85f, 0.95f);
    GateRenderer::drawText("[CONTROLS] [7]: ALL 7 GATES SHOWCASE | [4]: 4-GATE CASCADE | [G]: CYCLE GATE TYPE", 20.0f, 120.0f, 0.68f);
    GateRenderer::drawText("[CO2 TRANSFORMS] L-CLICK & DRAG: MOVE GATE | R-CLICK: PAN CANVAS | WHEEL / +/-: ZOOM", 20.0f, 100.0f, 0.68f);
    GateRenderer::drawText("[INTERACTION] KEYS 1-7: SELECT GATE | SPACEBAR: TOGGLE PIN SIGNAL (0/1)", 20.0f, 80.0f, 0.68f);

    char texBuf[90];
    snprintf(texBuf, sizeof(texBuf), "[CO6 TEXTURE]  [T] TOGGLE TEXTURE MAPPING: [%s] | [R] RESET VIEW", GateRenderer::areTexturesEnabled() ? "ENABLED" : "OFF");
    glColor3f(0.3f, 0.9f, 0.8f);
    GateRenderer::drawText(texBuf, 20.0f, 56.0f, 0.68f);

    char lightBuf[90];
    snprintf(lightBuf, sizeof(lightBuf), "[CO6 LIGHTING] [L] TOGGLE LIGHTING & GLOW:  [%s]", GateRenderer::isLightingEnabled() ? "ENABLED" : "OFF");
    glColor3f(0.9f, 0.85f, 0.2f);
    GateRenderer::drawText(lightBuf, 20.0f, 34.0f, 0.68f);

    // Camera Stats Box (Top Right)
    char statusBuf[128];
    snprintf(statusBuf, sizeof(statusBuf), "ZOOM: %.2fX | PAN: (%.0f, %.0f) | GATES: %d", camera.getZoom(), camera.getPanX(), camera.getPanY(), gateCount);
    glColor3f(0.09f, 0.12f, 0.18f);
    glBegin(GL_QUADS);
    glVertex2f(windowWidth - 360.0f, windowHeight - 45.0f);
    glVertex2f(windowWidth - 10.0f, windowHeight - 45.0f);
    glVertex2f(windowWidth - 10.0f, windowHeight - 10.0f);
    glVertex2f(windowWidth - 360.0f, windowHeight - 10.0f);
    glEnd();

    glColor3f(0.2f, 0.9f, 0.4f);
    GateRenderer::drawText(statusBuf, windowWidth - 350.0f, windowHeight - 32.0f, 0.72f);

    // Restore Projection & Modelview
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

int Canvas::pickGate(float worldX, float worldY) {
    for (int i = gateCount - 1; i >= 0; --i) {
        float halfW = gates[i].width * 0.5f + 15.0f;
        float halfH = gates[i].height * 0.5f + 10.0f;
        if (worldX >= gates[i].x - halfW && worldX <= gates[i].x + halfW &&
            worldY >= gates[i].y - halfH && worldY <= gates[i].y + halfH) {
            return gates[i].id;
        }
    }
    return -1;
}

void Canvas::selectGate(int id) {
    selectedGateId = id;
    for (int i = 0; i < gateCount; ++i) {
        gates[i].isSelected = (gates[i].id == id);
    }
}

void Canvas::moveSelectedGate(float dx, float dy) {
    if (selectedGateId == -1) return;
    for (int i = 0; i < gateCount; ++i) {
        if (gates[i].id == selectedGateId) {
            gates[i].x += dx;
            gates[i].y += dy;
            break;
        }
    }
}

void Canvas::setGatePosition(int id, float x, float y) {
    for (int i = 0; i < gateCount; ++i) {
        if (gates[i].id == id) {
            gates[i].x = x;
            gates[i].y = y;
            break;
        }
    }
}

void Canvas::onMouseDown(int button, float worldX, float worldY) {
    if (button == 0) { // Left Click
        int clickedId = pickGate(worldX, worldY);
        selectGate(clickedId);
        if (clickedId != -1) {
            isDraggingGate = true;
            dragStartGateX = worldX;
            dragStartGateY = worldY;
        }
    }
}

void Canvas::updateHover(float worldX, float worldY) {
    hoveredGateId = pickGate(worldX, worldY);
}

void Canvas::onMouseMove(float worldX, float worldY) {
    updateHover(worldX, worldY);
    if (isDraggingGate && selectedGateId != -1) {
        float dx = worldX - dragStartGateX;
        float dy = worldY - dragStartGateY;
        moveSelectedGate(dx, dy);
        dragStartGateX = worldX;
        dragStartGateY = worldY;
    }
}

void Canvas::onMouseUp(int button) {
    if (button == 0) {
        isDraggingGate = false;
    }
}

void Canvas::toggleSelectedGateInput() {
    if (selectedGateId == -1) return;
    for (int i = 0; i < gateCount; ++i) {
        if (gates[i].id == selectedGateId) {
            // Toggle primary input
            gates[i].inputA = (gates[i].inputA == 0) ? 1 : 0;
            break;
        }
    }
    evaluateCircuit();
}

void Canvas::cycleSelectedGateType() {
    if (selectedGateId == -1) return;
    for (int i = 0; i < gateCount; ++i) {
        if (gates[i].id == selectedGateId) {
            if (strcmp(gates[i].type, "AND") == 0) strcpy(gates[i].type, "NAND");
            else if (strcmp(gates[i].type, "NAND") == 0) strcpy(gates[i].type, "OR");
            else if (strcmp(gates[i].type, "OR") == 0) strcpy(gates[i].type, "NOR");
            else if (strcmp(gates[i].type, "NOR") == 0) strcpy(gates[i].type, "XOR");
            else if (strcmp(gates[i].type, "XOR") == 0) strcpy(gates[i].type, "XNOR");
            else if (strcmp(gates[i].type, "XNOR") == 0) strcpy(gates[i].type, "NOT");
            else strcpy(gates[i].type, "AND");
            break;
        }
    }
    evaluateCircuit();
}

int Canvas::getSelectedGateId() const { return selectedGateId; }
int Canvas::getGateCount() const { return gateCount; }

void Canvas::rotateSelectedGate(float deltaDegrees) {
    if (selectedGateId == -1) return;
    for (int i = 0; i < gateCount; ++i) {
        if (gates[i].id == selectedGateId) {
            gates[i].rotation = fmodf(gates[i].rotation + deltaDegrees, 360.0f);
            if (gates[i].rotation < 0.0f) gates[i].rotation += 360.0f;
            break;
        }
    }
}

void Canvas::reflectSelectedGateHorizontal() {
    if (selectedGateId == -1) return;
    for (int i = 0; i < gateCount; ++i) {
        if (gates[i].id == selectedGateId) {
            gates[i].flipH = !gates[i].flipH;
            break;
        }
    }
}

void Canvas::reflectSelectedGateVertical() {
    if (selectedGateId == -1) return;
    for (int i = 0; i < gateCount; ++i) {
        if (gates[i].id == selectedGateId) {
            gates[i].flipV = !gates[i].flipV;
            break;
        }
    }
}

void Canvas::scaleSelectedGate(float factorX, float factorY) {
    if (selectedGateId == -1) return;
    for (int i = 0; i < gateCount; ++i) {
        if (gates[i].id == selectedGateId) {
            gates[i].scaleX *= factorX;
            gates[i].scaleY *= factorY;
            break;
        }
    }
}

void Canvas::toggle3DMode() {
    mode3D = !mode3D;
}

bool Canvas::is3DModeActive() const {
    return mode3D;
}

void Canvas::rotate3D(float dYaw, float dPitch) {
    yaw3D += dYaw;
    pitch3D += dPitch;
    if (pitch3D > 85.0f) pitch3D = 85.0f;
    if (pitch3D < -85.0f) pitch3D = -85.0f;
}

void Canvas::zoom3D(float dDist) {
    distance3D += dDist;
    if (distance3D < 150.0f) distance3D = 150.0f;
    if (distance3D > 1800.0f) distance3D = 1800.0f;
}

void Canvas::reset3DView() {
    yaw3D = -25.0f;
    pitch3D = 30.0f;
    distance3D = 650.0f;
}

void Canvas::render3DView(int windowWidth, int windowHeight) {
    glClearColor(0.04f, 0.06f, 0.09f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = (float)windowWidth / (float)(windowHeight > 0 ? windowHeight : 1);
    float fov = 45.0f;
    float zNear = 10.0f;
    float zFar = 3000.0f;
    float halfH = tanf(fov * 3.14159265f / 360.0f) * zNear;
    float halfW = halfH * aspect;
    glFrustum(-halfW, halfW, -halfH, halfH, zNear, zFar);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // 3D Camera Orbit Transformation (Translation & Multi-Axis Rotation)
    glTranslatef(0.0f, 0.0f, -distance3D);
    glRotatef(pitch3D, 1.0f, 0.0f, 0.0f);
    glRotatef(yaw3D, 0.0f, 1.0f, 0.0f);

    // Render 3D Ground Substrate Grid
    glBegin(GL_LINES);
    glColor3f(0.10f, 0.16f, 0.22f);
    for (float g = -500.0f; g <= 500.0f; g += 50.0f) {
        glVertex3f(g, -500.0f, -22.0f);
        glVertex3f(g, 500.0f, -22.0f);
        glVertex3f(-500.0f, g, -22.0f);
        glVertex3f(500.0f, g, -22.0f);
    }
    glEnd();

    // Render 3D IC Gate Packages (DIP Package with Extruded Pins & Top LEDs)
    for (int i = 0; i < gateCount; ++i) {
        bool activeSelect = gates[i].isSelected || (gates[i].id == hoveredGateId);
        GateRenderer::draw3DGatePackage(gates[i].type, gates[i].x, gates[i].y, 0.0f,
                                      gates[i].width, gates[i].height, 22.0f,
                                      gates[i].inputA, gates[i].inputB, gates[i].output,
                                      activeSelect);
    }

    // Render 3D Circuit Interconnect Wires
    if (gateCount >= 4) {
        float x1, y1, x2, y2;
        glLineWidth(2.5f);
        glBegin(GL_LINES);
        for (int c = 0; c < 3; ++c) {
            int srcIdx = c;
            int dstIdx = c + 1;
            GateRenderer::getGatePinPosition(gates[srcIdx].type, gates[srcIdx].x, gates[srcIdx].y, gates[srcIdx].width, gates[srcIdx].height, 0, true, &x1, &y1);
            GateRenderer::getGatePinPosition(gates[dstIdx].type, gates[dstIdx].x, gates[dstIdx].y, gates[dstIdx].width, gates[dstIdx].height, 0, false, &x2, &y2);
            if (gates[srcIdx].output == 1) glColor3f(0.1f, 1.0f, 0.4f);
            else glColor3f(0.35f, 0.5f, 0.7f);
            glVertex3f(x1, y1, 0.0f);
            glVertex3f(x2, y2, 0.0f);
        }
        glEnd();
        glLineWidth(1.0f);
    }

    glDisable(GL_DEPTH_TEST);
}

const VisualGate* Canvas::getGate(int index) const {
    if (index >= 0 && index < gateCount) return &gates[index];
    return nullptr;
}

VisualGate* Canvas::getSelectedGate() {
    if (selectedGateId == -1) return nullptr;
    for (int i = 0; i < gateCount; ++i) {
        if (gates[i].id == selectedGateId) return &gates[i];
    }
    return nullptr;
}
