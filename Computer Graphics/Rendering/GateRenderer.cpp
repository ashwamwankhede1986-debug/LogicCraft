#include "GateRenderer.h"
#include <cmath>
#include <cstring>
#include <vector>
#include <iostream>

const float PI = 3.14159265358979323846f;

GLuint GateRenderer::boardTextureId = 0;
GLuint GateRenderer::chipTextureId = 0;
bool GateRenderer::texturesEnabled = true;
bool GateRenderer::lightingEnabled = true;

void GateRenderer::drawCircle(float cx, float cy, float r, int num_segments, bool filled) {
    drawMidpointCircle(cx, cy, r, filled);
}

void GateRenderer::drawMidpointCircle(float cx, float cy, float r, bool filled) {
    if (filled) {
        auto spans = CG::midpointCircleFilledSpans((int)cx, (int)cy, (int)r);
        glBegin(GL_LINES);
        for (const auto& span : spans) {
            glVertex2f((float)span.xStart, (float)span.y);
            glVertex2f((float)span.xEnd, (float)span.y);
        }
        glEnd();
    } else {
        auto pts = CG::midpointCircle((int)cx, (int)cy, (int)r);
        glPointSize(1.5f);
        glBegin(GL_POINTS);
        for (const auto& pt : pts) {
            glVertex2f((float)pt.x, (float)pt.y);
        }
        glEnd();
        glPointSize(1.0f);
    }
}

void GateRenderer::drawScanlinePolygon(const std::vector<CG::Point2D>& vertices) {
    auto spans = CG::scanlineFillPolygon(vertices);
    glBegin(GL_LINES);
    for (const auto& span : spans) {
        glVertex2f((float)span.xStart, (float)span.y);
        glVertex2f((float)span.xEnd, (float)span.y);
    }
    glEnd();
}

