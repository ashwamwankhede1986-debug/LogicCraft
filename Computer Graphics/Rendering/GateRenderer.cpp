#include "GateRenderer.h"
#include <cmath>
#include <cstring>

const float PI = 3.14159265358979323846f;

void GateRenderer::drawCircle(float cx, float cy, float r, int num_segments, bool filled) {
    if (filled) {
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy);
        for (int i = 0; i <= num_segments; i++) {
            float theta = 2.0f * PI * float(i) / float(num_segments);
            float dx = r * cosf(theta);
            float dy = r * sinf(theta);
            glVertex2f(cx + dx, cy + dy);
        }
        glEnd();
    } else {
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < num_segments; i++) {
            float theta = 2.0f * PI * float(i) / float(num_segments);
            float dx = r * cosf(theta);
            float dy = r * sinf(theta);
            glVertex2f(cx + dx, cy + dy);
        }
        glEnd();
    }
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
        case 'F':
            glVertex2f(x, y); glVertex2f(x, y + 14*s);
            glVertex2f(x, y + 14*s); glVertex2f(x + 9*s, y + 14*s);
            glVertex2f(x, y + 7*s); glVertex2f(x + 7*s, y + 7*s);
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
        case 'H':
            glVertex2f(x, y); glVertex2f(x, y + 14*s);
            glVertex2f(x + 9*s, y); glVertex2f(x + 9*s, y + 14*s);
            glVertex2f(x, y + 7*s); glVertex2f(x + 9*s, y + 7*s);
            break;
        case 'I':
            glVertex2f(x + 2*s, y + 14*s); glVertex2f(x + 8*s, y + 14*s);
            glVertex2f(x + 5*s, y + 14*s); glVertex2f(x + 5*s, y);
            glVertex2f(x + 2*s, y); glVertex2f(x + 8*s, y);
            break;
        case 'J':
            glVertex2f(x + 9*s, y + 14*s); glVertex2f(x + 9*s, y + 3*s);
            glVertex2f(x + 9*s, y + 3*s); glVertex2f(x + 7*s, y);
            glVertex2f(x + 7*s, y); glVertex2f(x + 2*s, y);
            glVertex2f(x + 2*s, y); glVertex2f(x, y + 4*s);
            break;
        case 'K':
            glVertex2f(x, y); glVertex2f(x, y + 14*s);
            glVertex2f(x + 9*s, y + 14*s); glVertex2f(x, y + 6*s);
            glVertex2f(x + 2*s, y + 7*s); glVertex2f(x + 9*s, y);
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
        case 'Q':
            glVertex2f(x + 2*s, y); glVertex2f(x + 8*s, y);
            glVertex2f(x + 8*s, y); glVertex2f(x + 10*s, y + 3*s);
            glVertex2f(x + 10*s, y + 3*s); glVertex2f(x + 10*s, y + 11*s);
            glVertex2f(x + 10*s, y + 11*s); glVertex2f(x + 8*s, y + 14*s);
            glVertex2f(x + 8*s, y + 14*s); glVertex2f(x + 2*s, y + 14*s);
            glVertex2f(x + 2*s, y + 14*s); glVertex2f(x, y + 11*s);
            glVertex2f(x, y + 11*s); glVertex2f(x, y + 3*s);
            glVertex2f(x, y + 3*s); glVertex2f(x + 2*s, y);
            glVertex2f(x + 5*s, y + 5*s); glVertex2f(x + 10*s, y);
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
        case 'W':
            glVertex2f(x, y + 14*s); glVertex2f(x + 2.5f*s, y);
            glVertex2f(x + 2.5f*s, y); glVertex2f(x + 5*s, y + 9*s);
            glVertex2f(x + 5*s, y + 9*s); glVertex2f(x + 7.5f*s, y);
            glVertex2f(x + 7.5f*s, y); glVertex2f(x + 10*s, y + 14*s);
            break;
        case 'X':
            glVertex2f(x, y); glVertex2f(x + 10*s, y + 14*s);
            glVertex2f(x, y + 14*s); glVertex2f(x + 10*s, y);
            break;
        case 'Y':
            glVertex2f(x, y + 14*s); glVertex2f(x + 5*s, y + 7*s);
            glVertex2f(x + 10*s, y + 14*s); glVertex2f(x + 5*s, y + 7*s);
            glVertex2f(x + 5*s, y + 7*s); glVertex2f(x + 5*s, y);
            break;
        case 'Z':
            glVertex2f(x, y + 14*s); glVertex2f(x + 9*s, y + 14*s);
            glVertex2f(x + 9*s, y + 14*s); glVertex2f(x, y);
            glVertex2f(x, y); glVertex2f(x + 9*s, y);
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
            break;
        case '1':
            glVertex2f(x + 2*s, y + 11*s); glVertex2f(x + 5*s, y + 14*s);
            glVertex2f(x + 5*s, y + 14*s); glVertex2f(x + 5*s, y);
            glVertex2f(x + 1*s, y); glVertex2f(x + 9*s, y);
            break;
        case '2':
            glVertex2f(x + 1*s, y + 12*s); glVertex2f(x + 5*s, y + 14*s);
            glVertex2f(x + 5*s, y + 14*s); glVertex2f(x + 9*s, y + 11*s);
            glVertex2f(x + 9*s, y + 11*s); glVertex2f(x, y);
            glVertex2f(x, y); glVertex2f(x + 10*s, y);
            break;
        case '3':
            glVertex2f(x + 1*s, y + 14*s); glVertex2f(x + 9*s, y + 14*s);
            glVertex2f(x + 9*s, y + 14*s); glVertex2f(x + 4*s, y + 8*s);
            glVertex2f(x + 4*s, y + 8*s); glVertex2f(x + 9*s, y + 5*s);
            glVertex2f(x + 9*s, y + 5*s); glVertex2f(x + 6*s, y);
            glVertex2f(x + 6*s, y); glVertex2f(x + 1*s, y);
            break;
        case '4':
            glVertex2f(x + 8*s, y); glVertex2f(x + 8*s, y + 14*s);
            glVertex2f(x + 8*s, y + 14*s); glVertex2f(x + 1*s, y + 5*s);
            glVertex2f(x + 1*s, y + 5*s); glVertex2f(x + 10*s, y + 5*s);
            break;
        case '5':
            glVertex2f(x + 9*s, y + 14*s); glVertex2f(x + 1*s, y + 14*s);
            glVertex2f(x + 1*s, y + 14*s); glVertex2f(x + 1*s, y + 8*s);
            glVertex2f(x + 1*s, y + 8*s); glVertex2f(x + 8*s, y + 8*s);
            glVertex2f(x + 8*s, y + 8*s); glVertex2f(x + 9*s, y + 4*s);
            glVertex2f(x + 9*s, y + 4*s); glVertex2f(x + 8*s, y);
            glVertex2f(x + 8*s, y); glVertex2f(x + 1*s, y);
            break;
        case '6':
            glVertex2f(x + 8*s, y + 14*s); glVertex2f(x + 2*s, y + 8*s);
            glVertex2f(x + 2*s, y + 8*s); glVertex2f(x + 2*s, y + 2*s);
            glVertex2f(x + 2*s, y + 2*s); glVertex2f(x + 6*s, y);
            glVertex2f(x + 6*s, y); glVertex2f(x + 9*s, y + 3*s);
            glVertex2f(x + 9*s, y + 3*s); glVertex2f(x + 9*s, y + 6*s);
            glVertex2f(x + 9*s, y + 6*s); glVertex2f(x + 6*s, y + 8*s);
            glVertex2f(x + 6*s, y + 8*s); glVertex2f(x + 2*s, y + 8*s);
            break;
        case '7':
            glVertex2f(x, y + 14*s); glVertex2f(x + 9*s, y + 14*s);
            glVertex2f(x + 9*s, y + 14*s); glVertex2f(x + 3*s, y);
            break;
        case '8':
            glVertex2f(x + 2*s, y); glVertex2f(x + 8*s, y);
            glVertex2f(x + 8*s, y); glVertex2f(x + 10*s, y + 4*s);
            glVertex2f(x + 10*s, y + 4*s); glVertex2f(x, y + 10*s);
            glVertex2f(x, y + 10*s); glVertex2f(x + 2*s, y + 14*s);
            glVertex2f(x + 2*s, y + 14*s); glVertex2f(x + 8*s, y + 14*s);
            glVertex2f(x + 8*s, y + 14*s); glVertex2f(x + 10*s, y + 10*s);
            glVertex2f(x + 10*s, y + 10*s); glVertex2f(x, y + 4*s);
            glVertex2f(x, y + 4*s); glVertex2f(x + 2*s, y);
            break;
        case '9':
            glVertex2f(x + 2*s, y); glVertex2f(x + 8*s, y + 6*s);
            glVertex2f(x + 8*s, y + 6*s); glVertex2f(x + 8*s, y + 12*s);
            glVertex2f(x + 8*s, y + 12*s); glVertex2f(x + 5*s, y + 14*s);
            glVertex2f(x + 5*s, y + 14*s); glVertex2f(x + 1*s, y + 11*s);
            glVertex2f(x + 1*s, y + 11*s); glVertex2f(x + 1*s, y + 8*s);
            glVertex2f(x + 1*s, y + 8*s); glVertex2f(x + 8*s, y + 8*s);
            break;
        case ':':
            glVertex2f(x + 4*s, y + 10*s); glVertex2f(x + 5*s, y + 10*s);
            glVertex2f(x + 4*s, y + 4*s); glVertex2f(x + 5*s, y + 4*s);
            break;
        case '-':
            glVertex2f(x + 1*s, y + 7*s); glVertex2f(x + 8*s, y + 7*s);
            break;
        case '_':
            glVertex2f(x, y); glVertex2f(x + 9*s, y);
            break;
        case '[':
            glVertex2f(x + 7*s, y + 14*s); glVertex2f(x + 2*s, y + 14*s);
            glVertex2f(x + 2*s, y + 14*s); glVertex2f(x + 2*s, y);
            glVertex2f(x + 2*s, y); glVertex2f(x + 7*s, y);
            break;
        case ']':
            glVertex2f(x + 2*s, y + 14*s); glVertex2f(x + 7*s, y + 14*s);
            glVertex2f(x + 7*s, y + 14*s); glVertex2f(x + 7*s, y);
            glVertex2f(x + 7*s, y); glVertex2f(x + 2*s, y);
            break;
        case '(':
            glVertex2f(x + 7*s, y + 14*s); glVertex2f(x + 3*s, y + 7*s);
            glVertex2f(x + 3*s, y + 7*s); glVertex2f(x + 7*s, y);
            break;
        case ')':
            glVertex2f(x + 3*s, y + 14*s); glVertex2f(x + 7*s, y + 7*s);
            glVertex2f(x + 7*s, y + 7*s); glVertex2f(x + 3*s, y);
            break;
        case '=':
            glVertex2f(x + 1*s, y + 9*s); glVertex2f(x + 9*s, y + 9*s);
            glVertex2f(x + 1*s, y + 5*s); glVertex2f(x + 9*s, y + 5*s);
            break;
        case '>':
            glVertex2f(x + 2*s, y + 13*s); glVertex2f(x + 8*s, y + 7*s);
            glVertex2f(x + 8*s, y + 7*s); glVertex2f(x + 2*s, y + 1*s);
            break;
        case '<':
            glVertex2f(x + 8*s, y + 13*s); glVertex2f(x + 2*s, y + 7*s);
            glVertex2f(x + 2*s, y + 7*s); glVertex2f(x + 8*s, y + 1*s);
            break;
        case '+':
            glVertex2f(x + 1*s, y + 7*s); glVertex2f(x + 9*s, y + 7*s);
            glVertex2f(x + 5*s, y + 11*s); glVertex2f(x + 5*s, y + 3*s);
            break;
        case '/':
            glVertex2f(x + 1*s, y); glVertex2f(x + 9*s, y + 14*s);
            break;
        case '.':
            glVertex2f(x + 4*s, y); glVertex2f(x + 5*s, y);
            glVertex2f(x + 4*s, y + 1*s); glVertex2f(x + 5*s, y + 1*s);
            break;
        default:
            // space or unsupported
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
    // Pin terminal circle
    if (state == 1) {
        glColor3f(0.2f, 0.9f, 0.3f); // Green for Logic HIGH (1)
    } else {
        glColor3f(0.7f, 0.2f, 0.2f); // Red for Logic LOW (0)
    }
    drawCircle(x, y, 4.0f, 12, true);

    // Terminal border
    glColor3f(1.0f, 1.0f, 1.0f);
    drawCircle(x, y, 4.0f, 12, false);

    // Label
    if (label) {
        glColor3f(0.85f, 0.85f, 0.95f);
        float offset = isInput ? -18.0f : 8.0f;
        drawText(label, x + offset, y - 4.0f, 0.65f);
    }
}

