#ifndef STACK_OPERATIONS_H
#define STACK_OPERATIONS_H

#include "../Gates/Gate.h"

/**
 * @file StackOperations.h
 * @brief LogicCraft Circuit Operation Undo Stack (CO3 - Stack Data Structure)
 *
 * ACADEMIC JUSTIFICATION:
 * In a visual circuit simulator, user modifications (adding gates, toggling inputs,
 * deleting components) must be reversible. A Stack enforces Last-In, First-Out (LIFO)
 * ordering, where the most recently applied canvas modification is the first one undone.
 *
 * ACADEMIC RESTRICTION COMPLIANCE:
 * Strictly procedural design using C-style structs and standalone functions.
 * No classes, inheritance, or OOP abstractions are introduced.
 */

// Action types that can be recorded on the Undo Stack
enum CircuitActionType {
    ACTION_ADD_GATE = 1,
    ACTION_DELETE_GATE = 2,
    ACTION_UPDATE_INPUT = 3,
    ACTION_ADD_CONNECTION = 4
};

// Record stored on the Stack
struct CircuitAction {
    CircuitActionType type;
    int gateId;
    char gateType[16];
    char gateName[32];
    int pinIndex;
    int previousValue;
    int newValue;
    int srcGateId;
    int dstGateId;
    int dstPin;
    char description[96];
};

const int MAX_STACK_CAPACITY = 32;

// Stack Data Structure
struct CircuitUndoStack {
    CircuitAction items[MAX_STACK_CAPACITY];
    int top; // Index of top element (-1 when empty)
};

// Procedural Stack Operations (LIFO)
void initUndoStack(CircuitUndoStack* stack);
bool isStackEmpty(const CircuitUndoStack* stack);
bool isStackFull(const CircuitUndoStack* stack);
bool pushAction(CircuitUndoStack* stack, const CircuitAction* action);
bool popAction(CircuitUndoStack* stack, CircuitAction* poppedAction);
bool peekAction(const CircuitUndoStack* stack, CircuitAction* topAction);
void displayUndoStack(const CircuitUndoStack* stack);
int getStackSize(const CircuitUndoStack* stack);
void clearUndoStack(CircuitUndoStack* stack);

// Helpers to construct specific actions
CircuitAction createAddGateAction(int gateId, const char* type, const char* name);
CircuitAction createDeleteGateAction(int gateId, const char* type, const char* name);
CircuitAction createUpdateInputAction(int gateId, int pin, int oldVal, int newVal);
CircuitAction createAddConnectionAction(int srcId, int dstId, int dstPin);

#endif // STACK_OPERATIONS_H