// Vector Stroke font character renderer using pure GL_LINES
static void drawStrokeChar(char c, float x, float y, float s) {
    glBegin(GL_LINES);
    switch (c) {
        case 'A':
            glVertex2f(x, y); glVertex2f(x + 5*s, y + 14*s);
            glVertex2f(x + 5*s, y + 14*s); glVertex2f(x + 10*s, y);
            glVertex2f(x + 2.5f*s, y + 6*s); glVertex2f(x + 7.5f*s, y + 6*s);
            break;
        case 'B':
            glVertex2f(x, y); glVertex2f(x, y + 14*s);
            glVertex2f(x, y + 14*s); glVertex2f(x + 7*s, y + 14*s);
            glVertex2f(x + 7*s, y + 14*s); glVertex2f(x + 9*s, y + 10*s);
            glVertex2f(x + 9*s, y + 10*s); glVertex2f(x + 7*s, y + 7*s);
            glVertex2f(x + 7*s, y + 7*s); glVertex2f(x, y + 7*s);
            glVertex2f(x, y + 7*s); glVertex2f(x + 8*s, y + 7*s);
            glVertex2f(x + 8*s, y + 7*s); glVertex2f(x + 10*s, y + 3*s);
            glVertex2f(x + 10*s, y + 3*s); glVertex2f(x + 8*s, y);
            glVertex2f(x + 8*s, y); glVertex2f(x, y);
            break;
        case 'C':
            glVertex2f(x + 9*s, y + 14*s); glVertex2f(x + 2*s, y + 14*s);
            glVertex2f(x + 2*s, y + 14*s); glVertex2f(x, y + 10*s);
            glVertex2f(x, y + 10*s); glVertex2f(x, y + 4*s);
            glVertex2f(x, y + 4*s); glVertex2f(x + 2*s, y);
            glVertex2f(x + 2*s, y); glVertex2f(x + 9*s, y);
            break;
        case 'D':
            glVertex2f(x, y); glVertex2f(x, y + 14*s);
            glVertex2f(x, y + 14*s); glVertex2f(x + 6*s, y + 14*s);
            glVertex2f(x + 6*s, y + 14*s); glVertex2f(x + 10*s, y + 9*s);
            glVertex2f(x + 10*s, y + 9*s); glVertex2f(x + 10*s, y + 5*s);
            glVertex2f(x + 10*s, y + 5*s); glVertex2f(x + 6*s, y);
            glVertex2f(x + 6*s, y); glVertex2f(x, y);
            break;
        case 'E':
            glVertex2f(x, y); glVertex2f(x, y + 14*s);
            glVertex2f(x, y + 14*s); glVertex2f(x + 9*s, y + 14*s);
            glVertex2f(x, y + 7*s); glVertex2f(x + 7*s, y + 7*s);
            glVertex2f(x, y); glVertex2f(x + 9*s, y);
            break;
        case 'G':
            glVertex2f(x + 9*s, y + 14*s); glVertex2f(x + 2*s, y + 14*s);
            glVertex2f(x + 2*s, y + 14*s); glVertex2f(x, y + 9*s);
            glVertex2f(x, y + 9*s); glVertex2f(x, y + 5*s);
            glVertex2f(x, y + 5*s); glVertex2f(x + 2*s, y);
            glVertex2f(x + 2*s, y); glVertex2f(x + 9*s, y);
            glVertex2f(x + 9*s, y); glVertex2f(x + 9*s, y + 6*s);
            glVertex2f(x + 9*s, y + 6*s); glVertex2f(x + 5*s, y + 6*s);
            break;
        case 'I':
            glVertex2f(x + 2*s, y + 14*s); glVertex2f(x + 8*s, y + 14*s);
            glVertex2f(x + 5*s, y + 14*s); glVertex2f(x + 5*s, y);
            glVertex2f(x + 2*s, y); glVertex2f(x + 8*s, y);
            break;
        case 'L':
            glVertex2f(x, y + 14*s); glVertex2f(x, y);
            glVertex2f(x, y); glVertex2f(x + 8*s, y);
            break;
        case 'M':
            glVertex2f(x, y); glVertex2f(x, y + 14*s);
            glVertex2f(x, y + 14*s); glVertex2f(x + 5*s, y + 6*s);
            glVertex2f(x + 5*s, y + 6*s); glVertex2f(x + 10*s, y + 14*s);
            glVertex2f(x + 10*s, y + 14*s); glVertex2f(x + 10*s, y);
            break;
        case 'N':
            glVertex2f(x, y); glVertex2f(x, y + 14*s);
            glVertex2f(x, y + 14*s); glVertex2f(x + 9*s, y);
            glVertex2f(x + 9*s, y); glVertex2f(x + 9*s, y + 14*s);
            break;
        case 'O':
            glVertex2f(x + 2*s, y); glVertex2f(x + 8*s, y);
            glVertex2f(x + 8*s, y); glVertex2f(x + 10*s, y + 3*s);
            glVertex2f(x + 10*s, y + 3*s); glVertex2f(x + 10*s, y + 11*s);
            glVertex2f(x + 10*s, y + 11*s); glVertex2f(x + 8*s, y + 14*s);
            glVertex2f(x + 8*s, y + 14*s); glVertex2f(x + 2*s, y + 14*s);
            glVertex2f(x + 2*s, y + 14*s); glVertex2f(x, y + 11*s);
            glVertex2f(x, y + 11*s); glVertex2f(x, y + 3*s);
            glVertex2f(x, y + 3*s); glVertex2f(x + 2*s, y);
            break;
        case 'P':
            glVertex2f(x, y); glVertex2f(x, y + 14*s);
            glVertex2f(x, y + 14*s); glVertex2f(x + 7*s, y + 14*s);
            glVertex2f(x + 7*s, y + 14*s); glVertex2f(x + 9*s, y + 11*s);
            glVertex2f(x + 9*s, y + 11*s); glVertex2f(x + 7*s, y + 8*s);
            glVertex2f(x + 7*s, y + 8*s); glVertex2f(x, y + 8*s);
            break;
        case 'R':
            glVertex2f(x, y); glVertex2f(x, y + 14*s);
            glVertex2f(x, y + 14*s); glVertex2f(x + 7*s, y + 14*s);
            glVertex2f(x + 7*s, y + 14*s); glVertex2f(x + 9*s, y + 11*s);
            glVertex2f(x + 9*s, y + 11*s); glVertex2f(x + 7*s, y + 8*s);
            glVertex2f(x + 7*s, y + 8*s); glVertex2f(x, y + 8*s);
            glVertex2f(x + 5*s, y + 8*s); glVertex2f(x + 9*s, y);
            break;
        case 'S':
            glVertex2f(x + 9*s, y + 13*s); glVertex2f(x + 2*s, y + 14*s);
            glVertex2f(x + 2*s, y + 14*s); glVertex2f(x, y + 10*s);
            glVertex2f(x, y + 10*s); glVertex2f(x + 9*s, y + 6*s);
            glVertex2f(x + 9*s, y + 6*s); glVertex2f(x + 8*s, y);
            glVertex2f(x + 8*s, y); glVertex2f(x, y + 1*s);
            break;
        case 'T':
            glVertex2f(x, y + 14*s); glVertex2f(x + 10*s, y + 14*s);
            glVertex2f(x + 5*s, y + 14*s); glVertex2f(x + 5*s, y);
            break;
        case 'U':
            glVertex2f(x, y + 14*s); glVertex2f(x, y + 3*s);
            glVertex2f(x, y + 3*s); glVertex2f(x + 3*s, y);
            glVertex2f(x + 3*s, y); glVertex2f(x + 7*s, y);
            glVertex2f(x + 7*s, y); glVertex2f(x + 10*s, y + 3*s);
            glVertex2f(x + 10*s, y + 3*s); glVertex2f(x + 10*s, y + 14*s);
            break;
        case 'V':
            glVertex2f(x, y + 14*s); glVertex2f(x + 5*s, y);
            glVertex2f(x + 5*s, y); glVertex2f(x + 10*s, y + 14*s);
            break;
        case 'X':
            glVertex2f(x, y); glVertex2f(x + 10*s, y + 14*s);
            glVertex2f(x + 10*s, y); glVertex2f(x, y + 14*s);
            break;
        case 'Y':
            glVertex2f(x, y + 14*s); glVertex2f(x + 5*s, y + 7*s);
            glVertex2f(x + 10*s, y + 14*s); glVertex2f(x + 5*s, y + 7*s);
            glVertex2f(x + 5*s, y + 7*s); glVertex2f(x + 5*s, y);
            break;
        case 'Z':
            glVertex2f(x, y + 14*s); glVertex2f(x + 10*s, y + 14*s);
            glVertex2f(x + 10*s, y + 14*s); glVertex2f(x, y);
            glVertex2f(x, y); glVertex2f(x + 10*s, y);
            break;
        case '0':
            glVertex2f(x + 2*s, y); glVertex2f(x + 8*s, y);
            glVertex2f(x + 8*s, y); glVertex2f(x + 10*s, y + 3*s);
            glVertex2f(x + 10*s, y + 3*s); glVertex2f(x + 10*s, y + 11*s);
            glVertex2f(x + 10*s, y + 11*s); glVertex2f(x + 8*s, y + 14*s);
            glVertex2f(x + 8*s, y + 14*s); glVertex2f(x + 2*s, y + 14*s);
            glVertex2f(x + 2*s, y + 14*s); glVertex2f(x, y + 11*s);
            glVertex2f(x, y + 11*s); glVertex2f(x, y + 3*s);
            glVertex2f(x, y + 3*s); glVertex2f(x + 2*s, y);
            glVertex2f(x + 2*s, y + 2*s); glVertex2f(x + 8*s, y + 12*s);
            break;
        case '1':
            glVertex2f(x + 3*s, y + 10*s); glVertex2f(x + 6*s, y + 14*s);
            glVertex2f(x + 6*s, y + 14*s); glVertex2f(x + 6*s, y);
            glVertex2f(x + 2*s, y); glVertex2f(x + 10*s, y);
            break;
        case '2':
            glVertex2f(x + 1*s, y + 11*s); glVertex2f(x + 4*s, y + 14*s);
            glVertex2f(x + 4*s, y + 14*s); glVertex2f(x + 8*s, y + 14*s);
            glVertex2f(x + 8*s, y + 14*s); glVertex2f(x + 10*s, y + 10*s);
            glVertex2f(x + 10*s, y + 10*s); glVertex2f(x, y);
            glVertex2f(x, y); glVertex2f(x + 10*s, y);
            break;
        case '3':
            glVertex2f(x + 1*s, y + 14*s); glVertex2f(x + 9*s, y + 14*s);
            glVertex2f(x + 9*s, y + 14*s); glVertex2f(x + 5*s, y + 8*s);
            glVertex2f(x + 5*s, y + 8*s); glVertex2f(x + 9*s, y + 4*s);
            glVertex2f(x + 9*s, y + 4*s); glVertex2f(x + 8*s, y);
            glVertex2f(x + 8*s, y); glVertex2f(x + 1*s, y);
            break;
        case '4':
            glVertex2f(x + 8*s, y); glVertex2f(x + 8*s, y + 14*s);
            glVertex2f(x + 8*s, y + 14*s); glVertex2f(x, y + 5*s);
            glVertex2f(x, y + 5*s); glVertex2f(x + 10*s, y + 5*s);
            break;
        case '5':
            glVertex2f(x + 9*s, y + 14*s); glVertex2f(x, y + 14*s);
            glVertex2f(x, y + 14*s); glVertex2f(x, y + 8*s);
            glVertex2f(x, y + 8*s); glVertex2f(x + 8*s, y + 8*s);
            glVertex2f(x + 8*s, y + 8*s); glVertex2f(x + 10*s, y + 4*s);
            glVertex2f(x + 10*s, y + 4*s); glVertex2f(x + 7*s, y);
            glVertex2f(x + 7*s, y); glVertex2f(x, y);
            break;
        case '6':
            glVertex2f(x + 8*s, y + 14*s); glVertex2f(x + 2*s, y + 8*s);
            glVertex2f(x + 2*s, y + 8*s); glVertex2f(x + 2*s, y + 2*s);
            glVertex2f(x + 2*s, y + 2*s); glVertex2f(x + 6*s, y);
            glVertex2f(x + 6*s, y); glVertex2f(x + 9*s, y + 3*s);
            glVertex2f(x + 9*s, y + 3*s); glVertex2f(x + 9*s, y + 6*s);
            glVertex2f(x + 9*s, y + 6*s); glVertex2f(x + 2*s, y + 6*s);
            break;
        case '7':
            glVertex2f(x, y + 14*s); glVertex2f(x + 10*s, y + 14*s);
            glVertex2f(x + 10*s, y + 14*s); glVertex2f(x + 3*s, y);
            break;
        case '8':
            glVertex2f(x + 2*s, y + 7*s); glVertex2f(x + 8*s, y + 7*s);
            glVertex2f(x + 2*s, y + 7*s); glVertex2f(x, y + 11*s);
            glVertex2f(x, y + 11*s); glVertex2f(x + 3*s, y + 14*s);
            glVertex2f(x + 3*s, y + 14*s); glVertex2f(x + 7*s, y + 14*s);
            glVertex2f(x + 7*s, y + 14*s); glVertex2f(x + 10*s, y + 11*s);
            glVertex2f(x + 10*s, y + 11*s); glVertex2f(x + 8*s, y + 7*s);
            glVertex2f(x + 2*s, y + 7*s); glVertex2f(x, y + 3*s);
            glVertex2f(x, y + 3*s); glVertex2f(x + 3*s, y);
            glVertex2f(x + 3*s, y); glVertex2f(x + 7*s, y);
            glVertex2f(x + 7*s, y); glVertex2f(x + 10*s, y + 3*s);
            glVertex2f(x + 10*s, y + 3*s); glVertex2f(x + 8*s, y + 7*s);
            break;
        case '9':
            glVertex2f(x + 8*s, y + 8*s); glVertex2f(x + 1*s, y + 8*s);
            glVertex2f(x + 1*s, y + 8*s); glVertex2f(x + 1*s, y + 11*s);
            glVertex2f(x + 1*s, y + 11*s); glVertex2f(x + 4*s, y + 14*s);
            glVertex2f(x + 4*s, y + 14*s); glVertex2f(x + 8*s, y + 14*s);
            glVertex2f(x + 8*s, y + 14*s); glVertex2f(x + 10*s, y + 9*s);
            glVertex2f(x + 10*s, y + 9*s); glVertex2f(x + 8*s, y);
            glVertex2f(x + 8*s, y); glVertex2f(x + 2*s, y);
            break;
        case '-':
            glVertex2f(x + 1*s, y + 7*s); glVertex2f(x + 9*s, y + 7*s);
            break;
        case '_':
            glVertex2f(x, y); glVertex2f(x + 10*s, y);
            break;
        default:
            break;
    }
    glEnd();
}