void GateRenderer::drawWire(float x1, float y1, float x2, float y2, int signalState) {
    glLineWidth(2.5f);
    if (signalState == 1) {
        glColor3f(0.1f, 1.0f, 0.4f); // Active green wire
    } else {
        glColor3f(0.35f, 0.5f, 0.7f); // Inactive cyan/slate wire
    }

    // Stepped orthogonal wire (Horizontal -> Vertical -> Horizontal)
    float midX = (x1 + x2) * 0.5f;

    glBegin(GL_LINE_STRIP);
    glVertex2f(x1, y1);
    glVertex2f(midX, y1);
    glVertex2f(midX, y2);
    glVertex2f(x2, y2);
    glEnd();

    // Signal state indicator dot at midpoint
    glColor3f(1.0f, 1.0f, 0.2f);
    drawCircle(midX, (y1 + y2) * 0.5f, 3.0f, 8, true);
    glLineWidth(1.0f);
}

void GateRenderer::drawANDGate(float x, float y, float w, float h, bool isSelected, int inputA, int inputB, int output) {
    float halfW = w * 0.5f;
    float halfH = h * 0.5f;

    float left = x - halfW;
    float straightRight = x + halfW * 0.2f;
    float right = x + halfW;
    float top = y + halfH;
    float bottom = y - halfH;

    // Body Fill (GL_POLYGON / GL_TRIANGLE_FAN)
    if (isSelected) {
        glColor3f(0.18f, 0.28f, 0.45f); // Selected soft blue
    } else {
        glColor3f(0.12f, 0.16f, 0.24f); // Dark Slate Body
    }

    // Left rectangle block
    glBegin(GL_QUADS);
    glVertex2f(left, bottom);
    glVertex2f(straightRight, bottom);
    glVertex2f(straightRight, top);
    glVertex2f(left, top);
    glEnd();

    // Semi-circular curved right side (GL_TRIANGLE_FAN)
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(straightRight, y);
    float radius = halfH;
    for (int i = -16; i <= 16; i++) {
        float angle = (float)i / 32.0f * PI; // -PI/2 to PI/2
        float px = straightRight + (halfW - halfW * 0.2f) * cosf(angle);
        float py = y + radius * sinf(angle);
        glVertex2f(px, py);
    }
    glEnd();

    // Border Outline using GL_LINE_STRIP (CO1 Primitives)
    glLineWidth(isSelected ? 3.0f : 2.0f);
    if (isSelected) {
        glColor3f(0.3f, 0.75f, 1.0f); // Bright blue selection glow
    } else {
        glColor3f(0.7f, 0.85f, 1.0f); // Crisp cyan-white border
    }

    glBegin(GL_LINE_STRIP);
    glVertex2f(straightRight, top);
    glVertex2f(left, top);
    glVertex2f(left, bottom);
    glVertex2f(straightRight, bottom);
    // Draw outer arc
    for (int i = -16; i <= 16; i++) {
        float angle = (float)i / 32.0f * PI;
        float px = straightRight + (halfW - halfW * 0.2f) * cosf(angle);
        float py = y + radius * sinf(angle);
        glVertex2f(px, py);
    }
    glEnd();
    glLineWidth(1.0f);

    // Input Pins (A and B)
    float pinInY1 = y + halfH * 0.45f;
    float pinInY2 = y - halfH * 0.45f;
    float pinLen = 22.0f;

    // Pin Wires
    glColor3f(0.7f, 0.8f, 0.9f);
    glBegin(GL_LINES);
    glVertex2f(left - pinLen, pinInY1); glVertex2f(left, pinInY1);
    glVertex2f(left - pinLen, pinInY2); glVertex2f(left, pinInY2);
    // Output Pin Wire
    glVertex2f(right, y); glVertex2f(right + pinLen, y);
    glEnd();

    // Terminals
    drawTerminal(left - pinLen, pinInY1, "A", inputA, true);
    drawTerminal(left - pinLen, pinInY2, "B", inputB, true);
    drawTerminal(right + pinLen, y, "OUT", output, false);

    // Gate Label
    glColor3f(1.0f, 1.0f, 1.0f);
    drawText("AND", x - 18.0f, y - 5.0f, 0.8f);
}

