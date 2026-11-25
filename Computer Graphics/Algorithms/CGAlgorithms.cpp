#include "CGAlgorithms.h"
#include <algorithm>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <iostream>

namespace CG {

const float CG_PI = 3.14159265358979323846f;
const float DEG_TO_RAD = CG_PI / 180.0f;

// =============================================================================
// 1. 2D TRANSFORMATION MATRIX (3x3 Homogeneous Coordinates)
// =============================================================================

Matrix3x3::Matrix3x3() {
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            m[r][c] = (r == c) ? 1.0f : 0.0f;
        }
    }
}

Matrix3x3 Matrix3x3::identity() {
    return Matrix3x3();
}

Matrix3x3 Matrix3x3::translation(float dx, float dy) {
    Matrix3x3 mat;
    mat.m[0][2] = dx;
    mat.m[1][2] = dy;
    return mat;
}

Matrix3x3 Matrix3x3::scaling(float sx, float sy) {
    Matrix3x3 mat;
    mat.m[0][0] = sx;
    mat.m[1][1] = sy;
    return mat;
}

Matrix3x3 Matrix3x3::rotation(float angleDegrees) {
    Matrix3x3 mat;
    float rad = angleDegrees * DEG_TO_RAD;
    float cosA = cosf(rad);
    float sinA = sinf(rad);
    mat.m[0][0] = cosA;
    mat.m[0][1] = -sinA;
    mat.m[1][0] = sinA;
    mat.m[1][1] = cosA;
    return mat;
}

Matrix3x3 Matrix3x3::reflection(bool reflectX, bool reflectY) {
    Matrix3x3 mat;
    if (reflectX) mat.m[0][0] = -1.0f; // Horizontal reflection (flip X)
    if (reflectY) mat.m[1][1] = -1.0f; // Vertical reflection (flip Y)
    return mat;
}

Matrix3x3 Matrix3x3::multiply(const Matrix3x3& a, const Matrix3x3& b) {
    Matrix3x3 res;
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            float sum = 0.0f;
            for (int k = 0; k < 3; ++k) {
                sum += a.m[r][k] * b.m[k][c];
            }
            res.m[r][c] = sum;
        }
    }
    return res;
}

Point2Df Matrix3x3::transform(const Point2Df& p) const {
    float x = m[0][0] * p.x + m[0][1] * p.y + m[0][2];
    float y = m[1][0] * p.x + m[1][1] * p.y + m[1][2];
    float w = m[2][0] * p.x + m[2][1] * p.y + m[2][2];
    if (std::abs(w) > 1e-6f && std::abs(w - 1.0f) > 1e-6f) {
        x /= w;
        y /= w;
    }
    return Point2Df(x, y);
}

Point2D Matrix3x3::transform(const Point2D& p) const {
    Point2Df pf((float)p.x, (float)p.y);
    Point2Df res = transform(pf);
    return Point2D((int)std::round(res.x), (int)std::round(res.y));
}

// =============================================================================
// 2. 3D TRANSFORMATION MATRIX (4x4 Homogeneous Coordinates)
// =============================================================================

Matrix4x4::Matrix4x4() {
    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            m[r][c] = (r == c) ? 1.0f : 0.0f;
        }
    }
}

Matrix4x4 Matrix4x4::identity() {
    return Matrix4x4();
}

Matrix4x4 Matrix4x4::translation(float dx, float dy, float dz) {
    Matrix4x4 mat;
    mat.m[0][3] = dx;
    mat.m[1][3] = dy;
    mat.m[2][3] = dz;
    return mat;
}

Matrix4x4 Matrix4x4::scaling(float sx, float sy, float sz) {
    Matrix4x4 mat;
    mat.m[0][0] = sx;
    mat.m[1][1] = sy;
    mat.m[2][2] = sz;
    return mat;
}