void GateRenderer::drawText(const char* text, float x, float y, float scale) {
    if (!text) return;
    float curX = x;
    int len = (int)strlen(text);
    for (int i = 0; i < len; ++i) {
        char ch = text[i];
        if (ch >= 'a' && ch <= 'z') ch = ch - 'a' + 'A';
        drawStrokeChar(ch, curX, y, scale);
        curX += 13.0f * scale;
    }
}

void GateRenderer::drawTerminal(float x, float y, const char* label, int state, bool isInput) {
    // Solder pad ring
    glLineWidth(2.0f);
    glColor3f(0.85f, 0.90f, 0.95f);
    drawCircle(x, y, 5.0f, 16, false);

    // Terminal inner fill based on logic state
    if (state == 1) {
        glColor3f(0.10f, 0.95f, 0.40f); // Bright emerald green for Logic HIGH (1)
    } else {
        glColor3f(0.42f, 0.48f, 0.56f); // Technical slate grey for Logic LOW (0)
    }
    drawCircle(x, y, 4.0f, 14, true);

    // Glow effect if active HIGH
    if (state == 1) {
        glColor4f(0.10f, 0.95f, 0.40f, 0.4f);
        drawCircle(x, y, 8.0f, 16, false);
    }
    glLineWidth(1.0f);

    // Text Label (A, B, IN, OUT)
    if (label) {
        glColor3f(0.90f, 0.92f, 0.96f);
        float offset = isInput ? -18.0f : 10.0f;
        drawText(label, x + offset, y - 4.0f, 0.65f);
    }
}

void GateRenderer::drawWire(float x1, float y1, float x2, float y2, int signalState) {
    drawBresenhamWire(x1, y1, x2, y2, signalState);
}

void GateRenderer::drawBresenhamWire(float x1, float y1, float x2, float y2, int signalState) {
    if (signalState == 1) {
        glColor3f(0.1f, 1.0f, 0.4f); // Active green wire
    } else {
        glColor3f(0.35f, 0.5f, 0.7f); // Inactive cyan/slate wire
    }

    // Discrete pixel point rasterization via manually implemented Bresenham algorithm
    auto pts = CG::bresenhamSteppedWire((int)x1, (int)y1, (int)x2, (int)y2);
    glPointSize(2.2f);
    glBegin(GL_POINTS);
    for (const auto& pt : pts) {
        glVertex2f((float)pt.x, (float)pt.y);
    }
    glEnd();
    glPointSize(1.0f);

    // Indicator dot at midpoint rendered via Midpoint Circle algorithm
    glColor3f(1.0f, 1.0f, 0.2f);
    drawMidpointCircle((x1 + x2) * 0.5f, (y1 + y2) * 0.5f, 3.5f, true);
}

void GateRenderer::drawInversionBubble(float cx, float cy, float r, bool isSelected) {
    // 1. Dark interior fill via Midpoint Circle horizontal scan spans
    glColor3f(0.06f, 0.08f, 0.12f);
    drawMidpointCircle(cx, cy, r, true);

    // 2. Crisp outer boundary ring via Midpoint Circle 8-way symmetric pixels
    if (isSelected) {
        glColor3f(0.45f, 0.88f, 1.0f);
    } else {
        glColor3f(0.22f, 0.68f, 0.94f);
    }
    drawMidpointCircle(cx, cy, r, false);
}

