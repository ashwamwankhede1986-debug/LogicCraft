#ifndef CG_ALGORITHMS_H
#define CG_ALGORITHMS_H

#include <vector>
#include <string>
#include <cmath>

/**
 * @file CGAlgorithms.h
 * @brief LogicCraft Core Computer Graphics Algorithms
 * 
 * Implements fundamental Computer Graphics algorithms without relying on OpenGL built-ins:
 * 1. Bresenham's Line Drawing Algorithm (All 8 octants, horizontal, vertical, diagonal)
 * 2. Midpoint Circle Drawing Algorithm (8-way symmetry, perimeters & filled circles)
 * 3. Scanline Polygon Filling Algorithm (Edge Table ET & Active Edge Table AET)
 * 4. 2D Transformations (3x3 Homogeneous Matrices: Translation, Scaling, Rotation, Reflection)
 * 5. 3D Transformations (4x4 Homogeneous Matrices: Translation, Scaling, 3-Axis Rotation, Projection)
 */

namespace CG {

struct Point2D {
    int x;
    int y;
    Point2D() : x(0), y(0) {}
    Point2D(int _x, int _y) : x(_x), y(_y) {}
};

struct Point2Df {
    float x;
    float y;
    Point2Df() : x(0.0f), y(0.0f) {}
    Point2Df(float _x, float _y) : x(_x), y(_y) {}
};

struct Point3Df {
    float x;
    float y;
    float z;
    Point3Df() : x(0.0f), y(0.0f), z(0.0f) {}
    Point3Df(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
};

struct Span {
    int y;
    int xStart;
    int xEnd;
    Span() : y(0), xStart(0), xEnd(0) {}
    Span(int _y, int _xs, int _xe) : y(_y), xStart(_xs), xEnd(_xe) {}
};

// -----------------------------------------------------------------------------
// 1. 2D Transformation Matrix (3x3 Homogeneous Coordinates)
// -----------------------------------------------------------------------------
struct Matrix3x3 {
    float m[3][3];

    Matrix3x3();
    static Matrix3x3 identity();
    static Matrix3x3 translation(float dx, float dy);
    static Matrix3x3 scaling(float sx, float sy);
    static Matrix3x3 rotation(float angleDegrees);
    static Matrix3x3 reflection(bool reflectX, bool reflectY);
    static Matrix3x3 multiply(const Matrix3x3& a, const Matrix3x3& b);

    Point2Df transform(const Point2Df& p) const;
    Point2D transform(const Point2D& p) const;
};

// -----------------------------------------------------------------------------
// 2. 3D Transformation Matrix (4x4 Homogeneous Coordinates)
// -----------------------------------------------------------------------------
struct Matrix4x4 {
    float m[4][4];

    Matrix4x4();
    static Matrix4x4 identity();
    static Matrix4x4 translation(float dx, float dy, float dz);
    static Matrix4x4 scaling(float sx, float sy, float sz);
    static Matrix4x4 rotationX(float angleDegrees);
    static Matrix4x4 rotationY(float angleDegrees);
    static Matrix4x4 rotationZ(float angleDegrees);
    static Matrix4x4 perspective(float fovDegrees, float aspect, float zNear, float zFar);
    static Matrix4x4 lookAt(const Point3Df& eye, const Point3Df& target, const Point3Df& up);
    static Matrix4x4 multiply(const Matrix4x4& a, const Matrix4x4& b);

    Point3Df transform(const Point3Df& p) const;
};

// -----------------------------------------------------------------------------
// Algorithm 1: Bresenham's Line Drawing Algorithm
// -----------------------------------------------------------------------------
std::vector<Point2D> bresenhamLine(int x0, int y0, int x1, int y1);
std::vector<Point2D> bresenhamSteppedWire(int x0, int y0, int x1, int y1);

// -----------------------------------------------------------------------------
// Algorithm 2: Midpoint Circle Drawing Algorithm
// -----------------------------------------------------------------------------
std::vector<Point2D> midpointCircle(int xc, int yc, int radius);
std::vector<Span> midpointCircleFilledSpans(int xc, int yc, int radius);

// -----------------------------------------------------------------------------
// Algorithm 3: Scanline Polygon Filling Algorithm (ET & AET)
// -----------------------------------------------------------------------------
struct ScanlineEdge {
    int yMax;
    float xCurrent;
    float invSlope; // dx / dy
};

std::vector<Span> scanlineFillPolygon(const std::vector<Point2D>& vertices);

// -----------------------------------------------------------------------------
// Algorithm 4 & 5: Component Transformation Helpers
// -----------------------------------------------------------------------------
Point2Df transformGatePoint(float px, float py, float gateCenterX, float gateCenterY, 
                           float rotationDegrees, bool flipH, bool flipV, float scaleX = 1.0f, float scaleY = 1.0f);

// -----------------------------------------------------------------------------
// Independent Automated Unit Test Verification
// -----------------------------------------------------------------------------
struct TestResults {
    int passed;
    int failed;
    std::vector<std::string> details;
};

TestResults runAllCGAlgorithmTests();

} // namespace CG

#endif // CG_ALGORITHMS_H
