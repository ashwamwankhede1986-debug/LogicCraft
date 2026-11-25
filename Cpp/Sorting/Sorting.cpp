#include "Sorting.h"
#include <iostream>
#include <iomanip>
#include <cstring>

static void swapComponents(ComponentRecord* a, ComponentRecord* b) {
    ComponentRecord temp = *a;
    *a = *b;
    *b = temp;
}

// Compare two ComponentRecords based on sort key: returns > 0 if a > b, < 0 if a < b, 0 if equal
static int compareComponents(const ComponentRecord* a, const ComponentRecord* b, SortKey key) {
    if (key == SORT_BY_ID) {
        if (a->id > b->id) return 1;
        if (a->id < b->id) return -1;
        return 0;
    } else if (key == SORT_BY_NAME) {
        return std::strcmp(a->name, b->name);
    } else if (key == SORT_BY_TYPE) {
        int cmpType = std::strcmp(a->type, b->type);
        if (cmpType != 0) return cmpType;
        return (a->id - b->id);
    }
    return 0;
}

void bubbleSort(ComponentRecord items[], int size, SortKey key, SortMetrics* metricsOut) {
    int comps = 0;
    int swaps = 0;

    if (items != nullptr && size > 1) {
        for (int i = 0; i < size - 1; ++i) {
            bool swappedInPass = false;
            for (int j = 0; j < size - i - 1; ++j) {
                comps++;
                if (compareComponents(&items[j], &items[j + 1], key) > 0) {
                    swapComponents(&items[j], &items[j + 1]);
                    swaps++;
                    swappedInPass = true;
                }
            }
            // Adaptive early termination if array is already sorted
            if (!swappedInPass) {
                break;
            }
        }
    }

    if (metricsOut) {
        metricsOut->comparisons = comps;
        metricsOut->swaps = swaps;
    }
}

void selectionSort(ComponentRecord items[], int size, SortKey key, SortMetrics* metricsOut) {
    int comps = 0;
    int swaps = 0;

    if (items != nullptr && size > 1) {
        for (int i = 0; i < size - 1; ++i) {
            int minIdx = i;
            for (int j = i + 1; j < size; ++j) {
                comps++;
                if (compareComponents(&items[j], &items[minIdx], key) < 0) {
                    minIdx = j;
                }
            }
            if (minIdx != i) {
                swapComponents(&items[i], &items[minIdx]);
                swaps++;
            }
        }
    }

    if (metricsOut) {
        metricsOut->comparisons = comps;
        metricsOut->swaps = swaps;
    }
}

void insertionSort(ComponentRecord items[], int size, SortKey key, SortMetrics* metricsOut) {
    int comps = 0;
    int swaps = 0;

    if (items != nullptr && size > 1) {
        for (int i = 1; i < size; ++i) {
            ComponentRecord keyItem = items[i];
            int j = i - 1;
            while (j >= 0) {
                comps++;
                if (compareComponents(&items[j], &keyItem, key) > 0) {
                    items[j + 1] = items[j];
                    swaps++;
                    j--;
                } else {
                    break;
                }
            }
            items[j + 1] = keyItem;
        }
    }

    if (metricsOut) {
        metricsOut->comparisons = comps;
        metricsOut->swaps = swaps;
    }
}

static void quickSortInternal(ComponentRecord items[], int low, int high, SortKey key, int* comps, int* swaps) {
    if (low < high) {
        ComponentRecord pivot = items[high];
        int i = low - 1;
        for (int j = low; j < high; ++j) {
            (*comps)++;
            if (compareComponents(&items[j], &pivot, key) <= 0) {
                i++;
                swapComponents(&items[i], &items[j]);
                (*swaps)++;
            }
        }
        swapComponents(&items[i + 1], &items[high]);
        (*swaps)++;
        int pi = i + 1;
        quickSortInternal(items, low, pi - 1, key, comps, swaps);
        quickSortInternal(items, pi + 1, high, key, comps, swaps);
    }
}

void quickSort(ComponentRecord items[], int size, SortKey key, SortMetrics* metricsOut) {
    int comps = 0;
    int swaps = 0;
    if (items != nullptr && size > 1) {
        quickSortInternal(items, 0, size - 1, key, &comps, &swaps);
    }
    if (metricsOut) {
        metricsOut->comparisons = comps;
        metricsOut->swaps = swaps;
    }
}

static void mergeInternal(ComponentRecord items[], int l, int m, int r, SortKey key, int* comps, int* swaps) {
    int n1 = m - l + 1;
    int n2 = r - m;
    ComponentRecord* L = new ComponentRecord[n1];
    ComponentRecord* R = new ComponentRecord[n2];

    for (int i = 0; i < n1; i++) L[i] = items[l + i];
    for (int j = 0; j < n2; j++) R[j] = items[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        (*comps)++;
        if (compareComponents(&L[i], &R[j], key) <= 0) {
            items[k++] = L[i++];
        } else {
            items[k++] = R[j++];
            (*swaps)++;
        }
    }
    while (i < n1) items[k++] = L[i++];
    while (j < n2) items[k++] = R[j++];

    delete[] L;
    delete[] R;
}

static void mergeSortInternal(ComponentRecord items[], int l, int r, SortKey key, int* comps, int* swaps) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSortInternal(items, l, m, key, comps, swaps);
        mergeSortInternal(items, m + 1, r, key, comps, swaps);
        mergeInternal(items, l, m, r, key, comps, swaps);
    }
}

void mergeSort(ComponentRecord items[], int size, SortKey key, SortMetrics* metricsOut) {
    int comps = 0;
    int swaps = 0;
    if (items != nullptr && size > 1) {
        mergeSortInternal(items, 0, size - 1, key, &comps, &swaps);
    }
    if (metricsOut) {
        metricsOut->comparisons = comps;
        metricsOut->swaps = swaps;
    }
}

void copyComponentArray(const ComponentRecord source[], ComponentRecord destination[], int size) {
    if (source && destination && size > 0) {
        for (int i = 0; i < size; ++i) {
            destination[i] = source[i];
        }
    }
}

void displayComponentArray(const ComponentRecord items[], int size, const char* title) {
    if (title) {
        std::cout << "\n--- [" << title << "] (Total Components: " << size << ") ---\n";
    }
    std::cout << "+-------+--------+----------------------+--------+--------+\n";
    std::cout << "| Index | ID     | Name                 | Type   | Output |\n";
    std::cout << "+-------+--------+----------------------+--------+--------+\n";

    if (items == nullptr || size == 0) {
        std::cout << "| [EMPTY COMPONENT ARRAY]                                         |\n";
    } else {
        for (int i = 0; i < size; ++i) {
            std::cout << "| " << std::setw(5) << i 
                      << " | " << std::setw(6) << items[i].id
                      << " | " << std::setw(20) << std::left << items[i].name << std::right
                      << " | " << std::setw(6) << items[i].type
                      << " | " << std::setw(6) << items[i].output
                      << " |\n";
        }
    }
    std::cout << "+-------+--------+----------------------+--------+--------+\n";
}
