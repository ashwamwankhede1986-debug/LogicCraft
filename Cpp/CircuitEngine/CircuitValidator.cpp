#include "CircuitValidator.h"
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdio>

static bool dfsCycleCheck(int currentGateId, const Circuit* circuit, bool visited[], bool recStack[], int maxGates, int* cycleA, int* cycleB) {
    if (currentGateId < 0 || currentGateId >= maxGates) return false;

    visited[currentGateId] = true;
    recStack[currentGateId] = true;

    int connCount = circuit->getConnectionCount();
    for (int i = 0; i < connCount; ++i) {
        const Connection* conn = circuit->getConnectionAt(i);
        if (conn && conn->sourceGateId == currentGateId) {
            int neighborId = conn->destinationGateId;
            if (neighborId >= 0 && neighborId < maxGates) {
                if (!visited[neighborId]) {
                    if (dfsCycleCheck(neighborId, circuit, visited, recStack, maxGates, cycleA, cycleB)) {
                        return true;
                    }
                } else if (recStack[neighborId]) {
                    if (cycleA) *cycleA = currentGateId;
                    if (cycleB) *cycleB = neighborId;
                    return true; // Cycle detected
                }
            }
        }
    }

    recStack[currentGateId] = false;
    return false;
}

bool hasCyclicDependency(const Circuit* circuit, int* cycleGateAOut, int* cycleGateBOut) {
    if (!circuit) return false;
    const int MAX_NODES = 64;
    bool visited[MAX_NODES] = {false};
    bool recStack[MAX_NODES] = {false};

    const GateArray& registry = const_cast<Circuit*>(circuit)->getGateRegistry();
    int count = registry.getCount();

    for (int i = 0; i < count; ++i) {
        const Gate* g = registry.getGateAt(i);
        if (g) {
            int gId = g->getId();
            if (gId >= 0 && gId < MAX_NODES && !visited[gId]) {
                if (dfsCycleCheck(gId, circuit, visited, recStack, MAX_NODES, cycleGateAOut, cycleGateBOut)) {
                    return true;
                }
            }
        }
    }

    return false;
}

