#include "HashTable.h"
#include <iostream>
#include <iomanip>
#include <cstring>

void initHashTable(ComponentHashTable* ht) {
    if (!ht) return;
    ht->count = 0;
    ht->totalCollisions = 0;
    for (int i = 0; i < HASH_TABLE_CAPACITY; ++i) {
        ht->table[i].state = SLOT_EMPTY;
        ht->table[i].originalHash = -1;
        ht->table[i].probeSteps = 0;
        ht->table[i].record.id = 0;
        ht->table[i].record.name[0] = '\0';
        ht->table[i].record.type[0] = '\0';
        ht->table[i].record.inputCount = 0;
        ht->table[i].record.output = 0;
    }
}

// Modular Hash Function for Component ID
int hashFunctionId(int id) {
    if (id < 0) id = -id;
    return id % HASH_TABLE_CAPACITY;
}

// Polynomial Rolling Hash for Component Name: sum(c * 31^k) % Capacity
int hashFunctionName(const char* name) {
    if (!name) return 0;
    unsigned long hash = 0;
    int len = (int)std::strlen(name);
    for (int i = 0; i < len; ++i) {
        hash = (hash * 31 + (unsigned char)name[i]) % HASH_TABLE_CAPACITY;
    }
    return (int)hash;
}

bool isHashTableFull(const ComponentHashTable* ht) {
    return (ht != nullptr && ht->count >= HASH_TABLE_CAPACITY);
}

bool isHashTableEmpty(const ComponentHashTable* ht) {
    return (ht == nullptr || ht->count == 0);
}

int getHashTableCount(const ComponentHashTable* ht) {
    return ht ? ht->count : 0;
}

int getHashTableCollisions(const ComponentHashTable* ht) {
    return ht ? ht->totalCollisions : 0;
}

bool insertHashTable(ComponentHashTable* ht, const ComponentRecord* record, int* collisionStepsOut) {
    if (collisionStepsOut) *collisionStepsOut = 0;

    if (!ht || !record || isHashTableFull(ht)) {
        if (ht && ht->count >= HASH_TABLE_CAPACITY) {
            std::cout << " [HASH TABLE ERROR] Table is FULL (" << HASH_TABLE_CAPACITY 
                      << "/" << HASH_TABLE_CAPACITY << "). Insertion rejected for ID " 
                      << (record ? record->id : -1) << ".\n";
        }
        return false;
    }

    int baseIndex = hashFunctionId(record->id);
    int probes = 0;
    int firstDeletedSlot = -1;

    for (int step = 0; step < HASH_TABLE_CAPACITY; ++step) {
        int index = (baseIndex + step) % HASH_TABLE_CAPACITY;

        // Check for existing identical key (update duplicate)
        if (ht->table[index].state == SLOT_OCCUPIED && ht->table[index].record.id == record->id) {
            ht->table[index].record = *record;
            if (collisionStepsOut) *collisionStepsOut = step;
            return true;
        }

        // Keep track of first recyclable deleted tombstone
        if (ht->table[index].state == SLOT_DELETED && firstDeletedSlot == -1) {
            firstDeletedSlot = index;
        }

        if (ht->table[index].state == SLOT_EMPTY) {
            int insertSlot = (firstDeletedSlot != -1) ? firstDeletedSlot : index;
            ht->table[insertSlot].record = *record;
            ht->table[insertSlot].state = SLOT_OCCUPIED;
            ht->table[insertSlot].originalHash = baseIndex;
            ht->table[insertSlot].probeSteps = (insertSlot - baseIndex + HASH_TABLE_CAPACITY) % HASH_TABLE_CAPACITY;

            ht->count++;
            if (step > 0) {
                ht->totalCollisions++;
            }
            if (collisionStepsOut) *collisionStepsOut = ht->table[insertSlot].probeSteps;
            return true;
        }
        probes++;
    }

    // If we only found a deleted slot after checking all
    if (firstDeletedSlot != -1) {
        ht->table[firstDeletedSlot].record = *record;
        ht->table[firstDeletedSlot].state = SLOT_OCCUPIED;
        ht->table[firstDeletedSlot].originalHash = baseIndex;
        ht->table[firstDeletedSlot].probeSteps = (firstDeletedSlot - baseIndex + HASH_TABLE_CAPACITY) % HASH_TABLE_CAPACITY;
        ht->count++;
        ht->totalCollisions++;
        if (collisionStepsOut) *collisionStepsOut = ht->table[firstDeletedSlot].probeSteps;
        return true;
    }

    return false;
}

