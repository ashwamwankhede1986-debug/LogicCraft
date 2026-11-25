#ifndef CIRCUIT_VALIDATOR_H
#define CIRCUIT_VALIDATOR_H

#include "Circuit.h"
#include "../Searching/Searching.h"
#include "../Sorting/Sorting.h"
#include "../Hashing/HashTable.h"

/**
 * @file CircuitValidator.h
 * @brief LogicCraft Smart Circuit Validation & Recommendation Engine (CO4 - Decision-Making)
 *
 * ACADEMIC RESTRICTION COMPLIANCE:
 * Pure procedural C++ implementation using C-style structs, arrays, pointers,
 * and standalone functions. No classes, inheritance, or OOP abstractions.
 *
 * Features:
 * - Checks connection integrity, pin boundaries, and self-loops
 * - Detects floating / missing inputs without signal sources
 * - Graph-based cycle detection preventing infinite evaluation loops
 * - Automated actionable engineering recommendations
 * - Evaluates circuit viability and coordinates Stack/Queue/Hashing integration
 * - 3 mandatory validation scenarios (Valid, Incomplete/Invalid, Complex multi-gate)
 */

enum ValidationStatus {
    CIRCUIT_VALID = 0,
    CIRCUIT_WARNING = 1,
    CIRCUIT_INVALID = 2
};

struct ValidationIssue {
    int code;
    char severity[12]; // "ERROR", "WARNING", "INFO"
    int gateId;
    int pinIndex;
    char message[128];
    char recommendation[128];
};

const int MAX_VALIDATION_ISSUES = 16;

struct ValidationReport {
    ValidationStatus status;
    int errorCount;
    int warningCount;
    int totalGates;
    int totalConnections;
    bool canEvaluate;
    ValidationIssue issues[MAX_VALIDATION_ISSUES];
    int issueCount;
    char summaryRecommendation[256];
};

// Procedural Circuit Validation Engine
ValidationReport validateCircuit(const Circuit* circuit);

// Graph cycle detection using procedural DFS recursion stack
bool hasCyclicDependency(const Circuit* circuit, int* cycleGateAOut, int* cycleGateBOut);

// Formatted display of validation findings
void displayValidationReport(const ValidationReport* report, const char* scenarioTitle);

// 3 Rigorous Verification Scenarios for Task 4 CO4
void runCircuitValidationScenarios();

#endif // CIRCUIT_VALIDATOR_H