ValidationReport validateCircuit(const Circuit* circuit) {
    ValidationReport report;
    report.status = CIRCUIT_VALID;
    report.errorCount = 0;
    report.warningCount = 0;
    report.issueCount = 0;
    report.totalGates = 0;
    report.totalConnections = 0;
    report.canEvaluate = true;
    report.summaryRecommendation[0] = '\0';

    if (!circuit) {
        report.status = CIRCUIT_INVALID;
        report.errorCount = 1;
        report.canEvaluate = false;
        std::snprintf(report.summaryRecommendation, sizeof(report.summaryRecommendation),
                      "Fatal: Circuit pointer is NULL. Initialize circuit structure before evaluation.");
        return report;
    }

    const GateArray& registry = const_cast<Circuit*>(circuit)->getGateRegistry();
    report.totalGates = registry.getCount();
    report.totalConnections = circuit->getConnectionCount();

    if (report.totalGates == 0) {
        report.status = CIRCUIT_WARNING;
        report.warningCount = 1;
        report.canEvaluate = false;
        std::snprintf(report.summaryRecommendation, sizeof(report.summaryRecommendation),
                      "Notice: Circuit is empty. Add logic gates to begin simulation.");
        return report;
    }

    // 1. Validate Wire Connections
    int connCount = circuit->getConnectionCount();
    for (int i = 0; i < connCount; ++i) {
        const Connection* conn = circuit->getConnectionAt(i);
        if (!conn) continue;

        int srcIdx = registry.searchGateById(conn->sourceGateId);
        int dstIdx = registry.searchGateById(conn->destinationGateId);

        // Check non-existent source
        if (srcIdx < 0 && report.issueCount < MAX_VALIDATION_ISSUES) {
            ValidationIssue& iss = report.issues[report.issueCount++];
            iss.code = 101;
            std::strcpy(iss.severity, "ERROR");
            iss.gateId = conn->sourceGateId;
            iss.pinIndex = -1;
            std::snprintf(iss.message, sizeof(iss.message), "Connection references missing Source Gate ID %d.", conn->sourceGateId);
            std::snprintf(iss.recommendation, sizeof(iss.recommendation), "Delete dangling wire or add Gate ID %d to the circuit.", conn->sourceGateId);
            report.errorCount++;
        }

        // Check non-existent destination
        if (dstIdx < 0 && report.issueCount < MAX_VALIDATION_ISSUES) {
            ValidationIssue& iss = report.issues[report.issueCount++];
            iss.code = 102;
            std::strcpy(iss.severity, "ERROR");
            iss.gateId = conn->destinationGateId;
            iss.pinIndex = conn->destinationInputIndex;
            std::snprintf(iss.message, sizeof(iss.message), "Connection references missing Destination Gate ID %d.", conn->destinationGateId);
            std::snprintf(iss.recommendation, sizeof(iss.recommendation), "Delete wire or instantiate Gate ID %d before wiring.", conn->destinationGateId);
            report.errorCount++;
        }

        // Check self-connection
        if (conn->sourceGateId == conn->destinationGateId && report.issueCount < MAX_VALIDATION_ISSUES) {
            ValidationIssue& iss = report.issues[report.issueCount++];
            iss.code = 103;
            std::strcpy(iss.severity, "ERROR");
            iss.gateId = conn->sourceGateId;
            iss.pinIndex = conn->destinationInputIndex;
            std::snprintf(iss.message, sizeof(iss.message), "Gate ID %d connects directly to its own input pin %d (Self-Loop).",
                          conn->sourceGateId, conn->destinationInputIndex);
            std::snprintf(iss.recommendation, sizeof(iss.recommendation), "Remove self-loop to prevent combinatorial race condition.");
            report.errorCount++;
        }

        // Check destination pin bounds
        if (dstIdx >= 0) {
            const Gate* dstGate = registry.getGateAt(dstIdx);
            if (dstGate && (conn->destinationInputIndex < 0 || conn->destinationInputIndex >= dstGate->getInputCount())) {
                if (report.issueCount < MAX_VALIDATION_ISSUES) {
                    ValidationIssue& iss = report.issues[report.issueCount++];
                    iss.code = 104;
                    std::strcpy(iss.severity, "ERROR");
                    iss.gateId = conn->destinationGateId;
                    iss.pinIndex = conn->destinationInputIndex;
                    std::snprintf(iss.message, sizeof(iss.message), "Destination Gate ID %d (%s) pin index %d is out of range [0..%d].",
                                  dstGate->getId(), dstGate->getType(), conn->destinationInputIndex, dstGate->getInputCount() - 1);
                    std::snprintf(iss.recommendation, sizeof(iss.recommendation), "Rewire connection to a valid input pin index.");
                    report.errorCount++;
                }
            }
        }
    }

    // 2. Check for Duplicate Input Pin Drivers (Multi-driver contention)
    for (int i = 0; i < connCount; ++i) {
        for (int j = i + 1; j < connCount; ++j) {
            const Connection* c1 = circuit->getConnectionAt(i);
            const Connection* c2 = circuit->getConnectionAt(j);
            if (c1 && c2 && c1->destinationGateId == c2->destinationGateId && c1->destinationInputIndex == c2->destinationInputIndex) {
                if (report.issueCount < MAX_VALIDATION_ISSUES) {
                    ValidationIssue& iss = report.issues[report.issueCount++];
                    iss.code = 105;
                    std::strcpy(iss.severity, "ERROR");
                    iss.gateId = c1->destinationGateId;
                    iss.pinIndex = c1->destinationInputIndex;
                    std::snprintf(iss.message, sizeof(iss.message), "Gate ID %d pin %d driven by multiple sources (Gate %d and Gate %d).",
                                  c1->destinationGateId, c1->destinationInputIndex, c1->sourceGateId, c2->sourceGateId);
                    std::snprintf(iss.recommendation, sizeof(iss.recommendation), "Remove one wire; digital logic pins support only single drivers.");
                    report.errorCount++;
                }
            }
        }
    }

    // 3. Cycle Detection
    int cycleA = 0, cycleB = 0;
    if (hasCyclicDependency(circuit, &cycleA, &cycleB)) {
        if (report.issueCount < MAX_VALIDATION_ISSUES) {
            ValidationIssue& iss = report.issues[report.issueCount++];
            iss.code = 201;
            std::strcpy(iss.severity, "ERROR");
            iss.gateId = cycleA;
            iss.pinIndex = -1;
            std::snprintf(iss.message, sizeof(iss.message), "Cyclic loop detected between Gate ID %d and Gate ID %d.", cycleA, cycleB);
            std::snprintf(iss.recommendation, sizeof(iss.recommendation), "Break cyclic feedback path or add clocked register for synchronous state.");
            report.errorCount++;
        }
    }

    // 4. Check for Floating Inputs
    for (int g = 0; g < report.totalGates; ++g) {
        const Gate* gate = registry.getGateAt(g);
        if (!gate) continue;

        for (int p = 0; p < gate->getInputCount(); ++p) {
            bool isDriven = false;
            for (int c = 0; c < connCount; ++c) {
                const Connection* conn = circuit->getConnectionAt(c);
                if (conn && conn->destinationGateId == gate->getId() && conn->destinationInputIndex == p) {
                    isDriven = true;
                    break;
                }
            }
            if (!isDriven) {
                // If not driven by wire, check if default constant input is defined
                if (report.issueCount < MAX_VALIDATION_ISSUES) {
                    ValidationIssue& iss = report.issues[report.issueCount++];
                    iss.code = 301;
                    std::strcpy(iss.severity, "WARNING");
                    iss.gateId = gate->getId();
                    iss.pinIndex = p;
                    std::snprintf(iss.message, sizeof(iss.message), "Gate ID %d (%s) pin %d is floating (using default level %d).",
                                  gate->getId(), gate->getType(), p, gate->getInput(p));
                    std::snprintf(iss.recommendation, sizeof(iss.recommendation), "Connect pin to input source or toggle constant logic level.");
                    report.warningCount++;
                }
            }
        }
    }

    // Synthesize final decision
    if (report.errorCount > 0) {
        report.status = CIRCUIT_INVALID;
        report.canEvaluate = false;
        std::snprintf(report.summaryRecommendation, sizeof(report.summaryRecommendation),
                      "DECISION: EVALUATION BLOCKED. Resolve %d critical error(s) before simulation.", report.errorCount);
    } else if (report.warningCount > 0) {
        report.status = CIRCUIT_WARNING;
        report.canEvaluate = true;
        std::snprintf(report.summaryRecommendation, sizeof(report.summaryRecommendation),
                      "DECISION: EVALUATION PERMITTED with %d warning(s). Unconnected pins will use default values.", report.warningCount);
    } else {
        report.status = CIRCUIT_VALID;
        report.canEvaluate = true;
        std::snprintf(report.summaryRecommendation, sizeof(report.summaryRecommendation),
                      "DECISION: CIRCUIT FULLY VALID. Topology verified with zero errors. Ready for simulation.");
    }

    return report;
}