Matrix4x4 Matrix4x4::rotationX(float angleDegrees) {
    Matrix4x4 mat;
    float rad = angleDegrees * DEG_TO_RAD;
    float c = cosf(rad);
    float s = sinf(rad);
    mat.m[1][1] = c;
    mat.m[1][2] = -s;
    mat.m[2][1] = s;
    mat.m[2][2] = c;
    return mat;
}

Matrix4x4 Matrix4x4::rotationY(float angleDegrees) {
    Matrix4x4 mat;
    float rad = angleDegrees * DEG_TO_RAD;
    float c = cosf(rad);
    float s = sinf(rad);
    mat.m[0][0] = c;
    mat.m[0][2] = s;
    mat.m[2][0] = -s;
    mat.m[2][2] = c;
    return mat;
}

Matrix4x4 Matrix4x4::rotationZ(float angleDegrees) {
    Matrix4x4 mat;
    float rad = angleDegrees * DEG_TO_RAD;
    float c = cosf(rad);
    float s = sinf(rad);
    mat.m[0][0] = c;
    mat.m[0][1] = -s;
    mat.m[1][0] = s;
    mat.m[1][1] = c;
    return mat;
}

Matrix4x4 Matrix4x4::perspective(float fovDegrees, float aspect, float zNear, float zFar) {
    Matrix4x4 mat;
    for (int r = 0; r < 4; ++r) for (int c = 0; c < 4; ++c) mat.m[r][c] = 0.0f;
    float tanHalfFov = tanf((fovDegrees * DEG_TO_RAD) * 0.5f);
    mat.m[0][0] = 1.0f / (aspect * tanHalfFov);
    mat.m[1][1] = 1.0f / tanHalfFov;
    mat.m[2][2] = -(zFar + zNear) / (zFar - zNear);
    mat.m[2][3] = -(2.0f * zFar * zNear) / (zFar - zNear);
    mat.m[3][2] = -1.0f;
    return mat;
}

Matrix4x4 Matrix4x4::lookAt(const Point3Df& eye, const Point3Df& target, const Point3Df& up) {
    Point3Df f(target.x - eye.x, target.y - eye.y, target.z - eye.z);
    float lenF = sqrtf(f.x*f.x + f.y*f.y + f.z*f.z);
    if (lenF > 1e-6f) { f.x /= lenF; f.y /= lenF; f.z /= lenF; }

    // s = f x up
    Point3Df s(f.y * up.z - f.z * up.y, f.z * up.x - f.x * up.z, f.x * up.y - f.y * up.x);
    float lenS = sqrtf(s.x*s.x + s.y*s.y + s.z*s.z);
    if (lenS > 1e-6f) { s.x /= lenS; s.y /= lenS; s.z /= lenS; }

    // u = s x f
    Point3Df u(s.y * f.z - s.z * f.y, s.z * f.x - s.x * f.z, s.x * f.y - s.y * f.x);

    Matrix4x4 mat;
    mat.m[0][0] = s.x;  mat.m[0][1] = s.y;  mat.m[0][2] = s.z;  mat.m[0][3] = -(s.x*eye.x + s.y*eye.y + s.z*eye.z);
    mat.m[1][0] = u.x;  mat.m[1][1] = u.y;  mat.m[1][2] = u.z;  mat.m[1][3] = -(u.x*eye.x + u.y*eye.y + u.z*eye.z);
    mat.m[2][0] = -f.x; mat.m[2][1] = -f.y; mat.m[2][2] = -f.z; mat.m[2][3] =  (f.x*eye.x + f.y*eye.y + f.z*eye.z);
    mat.m[3][0] = 0.0f; mat.m[3][1] = 0.0f; mat.m[3][2] = 0.0f; mat.m[3][3] = 1.0f;
    return mat;
}

Matrix4x4 Matrix4x4::multiply(const Matrix4x4& a, const Matrix4x4& b) {
    Matrix4x4 res;
    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            float sum = 0.0f;
            for (int k = 0; k < 4; ++k) {
                sum += a.m[r][k] * b.m[k][c];
            }
            res.m[r][c] = sum;
        }
    }
    return res;
}

