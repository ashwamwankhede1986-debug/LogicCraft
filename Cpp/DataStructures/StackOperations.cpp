#include "StackOperations.h"
#include <iostream>
#include <cstring>

/**
 * @file StackOperations.cpp
 * @brief Procedural Implementation of Circuit Undo Stack (CO3 - Stack)
 */

void initUndoStack(CircuitUndoStack* stack) {
    if (stack == nullptr) return;
    stack->top = -1;
}

bool isStackEmpty(const CircuitUndoStack* stack) {
    if (stack == nullptr) return true;
    return (stack->top == -1);
}

bool isStackFull(const CircuitUndoStack* stack) {
    if (stack == nullptr) return false;
    return (stack->top >= MAX_STACK_CAPACITY - 1);
}

bool pushAction(CircuitUndoStack* stack, const CircuitAction* action) {
    if (stack == nullptr || action == nullptr) {
        std::cout << "[Stack Error] Null pointer passed to pushAction.\n";
        return false;
    }

    // Boundary condition: Stack Overflow
    if (isStackFull(stack)) {
        std::cout << "[Stack Error: OVERFLOW] Undo stack reached maximum capacity (" 
                  << MAX_STACK_CAPACITY << "). Cannot push new action.\n";
        return false;
    }

    stack->top++;
    stack->items[stack->top] = *action;
    return true;
}

bool popAction(CircuitUndoStack* stack, CircuitAction* poppedAction) {
    if (stack == nullptr) {
        std::cout << "[Stack Error] Null pointer passed to popAction.\n";
        return false;
    }

    // Boundary condition: Stack Underflow
    if (isStackEmpty(stack)) {
        std::cout << "[Stack Error: UNDERFLOW] Undo stack is empty. No operation to undo.\n";
        return false;
    }

    if (poppedAction != nullptr) {
        *poppedAction = stack->items[stack->top];
    }
    stack->top--;
    return true;
}

bool peekAction(const CircuitUndoStack* stack, CircuitAction* topAction) {
    if (stack == nullptr || topAction == nullptr) return false;
    if (isStackEmpty(stack)) {
        return false;
    }
    *topAction = stack->items[stack->top];
    return true;
}

void displayUndoStack(const CircuitUndoStack* stack) {
    std::cout << "\n======================================================\n";
    std::cout << "      LOGICCRAFT CIRCUIT UNDO STACK (LIFO / CO3)      \n";
    std::cout << "======================================================\n";

    if (stack == nullptr || isStackEmpty(stack)) {
        std::cout << "Stack State: [EMPTY] (0 / " << MAX_STACK_CAPACITY << " elements)\n";
        std::cout << "======================================================\n\n";
        return;
    }

    std::cout << "Current Stack Depth: " << (stack->top + 1) << " / " << MAX_STACK_CAPACITY << "\n";
    std::cout << "Top Index: [" << stack->top << "] -> LIFO Next to Undo\n";
    std::cout << "------------------------------------------------------\n";

    for (int i = stack->top; i >= 0; i--) {
        const CircuitAction& a = stack->items[i];
        std::cout << "Level [" << i << "]" << (i == stack->top ? " <- [TOP] : " : "         : ");
        switch (a.type) {
            case ACTION_ADD_GATE:
                std::cout << "ADD_GATE -> [ID: " << a.gateId << "] " << a.gateType << " ('" << a.gateName << "')\n";
                break;
            case ACTION_DELETE_GATE:
                std::cout << "DELETE_GATE -> [ID: " << a.gateId << "] " << a.gateType << " ('" << a.gateName << "')\n";
                break;
            case ACTION_UPDATE_INPUT:
                std::cout << "UPDATE_INPUT -> [Gate: " << a.gateId << ", Pin: " << a.pinIndex 
                          << "] Old=" << a.previousValue << " -> New=" << a.newValue << "\n";
                break;
            case ACTION_ADD_CONNECTION:
                std::cout << "ADD_CONNECTION -> Wire Gate " << a.srcGateId << "[OUT] ===> Gate " 
                          << a.dstGateId << "[Pin " << a.dstPin << "]\n";
                break;
            default:
                std::cout << "UNKNOWN_ACTION\n";
                break;
        }
    }
    std::cout << "======================================================\n\n";
}

int getStackSize(const CircuitUndoStack* stack) {
    if (stack == nullptr) return 0;
    return stack->top + 1;
}

void clearUndoStack(CircuitUndoStack* stack) {
    if (stack != nullptr) {
        stack->top = -1;
    }
}

CircuitAction createAddGateAction(int gateId, const char* type, const char* name) {
    CircuitAction a;
    a.type = ACTION_ADD_GATE;
    a.gateId = gateId;
    std::strncpy(a.gateType, type ? type : "", sizeof(a.gateType) - 1);
    a.gateType[sizeof(a.gateType) - 1] = '\0';
    std::strncpy(a.gateName, name ? name : "", sizeof(a.gateName) - 1);
    a.gateName[sizeof(a.gateName) - 1] = '\0';
    a.pinIndex = 0;
    a.previousValue = 0;
    a.newValue = 0;
    a.srcGateId = 0;
    a.dstGateId = 0;
    a.dstPin = 0;
    std::snprintf(a.description, sizeof(a.description), "Add %s gate '%s' (ID: %d)", a.gateType, a.gateName, gateId);
    return a;
}

CircuitAction createDeleteGateAction(int gateId, const char* type, const char* name) {
    CircuitAction a;
    a.type = ACTION_DELETE_GATE;
    a.gateId = gateId;
    std::strncpy(a.gateType, type ? type : "", sizeof(a.gateType) - 1);
    a.gateType[sizeof(a.gateType) - 1] = '\0';
    std::strncpy(a.gateName, name ? name : "", sizeof(a.gateName) - 1);
    a.gateName[sizeof(a.gateName) - 1] = '\0';
    a.pinIndex = 0;
    a.previousValue = 0;
    a.newValue = 0;
    a.srcGateId = 0;
    a.dstGateId = 0;
    a.dstPin = 0;
    std::snprintf(a.description, sizeof(a.description), "Delete %s gate '%s' (ID: %d)", a.gateType, a.gateName, gateId);
    return a;
}

CircuitAction createUpdateInputAction(int gateId, int pin, int oldVal, int newVal) {
    CircuitAction a;
    a.type = ACTION_UPDATE_INPUT;
    a.gateId = gateId;
    a.gateType[0] = '\0';
    a.gateName[0] = '\0';
    a.pinIndex = pin;
    a.previousValue = oldVal;
    a.newValue = newVal;
    a.srcGateId = 0;
    a.dstGateId = 0;
    a.dstPin = 0;
    std::snprintf(a.description, sizeof(a.description), "Toggle Input Gate %d Pin %d (%d -> %d)", gateId, pin, oldVal, newVal);
    return a;
}

CircuitAction createAddConnectionAction(int srcId, int dstId, int dstPin) {
    CircuitAction a;
    a.type = ACTION_ADD_CONNECTION;
    a.gateId = 0;
    a.gateType[0] = '\0';
    a.gateName[0] = '\0';
    a.pinIndex = 0;
    a.previousValue = 0;
    a.newValue = 0;
    a.srcGateId = srcId;
    a.dstGateId = dstId;
    a.dstPin = dstPin;
    std::snprintf(a.description, sizeof(a.description), "Connect Gate %d [OUT] -> Gate %d [Pin %d]", srcId, dstId, dstPin);
    return a;
}