// ------------------------------------------------------------------------------
// 1. AND GATE: D-shaped body (flat input side, semi-circular curved output side)
// ------------------------------------------------------------------------------
void GateRenderer::drawANDGate(float x, float y, float w, float h, bool isSelected, int inputA, int inputB, int output, const char* name) {
    float halfW = w * 0.5f;
    float halfH = h * 0.5f;
    float left = x - halfW;
    float midX = x;
    float right = x + halfW;
    float top = y + halfH;
    float bottom = y - halfH;

    // Body Fill
    if (isSelected) {
        glColor4f(0.12f, 0.22f, 0.35f, 0.92f);
    } else {
        glColor4f(0.08f, 0.12f, 0.18f, 0.88f);
    }

    // Flat rectangular section
    glBegin(GL_QUADS);
    glVertex2f(left, bottom);
    glVertex2f(midX, bottom);
    glVertex2f(midX, top);
    glVertex2f(left, top);
    glEnd();

    // Semicircular output curved section
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(midX, y);
    for (int i = -16; i <= 16; i++) {
        float angle = (float)i / 16.0f * (PI * 0.5f);
        float px = midX + halfW * cosf(angle);
        float py = y + halfH * sinf(angle);
        glVertex2f(px, py);
    }
    glEnd();

    // Border Outline
    glLineWidth(isSelected ? 3.0f : 2.2f);
    if (isSelected) {
        glColor3f(0.45f, 0.88f, 1.0f);
    } else {
        glColor3f(0.22f, 0.68f, 0.94f);
    }

    glBegin(GL_LINE_STRIP);
    glVertex2f(midX, top);
    glVertex2f(left, top);
    glVertex2f(left, bottom);
    glVertex2f(midX, bottom);
    for (int i = -16; i <= 16; i++) {
        float angle = (float)i / 16.0f * (PI * 0.5f);
        float px = midX + halfW * cosf(angle);
        float py = y + halfH * sinf(angle);
        glVertex2f(px, py);
    }
    glEnd();

    // Selected subtle glow pass
    if (isSelected) {
        glLineWidth(5.0f);
        glColor4f(0.3f, 0.75f, 1.0f, 0.35f);
        glBegin(GL_LINE_STRIP);
        glVertex2f(midX, top);
        glVertex2f(left, top);
        glVertex2f(left, bottom);
        glVertex2f(midX, bottom);
        for (int i = -16; i <= 16; i++) {
            float angle = (float)i / 16.0f * (PI * 0.5f);
            glVertex2f(midX + halfW * cosf(angle), y + halfH * sinf(angle));
        }
        glEnd();
    }
    glLineWidth(1.0f);

    // Input & Output Wire Stubs
    float pinY1 = y + halfH * 0.45f;
    float pinY2 = y - halfH * 0.45f;
    float pinLen = 22.0f;

    glColor3f(0.60f, 0.70f, 0.82f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(left - pinLen, pinY1); glVertex2f(left, pinY1);
    glVertex2f(left - pinLen, pinY2); glVertex2f(left, pinY2);
    glVertex2f(right, y);             glVertex2f(right + pinLen, y);
    glEnd();
    glLineWidth(1.0f);

    // Solder Terminal Pads
    drawTerminal(left - pinLen, pinY1, "A", inputA, true);
    drawTerminal(left - pinLen, pinY2, "B", inputB, true);
    drawTerminal(right + pinLen, y, "OUT", output, false);

    // Gate Type Symbol Label (inside gate)
    glColor3f(0.95f, 0.98f, 1.0f);
    drawText("AND", x - 18.0f, y - 5.0f, 0.75f);

    // Component Instance Name (below gate)
    if (name) {
        glColor3f(0.60f, 0.68f, 0.78f);
        drawText(name, x - 28.0f, y - halfH - 14.0f, 0.55f);
    }
}

// ------------------------------------------------------------------------------
// 2. NAND GATE: AND gate shape with inversion circle (bubble) at output
// ------------------------------------------------------------------------------
void GateRenderer::drawNANDGate(float x, float y, float w, float h, bool isSelected, int inputA, int inputB, int output, const char* name) {
    float halfW = w * 0.5f;
    float halfH = h * 0.5f;
    float bubbleR = 5.5f;
    float right = x + halfW;
    float left = x - halfW;
    float apexX = right - (bubbleR * 2.0f);
    float midX = (left + apexX) * 0.5f;
    float arcRx = apexX - midX;
    float top = y + halfH;
    float bottom = y - halfH;

    // Body Fill
    if (isSelected) {
        glColor4f(0.12f, 0.22f, 0.35f, 0.92f);
    } else {
        glColor4f(0.08f, 0.12f, 0.18f, 0.88f);
    }

    glBegin(GL_QUADS);
    glVertex2f(left, bottom);
    glVertex2f(midX, bottom);
    glVertex2f(midX, top);
    glVertex2f(left, top);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(midX, y);
    for (int i = -16; i <= 16; i++) {
        float angle = (float)i / 16.0f * (PI * 0.5f);
        glVertex2f(midX + arcRx * cosf(angle), y + halfH * sinf(angle));
    }
    glEnd();

    // Border Outline
    glLineWidth(isSelected ? 3.0f : 2.2f);
    if (isSelected) {
        glColor3f(0.45f, 0.88f, 1.0f);
    } else {
        glColor3f(0.22f, 0.68f, 0.94f);
    }

    glBegin(GL_LINE_STRIP);
    glVertex2f(midX, top);
    glVertex2f(left, top);
    glVertex2f(left, bottom);
    glVertex2f(midX, bottom);
    for (int i = -16; i <= 16; i++) {
        float angle = (float)i / 16.0f * (PI * 0.5f);
        glVertex2f(midX + arcRx * cosf(angle), y + halfH * sinf(angle));
    }
    glEnd();

    if (isSelected) {
        glLineWidth(5.0f);
        glColor4f(0.3f, 0.75f, 1.0f, 0.35f);
        glBegin(GL_LINE_STRIP);
        glVertex2f(midX, top);
        glVertex2f(left, top);
        glVertex2f(left, bottom);
        glVertex2f(midX, bottom);
        for (int i = -16; i <= 16; i++) {
            float angle = (float)i / 16.0f * (PI * 0.5f);
            glVertex2f(midX + arcRx * cosf(angle), y + halfH * sinf(angle));
        }
        glEnd();
    }
    glLineWidth(1.0f);

    // Inversion Bubble at output
    drawInversionBubble(right - bubbleR, y, bubbleR, isSelected);

    // Wire Stubs
    float pinY1 = y + halfH * 0.45f;
    float pinY2 = y - halfH * 0.45f;
    float pinLen = 22.0f;

    glColor3f(0.60f, 0.70f, 0.82f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(left - pinLen, pinY1); glVertex2f(left, pinY1);
    glVertex2f(left - pinLen, pinY2); glVertex2f(left, pinY2);
    glVertex2f(right, y);             glVertex2f(right + pinLen, y);
    glEnd();
    glLineWidth(1.0f);

    drawTerminal(left - pinLen, pinY1, "A", inputA, true);
    drawTerminal(left - pinLen, pinY2, "B", inputB, true);
    drawTerminal(right + pinLen, y, "OUT", output, false);

    glColor3f(0.95f, 0.98f, 1.0f);
    drawText("NAND", x - 22.0f, y - 5.0f, 0.70f);

    if (name) {
        glColor3f(0.60f, 0.68f, 0.78f);
        drawText(name, x - 28.0f, y - halfH - 14.0f, 0.55f);
    }
}

// ------------------------------------------------------------------------------
// 3. OR GATE: Curved concave input side, pointed convex sweeping output side
// ------------------------------------------------------------------------------
void GateRenderer::drawORGate(float x, float y, float w, float h, bool isSelected, int inputA, int inputB, int output, const char* name) {
    float halfW = w * 0.5f;
    float halfH = h * 0.5f;
    float left = x - halfW;
    float right = x + halfW;
    float top = y + halfH;
    float bottom = y - halfH;
    float depth = w * 0.22f;

    // Body Fill (Polygon perimeter)
    if (isSelected) {
        glColor4f(0.12f, 0.22f, 0.35f, 0.92f);
    } else {
        glColor4f(0.08f, 0.12f, 0.18f, 0.88f);
    }

    glBegin(GL_POLYGON);
    // Apex
    glVertex2f(right, y);
    // Top convex curve backwards
    for (int i = 20; i >= 0; i--) {
        float t = (float)i / 20.0f;
        float px = left + w * t;
        float py = top - halfH * (0.25f * t + 0.75f * t * t);
        glVertex2f(px, py);
    }
    // Concave back curve downwards
    for (int i = 16; i >= -16; i--) {
        float angle = (float)i / 16.0f * (PI * 0.5f);
        float px = left + depth * cosf(angle);
        float py = y + halfH * sinf(angle);
        glVertex2f(px, py);
    }
    // Bottom convex curve forward to apex
    for (int i = 0; i <= 20; i++) {
        float t = (float)i / 20.0f;
        float px = left + w * t;
        float py = bottom + halfH * (0.25f * t + 0.75f * t * t);
        glVertex2f(px, py);
    }
    glEnd();

    // Border Outline
    glLineWidth(isSelected ? 3.0f : 2.2f);
    if (isSelected) {
        glColor3f(0.45f, 0.88f, 1.0f);
    } else {
        glColor3f(0.22f, 0.68f, 0.94f);
    }

    // Continuous outline
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 20; i++) {
        float t = (float)i / 20.0f;
        glVertex2f(left + w * t, top - halfH * (0.25f * t + 0.75f * t * t));
    }
    for (int i = 20; i >= 0; i--) {
        float t = (float)i / 20.0f;
        glVertex2f(left + w * t, bottom + halfH * (0.25f * t + 0.75f * t * t));
    }
    glEnd();

    // Back curve
    glBegin(GL_LINE_STRIP);
    for (int i = -16; i <= 16; i++) {
        float angle = (float)i / 16.0f * (PI * 0.5f);
        glVertex2f(left + depth * cosf(angle), y + halfH * sinf(angle));
    }
    glEnd();

    if (isSelected) {
        glLineWidth(5.0f);
        glColor4f(0.3f, 0.75f, 1.0f, 0.35f);
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i <= 20; i++) {
            float t = (float)i / 20.0f;
            glVertex2f(left + w * t, top - halfH * (0.25f * t + 0.75f * t * t));
        }
        for (int i = 20; i >= 0; i--) {
            float t = (float)i / 20.0f;
            glVertex2f(left + w * t, bottom + halfH * (0.25f * t + 0.75f * t * t));
        }
        glEnd();
        glBegin(GL_LINE_STRIP);
        for (int i = -16; i <= 16; i++) {
            float angle = (float)i / 16.0f * (PI * 0.5f);
            glVertex2f(left + depth * cosf(angle), y + halfH * sinf(angle));
        }
        glEnd();
    }
    glLineWidth(1.0f);

    // Wire Stubs (connecting precisely to curved back)
    float pinY1 = y + halfH * 0.45f;
    float pinY2 = y - halfH * 0.45f;
    float backX1 = left + depth * sqrtf(1.0f - 0.45f * 0.45f);
    float backX2 = backX1;
    float pinLen = 22.0f;

    glColor3f(0.60f, 0.70f, 0.82f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(left - pinLen, pinY1); glVertex2f(backX1, pinY1);
    glVertex2f(left - pinLen, pinY2); glVertex2f(backX2, pinY2);
    glVertex2f(right, y);             glVertex2f(right + pinLen, y);
    glEnd();
    glLineWidth(1.0f);

    drawTerminal(left - pinLen, pinY1, "A", inputA, true);
    drawTerminal(left - pinLen, pinY2, "B", inputB, true);
    drawTerminal(right + pinLen, y, "OUT", output, false);

    glColor3f(0.95f, 0.98f, 1.0f);
    drawText("OR", x - 12.0f, y - 5.0f, 0.75f);

    if (name) {
        glColor3f(0.60f, 0.68f, 0.78f);
        drawText(name, x - 28.0f, y - halfH - 14.0f, 0.55f);
    }
}

// ------------------------------------------------------------------------------
// 4. NOR GATE: OR gate shape with inversion circle (bubble) at output
// ------------------------------------------------------------------------------
void GateRenderer::drawNORGate(float x, float y, float w, float h, bool isSelected, int inputA, int inputB, int output, const char* name) {
    float halfW = w * 0.5f;
    float halfH = h * 0.5f;
    float bubbleR = 5.5f;
    float left = x - halfW;
    float right = x + halfW;
    float apexX = right - (bubbleR * 2.0f);
    float bodyW = apexX - left;
    float top = y + halfH;
    float bottom = y - halfH;
    float depth = w * 0.22f;

    // Body Fill
    if (isSelected) {
        glColor4f(0.12f, 0.22f, 0.35f, 0.92f);
    } else {
        glColor4f(0.08f, 0.12f, 0.18f, 0.88f);
    }

    glBegin(GL_POLYGON);
    glVertex2f(apexX, y);
    for (int i = 20; i >= 0; i--) {
        float t = (float)i / 20.0f;
        float px = left + bodyW * t;
        float py = top - halfH * (0.25f * t + 0.75f * t * t);
        glVertex2f(px, py);
    }
    for (int i = 16; i >= -16; i--) {
        float angle = (float)i / 16.0f * (PI * 0.5f);
        float px = left + depth * cosf(angle);
        float py = y + halfH * sinf(angle);
        glVertex2f(px, py);
    }
    for (int i = 0; i <= 20; i++) {
        float t = (float)i / 20.0f;
        float px = left + bodyW * t;
        float py = bottom + halfH * (0.25f * t + 0.75f * t * t);
        glVertex2f(px, py);
    }
    glEnd();

    // Border Outline
    glLineWidth(isSelected ? 3.0f : 2.2f);
    if (isSelected) {
        glColor3f(0.45f, 0.88f, 1.0f);
    } else {
        glColor3f(0.22f, 0.68f, 0.94f);
    }

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 20; i++) {
        float t = (float)i / 20.0f;
        glVertex2f(left + bodyW * t, top - halfH * (0.25f * t + 0.75f * t * t));
    }
    for (int i = 20; i >= 0; i--) {
        float t = (float)i / 20.0f;
        glVertex2f(left + bodyW * t, bottom + halfH * (0.25f * t + 0.75f * t * t));
    }
    glEnd();

    glBegin(GL_LINE_STRIP);
    for (int i = -16; i <= 16; i++) {
        float angle = (float)i / 16.0f * (PI * 0.5f);
        glVertex2f(left + depth * cosf(angle), y + halfH * sinf(angle));
    }
    glEnd();

    if (isSelected) {
        glLineWidth(5.0f);
        glColor4f(0.3f, 0.75f, 1.0f, 0.35f);
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i <= 20; i++) {
            float t = (float)i / 20.0f;
            glVertex2f(left + bodyW * t, top - halfH * (0.25f * t + 0.75f * t * t));
        }
        for (int i = 20; i >= 0; i--) {
            float t = (float)i / 20.0f;
            glVertex2f(left + bodyW * t, bottom + halfH * (0.25f * t + 0.75f * t * t));
        }
        glEnd();
    }
    glLineWidth(1.0f);

    // Inversion Bubble
    drawInversionBubble(right - bubbleR, y, bubbleR, isSelected);

    // Wire Stubs
    float pinY1 = y + halfH * 0.45f;
    float pinY2 = y - halfH * 0.45f;
    float backX1 = left + depth * sqrtf(1.0f - 0.45f * 0.45f);
    float pinLen = 22.0f;

    glColor3f(0.60f, 0.70f, 0.82f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(left - pinLen, pinY1); glVertex2f(backX1, pinY1);
    glVertex2f(left - pinLen, pinY2); glVertex2f(backX1, pinY2);
    glVertex2f(right, y);             glVertex2f(right + pinLen, y);
    glEnd();
    glLineWidth(1.0f);

    drawTerminal(left - pinLen, pinY1, "A", inputA, true);
    drawTerminal(left - pinLen, pinY2, "B", inputB, true);
    drawTerminal(right + pinLen, y, "OUT", output, false);

    glColor3f(0.95f, 0.98f, 1.0f);
    drawText("NOR", x - 18.0f, y - 5.0f, 0.75f);

    if (name) {
        glColor3f(0.60f, 0.68f, 0.78f);
        drawText(name, x - 28.0f, y - halfH - 14.0f, 0.55f);
    }
}