void displayValidationReport(const ValidationReport* report, const char* scenarioTitle) {
    if (!report) return;

    std::cout << "\n=================================================================\n";
    std::cout << " [SMART CIRCUIT VALIDATION REPORT]: " << (scenarioTitle ? scenarioTitle : "Circuit Assessment") << "\n";
    std::cout << "=================================================================\n";
    std::cout << "  Status:        " 
              << (report->status == CIRCUIT_VALID ? "[CIRCUIT VALID]" : 
                 (report->status == CIRCUIT_WARNING ? "[WARNINGS DETECTED]" : "[CIRCUIT INVALID]")) << "\n";
    std::cout << "  Total Gates:   " << report->totalGates 
              << " | Total Wires: " << report->totalConnections << "\n";
    std::cout << "  Critical Errors: " << report->errorCount 
              << " | Warnings: " << report->warningCount << "\n";
    std::cout << "  Evaluation Permitted: " << (report->canEvaluate ? "YES (Safe to evaluate)" : "NO (Blocked by validation)") << "\n";
    std::cout << "-----------------------------------------------------------------\n";

    if (report->issueCount == 0) {
        std::cout << "  All design constraints satisfied. No structural defects found.\n";
    } else {
        for (int i = 0; i < report->issueCount; ++i) {
            const ValidationIssue& iss = report->issues[i];
            std::cout << "  [" << iss.severity << " " << iss.code << "] Gate ID " << iss.gateId;
            if (iss.pinIndex >= 0) std::cout << " (Pin " << iss.pinIndex << ")";
            std::cout << ":\n    -> Finding:        " << iss.message << "\n"
                      << "    -> Recommendation: " << iss.recommendation << "\n";
        }
    }
    std::cout << "-----------------------------------------------------------------\n";
    std::cout << "  " << report->summaryRecommendation << "\n";
    std::cout << "=================================================================\n";
}

