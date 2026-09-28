#ifndef CANVAS_H
#define CANVAS_H

#include "../Transformations/Camera.h"
#include <string>

/**
 * @file Canvas.h
 * @brief LogicCraft Circuit Canvas & Visual Component Management (CO1 & CO2)
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

    bool isDraggingGate;
    float dragStartGateX;
    float dragStartGateY;

public:
    Canvas();

    void initDefaultCircuit();

    // Rendering
    void render(const Camera& camera);
    void renderGrid(const Camera& camera);
    void renderWires();
    void renderHUD(int windowWidth, int windowHeight, const Camera& camera);

    // Interaction & Transformations (CO2)
    int pickGate(float worldX, float worldY);
    void selectGate(int id);
    void moveSelectedGate(float dx, float dy);
    void setGatePosition(int id, float x, float y);

    void onMouseDown(int button, float worldX, float worldY);
    void onMouseMove(float worldX, float worldY);
    void onMouseUp(int button);

    // Toggle logic input for demonstration
    void toggleSelectedGateInput();

    // Evaluation
    void evaluateCircuit();

    int getSelectedGateId() const;
    int getGateCount() const;
};

#endif // CANVAS_H
