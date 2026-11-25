#include "Searching.h"
#include <iomanip>

bool isValidComponentId(int id) {
    return id > 0;
}

bool isValidComponentName(const char* name) {
    return (name != nullptr && std::strlen(name) > 0);
}

int linearSearchById(const ComponentRecord items[], int size, int targetId, int* comparisonsOut) {
    int comps = 0;
    if (comparisonsOut) *comparisonsOut = 0;

    if (!isValidComponentId(targetId) || items == nullptr || size <= 0) {
        return -1;
    }

    for (int i = 0; i < size; ++i) {
        comps++;
        if (items[i].id == targetId) {
            if (comparisonsOut) *comparisonsOut = comps;
            return i;
        }
    }

    if (comparisonsOut) *comparisonsOut = comps;
    return -1; // Not found
}

int linearSearchByName(const ComponentRecord items[], int size, const char* targetName, int* comparisonsOut) {
    int comps = 0;
    if (comparisonsOut) *comparisonsOut = 0;

    if (!isValidComponentName(targetName) || items == nullptr || size <= 0) {
        return -1;
    }

    for (int i = 0; i < size; ++i) {
        comps++;
        if (std::strcmp(items[i].name, targetName) == 0) {
            if (comparisonsOut) *comparisonsOut = comps;
            return i;
        }
    }

    if (comparisonsOut) *comparisonsOut = comps;
    return -1;
}

int binarySearchById(const ComponentRecord items[], int size, int targetId, int* comparisonsOut) {
    int comps = 0;
    if (comparisonsOut) *comparisonsOut = 0;

    if (!isValidComponentId(targetId) || items == nullptr || size <= 0) {
        return -1;
    }

    int low = 0;
    int high = size - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        comps++;

        if (items[mid].id == targetId) {
            if (comparisonsOut) *comparisonsOut = comps;
            return mid;
        } else if (items[mid].id < targetId) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    if (comparisonsOut) *comparisonsOut = comps;
    return -1; // Not found
}

int binarySearchByName(const ComponentRecord items[], int size, const char* targetName, int* comparisonsOut) {
    int comps = 0;
    if (comparisonsOut) *comparisonsOut = 0;

    if (!isValidComponentName(targetName) || items == nullptr || size <= 0) {
        return -1;
    }

    int low = 0;
    int high = size - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        comps++;

        int cmp = std::strcmp(items[mid].name, targetName);
        if (cmp == 0) {
            if (comparisonsOut) *comparisonsOut = comps;
            return mid;
        } else if (cmp < 0) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    if (comparisonsOut) *comparisonsOut = comps;
    return -1;
}

void displaySearchResult(int foundIndex, const ComponentRecord items[], int comparisons, const char* searchMethod) {
    std::cout << "  [" << searchMethod << "] Comparisons: " << comparisons;
    if (foundIndex >= 0 && items != nullptr) {
        std::cout << " -> FOUND at index [" << foundIndex << "]: "
                  << items[foundIndex].name << " (ID: " << items[foundIndex].id 
                  << ", Type: " << items[foundIndex].type << ", Out: " << items[foundIndex].output << ")\n";
    } else {
        std::cout << " -> NOT FOUND in array.\n";
    }
}
