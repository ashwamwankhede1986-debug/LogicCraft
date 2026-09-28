#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include "GLWindow.h"
#include <iostream>

LRESULT CALLBACK GLWindow::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    GLWindow* app = (GLWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

    switch (message) {
        case WM_CREATE: {
            CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
            SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)cs->lpCreateParams);
            return 0;
        }
        case WM_SIZE: {
            if (app) {
                int w = LOWORD(lParam);
                int h = HIWORD(lParam);
                app->handleResize(w, h);
            }
            return 0;
        }
        case WM_LBUTTONDOWN: {
            if (app) {
                int x = LOWORD(lParam);
                int y = HIWORD(lParam);
                app->handleMouseDown(0, x, y);
            }
            return 0;
        }
        case WM_LBUTTONUP: {
            if (app) {
                int x = LOWORD(lParam);
                int y = HIWORD(lParam);
                app->handleMouseUp(0, x, y);
            }
            return 0;
        }
        case WM_RBUTTONDOWN: {
            if (app) {
                int x = LOWORD(lParam);
                int y = HIWORD(lParam);
                app->handleMouseDown(1, x, y);
            }
            return 0;
        }
        case WM_RBUTTONUP: {
            if (app) {
                int x = LOWORD(lParam);
                int y = HIWORD(lParam);
                app->handleMouseUp(1, x, y);
            }
            return 0;
        }
        case WM_MOUSEMOVE: {
            if (app) {
                int x = LOWORD(lParam);
                int y = HIWORD(lParam);
                app->handleMouseMove(x, y);
            }
            return 0;
        }
        case WM_MOUSEWHEEL: {
            if (app) {
                short delta = GET_WHEEL_DELTA_WPARAM(wParam);
                POINT pt = { LOWORD(lParam), HIWORD(lParam) };
                ScreenToClient(hWnd, &pt);
                app->handleMouseWheel(delta, pt.x, pt.y);
            }
            return 0;
        }
        case WM_KEYDOWN: {
            if (app) {
                app->handleKeyDown(wParam);
            }
            return 0;
        }
        case WM_CLOSE: {
            PostQuitMessage(0);
            return 0;
        }
        case WM_DESTROY: {
            return 0;
        }
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

GLWindow::GLWindow(int w, int h, const char* title) 
    : hwnd(NULL), hdc(NULL), hglrc(NULL), width(w), height(h), isRunning(false),
      isRightDragging(false), lastMouseX(0), lastMouseY(0) {
}

GLWindow::~GLWindow() {
    cleanup();
}

bool GLWindow::init() {
    HINSTANCE hInstance = GetModuleHandle(NULL);

    WNDCLASS wc = {0};
    wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = GLWindow::WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"LogicCraftGLWindowClass";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    if (!RegisterClass(&wc)) {
        if (GetLastError() != ERROR_CLASS_ALREADY_EXISTS) {
            std::cerr << "[OpenGL Error] Failed to register window class!\n";
            return false;
        }
    }

    RECT rect = {0, 0, width, height};
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

    hwnd = CreateWindowEx(
        0,
        L"LogicCraftGLWindowClass",
        L"LogicCraft — Visual Logic Gate Simulator (Review 2: CO1 & CO2)",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rect.right - rect.left, rect.bottom - rect.top,
        NULL, NULL, hInstance, this
    );

    if (!hwnd) {
        std::cerr << "[OpenGL Error] Failed to create window!\n";
        return false;
    }

    hdc = GetDC(hwnd);

    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        32,
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        24, // Depth bits
        8,  // Stencil bits
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };

    int format = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, format, &pfd);

    hglrc = wglCreateContext(hdc);
    if (!hglrc) {
        std::cerr << "[OpenGL Error] Failed to create WGL OpenGL context!\n";
        return false;
    }

    wglMakeCurrent(hdc, hglrc);

    // Initialize Camera Viewport
    camera.setViewport(width, height);

    // Enable anti-aliased smooth lines
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    std::cout << "[OpenGL Init] Context initialized successfully.\n";
    std::cout << " -> Vendor:   " << glGetString(GL_VENDOR) << "\n";
    std::cout << " -> Renderer: " << glGetString(GL_RENDERER) << "\n";
    std::cout << " -> Version:  " << glGetString(GL_VERSION) << "\n";

    isRunning = true;
    return true;
}

void GLWindow::handleResize(int newW, int newH) {
    width = newW;
    height = newH;
    camera.setViewport(width, height);
}

