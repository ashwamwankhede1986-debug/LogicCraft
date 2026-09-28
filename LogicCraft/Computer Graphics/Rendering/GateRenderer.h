#ifndef GATE_RENDERER_H
#define GATE_RENDERER_H

#include <windows.h>
#include <GL/gl.h>

/**
 * @file GateRenderer.h
 * @brief LogicCraft OpenGL Primitives Renderer (CO1 - OpenGL Primitives)
 *
 * Renders:
 * - AND gate symbol (GL_POLYGON, GL_LINE_STRIP, GL_TRIANGLE_FAN)
 * - OR gate symbol (GL_LINE_STRIP, GL_TRIANGLES)
 * - NOT gate symbol (GL_TRIANGLES, GL_LINE_LOOP circle bubble)
 * - Input/Output terminals & pin pads
 * - Connecting wires (GL_LINES)
 * - Vector stroke labels
 */

class GateRenderer {
public:
    static void drawCircle(float cx, float cy, float r, int num_segments, bool filled);
    static void drawText(const char* text, float x, float y, float scale = 1.0f);

    // Gate Rendering Methods (CO1)
    static void drawANDGate(float x, float y, float width, float height, bool isSelected, int inputA = 1, int inputB = 0, int output = 0);
    static void drawORGate(float x, float y, float width, float height, bool isSelected, int inputA = 0, int inputB = 1, int output = 1);
    static void drawNOTGate(float x, float y, float width, float height, bool isSelected, int input = 1, int output = 0);

    // Wire Connection Rendering (CO1)
    static void drawWire(float x1, float y1, float x2, float y2, int signalState = 0);

    // Pin Terminal
    static void drawTerminal(float x, float y, const char* label, int state, bool isInput);
};

#endif // GATE_RENDERER_H
