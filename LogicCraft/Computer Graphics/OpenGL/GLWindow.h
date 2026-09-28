#ifndef GL_WINDOW_H
#define GL_WINDOW_H

#include <windows.h>
#include <GL/gl.h>
#include "../Transformations/Camera.h"
#include "../Canvas/Canvas.h"

/**
 * @file GLWindow.h
 * @brief Native Win32 OpenGL Application Window (CO1 & CO2)
 */

class GLWindow {
private:
    HWND hwnd;
    HDC hdc;
    HGLRC hglrc;
    int width;
    int height;
    bool isRunning;

    Camera camera;
    Canvas canvas;

    bool isRightDragging;
    int lastMouseX;
    int lastMouseY;

    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
    GLWindow(int w = 1024, int h = 720, const char* title = "LogicCraft — Visual Logic Gate Simulator (Review 2)");
    ~GLWindow();

    bool init();
    void run();
    void renderFrame();
    void cleanup();

    // Event Handlers
    void handleResize(int newW, int newH);
    void handleMouseDown(int button, int x, int y);
    void handleMouseMove(int x, int y);
    void handleMouseUp(int button, int x, int y);
    void handleMouseWheel(short delta, int x, int y);
    void handleKeyDown(WPARAM key);

    Camera& getCamera();
    Canvas& getCanvas();
};

#endif // GL_WINDOW_H