Point3Df Matrix4x4::transform(const Point3Df& p) const {
    float x = m[0][0] * p.x + m[0][1] * p.y + m[0][2] * p.z + m[0][3];
    float y = m[1][0] * p.x + m[1][1] * p.y + m[1][2] * p.z + m[1][3];
    float z = m[2][0] * p.x + m[2][1] * p.y + m[2][2] * p.z + m[2][3];
    float w = m[3][0] * p.x + m[3][1] * p.y + m[3][2] * p.z + m[3][3];
    if (std::abs(w) > 1e-6f && std::abs(w - 1.0f) > 1e-6f) {
        x /= w;
        y /= w;
        z /= w;
    }
    return Point3Df(x, y, z);
}

// =============================================================================
// ALGORITHM 1: BRESENHAM'S LINE DRAWING ALGORITHM (All 8 Octants)
// =============================================================================

std::vector<Point2D> bresenhamLine(int x0, int y0, int x1, int y1) {
    std::vector<Point2D> points;

    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    int currX = x0;
    int currY = y0;

    while (true) {
        points.push_back(Point2D(currX, currY));
        if (currX == x1 && currY == y1) break;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            currX += sx;
        }
        if (e2 < dx) {
            err += dx;
            currY += sy;
        }
    }

    return points;
}

std::vector<Point2D> bresenhamSteppedWire(int x0, int y0, int x1, int y1) {
    std::vector<Point2D> result;
    int midX = (x0 + x1) / 2;

    // Segment 1: Horizontal from (x0, y0) to (midX, y0)
    std::vector<Point2D> seg1 = bresenhamLine(x0, y0, midX, y0);
    result.insert(result.end(), seg1.begin(), seg1.end());

    // Segment 2: Vertical from (midX, y0) to (midX, y1)
    std::vector<Point2D> seg2 = bresenhamLine(midX, y0, midX, y1);
    // Skip duplicate point at (midX, y0)
    if (!seg2.empty()) {
        result.insert(result.end(), seg2.begin() + 1, seg2.end());
    }

    // Segment 3: Horizontal from (midX, y1) to (x1, y1)
    std::vector<Point2D> seg3 = bresenhamLine(midX, y1, x1, y1);
    if (!seg3.empty()) {
        result.insert(result.end(), seg3.begin() + 1, seg3.end());
    }

    return result;
}

// =============================================================================
// ALGORITHM 2: MIDPOINT CIRCLE DRAWING ALGORITHM (8-Way Symmetry)
// =============================================================================

std::vector<Point2D> midpointCircle(int xc, int yc, int radius) {
    std::vector<Point2D> points;
    if (radius <= 0) {
        points.push_back(Point2D(xc, yc));
        return points;
    }

    int x = 0;
    int y = radius;
    int p = 1 - radius; // Decision parameter: 5/4 - r rounded to integer

    auto addSymmetric8 = [&](int cx, int cy, int curX, int curY) {
        points.push_back(Point2D(cx + curX, cy + curY));
        points.push_back(Point2D(cx - curX, cy + curY));
        points.push_back(Point2D(cx + curX, cy - curY));
        points.push_back(Point2D(cx - curX, cy - curY));
        points.push_back(Point2D(cx + curY, cy + curX));
        points.push_back(Point2D(cx - curY, cy + curX));
        points.push_back(Point2D(cx + curY, cy - curX));
        points.push_back(Point2D(cx - curY, cy - curX));
    };

    addSymmetric8(xc, yc, x, y);

    while (x < y) {
        x++;
        if (p < 0) {
            p += 2 * x + 1; // Midpoint inside circle
        } else {
            y--;
            p += 2 * (x - y) + 1; // Midpoint outside or on circle
        }
        addSymmetric8(xc, yc, x, y);
    }

    return points;
}