void GateRenderer::drawORGate(float x, float y, float w, float h, bool isSelected, int inputA, int inputB, int output) {
    float halfW = w * 0.5f;
    float halfH = h * 0.5f;

    float left = x - halfW;
    float right = x + halfW;
    float top = y + halfH;
    float bottom = y - halfH;
    float indentDepth = w * 0.22f; // Concave back depth

    // 1. Properly adjusted Body Fill Color (Rich Royal Amethyst Violet)
    if (isSelected) {
        glColor3f(0.30f, 0.18f, 0.48f); // Luminous Selected Purple
    } else {
        glColor3f(0.20f, 0.13f, 0.32f); // Deep Royal Amethyst Purple (Solid, distinct from background)
    }

    // 2. Star-Convex Artifact-Free Body Fill using GL_TRIANGLE_FAN from interior focus point
    float centerX = left + w * 0.45f;
    float centerY = y;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(centerX, centerY); // Fan Center

    // Right Apex
    glVertex2f(right, y);

    // Sweeping Top Curve (Right to Left)
    const int STEPS = 20;
    for (int i = STEPS; i >= 0; --i) {
        float t = (float)i / (float)STEPS;
        float px = left + w * t;
        float py = y + halfH * (1.0f - t * t);
        glVertex2f(px, py);
    }

    // Concave Back Curve (Top to Bottom)
    for (int i = STEPS; i >= -STEPS; --i) {
        float angle = ((float)i / (float)STEPS) * (PI * 0.5f); // PI/2 down to -PI/2
        float px = left + indentDepth * cosf(angle);
        float py = y + halfH * sinf(angle);
        glVertex2f(px, py);
    }

    // Sweeping Bottom Curve (Left to Right)
    for (int i = 0; i <= STEPS; ++i) {
        float t = (float)i / (float)STEPS;
        float px = left + w * t;
        float py = y - halfH * (1.0f - t * t);
        glVertex2f(px, py);
    }
    glEnd();

    // 3. Crisp, Vibrant Outline (Vivid Neon Violet)
    glLineWidth(isSelected ? 3.2f : 2.2f);
    if (isSelected) {
        glColor3f(0.95f, 0.65f, 1.0f); // Bright Glowing Lilac
    } else {
        glColor3f(0.78f, 0.48f, 1.0f); // Vibrant Electric Purple
    }

    // Top Curve Outline
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= STEPS; ++i) {
        float t = (float)i / (float)STEPS;
        float px = left + w * t;
        float py = y + halfH * (1.0f - t * t);
        glVertex2f(px, py);
    }
    glEnd();

    // Bottom Curve Outline
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= STEPS; ++i) {
        float t = (float)i / (float)STEPS;
        float px = left + w * t;
        float py = y - halfH * (1.0f - t * t);
        glVertex2f(px, py);
    }
    glEnd();

    // Concave Back Curve Outline
    glBegin(GL_LINE_STRIP);
    for (int i = -STEPS; i <= STEPS; ++i) {
        float angle = ((float)i / (float)STEPS) * (PI * 0.5f);
        float px = left + indentDepth * cosf(angle);
        float py = y + halfH * sinf(angle);
        glVertex2f(px, py);
    }
    glEnd();
    glLineWidth(1.0f);

    // 4. Input / Output Terminal Wires aligned to the curved boundary
    float pinInY1 = y + halfH * 0.45f;
    float pinInY2 = y - halfH * 0.45f;
    float pinLen = 22.0f;

    // Contact point on the concave back curve at pin height
    float angleContact = asinf(0.45f);
    float contactX = left + indentDepth * cosf(angleContact);

    glColor3f(0.7f, 0.8f, 0.9f);
    glBegin(GL_LINES);
    glVertex2f(left - pinLen, pinInY1); glVertex2f(contactX, pinInY1);
    glVertex2f(left - pinLen, pinInY2); glVertex2f(contactX, pinInY2);
    glVertex2f(right, y); glVertex2f(right + pinLen, y);
    glEnd();

    drawTerminal(left - pinLen, pinInY1, "A", inputA, true);
    drawTerminal(left - pinLen, pinInY2, "B", inputB, true);
    drawTerminal(right + pinLen, y, "OUT", output, false);

    // 5. Gate Text Label
    glColor3f(1.0f, 1.0f, 1.0f);
    drawText("OR", x - 10.0f, y - 5.0f, 0.85f);
}

