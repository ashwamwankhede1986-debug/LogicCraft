#ifndef CAMERA_H
#define CAMERA_H

#include <windows.h>
#include <GL/gl.h>

/**
 * @file Camera.h
 * @brief Viewport, Coordinate Conversion, and Transformation Matrix (CO2 - Transformation & Clipping)
 *
 * Implements:
 * - 2D Orthographic Camera Viewport
 * - Translation (Panning canvas)
 * - Scaling (Zooming canvas in/out)
 * - Screen-to-World Coordinate Conversion for accurate mouse picking
 * - Clipping boundary checks
 */

class Camera {
private:
    float panX;
    float panY;
    float zoomFactor;

    int windowWidth;
    int windowHeight;

    // Clipping bounds in world space
    float clipLeft;
    float clipRight;
    float clipBottom;
    float clipTop;

public:
    Camera();

    void setViewport(int width, int height);
    void applyViewTransform();

    // Transformations (CO2)
    void pan(float dx, float dy);
    void zoom(float factor, float focalScreenX = 0, float focalScreenY = 0);
    void reset();

    // Coordinate Conversion (Screen Pixels -> World Space)
    void screenToWorld(int screenX, int screenY, float* worldX, float* worldY) const;

    // Clipping test (CO2)
    bool isBoxVisible(float minX, float minY, float maxX, float maxY) const;

    // Getters
    float getPanX() const;
    float getPanY() const;
    float getZoom() const;
    int getWidth() const;
    int getHeight() const;
};

#endif // CAMERA_H
