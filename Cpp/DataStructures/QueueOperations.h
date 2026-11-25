#ifndef QUEUE_OPERATIONS_H
#define QUEUE_OPERATIONS_H

#include "../Gates/Gate.h"

/**
 * @file QueueOperations.h
 * @brief LogicCraft Simulation Event Propagation Queue (CO3 - Queue Data Structure)
 *
 * ACADEMIC JUSTIFICATION:
 * When an input terminal or gate output transitions (0 -> 1 or 1 -> 0), the resulting
 * signal propagation must be evaluated in chronological arrival order. A FIFO (First-In,
 * First-Out) Queue schedules gate evaluation jobs, ensuring that upstream gate changes
 * settle before downstream gates read the updated wires.
 *
 * ACADEMIC RESTRICTION COMPLIANCE:
 * Strictly procedural design using C-style circular array buffer and standalone functions.
 * No classes, inheritance, or OOP abstractions are introduced.
 */

// Event payload placed into the simulation queue
struct SimulationEvent {
    int eventId;
    int targetGateId;
    int targetPinIndex;
    int signalValue;     // 0 or 1
    int simulationStep;
    char eventDescription[80];
};

const int MAX_QUEUE_CAPACITY = 32;

// Circular Queue Data Structure
struct SimulationQueue {
    SimulationEvent events[MAX_QUEUE_CAPACITY];
    int front; // Index of the front element
    int rear;  // Index of the rear element
    int count; // Number of active elements
};

// Procedural Queue Operations (FIFO)
void initSimulationQueue(SimulationQueue* queue);
bool isQueueEmpty(const SimulationQueue* queue);
bool isQueueFull(const SimulationQueue* queue);
bool enqueueEvent(SimulationQueue* queue, const SimulationEvent* event);
bool dequeueEvent(SimulationQueue* queue, SimulationEvent* dequeuedEvent);
bool peekQueue(const SimulationQueue* queue, SimulationEvent* frontEvent);
void displaySimulationQueue(const SimulationQueue* queue);
int getQueueSize(const SimulationQueue* queue);
void clearSimulationQueue(SimulationQueue* queue);

// Event constructor helper
SimulationEvent createSimulationEvent(int id, int gateId, int pinIdx, int signalVal, int step, const char* desc = "");

#endif // QUEUE_OPERATIONS_H
