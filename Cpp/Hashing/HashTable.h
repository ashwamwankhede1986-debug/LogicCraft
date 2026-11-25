#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "../Searching/Searching.h"

/**
 * @file HashTable.h
 * @brief LogicCraft Procedural Hash Table with Linear Probing (CO5 - Hashing)
 *
 * ACADEMIC RESTRICTION COMPLIANCE:
 * Pure procedural C++ implementation using C-style structs, arrays, pointers,
 * and standalone functions. No classes, inheritance, or OOP abstractions.
 *
 * Features:
 * - Hash table with prime capacity (17) for low collision rate
 * - Suitable modular and polynomial rolling hash functions
 * - Open addressing with Linear Probing collision resolution
 * - Explicit collision detection and probe counting
 * - Full-table detection and boundary safety
 * - Tabular visualization of buckets, slots, hashes, and probe distances
 */

const int HASH_TABLE_CAPACITY = 17; // Prime size ensures uniform distribution

enum HashSlotState {
    SLOT_EMPTY = 0,
    SLOT_OCCUPIED = 1,
    SLOT_DELETED = 2
};

struct HashSlot {
    ComponentRecord record;
    HashSlotState state;
    int originalHash;
    int probeSteps; // Number of probes from original hash to this slot
};

struct ComponentHashTable {
    HashSlot table[HASH_TABLE_CAPACITY];
    int count;
    int totalCollisions;
};

// Initialize hash table
void initHashTable(ComponentHashTable* ht);

// Hash functions
int hashFunctionId(int id);
int hashFunctionName(const char* name);

// Insert record with Linear Probing
bool insertHashTable(ComponentHashTable* ht, const ComponentRecord* record, int* collisionStepsOut);

// Search record by ID
int searchHashTableById(const ComponentHashTable* ht, int id, ComponentRecord* foundRecordOut, int* probesOut);

// Search record by Name
int searchHashTableByName(const ComponentHashTable* ht, const char* name, ComponentRecord* foundRecordOut, int* probesOut);

// Delete record by ID (marks slot as DELETED / tombstone)
bool deleteHashTableById(ComponentHashTable* ht, int id);

// Queries and Display
bool isHashTableFull(const ComponentHashTable* ht);
bool isHashTableEmpty(const ComponentHashTable* ht);
int getHashTableCount(const ComponentHashTable* ht);
int getHashTableCollisions(const ComponentHashTable* ht);
void displayHashTable(const ComponentHashTable* ht);
void clearHashTable(ComponentHashTable* ht);

#endif // HASH_TABLE_H