void GLWindow::handleMouseDown(int button, int x, int y) {
    lastMouseX = x;
    lastMouseY = y;

    float worldX, worldY;
    camera.screenToWorld(x, y, &worldX, &worldY);

    if (button == 0) { // Left Button
        canvas.onMouseDown(0, worldX, worldY);
    } else if (button == 1) { // Right Button
        isRightDragging = true;
    }
}

void GLWindow::handleMouseMove(int x, int y) {
    int dx = x - lastMouseX;
    int dy = y - lastMouseY;

    if (isRightDragging) {
        // Pan canvas (Translation)
        camera.pan((float)dx, (float)dy);
    } else {
        float worldX, worldY;
        camera.screenToWorld(x, y, &worldX, &worldY);
        canvas.onMouseMove(worldX, worldY);
    }

    lastMouseX = x;
    lastMouseY = y;
}

void GLWindow::handleMouseUp(int button, int x, int y) {
    if (button == 0) {
        canvas.onMouseUp(0);
    } else if (button == 1) {
        isRightDragging = false;
    }
}

void GLWindow::handleMouseWheel(short delta, int x, int y) {
    float factor = (delta > 0) ? 1.15f : 0.87f;
    camera.zoom(factor, (float)x, (float)y);
}

void GLWindow::handleKeyDown(WPARAM key) {
    switch (key) {
        case VK_ESCAPE:
            isRunning = false;
            break;
        case '1':
            canvas.selectGate(1);
            std::cout << "[Canvas] Selected AND Gate (ID: 1)\n";
            break;
        case '2':
            canvas.selectGate(2);
            std::cout << "[Canvas] Selected OR Gate (ID: 2)\n";
            break;
        case '3':
            canvas.selectGate(3);
            std::cout << "[Canvas] Selected NOT Gate (ID: 3)\n";
            break;
        case 'R':
            camera.reset();
            std::cout << "[Camera] View reset to default (Pan: 0, 0 | Zoom: 1.0X)\n";
            break;
        case VK_SPACE:
            canvas.toggleSelectedGateInput();
            std::cout << "[Circuit] Toggled input state & re-evaluated circuit logic.\n";
            break;
        case VK_OEM_PLUS:
        case VK_ADD:
            camera.zoom(1.15f);
            break;
        case VK_OEM_MINUS:
        case VK_SUBTRACT:
            camera.zoom(0.87f);
            break;
        case VK_LEFT:
            if (canvas.getSelectedGateId() != -1) canvas.moveSelectedGate(-10.0f, 0.0f);
            else camera.pan(15.0f, 0.0f);
            break;
        case VK_RIGHT:
            if (canvas.getSelectedGateId() != -1) canvas.moveSelectedGate(10.0f, 0.0f);
            else camera.pan(-15.0f, 0.0f);
            break;
        case VK_UP:
            if (canvas.getSelectedGateId() != -1) canvas.moveSelectedGate(0.0f, 10.0f);
            else camera.pan(0.0f, -15.0f);
            break;
        case VK_DOWN:
            if (canvas.getSelectedGateId() != -1) canvas.moveSelectedGate(0.0f, -10.0f);
            else camera.pan(0.0f, 15.0f);
            break;
        default:
            break;
    }
}

void GLWindow::renderFrame() {
    // 1. Apply View Transformation (CO2 Viewport & Ortho Camera)
    camera.applyViewTransform();

    // 2. Render Canvas, Grid, Gates, and Wires (CO1 Primitives + CO2 Clipping)
    canvas.render(camera);

    // 3. Render 2D Screen-space HUD Overlay
    canvas.renderHUD(width, height, camera);

    // Swap double buffer
    SwapBuffers(hdc);
}

void GLWindow::run() {
    MSG msg;
    while (isRunning) {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                isRunning = false;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (isRunning) {
            renderFrame();
            Sleep(16); // ~60 FPS
        }
    }
}

void GLWindow::cleanup() {
    if (hglrc) {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(hglrc);
        hglrc = NULL;
    }
    if (hdc && hwnd) {
        ReleaseDC(hwnd, hdc);
        hdc = NULL;
    }
    if (hwnd) {
        DestroyWindow(hwnd);
        hwnd = NULL;
    }
}

Camera& GLWindow::getCamera() { return camera; }
Canvas& GLWindow::getCanvas() { return canvas; }
