#include "Camera.h"
#include <algorithm>

Camera::Camera() 
    : panX(0.0f), panY(0.0f), zoomFactor(1.0f), 
      windowWidth(1024), windowHeight(720),
      clipLeft(-512.0f), clipRight(512.0f), clipBottom(-360.0f), clipTop(360.0f) {}

void Camera::setViewport(int width, int height) {
    windowWidth = (width > 0) ? width : 1;
    windowHeight = (height > 0) ? height : 1;
    glViewport(0, 0, windowWidth, windowHeight);
}

void Camera::applyViewTransform() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float halfW = (windowWidth / 2.0f) / zoomFactor;
    float halfH = (windowHeight / 2.0f) / zoomFactor;

    clipLeft = -halfW - panX;
    clipRight = halfW - panX;
    clipBottom = -halfH - panY;
    clipTop = halfH - panY;

    // Set 2D orthographic projection
    glOrtho(-halfW, halfW, -halfH, halfH, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // Translation transformation (CO2)
    glTranslatef(panX, panY, 0.0f);
}

void Camera::pan(float dx, float dy) {
    // Adjust pan speed according to current zoom factor
    panX += dx / zoomFactor;
    panY -= dy / zoomFactor; // Invert Y for standard screen coordinates
}

void Camera::zoom(float factor, float focalScreenX, float focalScreenY) {
    float prevZoom = zoomFactor;
    zoomFactor *= factor;

    // Clamp zoom to reasonable limits
    if (zoomFactor < 0.2f) zoomFactor = 0.2f;
    if (zoomFactor > 5.0f) zoomFactor = 5.0f;
}

void Camera::reset() {
    panX = 0.0f;
    panY = 0.0f;
    zoomFactor = 1.0f;
}

void Camera::screenToWorld(int screenX, int screenY, float* worldX, float* worldY) const {
    // Convert screen coordinates (0 to width, 0 to height) to World Coordinates
    float centeredX = (float)screenX - (windowWidth / 2.0f);
    float centeredY = (windowHeight / 2.0f) - (float)screenY; // Invert Y

    if (worldX) *worldX = (centeredX / zoomFactor) - panX;
    if (worldY) *worldY = (centeredY / zoomFactor) - panY;
}

bool Camera::isBoxVisible(float minX, float minY, float maxX, float maxY) const {
    // Basic Axis-Aligned Bounding Box (AABB) Clipping against view frustum
    if (maxX < clipLeft || minX > clipRight) return false;
    if (maxY < clipBottom || minY > clipTop) return false;
    return true;
}

float Camera::getPanX() const { return panX; }
float Camera::getPanY() const { return panY; }
float Camera::getZoom() const { return zoomFactor; }
int Camera::getWidth() const { return windowWidth; }
int Camera::getHeight() const { return windowHeight; }
