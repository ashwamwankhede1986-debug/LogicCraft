#include <iostream>
#include <string>
using namespace std;

// Node represents one gate in the circuit
struct Node
{
    int gateId;
    string gateType;

    Node* prev;
    Node* next;
};

class GateList
{
private:
    Node* head;
    Node* tail;

public:

    GateList()
    {
        head = nullptr;
        tail = nullptr;
    }

    // Insert a gate at the end
    void insertGate(int id, string type)
    {
        Node* newNode = new Node;

        newNode->gateId = id;
        newNode->gateType = type;
        newNode->prev = nullptr;
        newNode->next = nullptr;

        if (head == nullptr)
        {
            head = tail = newNode;
        }
        else
        {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }

        cout << "Gate inserted successfully.\n";
    }

    // Delete a gate
    void deleteGate(int id)
    {
        if (head == nullptr)
        {
            cout << "List is empty.\n";
            return;
        }

        Node* current = head;

        while (current != nullptr && current->gateId != id)
        {
            current = current->next;
        }

        if (current == nullptr)
        {
            cout << "Gate not found.\n";
            return;
        }

        if (current == head)
            head = current->next;

        if (current == tail)
            tail = current->prev;

        if (current->prev != nullptr)
            current->prev->next = current->next;

        if (current->next != nullptr)
            current->next->prev = current->prev;

        delete current;

        cout << "Gate deleted successfully.\n";
    }

    // Update gate type
    void updateGate(int id, string newType)
    {
        Node* current = head;

        while (current != nullptr)
        {
            if (current->gateId == id)
            {
                current->gateType = newType;

                cout << "Gate updated successfully.\n";
                return;
            }

            current = current->next;
        }

        cout << "Gate not found.\n";
    }

    // Search gate
    void searchGate(int id)
    {
        Node* current = head;

        while (current != nullptr)
        {
            if (current->gateId == id)
            {
                cout << "\nGate found!\n";
                cout << "Gate ID   : " << current->gateId << endl;
                cout << "Gate Type : " << current->gateType << endl;
                return;
            }

            current = current->next;
        }

        cout << "Gate not found.\n";
    }

    // Display from first to last
    void displayForward()
    {
        if (head == nullptr)
        {
            cout << "List is empty.\n";
            return;
        }

        Node* current = head;

        cout << "\nCircuit Gates (Forward):\n";

        while (current != nullptr)
        {
            cout << "[" << current->gateId
                 << ": " << current->gateType << "]";

            if (current->next != nullptr)
                cout << " <-> ";

            current = current->next;
        }

        cout << endl;
    }

    // Display from last to first
    void displayBackward()
    {
        if (tail == nullptr)
        {
            cout << "List is empty.\n";
            return;
        }

        Node* current = tail;

        cout << "\nCircuit Gates (Backward):\n";

        while (current != nullptr)
        {
            cout << "[" << current->gateId
                 << ": " << current->gateType << "]";

            if (current->prev != nullptr)
                cout << " <-> ";

            current = current->prev;
        }

        cout << endl;
    }
};


int main()
{
    GateList circuit;

    int choice;

    cout << "=========================================\n";
    cout << " DIGITAL LOGIC CIRCUIT VISUAL WORKBENCH\n";
    cout << "       CO2 - DOUBLY LINKED LIST\n";
    cout << "=========================================\n";

    do
    {
        cout << "\n========== MENU ==========\n";
        cout << "1. Insert Gate\n";
        cout << "2. Delete Gate\n";
        cout << "3. Update Gate\n";
        cout << "4. Search Gate\n";
        cout << "5. Display Forward\n";
        cout << "6. Display Backward\n";
        cout << "7. Exit\n";
        cout << "==========================\n";

        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            int id;
            string type;

            cout << "\nEnter Gate ID: ";
            cin >> id;

            cout << "Enter Gate Type (AND/OR/NOT/NAND/NOR/XOR): ";
            cin >> type;

            circuit.insertGate(id, type);

            break;
        }

        case 2:
        {
            int id;

            cout << "\nEnter Gate ID to delete: ";
            cin >> id;

            circuit.deleteGate(id);

            break;
        }

        case 3:
        {
            int id;
            string newType;

            cout << "\nEnter Gate ID to update: ";
            cin >> id;

            cout << "Enter new Gate Type: ";
            cin >> newType;

            circuit.updateGate(id, newType);

            break;
        }

        case 4:
        {
            int id;

            cout << "\nEnter Gate ID to search: ";
            cin >> id;

            circuit.searchGate(id);

            break;
        }

        case 5:
            circuit.displayForward();
            break;

        case 6:
            circuit.displayBackward();
            break;

        case 7:
            cout << "\nExiting program...\n";
            break;

        default:
            cout << "\nInvalid choice. Please try again.\n";
        }

    } while (choice != 7);

    return 0;
}