void GateRenderer::drawNOTGate(float x, float y, float w, float h, bool isSelected, int input, int output) {
    float halfW = w * 0.5f;
    float halfH = h * 0.5f;

    float left = x - halfW;
    float bubbleR = 5.5f;
    float triangleTip = x + halfW - (bubbleR * 2.0f);
    float top = y + halfH;
    float bottom = y - halfH;

    // Body Fill (GL_TRIANGLES)
    if (isSelected) {
        glColor3f(0.35f, 0.22f, 0.18f);
    } else {
        glColor3f(0.25f, 0.16f, 0.12f);
    }

    glBegin(GL_TRIANGLES);
    glVertex2f(left, top);
    glVertex2f(left, bottom);
    glVertex2f(triangleTip, y);
    glEnd();

    // Outline (GL_LINE_LOOP)
    glLineWidth(isSelected ? 3.0f : 2.0f);
    if (isSelected) {
        glColor3f(1.0f, 0.6f, 0.2f);
    } else {
        glColor3f(1.0f, 0.8f, 0.5f);
    }

    glBegin(GL_LINE_LOOP);
    glVertex2f(left, top);
    glVertex2f(left, bottom);
    glVertex2f(triangleTip, y);
    glEnd();

    // Inversion Bubble Circle (GL_LINE_LOOP and filled inside)
    glColor3f(0.1f, 0.12f, 0.16f);
    drawCircle(triangleTip + bubbleR, y, bubbleR, 16, true);

    glColor3f(1.0f, 0.8f, 0.5f);
    drawCircle(triangleTip + bubbleR, y, bubbleR, 16, false);
    glLineWidth(1.0f);

    // Input/Output Wires
    float pinLen = 22.0f;
    glColor3f(0.7f, 0.8f, 0.9f);
    glBegin(GL_LINES);
    glVertex2f(left - pinLen, y); glVertex2f(left, y);
    glVertex2f(triangleTip + bubbleR * 2.0f, y); glVertex2f(triangleTip + bubbleR * 2.0f + pinLen, y);
    glEnd();

    drawTerminal(left - pinLen, y, "IN", input, true);
    drawTerminal(triangleTip + bubbleR * 2.0f + pinLen, y, "OUT", output, false);

    glColor3f(1.0f, 1.0f, 1.0f);
    drawText("NOT", x - 18.0f, y - 5.0f, 0.75f);
}