std::vector<Span> midpointCircleFilledSpans(int xc, int yc, int radius) {
    std::vector<Span> spans;
    if (radius <= 0) {
        spans.push_back(Span(yc, xc, xc));
        return spans;
    }

    int x = 0;
    int y = radius;
    int p = 1 - radius;

    // We collect horizontal line spans for each y-coordinate
    std::vector<int> minX(2 * radius + 1, 100000);
    std::vector<int> maxX(2 * radius + 1, -100000);

    auto updateSpan = [&](int curY, int curX) {
        int idx = curY - (yc - radius);
        if (idx >= 0 && idx < (int)minX.size()) {
            if (curX < minX[idx]) minX[idx] = curX;
            if (curX > maxX[idx]) maxX[idx] = curX;
        }
    };

    auto plot8 = [&](int curX, int curY) {
        updateSpan(yc + curY, xc + curX);
        updateSpan(yc + curY, xc - curX);
        updateSpan(yc - curY, xc + curX);
        updateSpan(yc - curY, xc - curX);
        updateSpan(yc + curX, xc + curY);
        updateSpan(yc + curX, xc - curY);
        updateSpan(yc - curX, xc + curY);
        updateSpan(yc - curX, xc - curY);
    };

    plot8(x, y);
    while (x < y) {
        x++;
        if (p < 0) {
            p += 2 * x + 1;
        } else {
            y--;
            p += 2 * (x - y) + 1;
        }
        plot8(x, y);
    }

    for (int i = 0; i < (int)minX.size(); ++i) {
        if (minX[i] <= maxX[i]) {
            spans.push_back(Span((yc - radius) + i, minX[i], maxX[i]));
        }
    }

    return spans;
}

// =============================================================================
// ALGORITHM 3: SCANLINE POLYGON FILLING ALGORITHM (ET & AET)
// =============================================================================

std::vector<Span> scanlineFillPolygon(const std::vector<Point2D>& vertices) {
    std::vector<Span> spans;
    int n = (int)vertices.size();
    if (n < 3) return spans;

    // Find bounding box for scanlines
    int minY = vertices[0].y;
    int maxY = vertices[0].y;
    for (int i = 1; i < n; ++i) {
        if (vertices[i].y < minY) minY = vertices[i].y;
        if (vertices[i].y > maxY) maxY = vertices[i].y;
    }

    // Build Edge Table (ET) bucketed by yMin
    struct ETRecord {
        int yMax;
        float xCurrent;
        float invSlope; // dx / dy
    };

    int numBuckets = maxY - minY + 1;
    std::vector<std::vector<ETRecord>> ET(numBuckets);

    for (int i = 0; i < n; ++i) {
        Point2D p1 = vertices[i];
        Point2D p2 = vertices[(i + 1) % n];

        if (p1.y == p2.y) continue; // Ignore horizontal edges

        Point2D lower = (p1.y < p2.y) ? p1 : p2;
        Point2D upper = (p1.y < p2.y) ? p2 : p1;

        ETRecord edge;
        edge.yMax = upper.y;
        edge.xCurrent = (float)lower.x;
        edge.invSlope = (float)(upper.x - lower.x) / (float)(upper.y - lower.y);

        int bucketIdx = lower.y - minY;
        if (bucketIdx >= 0 && bucketIdx < numBuckets) {
            ET[bucketIdx].push_back(edge);
        }
    }

    // Active Edge Table (AET)
    std::vector<ETRecord> AET;

    for (int y = minY; y <= maxY; ++y) {
        // 1. Add edges from ET[y] into AET
        int bucketIdx = y - minY;
        if (bucketIdx >= 0 && bucketIdx < numBuckets) {
            for (const auto& edge : ET[bucketIdx]) {
                AET.push_back(edge);
            }
        }

        // 2. Remove edges from AET where yMax == y
        AET.erase(std::remove_if(AET.begin(), AET.end(), [y](const ETRecord& e) {
            return e.yMax <= y;
        }), AET.end());

        // 3. Sort AET by current x value
        std::sort(AET.begin(), AET.end(), [](const ETRecord& a, const ETRecord& b) {
            return a.xCurrent < b.xCurrent;
        });

        // 4. Fill spans between pairs of edge intersections (parity rule)
        for (size_t i = 0; i + 1 < AET.size(); i += 2) {
            int xStart = (int)std::ceil(AET[i].xCurrent);
            int xEnd = (int)std::floor(AET[i + 1].xCurrent);
            if (xStart <= xEnd) {
                spans.push_back(Span(y, xStart, xEnd));
            }
        }

        // 5. Update x coordinates for next scanline
        for (auto& edge : AET) {
            edge.xCurrent += edge.invSlope;
        }
    }

    return spans;
}

