#ifndef GATE_RENDERER_H
#define GATE_RENDERER_H

#include <windows.h>
#include <GL/gl.h>
#include <vector>
#include "../Algorithms/CGAlgorithms.h"

/**
 * @file GateRenderer.h
 * @brief LogicCraft OpenGL Primitives & Logic Gate Symbol Renderer
 * 
 * Integrated with fundamental Computer Graphics algorithms:
 * - Bresenham's Line Algorithm: circuit wire routing & orthogonal traces
 * - Midpoint Circle Algorithm: circular terminals, indicators & inversion bubbles
 * - Scanline Polygon Filling: logic gate bodies and IC packages
 * - 2D Transformations: translation, scaling, rotation, reflection
 * - 3D Transformations: 3D IC package extrusion, perspective projection, camera orbit
 */

class GateRenderer {
private:
    static GLuint boardTextureId;
    static GLuint chipTextureId;
    static bool texturesEnabled;
    static bool lightingEnabled;

public:
    // Core Primitive Drawing via Fundamental Algorithms
    static void drawCircle(float cx, float cy, float r, int num_segments = 20, bool filled = true);
    static void drawMidpointCircle(float cx, float cy, float r, bool filled = true);
    static void drawText(const char* text, float x, float y, float scale = 1.0f);
    static void drawInversionBubble(float cx, float cy, float r, bool isSelected = false);
    static void drawScanlinePolygon(const std::vector<CG::Point2D>& vertices);

    // Gate Rendering Methods
    static void drawANDGate(float x, float y, float width, float height, bool isSelected, int inputA = 1, int inputB = 0, int output = 0, const char* name = nullptr);
    static void drawNANDGate(float x, float y, float width, float height, bool isSelected, int inputA = 1, int inputB = 0, int output = 0, const char* name = nullptr);
    static void drawORGate(float x, float y, float width, float height, bool isSelected, int inputA = 0, int inputB = 1, int output = 1, const char* name = nullptr);
    static void drawNORGate(float x, float y, float width, float height, bool isSelected, int inputA = 0, int inputB = 1, int output = 0, const char* name = nullptr);
    static void drawNOTGate(float x, float y, float width, float height, bool isSelected, int input = 1, int output = 0, const char* name = nullptr);
    static void drawXORGate(float x, float y, float width, float height, bool isSelected, int inputA = 0, int inputB = 1, int output = 1, const char* name = nullptr);
    static void drawXNORGate(float x, float y, float width, float height, bool isSelected, int inputA = 0, int inputB = 1, int output = 0, const char* name = nullptr);

    // Master Dispatcher (with 2D Component Transformations)
    static void drawGate(const char* type, float x, float y, float width, float height, bool isSelected, 
                         int inputA, int inputB, int output, const char* name = nullptr,
                         float rotation = 0.0f, bool flipH = false, bool flipV = false, float scaleX = 1.0f, float scaleY = 1.0f);

    // Precise Terminal Coordinate Calculation (Preserves wire connectivity through 2D transformations)
    static void getGatePinPosition(const char* type, float gateX, float gateY, float width, float height, 
                                   int pinIndex, bool isOutput, float* outX, float* outY,
                                   float rotation = 0.0f, bool flipH = false, bool flipV = false, float scaleX = 1.0f, float scaleY = 1.0f);

    // Wire Connection Rendering via Bresenham's Line Algorithm
    static void drawWire(float x1, float y1, float x2, float y2, int signalState = 0);
    static void drawBresenhamWire(float x1, float y1, float x2, float y2, int signalState = 0);

    // Pin Terminal
    static void drawTerminal(float x, float y, const char* label, int state, bool isInput);

    // 3D Visualization Mode
    static void draw3DGatePackage(const char* type, float x, float y, float z, float width, float height, float depth,
                                  int inputA, int inputB, int output, bool isSelected);

    // Texture Mapping & Lighting Effects
    static bool initTextures();
    static bool initLighting();
    static void toggleTextures();
    static void toggleLighting();
    static bool areTexturesEnabled();
    static bool isLightingEnabled();
    static void setTextureMode(bool enable);
    static void setLightingMode(bool enable);

    static void bindBoardTexture();
    static void bindChipTexture();
    static void unbindTexture();

    static void applyMaterial(bool isSelected, bool isActiveSignal);
    static void resetMaterial();

    static void drawTexturedSubstrate(float minX, float minY, float maxX, float maxY);
};

#endif // GATE_RENDERER_H
