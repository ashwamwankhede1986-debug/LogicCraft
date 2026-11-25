#ifndef CANVAS_H
#define CANVAS_H

#include "../Transformations/Camera.h"
#include <string>

/**
 * @file Canvas.h
 * @brief LogicCraft Circuit Canvas & Visual Component Management (CO1, CO2, CO4, CO6)
 * Supports 2D component transformations (Rotation, Reflection, Scaling) and 3D Visualization Mode
 */

struct VisualGate {
    int id;
    char type[16];
    char label[32];
    float x;
    float y;
    float width;
    float height;
    bool isSelected;

    // Transformations (2D)
    float rotation; // Degrees: 0, 90, 180, 270
    bool flipH;     // Horizontal reflection
    bool flipV;     // Vertical reflection
    float scaleX;   // Component local scale
    float scaleY;

    // Logic state
    int inputA;
    int inputB;
    int output;
};

const int MAX_VISUAL_GATES = 16;

class Canvas {
private:
    VisualGate gates[MAX_VISUAL_GATES];
    int gateCount;
    int selectedGateId;
    int hoveredGateId;

    bool isDraggingGate;
    float dragStartGateX;
    float dragStartGateY;

    // 3D Visualization State
    bool mode3D;
    float yaw3D;
    float pitch3D;
    float distance3D;

public:
    Canvas();

    void initDefaultCircuit();
    void initAll7GatesShowcase();

    // Rendering
    void render(const Camera& camera);
    void renderGrid(const Camera& camera);
    void renderWires();
    void renderHUD(int windowWidth, int windowHeight, const Camera& camera);
    void render3DView(int windowWidth, int windowHeight);

    // Interaction & Transformations
    int pickGate(float worldX, float worldY);
    void selectGate(int id);
    void updateHover(float worldX, float worldY);
    void moveSelectedGate(float dx, float dy);
    void setGatePosition(int id, float x, float y);

    // Component Transformations
    void rotateSelectedGate(float deltaDegrees = 90.0f);
    void reflectSelectedGateHorizontal();
    void reflectSelectedGateVertical();
    void scaleSelectedGate(float factorX, float factorY);

    // 3D Mode Controls
    void toggle3DMode();
    bool is3DModeActive() const;
    void rotate3D(float dYaw, float dPitch);
    void zoom3D(float dDist);
    void reset3DView();

    void onMouseDown(int button, float worldX, float worldY);
    void onMouseMove(float worldX, float worldY);
    void onMouseUp(int button);

    // Toggle logic input for demonstration
    void toggleSelectedGateInput();
    void cycleSelectedGateType();

    // Evaluation
    void evaluateCircuit();

    int getSelectedGateId() const;
    int getGateCount() const;
    const VisualGate* getGate(int index) const;
    VisualGate* getSelectedGate();
};

#endif // CANVAS_H
