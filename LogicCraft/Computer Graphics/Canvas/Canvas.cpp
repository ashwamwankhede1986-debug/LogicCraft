#include "Canvas.h"
#include "../Rendering/GateRenderer.h"
#include <iostream>
#include <cmath>
#include <cstring>
#include <cstdio>

Canvas::Canvas() : gateCount(0), selectedGateId(-1), isDraggingGate(false), dragStartGateX(0), dragStartGateY(0) {
    initDefaultCircuit();
}

void Canvas::initDefaultCircuit() {
    gateCount = 3;

    // Gate 1: AND Gate
    gates[0].id = 1;
    strcpy(gates[0].type, "AND");
    strcpy(gates[0].label, "Primary_AND");
    gates[0].x = -240.0f;
    gates[0].y = 80.0f;
    gates[0].width = 90.0f;
    gates[0].height = 60.0f;
    gates[0].isSelected = false;
    gates[0].inputA = 1;
    gates[0].inputB = 0;
    gates[0].output = 0;

    // Gate 2: OR Gate
    gates[1].id = 2;
    strcpy(gates[1].type, "OR");
    strcpy(gates[1].label, "Secondary_OR");
    gates[1].x = 60.0f;
    gates[1].y = -40.0f;
    gates[1].width = 90.0f;
    gates[1].height = 60.0f;
    gates[1].isSelected = false;
    gates[1].inputA = 0; // Tied to Gate 1 output
    gates[1].inputB = 1; // External Input C
    gates[1].output = 1;

    // Gate 3: NOT Gate
    gates[2].id = 3;
    strcpy(gates[2].type, "NOT");
    strcpy(gates[2].label, "Output_Inverter");
    gates[2].x = 320.0f;
    gates[2].y = -40.0f;
    gates[2].width = 75.0f;
    gates[2].height = 50.0f;
    gates[2].isSelected = false;
    gates[2].inputA = 1; // Tied to Gate 2 output
    gates[2].inputB = 0;
    gates[2].output = 0;

    evaluateCircuit();
}

void Canvas::evaluateCircuit() {
    // Gate 1 (AND)
    gates[0].output = (gates[0].inputA && gates[0].inputB) ? 1 : 0;

    // Gate 2 (OR): inputA comes from Gate 1 output
    gates[1].inputA = gates[0].output;
    gates[1].output = (gates[1].inputA || gates[1].inputB) ? 1 : 0;

    // Gate 3 (NOT): inputA comes from Gate 2 output
    gates[2].inputA = gates[1].output;
    gates[2].output = (gates[2].inputA == 0) ? 1 : 0;
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
    // Wire 1: Gate 1 OUT -> Gate 2 Pin A
    float g1OutX = gates[0].x + gates[0].width * 0.5f + 22.0f;
    float g1OutY = gates[0].y;

    float g2InX = gates[1].x - gates[1].width * 0.5f - 22.0f;
    float g2InY = gates[1].y + gates[1].height * 0.5f * 0.45f;

    GateRenderer::drawWire(g1OutX, g1OutY, g2InX, g2InY, gates[0].output);

    // Wire 2: Gate 2 OUT -> Gate 3 Pin IN
    float g2OutX = gates[1].x + gates[1].width * 0.5f + 22.0f;
    float g2OutY = gates[1].y;

    float g3InX = gates[2].x - gates[2].width * 0.5f - 22.0f;
    float g3InY = gates[2].y;

    GateRenderer::drawWire(g2OutX, g2OutY, g3InX, g3InY, gates[1].output);
}

