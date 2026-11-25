#include "QueueOperations.h"
#include <iostream>
#include <cstring>
#include <cstdio>

/**
 * @file QueueOperations.cpp
 * @brief Procedural Implementation of FIFO Simulation Event Queue (CO3 - Queue)
 */

void initSimulationQueue(SimulationQueue* queue) {
    if (queue == nullptr) return;
    queue->front = 0;
    queue->rear = -1;
    queue->count = 0;
}

bool isQueueEmpty(const SimulationQueue* queue) {
    if (queue == nullptr) return true;
    return (queue->count == 0);
}

bool isQueueFull(const SimulationQueue* queue) {
    if (queue == nullptr) return false;
    return (queue->count >= MAX_QUEUE_CAPACITY);
}

bool enqueueEvent(SimulationQueue* queue, const SimulationEvent* event) {
    if (queue == nullptr || event == nullptr) {
        std::cout << "[Queue Error] Null pointer passed to enqueueEvent.\n";
        return false;
    }

    // Boundary condition: Queue Overflow
    if (isQueueFull(queue)) {
        std::cout << "[Queue Error: OVERFLOW] Simulation event queue is full (" 
                  << MAX_QUEUE_CAPACITY << " events). Dropping event ID " << event->eventId << ".\n";
        return false;
    }

    // Circular increment of rear pointer
    queue->rear = (queue->rear + 1) % MAX_QUEUE_CAPACITY;
    queue->events[queue->rear] = *event;
    queue->count++;
    return true;
}

bool dequeueEvent(SimulationQueue* queue, SimulationEvent* dequeuedEvent) {
    if (queue == nullptr) {
        std::cout << "[Queue Error] Null pointer passed to dequeueEvent.\n";
        return false;
    }

    // Boundary condition: Queue Underflow
    if (isQueueEmpty(queue)) {
        std::cout << "[Queue Error: UNDERFLOW] Simulation event queue is empty. No pending signal.\n";
        return false;
    }

    if (dequeuedEvent != nullptr) {
        *dequeuedEvent = queue->events[queue->front];
    }

    // Circular increment of front pointer
    queue->front = (queue->front + 1) % MAX_QUEUE_CAPACITY;
    queue->count--;
    return true;
}

bool peekQueue(const SimulationQueue* queue, SimulationEvent* frontEvent) {
    if (queue == nullptr || frontEvent == nullptr) return false;
    if (isQueueEmpty(queue)) {
        return false;
    }
    *frontEvent = queue->events[queue->front];
    return true;
}

void displaySimulationQueue(const SimulationQueue* queue) {
    std::cout << "\n======================================================\n";
    std::cout << "    LOGICCRAFT SIMULATION EVENT QUEUE (FIFO / CO3)    \n";
    std::cout << "======================================================\n";

    if (queue == nullptr || isQueueEmpty(queue)) {
        std::cout << "Queue State: [EMPTY] (0 / " << MAX_QUEUE_CAPACITY << " pending jobs)\n";
        std::cout << "======================================================\n\n";
        return;
    }

    std::cout << "Current Queue Size: " << queue->count << " / " << MAX_QUEUE_CAPACITY << "\n";
    std::cout << "Front Index: [" << queue->front << "] | Rear Index: [" << queue->rear << "]\n";
    std::cout << "------------------------------------------------------\n";

    for (int i = 0; i < queue->count; i++) {
        int idx = (queue->front + i) % MAX_QUEUE_CAPACITY;
        const SimulationEvent& ev = queue->events[idx];
        std::cout << "Job [" << i << "]" << (i == 0 ? " <- [FRONT / FIFO NEXT] : " : "                   : ");
        std::cout << "Event #" << ev.eventId << " -> Target Gate " << ev.targetGateId 
                  << " (Pin " << ev.targetPinIndex << " = " << ev.signalValue << ") | Step " 
                  << ev.simulationStep;
        if (ev.eventDescription[0] != '\0') {
            std::cout << " [" << ev.eventDescription << "]";
        }
        std::cout << "\n";
    }
    std::cout << "======================================================\n\n";
}

int getQueueSize(const SimulationQueue* queue) {
    if (queue == nullptr) return 0;
    return queue->count;
}

void clearSimulationQueue(SimulationQueue* queue) {
    if (queue != nullptr) {
        queue->front = 0;
        queue->rear = -1;
        queue->count = 0;
    }
}

SimulationEvent createSimulationEvent(int id, int gateId, int pinIdx, int signalVal, int step, const char* desc) {
    SimulationEvent ev;
    ev.eventId = id;
    ev.targetGateId = gateId;
    ev.targetPinIndex = pinIdx;
    ev.signalValue = signalVal;
    ev.simulationStep = step;
    if (desc != nullptr) {
        std::strncpy(ev.eventDescription, desc, sizeof(ev.eventDescription) - 1);
        ev.eventDescription[sizeof(ev.eventDescription) - 1] = '\0';
    } else {
        ev.eventDescription[0] = '\0';
    }
    return ev;
}