// ------------------------------------------------------------------------------
// 5. NOT GATE: Triangle with inversion circle (bubble) at output
// ------------------------------------------------------------------------------
void GateRenderer::drawNOTGate(float x, float y, float w, float h, bool isSelected, int input, int output, const char* name) {
    float halfW = w * 0.5f;
    float halfH = h * 0.5f;
    float bubbleR = 5.5f;
    float left = x - halfW;
    float right = x + halfW;
    float apexX = right - (bubbleR * 2.0f);
    float top = y + halfH;
    float bottom = y - halfH;

    // Body Fill (GL_TRIANGLES)
    if (isSelected) {
        glColor4f(0.12f, 0.22f, 0.35f, 0.92f);
    } else {
        glColor4f(0.08f, 0.12f, 0.18f, 0.88f);
    }

    glBegin(GL_TRIANGLES);
    glVertex2f(left, top);
    glVertex2f(left, bottom);
    glVertex2f(apexX, y);
    glEnd();

    // Outline
    glLineWidth(isSelected ? 3.0f : 2.2f);
    if (isSelected) {
        glColor3f(0.45f, 0.88f, 1.0f);
    } else {
        glColor3f(0.22f, 0.68f, 0.94f);
    }

    glBegin(GL_LINE_LOOP);
    glVertex2f(left, top);
    glVertex2f(left, bottom);
    glVertex2f(apexX, y);
    glEnd();

    if (isSelected) {
        glLineWidth(5.0f);
        glColor4f(0.3f, 0.75f, 1.0f, 0.35f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(left, top);
        glVertex2f(left, bottom);
        glVertex2f(apexX, y);
        glEnd();
    }
    glLineWidth(1.0f);

    // Inversion Bubble
    drawInversionBubble(right - bubbleR, y, bubbleR, isSelected);

    // Input & Output Wire Stubs
    float pinLen = 22.0f;
    glColor3f(0.60f, 0.70f, 0.82f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(left - pinLen, y); glVertex2f(left, y);
    glVertex2f(right, y);         glVertex2f(right + pinLen, y);
    glEnd();
    glLineWidth(1.0f);

    drawTerminal(left - pinLen, y, "IN", input, true);
    drawTerminal(right + pinLen, y, "OUT", output, false);

    glColor3f(0.95f, 0.98f, 1.0f);
    drawText("NOT", x - 16.0f, y - 5.0f, 0.70f);

    if (name) {
        glColor3f(0.60f, 0.68f, 0.78f);
        drawText(name, x - 24.0f, y - halfH - 14.0f, 0.55f);
    }
}

// ------------------------------------------------------------------------------
// 6. XOR GATE: OR shape with additional detached curved input arc
// ------------------------------------------------------------------------------
void GateRenderer::drawXORGate(float x, float y, float w, float h, bool isSelected, int inputA, int inputB, int output, const char* name) {
    float halfW = w * 0.5f;
    float halfH = h * 0.5f;
    float left = x - halfW;
    float right = x + halfW;
    float top = y + halfH;
    float bottom = y - halfH;

    float arcOffset = 10.0f;
    float bodyLeft = left + arcOffset;
    float bodyW = w - arcOffset;
    float depth = bodyW * 0.22f;

    // Body Fill
    if (isSelected) {
        glColor4f(0.12f, 0.22f, 0.35f, 0.92f);
    } else {
        glColor4f(0.08f, 0.12f, 0.18f, 0.88f);
    }

    glBegin(GL_POLYGON);
    glVertex2f(right, y);
    for (int i = 20; i >= 0; i--) {
        float t = (float)i / 20.0f;
        float px = bodyLeft + bodyW * t;
        float py = top - halfH * (0.25f * t + 0.75f * t * t);
        glVertex2f(px, py);
    }
    for (int i = 16; i >= -16; i--) {
        float angle = (float)i / 16.0f * (PI * 0.5f);
        float px = bodyLeft + depth * cosf(angle);
        float py = y + halfH * sinf(angle);
        glVertex2f(px, py);
    }
    for (int i = 0; i <= 20; i++) {
        float t = (float)i / 20.0f;
        float px = bodyLeft + bodyW * t;
        float py = bottom + halfH * (0.25f * t + 0.75f * t * t);
        glVertex2f(px, py);
    }
    glEnd();

    // Body Outline
    glLineWidth(isSelected ? 3.0f : 2.2f);
    if (isSelected) {
        glColor3f(0.45f, 0.88f, 1.0f);
    } else {
        glColor3f(0.22f, 0.68f, 0.94f);
    }

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 20; i++) {
        float t = (float)i / 20.0f;
        glVertex2f(bodyLeft + bodyW * t, top - halfH * (0.25f * t + 0.75f * t * t));
    }
    for (int i = 20; i >= 0; i--) {
        float t = (float)i / 20.0f;
        glVertex2f(bodyLeft + bodyW * t, bottom + halfH * (0.25f * t + 0.75f * t * t));
    }
    glEnd();

    glBegin(GL_LINE_STRIP);
    for (int i = -16; i <= 16; i++) {
        float angle = (float)i / 16.0f * (PI * 0.5f);
        glVertex2f(bodyLeft + depth * cosf(angle), y + halfH * sinf(angle));
    }
    glEnd();

    // Additional Detached Curved Line on Input Side (Characteristic XOR Symbol)
    glBegin(GL_LINE_STRIP);
    for (int i = -16; i <= 16; i++) {
        float angle = (float)i / 16.0f * (PI * 0.5f);
        glVertex2f(left + depth * cosf(angle), y + halfH * sinf(angle));
    }
    glEnd();

    if (isSelected) {
        glLineWidth(5.0f);
        glColor4f(0.3f, 0.75f, 1.0f, 0.35f);
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i <= 20; i++) {
            float t = (float)i / 20.0f;
            glVertex2f(bodyLeft + bodyW * t, top - halfH * (0.25f * t + 0.75f * t * t));
        }
        for (int i = 20; i >= 0; i--) {
            float t = (float)i / 20.0f;
            glVertex2f(bodyLeft + bodyW * t, bottom + halfH * (0.25f * t + 0.75f * t * t));
        }
        glEnd();
        glBegin(GL_LINE_STRIP);
        for (int i = -16; i <= 16; i++) {
            float angle = (float)i / 16.0f * (PI * 0.5f);
            glVertex2f(left + depth * cosf(angle), y + halfH * sinf(angle));
        }
        glEnd();
    }
    glLineWidth(1.0f);

    // Wire Stubs (connecting to detached input arc)
    float pinY1 = y + halfH * 0.45f;
    float pinY2 = y - halfH * 0.45f;
    float arcX1 = left + depth * sqrtf(1.0f - 0.45f * 0.45f);
    float pinLen = 22.0f;

    glColor3f(0.60f, 0.70f, 0.82f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(left - pinLen, pinY1); glVertex2f(arcX1, pinY1);
    glVertex2f(left - pinLen, pinY2); glVertex2f(arcX1, pinY2);
    glVertex2f(right, y);             glVertex2f(right + pinLen, y);
    glEnd();
    glLineWidth(1.0f);

    drawTerminal(left - pinLen, pinY1, "A", inputA, true);
    drawTerminal(left - pinLen, pinY2, "B", inputB, true);
    drawTerminal(right + pinLen, y, "OUT", output, false);

    glColor3f(0.95f, 0.98f, 1.0f);
    drawText("XOR", x - 12.0f, y - 5.0f, 0.75f);

    if (name) {
        glColor3f(0.60f, 0.68f, 0.78f);
        drawText(name, x - 28.0f, y - halfH - 14.0f, 0.55f);
    }
}

// ------------------------------------------------------------------------------
// 7. XNOR GATE: XOR shape with inversion circle (bubble) at output
// ------------------------------------------------------------------------------
void GateRenderer::drawXNORGate(float x, float y, float w, float h, bool isSelected, int inputA, int inputB, int output, const char* name) {
    float halfW = w * 0.5f;
    float halfH = h * 0.5f;
    float bubbleR = 5.5f;
    float left = x - halfW;
    float right = x + halfW;
    float apexX = right - (bubbleR * 2.0f);
    float top = y + halfH;
    float bottom = y - halfH;

    float arcOffset = 10.0f;
    float bodyLeft = left + arcOffset;
    float bodyW = apexX - bodyLeft;
    float depth = bodyW * 0.22f;

    // Body Fill
    if (isSelected) {
        glColor4f(0.12f, 0.22f, 0.35f, 0.92f);
    } else {
        glColor4f(0.08f, 0.12f, 0.18f, 0.88f);
    }

    glBegin(GL_POLYGON);
    glVertex2f(apexX, y);
    for (int i = 20; i >= 0; i--) {
        float t = (float)i / 20.0f;
        float px = bodyLeft + bodyW * t;
        float py = top - halfH * (0.25f * t + 0.75f * t * t);
        glVertex2f(px, py);
    }
    for (int i = 16; i >= -16; i--) {
        float angle = (float)i / 16.0f * (PI * 0.5f);
        float px = bodyLeft + depth * cosf(angle);
        float py = y + halfH * sinf(angle);
        glVertex2f(px, py);
    }
    for (int i = 0; i <= 20; i++) {
        float t = (float)i / 20.0f;
        float px = bodyLeft + bodyW * t;
        float py = bottom + halfH * (0.25f * t + 0.75f * t * t);
        glVertex2f(px, py);
    }
    glEnd();

    // Body Outline
    glLineWidth(isSelected ? 3.0f : 2.2f);
    if (isSelected) {
        glColor3f(0.45f, 0.88f, 1.0f);
    } else {
        glColor3f(0.22f, 0.68f, 0.94f);
    }

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 20; i++) {
        float t = (float)i / 20.0f;
        glVertex2f(bodyLeft + bodyW * t, top - halfH * (0.25f * t + 0.75f * t * t));
    }
    for (int i = 20; i >= 0; i--) {
        float t = (float)i / 20.0f;
        glVertex2f(bodyLeft + bodyW * t, bottom + halfH * (0.25f * t + 0.75f * t * t));
    }
    glEnd();

    glBegin(GL_LINE_STRIP);
    for (int i = -16; i <= 16; i++) {
        float angle = (float)i / 16.0f * (PI * 0.5f);
        glVertex2f(bodyLeft + depth * cosf(angle), y + halfH * sinf(angle));
    }
    glEnd();

    // Detached Input Arc
    glBegin(GL_LINE_STRIP);
    for (int i = -16; i <= 16; i++) {
        float angle = (float)i / 16.0f * (PI * 0.5f);
        glVertex2f(left + depth * cosf(angle), y + halfH * sinf(angle));
    }
    glEnd();

    if (isSelected) {
        glLineWidth(5.0f);
        glColor4f(0.3f, 0.75f, 1.0f, 0.35f);
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i <= 20; i++) {
            float t = (float)i / 20.0f;
            glVertex2f(bodyLeft + bodyW * t, top - halfH * (0.25f * t + 0.75f * t * t));
        }
        for (int i = 20; i >= 0; i--) {
            float t = (float)i / 20.0f;
            glVertex2f(bodyLeft + bodyW * t, bottom + halfH * (0.25f * t + 0.75f * t * t));
        }
        glEnd();
        glBegin(GL_LINE_STRIP);
        for (int i = -16; i <= 16; i++) {
            float angle = (float)i / 16.0f * (PI * 0.5f);
            glVertex2f(left + depth * cosf(angle), y + halfH * sinf(angle));
        }
        glEnd();
    }
    glLineWidth(1.0f);

    // Inversion Bubble
    drawInversionBubble(right - bubbleR, y, bubbleR, isSelected);

    // Wire Stubs
    float pinY1 = y + halfH * 0.45f;
    float pinY2 = y - halfH * 0.45f;
    float arcX1 = left + depth * sqrtf(1.0f - 0.45f * 0.45f);
    float pinLen = 22.0f;

    glColor3f(0.60f, 0.70f, 0.82f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(left - pinLen, pinY1); glVertex2f(arcX1, pinY1);
    glVertex2f(left - pinLen, pinY2); glVertex2f(arcX1, pinY2);
    glVertex2f(right, y);             glVertex2f(right + pinLen, y);
    glEnd();
    glLineWidth(1.0f);

    drawTerminal(left - pinLen, pinY1, "A", inputA, true);
    drawTerminal(left - pinLen, pinY2, "B", inputB, true);
    drawTerminal(right + pinLen, y, "OUT", output, false);

    glColor3f(0.95f, 0.98f, 1.0f);
    drawText("XNOR", x - 20.0f, y - 5.0f, 0.70f);

    if (name) {
        glColor3f(0.60f, 0.68f, 0.78f);
        drawText(name, x - 28.0f, y - halfH - 14.0f, 0.55f);
    }
}

