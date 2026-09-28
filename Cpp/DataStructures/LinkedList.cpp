#include "LinkedList.h"

CircuitLinkedList::CircuitLinkedList() : head(nullptr), count(0) {}

CircuitLinkedList::~CircuitLinkedList() {
    clear();
}

int CircuitLinkedList::computeOutput(const char* type, int inCount, const int inps[]) {
    if (strcmp(type, "AND") == 0) {
        for (int i = 0; i < inCount; ++i) {
            if (inps[i] == 0) return 0;
        }
        return 1;
    } else if (strcmp(type, "OR") == 0) {
        for (int i = 0; i < inCount; ++i) {
            if (inps[i] == 1) return 1;
        }
        return 0;
    } else if (strcmp(type, "NOT") == 0) {
        return (inps[0] == 0) ? 1 : 0;
    } else if (strcmp(type, "XOR") == 0) {
        int ones = 0;
        for (int i = 0; i < inCount; ++i) {
            if (inps[i] == 1) ones++;
        }
        return (ones % 2 != 0) ? 1 : 0;
    }
    return 0;
}

void CircuitLinkedList::insertNode(int id, const char* type, const char* name, int inputCount, const int initialInputs[]) {
    // Check if ID already exists
    if (searchNode(id) != nullptr) {
        std::cerr << "[Linked List Error] Gate ID " << id << " already exists in the dynamic list!\n";
        return;
    }

    GateNode* newNode = new GateNode();
    newNode->gateId = id;
    strncpy(newNode->gateType, type, sizeof(newNode->gateType) - 1);
    newNode->gateType[sizeof(newNode->gateType) - 1] = '\0';

    strncpy(newNode->gateName, name, sizeof(newNode->gateName) - 1);
    newNode->gateName[sizeof(newNode->gateName) - 1] = '\0';

    newNode->inputCount = (inputCount > 4) ? 4 : inputCount;
    for (int i = 0; i < 4; ++i) {
        if (i < newNode->inputCount && initialInputs != nullptr) {
            newNode->inputs[i] = (initialInputs[i] != 0) ? 1 : 0;
        } else {
            newNode->inputs[i] = 0;
        }
    }
    newNode->outputValue = computeOutput(newNode->gateType, newNode->inputCount, newNode->inputs);
    newNode->next = nullptr;

    // Append to end of linked list
    if (head == nullptr) {
        head = newNode;
    } else {
        GateNode* curr = head;
        while (curr->next != nullptr) {
            curr = curr->next;
        }
        curr->next = newNode;
    }
    count++;

    std::cout << "[Linked List Insert] Added " << type << " Gate '" << name 
              << "' (ID: " << id << ") to dynamic circuit. Total Nodes: " << count << ".\n";
}

bool CircuitLinkedList::deleteNode(int id) {
    if (head == nullptr) {
        std::cerr << "[Linked List Delete] List is empty!\n";
        return false;
    }

    GateNode* curr = head;
    GateNode* prev = nullptr;

    while (curr != nullptr && curr->gateId != id) {
        prev = curr;
        curr = curr->next;
    }

    if (curr == nullptr) {
        std::cerr << "[Linked List Delete] Gate ID " << id << " not found!\n";
        return false;
    }

    if (prev == nullptr) {
        // Deleting head node
        head = curr->next;
    } else {
        prev->next = curr->next;
    }

    std::cout << "[Linked List Delete] Successfully removed Gate ID " << id 
              << " ('" << curr->gateName << "'). Memory safely freed.\n";
    delete curr;
    count--;
    return true;
}

bool CircuitLinkedList::updateNode(int id, const char* newName, const int newInputs[]) {
    GateNode* node = searchNode(id);
    if (node == nullptr) {
        std::cerr << "[Linked List Update] Gate ID " << id << " not found!\n";
        return false;
    }

    if (newName != nullptr && strlen(newName) > 0) {
        strncpy(node->gateName, newName, sizeof(node->gateName) - 1);
        node->gateName[sizeof(node->gateName) - 1] = '\0';
    }

    if (newInputs != nullptr) {
        for (int i = 0; i < node->inputCount; ++i) {
            node->inputs[i] = (newInputs[i] != 0) ? 1 : 0;
        }
        node->outputValue = computeOutput(node->gateType, node->inputCount, node->inputs);
    }

    std::cout << "[Linked List Update] Updated Gate ID " << id 
              << " -> New Name: '" << node->gateName 
              << "', New Output: " << node->outputValue << ".\n";
    return true;
}

GateNode* CircuitLinkedList::searchNode(int id) const {
    GateNode* curr = head;
    while (curr != nullptr) {
        if (curr->gateId == id) {
            return curr;
        }
        curr = curr->next;
    }
    return nullptr;
}

void CircuitLinkedList::traverseAndDisplay() const {
    std::cout << "\n========================================================================\n";
    std::cout << "         DYNAMIC CIRCUIT COMPONENT LINKED LIST (CO2)                     \n";
    std::cout << "========================================================================\n";
    std::cout << "Active Dynamic Nodes: " << count << " | Head Address: " << head << "\n";
    
    if (head == nullptr) {
        std::cout << "  (Circuit linked list is currently empty)\n";
    }

    GateNode* curr = head;
    int index = 0;
    while (curr != nullptr) {
        std::cout << "Node [" << index << "] @ " << curr << " -> "
                  << "[ID: " << curr->gateId << "] " 
                  << curr->gateType << " ('" << curr->gateName << "') | Inputs: [";
        for (int i = 0; i < curr->inputCount; ++i) {
            std::cout << curr->inputs[i] << (i < curr->inputCount - 1 ? ", " : "");
        }
        std::cout << "] -> Output: " << curr->outputValue 
                  << " | Next: " << curr->next << "\n";

        curr = curr->next;
        index++;
    }
    std::cout << "========================================================================\n\n";
}

void CircuitLinkedList::evaluateAll() {
    GateNode* curr = head;
    while (curr != nullptr) {
        curr->outputValue = computeOutput(curr->gateType, curr->inputCount, curr->inputs);
        curr = curr->next;
    }
}

void CircuitLinkedList::clear() {
    GateNode* curr = head;
    while (curr != nullptr) {
        GateNode* nextNode = curr->next;
        delete curr;
        curr = nextNode;
    }
    head = nullptr;
    count = 0;
}

int CircuitLinkedList::getCount() const { return count; }