void Canvas::render(const Camera& camera) {
    // Clear canvas background
    glClearColor(0.07f, 0.09f, 0.13f, 1.0f); // Sleek modern dark IDE background
    glClear(GL_COLOR_BUFFER_BIT);

    // 1. Render Grid (CO1 Primitives)
    renderGrid(camera);

    // 2. Render Connecting Wires (CO1 Primitives)
    renderWires();

    // 3. Render Gates with Viewport Clipping (CO2 Clipping & Transformations)
    for (int i = 0; i < gateCount; ++i) {
        float minX = gates[i].x - gates[i].width * 0.5f - 30.0f;
        float maxX = gates[i].x + gates[i].width * 0.5f + 30.0f;
        float minY = gates[i].y - gates[i].height * 0.5f - 10.0f;
        float maxY = gates[i].y + gates[i].height * 0.5f + 10.0f;

        // Viewport Clipping check (CO2)
        if (!camera.isBoxVisible(minX, minY, maxX, maxY)) {
            continue; // Clipped out of current viewport
        }

        glPushMatrix();
        // Model Transformation (Translation)
        glTranslatef(gates[i].x, gates[i].y, 0.0f);

        if (strcmp(gates[i].type, "AND") == 0) {
            GateRenderer::drawANDGate(0, 0, gates[i].width, gates[i].height, gates[i].isSelected, gates[i].inputA, gates[i].inputB, gates[i].output);
        } else if (strcmp(gates[i].type, "OR") == 0) {
            GateRenderer::drawORGate(0, 0, gates[i].width, gates[i].height, gates[i].isSelected, gates[i].inputA, gates[i].inputB, gates[i].output);
        } else if (strcmp(gates[i].type, "NOT") == 0) {
            GateRenderer::drawNOTGate(0, 0, gates[i].width, gates[i].height, gates[i].isSelected, gates[i].inputA, gates[i].output);
        }

        glPopMatrix();
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

    // 1. Top Header Banner
    glColor3f(0.12f, 0.15f, 0.22f);
    glBegin(GL_QUADS);
    glVertex2f(10.0f, windowHeight - 48.0f);
    glVertex2f(520.0f, windowHeight - 48.0f);
    glVertex2f(520.0f, windowHeight - 10.0f);
    glVertex2f(10.0f, windowHeight - 10.0f);
    glEnd();

    glColor3f(0.3f, 0.7f, 1.0f);
    GateRenderer::drawText("LOGICCRAFT: VISUAL LOGIC GATE SIMULATOR", 20.0f, windowHeight - 32.0f, 0.78f);

    // 2. Camera Stats Box (Top Right)
    char statusBuf[128];
    snprintf(statusBuf, sizeof(statusBuf), "ZOOM: %.2fX | PAN: (%.0f, %.0f)", camera.getZoom(), camera.getPanX(), camera.getPanY());
    glColor3f(0.09f, 0.12f, 0.18f);
    glBegin(GL_QUADS);
    glVertex2f(windowWidth - 310.0f, windowHeight - 45.0f);
    glVertex2f(windowWidth - 10.0f, windowHeight - 45.0f);
    glVertex2f(windowWidth - 10.0f, windowHeight - 10.0f);
    glVertex2f(windowWidth - 310.0f, windowHeight - 10.0f);
    glEnd();

    glColor3f(0.2f, 0.9f, 0.4f);
    GateRenderer::drawText(statusBuf, windowWidth - 300.0f, windowHeight - 32.0f, 0.75f);

    // 3. Bottom Help Controls Box
    glColor3f(0.09f, 0.12f, 0.18f);
    glBegin(GL_QUADS);
    glVertex2f(10.0f, 10.0f);
    glVertex2f(660.0f, 10.0f);
    glVertex2f(660.0f, 95.0f);
    glVertex2f(10.0f, 95.0f);
    glEnd();

    glColor3f(0.8f, 0.85f, 0.95f);
    GateRenderer::drawText("L-CLICK & DRAG : MOVE GATE", 20.0f, 74.0f, 0.7f);
    GateRenderer::drawText("R-CLICK DRAG / ARROWS : PAN CANVAS", 20.0f, 54.0f, 0.7f);
    GateRenderer::drawText("MOUSE WHEEL / +/- : ZOOM | R: RESET VIEW", 20.0f, 34.0f, 0.7f);
    GateRenderer::drawText("KEYS 1, 2, 3: SELECT GATE | SPACE: TOGGLE INPUT", 20.0f, 16.0f, 0.7f);

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

void Canvas::onMouseMove(float worldX, float worldY) {
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

int Canvas::getSelectedGateId() const { return selectedGateId; }
int Canvas::getGateCount() const { return gateCount; }