// ------------------------------------------------------------------------------
// MASTER GATE DISPATCHER (With 2D Component Transformations: Rotation & Reflection)
// ------------------------------------------------------------------------------
void GateRenderer::drawGate(const char* type, float x, float y, float width, float height, bool isSelected, 
                         int inputA, int inputB, int output, const char* name,
                         float rotation, bool flipH, bool flipV, float scaleX, float scaleY) {
    if (!type) return;

    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    if (rotation != 0.0f) {
        glRotatef(rotation, 0.0f, 0.0f, 1.0f);
    }
    if (flipH || flipV || scaleX != 1.0f || scaleY != 1.0f) {
        glScalef(flipH ? -scaleX : scaleX, flipV ? -scaleY : scaleY, 1.0f);
    }
    glTranslatef(-x, -y, 0.0f);

    if (strcmp(type, "AND") == 0) {
        drawANDGate(x, y, width, height, isSelected, inputA, inputB, output, name);
    } else if (strcmp(type, "NAND") == 0) {
        drawNANDGate(x, y, width, height, isSelected, inputA, inputB, output, name);
    } else if (strcmp(type, "OR") == 0) {
        drawORGate(x, y, width, height, isSelected, inputA, inputB, output, name);
    } else if (strcmp(type, "NOR") == 0) {
        drawNORGate(x, y, width, height, isSelected, inputA, inputB, output, name);
    } else if (strcmp(type, "XOR") == 0) {
        drawXORGate(x, y, width, height, isSelected, inputA, inputB, output, name);
    } else if (strcmp(type, "XNOR") == 0) {
        drawXNORGate(x, y, width, height, isSelected, inputA, inputB, output, name);
    } else if (strcmp(type, "NOT") == 0) {
        drawNOTGate(x, y, width, height, isSelected, inputA, output, name);
    } else {
        drawANDGate(x, y, width, height, isSelected, inputA, inputB, output, name);
    }

    glPopMatrix();
}

