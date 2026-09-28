#include "Gate.h"

Gate::Gate() : id(0), inputCount(0), output(0) {
    strcpy(type, "NONE");
    strcpy(name, "Unnamed");
    for (int i = 0; i < MAX_INPUTS; ++i) {
        inputs[i] = 0;
    }
}

Gate::Gate(int gateId, const char* gateType, const char* gateName, int inCount, const int initialInputs[])
    : id(gateId), inputCount(inCount), output(0) {
    strncpy(type, gateType, sizeof(type) - 1);
    type[sizeof(type) - 1] = '\0';

    strncpy(name, gateName, sizeof(name) - 1);
    name[sizeof(name) - 1] = '\0';

    for (int i = 0; i < MAX_INPUTS; ++i) {
        if (i < inCount && initialInputs != nullptr) {
            inputs[i] = (initialInputs[i] != 0) ? 1 : 0;
        } else {
            inputs[i] = 0;
        }
    }
    evaluate();
}

int Gate::evaluate() {
    if (strcmp(type, "AND") == 0) {
        output = 1;
        for (int i = 0; i < inputCount; ++i) {
            if (inputs[i] == 0) {
                output = 0;
                break;
            }
        }
    } else if (strcmp(type, "OR") == 0) {
        output = 0;
        for (int i = 0; i < inputCount; ++i) {
            if (inputs[i] == 1) {
                output = 1;
                break;
            }
        }
    } else if (strcmp(type, "NOT") == 0) {
        output = (inputs[0] == 0) ? 1 : 0;
    } else if (strcmp(type, "XOR") == 0) {
        int ones = 0;
        for (int i = 0; i < inputCount; ++i) {
            if (inputs[i] == 1) ones++;
        }
        output = (ones % 2 != 0) ? 1 : 0;
    } else if (strcmp(type, "NAND") == 0) {
        int andOut = 1;
        for (int i = 0; i < inputCount; ++i) {
            if (inputs[i] == 0) { andOut = 0; break; }
        }
        output = (andOut == 1) ? 0 : 1;
    } else if (strcmp(type, "NOR") == 0) {
        int orOut = 0;
        for (int i = 0; i < inputCount; ++i) {
            if (inputs[i] == 1) { orOut = 1; break; }
        }
        output = (orOut == 1) ? 0 : 1;
    } else {
        output = 0;
    }
    return output;
}

int Gate::getId() const { return id; }
const char* Gate::getType() const { return type; }
const char* Gate::getName() const { return name; }
int Gate::getInputCount() const { return inputCount; }
int Gate::getInput(int index) const {
    if (index >= 0 && index < inputCount) return inputs[index];
    return 0;
}
int Gate::getOutput() const { return output; }

void Gate::setId(int newId) { id = newId; }
void Gate::setType(const char* newType) {
    strncpy(type, newType, sizeof(type) - 1);
    type[sizeof(type) - 1] = '\0';
    evaluate();
}
void Gate::setName(const char* newName) {
    strncpy(name, newName, sizeof(name) - 1);
    name[sizeof(name) - 1] = '\0';
}
void Gate::setInput(int index, int val) {
    if (index >= 0 && index < MAX_INPUTS) {
        inputs[index] = (val != 0) ? 1 : 0;
        evaluate();
    }
}
void Gate::setInputs(const int newInputs[], int count) {
    inputCount = count;
    for (int i = 0; i < count && i < MAX_INPUTS; ++i) {
        inputs[i] = (newInputs[i] != 0) ? 1 : 0;
    }
    evaluate();
}

void Gate::display() const {
    std::cout << "[ID: " << id << "] " << type << " Gate ('" << name << "') | Inputs: [";
    for (int i = 0; i < inputCount; ++i) {
        std::cout << inputs[i] << (i < inputCount - 1 ? ", " : "");
    }
    std::cout << "] -> Output: " << output << "\n";
}