// =============================================================================
// COMPONENT 2D TRANSFORMATION HELPER (Rotation, Reflection & Scale)
// =============================================================================

Point2Df transformGatePoint(float px, float py, float gateCenterX, float gateCenterY, 
                           float rotationDegrees, bool flipH, bool flipV, float scaleX, float scaleY) {
    // 1. Translate to local origin relative to gate center
    float localX = px - gateCenterX;
    float localY = py - gateCenterY;

    // 2. Scale
    localX *= scaleX;
    localY *= scaleY;

    // 3. Reflection
    if (flipH) localX = -localX;
    if (flipV) localY = -localY;

    // 4. Rotation around gate center
    float rad = rotationDegrees * DEG_TO_RAD;
    float cosA = cosf(rad);
    float sinA = sinf(rad);
    float rotX = localX * cosA - localY * sinA;
    float rotY = localX * sinA + localY * cosA;

    // 5. Translate back to world coordinates
    return Point2Df(gateCenterX + rotX, gateCenterY + rotY);
}

// =============================================================================
// INDEPENDENT AUTOMATED UNIT TEST SUITE
// =============================================================================

TestResults runAllCGAlgorithmTests() {
    TestResults tr;
    tr.passed = 0;
    tr.failed = 0;

    auto check = [&](bool condition, const std::string& name, const std::string& detail) {
        if (condition) {
            tr.passed++;
            tr.details.push_back("[PASS] " + name + ": " + detail);
        } else {
            tr.failed++;
            tr.details.push_back("[FAIL] " + name + ": " + detail);
        }
    };

    // -------------------------------------------------------------------------
    // Test 1: Bresenham Line - Horizontal, Vertical, Diagonal, Arbitrary Octants
    // -------------------------------------------------------------------------
    {
        auto ptsH = bresenhamLine(0, 0, 10, 0);
        check(ptsH.size() == 11 && ptsH.front().x == 0 && ptsH.back().x == 10 && ptsH.back().y == 0,
              "Bresenham Horizontal Line", "Points count: 11, strictly collinear on y=0");

        auto ptsV = bresenhamLine(5, -5, 5, 5);
        check(ptsV.size() == 11 && ptsV.front().y == -5 && ptsV.back().y == 5 && ptsV.back().x == 5,
              "Bresenham Vertical Line", "Points count: 11, strictly collinear on x=5");

        auto ptsDiag = bresenhamLine(0, 0, 8, 8);
        bool diagValid = (ptsDiag.size() == 9);
        for (size_t i = 0; i < ptsDiag.size(); ++i) {
            if (ptsDiag[i].x != (int)i || ptsDiag[i].y != (int)i) diagValid = false;
        }
        check(diagValid, "Bresenham Diagonal Line (m=1)", "Slope exactly 1.0, 9 discrete pixel steps");

        auto ptsSteep = bresenhamLine(0, 0, -4, 12);
        check(ptsSteep.size() == 13 && ptsSteep.front().x == 0 && ptsSteep.back().x == -4 && ptsSteep.back().y == 12,
              "Bresenham Steep Negative-Slope (Octant 3)", "Correct step direction, |m| > 1, endpoints exact");

        auto steppedWire = bresenhamSteppedWire(10, 20, 70, 80);
        check(steppedWire.front().x == 10 && steppedWire.front().y == 20 &&
              steppedWire.back().x == 70 && steppedWire.back().y == 80 && steppedWire.size() > 50,
              "Bresenham Circuit Wire Routing", "Orthogonal 3-segment trace generated via Bresenham");
    }

    // -------------------------------------------------------------------------
    // Test 2: Midpoint Circle Drawing - 8-Octant Symmetry & Filled Spans
    // -------------------------------------------------------------------------
    {
        int r = 15;
        auto circlePts = midpointCircle(0, 0, r);
        check(circlePts.size() >= 80, "Midpoint Circle Perimeter", 
              "Generated " + std::to_string(circlePts.size()) + " perimeter boundary pixels with 8-way symmetry");

        bool radiusAccurate = true;
        for (const auto& pt : circlePts) {
            float dist = sqrtf((float)(pt.x * pt.x + pt.y * pt.y));
            if (std::abs(dist - r) > 1.25f) radiusAccurate = false;
        }
        check(radiusAccurate, "Midpoint Circle Radius Precision", "All pixels within integer quantization bound (|dist - r| <= 1.25)");

        auto filledSpans = midpointCircleFilledSpans(50, 50, 10);
        check(filledSpans.size() == 21 && filledSpans.front().y == 40 && filledSpans.back().y == 60,
              "Midpoint Circle Solid Span Rasterizer", "Filled 21 continuous horizontal scanline spans (height 2*r+1)");
    }

    // -------------------------------------------------------------------------
    // Test 3: Scanline Polygon Filling - Convex Triangle & Gate Profile
    // -------------------------------------------------------------------------
    {
        std::vector<Point2D> triangle = { Point2D(10, 10), Point2D(30, 10), Point2D(20, 30) };
        auto triSpans = scanlineFillPolygon(triangle);
        check(triSpans.size() >= 20, "Scanline Fill Triangle", "Rasterized 20 continuous scanlines from y=10 to y=29 (standard half-open scanline)");

        // Concave logic-gate notch polygon
        std::vector<Point2D> gatePolygon = {
            Point2D(0, 0), Point2D(40, 0), Point2D(50, 15), Point2D(40, 30), Point2D(0, 30), Point2D(10, 15)
        };
        auto gateSpans = scanlineFillPolygon(gatePolygon);
        check(gateSpans.size() > 20, "Scanline Fill Custom Gate Body", 
              "Rasterized concave polygon with " + std::to_string(gateSpans.size()) + " spans");
    }

    // -------------------------------------------------------------------------
    // Test 4: 2D Transformations (Translation, Scale, Rotation, Reflection)
    // -------------------------------------------------------------------------
    {
        Matrix3x3 T = Matrix3x3::translation(25.0f, -15.0f);
        Point2Df p1(10.0f, 20.0f);
        Point2Df resT = T.transform(p1);
        check(std::abs(resT.x - 35.0f) < 1e-4f && std::abs(resT.y - 5.0f) < 1e-4f,
              "2D Translation Transform", "(10, 20) translated by (25, -15) -> (35, 5)");

        Matrix3x3 R = Matrix3x3::rotation(90.0f);
        Point2Df p2(10.0f, 0.0f);
        Point2Df resR = R.transform(p2);
        check(std::abs(resR.x) < 1e-4f && std::abs(resR.y - 10.0f) < 1e-4f,
              "2D Rotation Transform (90 deg)", "(10, 0) rotated by 90 deg -> (0, 10)");

        Matrix3x3 M = Matrix3x3::reflection(true, false); // Flip X
        Point2Df p3(15.0f, 30.0f);
        Point2Df resM = M.transform(p3);
        check(std::abs(resM.x + 15.0f) < 1e-4f && std::abs(resM.y - 30.0f) < 1e-4f,
              "2D Reflection Transform (Flip X)", "(15, 30) reflected across Y-axis -> (-15, 30)");

        // Composite Matrix: M = T * R * S
        Matrix3x3 S = Matrix3x3::scaling(2.0f, 2.0f);
        Matrix3x3 TR = Matrix3x3::multiply(T, R);
        Matrix3x3 TRS = Matrix3x3::multiply(TR, S);
        Point2Df resTRS = TRS.transform(Point2Df(5.0f, 0.0f)); // scale to 10, rotate 90 -> (0, 10), translate -> (25, -5)
        check(std::abs(resTRS.x - 25.0f) < 1e-4f && std::abs(resTRS.y - (-5.0f)) < 1e-4f,
              "2D Composite Matrix Multiplier (T * R * S)", "Matrix concatenation verified exact: (5,0) -> (25,-5)");
    }

    // -------------------------------------------------------------------------
    // Test 5: 3D Transformations (Translation, 3D Rotation, Perspective)
    // -------------------------------------------------------------------------
    {
        Matrix4x4 T3 = Matrix4x4::translation(10.0f, -20.0f, 30.0f);
        Point3Df p3d(1.0f, 2.0f, 3.0f);
        Point3Df resT3 = T3.transform(p3d);
        check(std::abs(resT3.x - 11.0f) < 1e-4f && std::abs(resT3.y + 18.0f) < 1e-4f && std::abs(resT3.z - 33.0f) < 1e-4f,
              "3D Translation Transform", "(1, 2, 3) translated -> (11, -18, 33)");

        Matrix4x4 Ry = Matrix4x4::rotationY(90.0f);
        Point3Df pY(1.0f, 0.0f, 0.0f);
        Point3Df resRy = Ry.transform(pY);
        check(std::abs(resRy.x) < 1e-4f && std::abs(resRy.z + 1.0f) < 1e-4f,
              "3D Rotation Transform (Y-Axis 90 deg)", "(1, 0, 0) rotated around Y-axis -> (0, 0, -1)");

        Matrix4x4 proj = Matrix4x4::perspective(60.0f, 16.0f / 9.0f, 0.1f, 100.0f);
        Point3Df viewPoint(0.0f, 0.0f, -5.0f);
        Point3Df projPoint = proj.transform(viewPoint);
        check(std::abs(projPoint.x) < 1e-4f && std::abs(projPoint.y) < 1e-4f && projPoint.z >= -1.0f && projPoint.z <= 1.0f,
              "3D Perspective Projection Matrix", "Projected point falls into normalized device coordinates (NDC)");
    }

    // -------------------------------------------------------------------------
    // Test 6: Gate Pin Transformation & Wire Integrity Preservation
    // -------------------------------------------------------------------------
    {
        // Untransformed gate center at (100, 100), input pin at (55, 80) (dx = -45, dy = -20)
        // Rotate by 90 deg clockwise (-90 deg or +90 deg standard)
        Point2Df pinRot90 = transformGatePoint(55.0f, 80.0f, 100.0f, 100.0f, 90.0f, false, false);
        // local dx = -45, dy = -20. Rot 90: x' = -45*0 - (-20)*1 = 20, y' = -45*1 + (-20)*0 = -45.
        // World: 100 + 20 = 120, 100 - 45 = 55.
        check(std::abs(pinRot90.x - 120.0f) < 1e-3f && std::abs(pinRot90.y - 55.0f) < 1e-3f,
              "Gate Pin Rotation (90 deg)", "Pin transformed accurately around gate origin");

        Point2Df pinFlipH = transformGatePoint(55.0f, 80.0f, 100.0f, 100.0f, 0.0f, true, false);
        // local dx = -45, dy = -20. FlipH: dx' = 45, dy' = -20. World: (145, 80)
        check(std::abs(pinFlipH.x - 145.0f) < 1e-3f && std::abs(pinFlipH.y - 80.0f) < 1e-3f,
              "Gate Pin Horizontal Reflection (Flip H)", "Input pin reflects to right terminal");

        // Verify that wire endpoints connecting to this pin match the transformed coordinate exactly
        auto wirePoints = bresenhamLine((int)pinFlipH.x, (int)pinFlipH.y, 200, 80);
        check(wirePoints.front().x == 145 && wirePoints.front().y == 80 && wirePoints.back().x == 200,
              "Wire Connectivity After Component Transformation", "Wire anchors continuously to transformed gate pin");
    }

    return tr;
}

} // namespace CG