// ------------------------------------------------------------------------------
// PRECISE PIN COORDINATES (Preserves Wire Connectivity across Transformations)
// ------------------------------------------------------------------------------
void GateRenderer::getGatePinPosition(const char* type, float gateX, float gateY, float w, float h, 
                               int pinIndex, bool isOutput, float* outX, float* outY,
                               float rotation, bool flipH, bool flipV, float scaleX, float scaleY) {
    if (!outX || !outY) return;

    float halfW = w * 0.5f;
    float halfH = h * 0.5f;
    float pinLen = 22.0f;

    float localX = 0.0f;
    float localY = 0.0f;

    if (isOutput) {
        localX = halfW + pinLen;
        localY = 0.0f;
    } else {
        localX = -halfW - pinLen;
        if (type && std::strcmp(type, "NOT") == 0) {
            localY = 0.0f;
        } else {
            if (pinIndex == 0) {
                localY = halfH * 0.45f;
            } else {
                localY = -halfH * 0.45f;
            }
        }
    }

    // Apply 2D Transformation to pin coordinate relative to gate center
    CG::Point2Df transformed = CG::transformGatePoint(gateX + localX, gateY + localY, gateX, gateY,
                                                      rotation, flipH, flipV, scaleX, scaleY);
    *outX = transformed.x;
    *outY = transformed.y;
}

// ------------------------------------------------------------------------------
// 3D VISUALIZATION MODE: DUAL-IN-LINE (DIP) IC PACKAGE WITH EXTRUDED PINS
// ------------------------------------------------------------------------------
void GateRenderer::draw3DGatePackage(const char* type, float x, float y, float z, float width, float height, float depth,
                                  int inputA, int inputB, int output, bool isSelected) {
    glPushMatrix();
    glTranslatef(x, y, z);

    float halfW = width * 0.5f;
    float halfH = height * 0.5f;
    float halfD = depth * 0.5f;

    // DIP Package Main Body (Dark Epoxy Resin / Silicon Substrate)
    if (isSelected) {
        glColor3f(0.12f, 0.22f, 0.35f);
    } else {
        glColor3f(0.08f, 0.10f, 0.14f);
    }

    // Top face
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-halfW, -halfH, halfD);
    glVertex3f(halfW, -halfH, halfD);
    glVertex3f(halfW, halfH, halfD);
    glVertex3f(-halfW, halfH, halfD);

    // Bottom face
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-halfW, -halfH, -halfD);
    glVertex3f(-halfW, halfH, -halfD);
    glVertex3f(halfW, halfH, -halfD);
    glVertex3f(halfW, -halfH, -halfD);

    // Front face
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-halfW, -halfH, -halfD);
    glVertex3f(halfW, -halfH, -halfD);
    glVertex3f(halfW, -halfH, halfD);
    glVertex3f(-halfW, -halfH, halfD);

    // Back face
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-halfW, halfH, -halfD);
    glVertex3f(-halfW, halfH, halfD);
    glVertex3f(halfW, halfH, halfD);
    glVertex3f(halfW, halfH, -halfD);

    // Left face
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-halfW, -halfH, -halfD);
    glVertex3f(-halfW, -halfH, halfD);
    glVertex3f(-halfW, halfH, halfD);
    glVertex3f(-halfW, halfH, -halfD);

    // Right face
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(halfW, -halfH, -halfD);
    glVertex3f(halfW, halfH, -halfD);
    glVertex3f(halfW, halfH, halfD);
    glVertex3f(halfW, -halfH, halfD);
    glEnd();

    // 3D Silver Metallic Lead Pins on Left (Inputs) and Right (Output)
    glColor3f(0.78f, 0.82f, 0.90f); // Bright tin/silver leadframe
    float pinL = 16.0f;
    float pinThick = 3.5f;

    // Input Pin 1 (Top left)
    glBegin(GL_QUADS);
    glVertex3f(-halfW - pinL, halfH * 0.45f - pinThick*0.5f, 0.0f);
    glVertex3f(-halfW,        halfH * 0.45f - pinThick*0.5f, 0.0f);
    glVertex3f(-halfW,        halfH * 0.45f + pinThick*0.5f, 0.0f);
    glVertex3f(-halfW - pinL, halfH * 0.45f + pinThick*0.5f, 0.0f);

    // Input Pin 2 (Bottom left)
    glVertex3f(-halfW - pinL, -halfH * 0.45f - pinThick*0.5f, 0.0f);
    glVertex3f(-halfW,        -halfH * 0.45f - pinThick*0.5f, 0.0f);
    glVertex3f(-halfW,        -halfH * 0.45f + pinThick*0.5f, 0.0f);
    glVertex3f(-halfW - pinL, -halfH * 0.45f + pinThick*0.5f, 0.0f);

    // Output Pin (Right center)
    glVertex3f(halfW,        -pinThick*0.5f, 0.0f);
    glVertex3f(halfW + pinL, -pinThick*0.5f, 0.0f);
    glVertex3f(halfW + pinL, pinThick*0.5f,  0.0f);
    glVertex3f(halfW,        pinThick*0.5f,  0.0f);
    glEnd();

    // Signal LEDs on top face
    if (inputA == 1) glColor3f(0.1f, 1.0f, 0.4f); else glColor3f(0.3f, 0.35f, 0.4f);
    drawMidpointCircle(-halfW * 0.4f, halfH * 0.45f, 3.5f, true);

    if (inputB == 1) glColor3f(0.1f, 1.0f, 0.4f); else glColor3f(0.3f, 0.35f, 0.4f);
    drawMidpointCircle(-halfW * 0.4f, -halfH * 0.45f, 3.5f, true);

    if (output == 1) glColor3f(0.1f, 1.0f, 0.4f); else glColor3f(0.3f, 0.35f, 0.4f);
    drawMidpointCircle(halfW * 0.4f, 0.0f, 4.0f, true);

    // Gate Type Text on 3D Top Face
    glColor3f(0.85f, 0.90f, 0.95f);
    drawText(type ? type : "GATE", -16.0f, -4.0f, 0.6f);

    // Pin 1 Index Notch
    glColor3f(0.2f, 0.25f, 0.3f);
    drawMidpointCircle(-halfW, 0.0f, 4.0f, false);

    glPopMatrix();
}