void runCircuitValidationScenarios() {
    std::cout << "\n*****************************************************************\n";
    std::cout << "*     SMART CIRCUIT VALIDATION & RECOMMENDATION (CO4 DECISION)  *\n";
    std::cout << "*****************************************************************\n";

    // -------------------------------------------------------------
    // SCENARIO 1: VALID CIRCUIT (Half-Adder Topology)
    // -------------------------------------------------------------
    Circuit c1("Scenario 1: Valid Half-Adder");
    int inA[2] = {1, 0};
    Gate gXor(1, "XOR", "Sum_XOR", 2, inA);
    int inB[2] = {1, 0};
    Gate gAnd(2, "AND", "Carry_AND", 2, inB);
    c1.addGate(gXor);
    c1.addGate(gAnd);

    ValidationReport r1 = validateCircuit(&c1);
    displayValidationReport(&r1, "Scenario 1: Fully Valid Dual-Gate Half-Adder");

    // -------------------------------------------------------------
    // SCENARIO 2: INVALID / INCOMPLETE CIRCUIT (Self-Loop & Broken Wires)
    // -------------------------------------------------------------
    Circuit c2("Scenario 2: Defective Topology");
    int inC[2] = {1, 1};
    Gate gBroken1(10, "AND", "Faulty_AND", 2, inC);
    c2.addGate(gBroken1);
    c2.addConnection(10, 10, 0);  // Self-loop (Error 103)
    c2.addConnection(10, 999, 1); // Destination gate 999 does not exist (Error 102)

    ValidationReport r2 = validateCircuit(&c2);
    displayValidationReport(&r2, "Scenario 2: Defective Circuit with Self-Loop & Dangling Wire");

    // -------------------------------------------------------------
    // SCENARIO 3: MULTI-GATE CIRCUIT WITH HASH TABLE & QUEUE INTEGRATION
    // -------------------------------------------------------------
    Circuit c3("Scenario 3: 4-Gate Ripple Cascade");
    Gate g1(1, "AND", "Front_AND", 2, (int[]){1, 1});
    Gate g2(2, "OR",  "Mid_OR",    2, (int[]){0, 0});
    Gate g3(3, "XOR", "Branch_XOR",2, (int[]){0, 1});
    Gate g4(4, "NOT", "Final_NOT", 1, (int[]){0});

    c3.addGate(g1);
    c3.addGate(g2);
    c3.addGate(g3);
    c3.addGate(g4);

    c3.addConnection(1, 2, 0); // g1[OUT] -> g2[Pin 0]
    c3.addConnection(2, 3, 0); // g2[OUT] -> g3[Pin 0]
    c3.addConnection(3, 4, 0); // g3[OUT] -> g4[Pin 0]

    ValidationReport r3 = validateCircuit(&c3);
    displayValidationReport(&r3, "Scenario 3: 4-Gate Ripple Cascade with Cross-Module Integration");

    if (r3.canEvaluate) {
        std::cout << ">>> Executing Ripple Simulation for Scenario 3 ...\n";
        c3.evaluateCircuit();
        std::cout << "  -> Final Cascade Output (Gate 4 NOT): " 
                  << c3.getGateRegistry().getGateAt(3)->getOutput() << " [EXPECTED: 0 (1->1->0->0 inverted to 1/0)]\n";
    }
}