int searchHashTableById(const ComponentHashTable* ht, int id, ComponentRecord* foundRecordOut, int* probesOut) {
    if (probesOut) *probesOut = 0;
    if (!ht || isHashTableEmpty(ht) || !isValidComponentId(id)) {
        return -1;
    }

    int baseIndex = hashFunctionId(id);
    int probes = 0;

    for (int step = 0; step < HASH_TABLE_CAPACITY; ++step) {
        probes++;
        int index = (baseIndex + step) % HASH_TABLE_CAPACITY;

        if (ht->table[index].state == SLOT_EMPTY) {
            if (probesOut) *probesOut = probes;
            return -1; // End of probe chain
        }

        if (ht->table[index].state == SLOT_OCCUPIED && ht->table[index].record.id == id) {
            if (foundRecordOut) *foundRecordOut = ht->table[index].record;
            if (probesOut) *probesOut = probes;
            return index;
        }
    }

    if (probesOut) *probesOut = probes;
    return -1;
}

int searchHashTableByName(const ComponentHashTable* ht, const char* name, ComponentRecord* foundRecordOut, int* probesOut) {
    if (probesOut) *probesOut = 0;
    if (!ht || isHashTableEmpty(ht) || !isValidComponentName(name)) {
        return -1;
    }

    int baseIndex = hashFunctionName(name);
    int probes = 0;

    // Linear probe starting from baseIndex
    for (int step = 0; step < HASH_TABLE_CAPACITY; ++step) {
        probes++;
        int index = (baseIndex + step) % HASH_TABLE_CAPACITY;

        if (ht->table[index].state == SLOT_EMPTY) {
            // Because names can hash differently from IDs, also check remaining slots if not found
            break;
        }

        if (ht->table[index].state == SLOT_OCCUPIED && std::strcmp(ht->table[index].record.name, name) == 0) {
            if (foundRecordOut) *foundRecordOut = ht->table[index].record;
            if (probesOut) *probesOut = probes;
            return index;
        }
    }

    // Secondary scan across occupied slots
    for (int i = 0; i < HASH_TABLE_CAPACITY; ++i) {
        if (ht->table[i].state == SLOT_OCCUPIED && std::strcmp(ht->table[i].record.name, name) == 0) {
            if (foundRecordOut) *foundRecordOut = ht->table[i].record;
            if (probesOut) *probesOut = probes + i;
            return i;
        }
    }

    if (probesOut) *probesOut = probes;
    return -1;
}

bool deleteHashTableById(ComponentHashTable* ht, int id) {
    if (!ht || isHashTableEmpty(ht) || !isValidComponentId(id)) {
        return false;
    }

    int probes = 0;
    int index = searchHashTableById(ht, id, nullptr, &probes);
    if (index >= 0) {
        ht->table[index].state = SLOT_DELETED; // Tombstone to preserve probing chain
        ht->count--;
        return true;
    }
    return false;
}

void clearHashTable(ComponentHashTable* ht) {
    initHashTable(ht);
}

void displayHashTable(const ComponentHashTable* ht) {
    if (!ht) return;

    std::cout << "\n=================================================================\n";
    std::cout << " [HASH TABLE INSPECTION (CAPACITY: " << HASH_TABLE_CAPACITY 
              << " | OCCUPIED: " << ht->count << " | TOTAL COLLISIONS: " << ht->totalCollisions << ")]\n";
    std::cout << "=================================================================\n";
    std::cout << "+--------+-----------+--------+----------------------+--------+--------+--------+\n";
    std::cout << "| Bucket | Status    | ID     | Name                 | Type   | Hash   | Probes |\n";
    std::cout << "+--------+-----------+--------+----------------------+--------+--------+--------+\n";

    for (int i = 0; i < HASH_TABLE_CAPACITY; ++i) {
        const HashSlot& slot = ht->table[i];
        std::cout << "| " << std::setw(6) << i << " | ";
        if (slot.state == SLOT_OCCUPIED) {
            std::cout << std::setw(9) << "OCCUPIED" << " | "
                      << std::setw(6) << slot.record.id << " | "
                      << std::setw(20) << std::left << slot.record.name << std::right << " | "
                      << std::setw(6) << slot.record.type << " | "
                      << std::setw(6) << slot.originalHash << " | "
                      << std::setw(6) << slot.probeSteps << " |\n";
        } else if (slot.state == SLOT_DELETED) {
            std::cout << std::setw(9) << "[TOMBST]" << " | "
                      << std::setw(6) << "-" << " | "
                      << std::setw(20) << std::left << "<DELETED RECORD>" << std::right << " | "
                      << std::setw(6) << "-" << " | "
                      << std::setw(6) << slot.originalHash << " | "
                      << std::setw(6) << "-" << " |\n";
        } else {
            std::cout << std::setw(9) << "EMPTY" << " | "
                      << std::setw(6) << "-" << " | "
                      << std::setw(20) << std::left << "<FREE BUCKET>" << std::right << " | "
                      << std::setw(6) << "-" << " | "
                      << std::setw(6) << "-" << " | "
                      << std::setw(6) << "-" << " |\n";
        }
    }
    std::cout << "+--------+-----------+--------+----------------------+--------+--------+--------+\n";
}