// ==============================================================================
// CO6: TEXTURE MAPPING & LIGHTING EFFECTS IMPLEMENTATION
// ==============================================================================

bool GateRenderer::initTextures() {
    const int boardDim = 128;
    std::vector<unsigned char> boardPixels(boardDim * boardDim * 4);

    for (int r = 0; r < boardDim; r++) {
        for (int c = 0; c < boardDim; c++) {
            int idx = (r * boardDim + c) * 4;
            unsigned char red = 16;
            unsigned char green = 22;
            unsigned char blue = 30;

            if (r % 16 == 0 || c % 16 == 0) {
                red = 26; green = 38; blue = 52;
            }
            if ((r + c) % 32 == 0) {
                red = 30; green = 44; blue = 60;
            }
            if ((r % 32 == 16) && (c % 32 == 16)) {
                red = 45; green = 65; blue = 88;
            }

            boardPixels[idx + 0] = red;
            boardPixels[idx + 1] = green;
            boardPixels[idx + 2] = blue;
            boardPixels[idx + 3] = 255;
        }
    }

    if (boardTextureId == 0) {
        glGenTextures(1, &boardTextureId);
    }
    glBindTexture(GL_TEXTURE_2D, boardTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, boardDim, boardDim, 0, GL_RGBA, GL_UNSIGNED_BYTE, boardPixels.data());

    const int chipDim = 64;
    std::vector<unsigned char> chipPixels(chipDim * chipDim * 4);

    for (int r = 0; r < chipDim; r++) {
        for (int c = 0; c < chipDim; c++) {
            int idx = (r * chipDim + c) * 4;
            unsigned char baseColor = 28;
            if (r < 3 || r >= chipDim - 3 || c < 3 || c >= chipDim - 3) {
                baseColor = 55;
            } else if ((r ^ c) % 8 == 0) {
                baseColor = 34;
            }
            chipPixels[idx + 0] = baseColor;
            chipPixels[idx + 1] = baseColor + 4;
            chipPixels[idx + 2] = baseColor + 8;
            chipPixels[idx + 3] = 255;
        }
    }

    if (chipTextureId == 0) {
        glGenTextures(1, &chipTextureId);
    }
    glBindTexture(GL_TEXTURE_2D, chipTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, chipDim, chipDim, 0, GL_RGBA, GL_UNSIGNED_BYTE, chipPixels.data());

    glBindTexture(GL_TEXTURE_2D, 0);
    return true;
}

bool GateRenderer::initLighting() {
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    GLfloat lightAmbient[] = { 0.25f, 0.28f, 0.35f, 1.0f };
    GLfloat lightDiffuse[] = { 0.75f, 0.85f, 0.95f, 1.0f };
    GLfloat lightSpecular[] = { 0.9f, 0.95f, 1.0f, 1.0f };
    GLfloat lightPos[] = { 200.0f, 300.0f, 400.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    glEnable(GL_LIGHT0);
    if (lightingEnabled) {
        glEnable(GL_LIGHTING);
    } else {
        glDisable(GL_LIGHTING);
    }
    return true;
}

void GateRenderer::toggleTextures() {
    texturesEnabled = !texturesEnabled;
}

void GateRenderer::toggleLighting() {
    lightingEnabled = !lightingEnabled;
    if (lightingEnabled) {
        glEnable(GL_LIGHTING);
    } else {
        glDisable(GL_LIGHTING);
    }
}

bool GateRenderer::areTexturesEnabled() { return texturesEnabled; }
bool GateRenderer::isLightingEnabled() { return lightingEnabled; }
void GateRenderer::setTextureMode(bool enable) { texturesEnabled = enable; }
void GateRenderer::setLightingMode(bool enable) {
    lightingEnabled = enable;
    if (lightingEnabled) glEnable(GL_LIGHTING);
    else glDisable(GL_LIGHTING);
}

void GateRenderer::bindBoardTexture() {
    if (texturesEnabled && boardTextureId != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, boardTextureId);
    }
}

void GateRenderer::bindChipTexture() {
    if (texturesEnabled && chipTextureId != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, chipTextureId);
    }
}

void GateRenderer::unbindTexture() {
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

void GateRenderer::applyMaterial(bool isSelected, bool isActiveSignal) {
    if (!lightingEnabled) return;
    GLfloat matSpecular[] = { 0.5f, 0.7f, 0.9f, 1.0f };
    GLfloat matShininess[] = { 32.0f };
    GLfloat matEmission[] = { 0.0f, 0.0f, 0.0f, 1.0f };

    if (isActiveSignal) {
        matEmission[0] = 0.05f;
        matEmission[1] = 0.35f;
        matEmission[2] = 0.15f;
    } else if (isSelected) {
        matEmission[0] = 0.1f;
        matEmission[1] = 0.25f;
        matEmission[2] = 0.4f;
    }

    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmission);
}

void GateRenderer::resetMaterial() {
    if (!lightingEnabled) return;
    GLfloat noEmission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, noEmission);
}

void GateRenderer::drawTexturedSubstrate(float minX, float minY, float maxX, float maxY) {
    if (texturesEnabled && boardTextureId != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, boardTextureId);
        glColor3f(1.0f, 1.0f, 1.0f);

        float repeatS = (maxX - minX) / 128.0f;
        float repeatT = (maxY - minY) / 128.0f;

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);       glVertex2f(minX, minY);
        glTexCoord2f(repeatS, 0.0f);    glVertex2f(maxX, minY);
        glTexCoord2f(repeatS, repeatT); glVertex2f(maxX, maxY);
        glTexCoord2f(0.0f, repeatT);    glVertex2f(minX, maxY);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
    } else {
        glColor3f(0.07f, 0.09f, 0.13f);
        glBegin(GL_QUADS);
        glVertex2f(minX, minY);
        glVertex2f(maxX, minY);
        glVertex2f(maxX, maxY);
        glVertex2f(minX, maxY);
        glEnd();
    }
}
