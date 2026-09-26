#include <iostream>
#include <string>
using namespace std;

// Node represents one gate in the circuit
struct Node
{
    int gateId;
    string gateType;
    Node* next;
};

class GateList
{
private:
    Node* head;

public:

    // Constructor
    GateList()
    {
        head = nullptr;
    }

    // Insert a new gate
    void insertGate(int id, string type)
    {
        Node* newNode = new Node;

        newNode->gateId = id;
        newNode->gateType = type;
        newNode->next = nullptr;

        // If list is empty
        if (head == nullptr)
        {
            head = newNode;
            return;
        }

        // Move to the last node
        Node* current = head;

        while (current->next != nullptr)
        {
            current = current->next;
        }

        // Add new node at the end
        current->next = newNode;
    }

    // Delete a gate using its ID
    void deleteGate(int id)
    {
        if (head == nullptr)
        {
            cout << "List is empty." << endl;
            return;
        }

        // If the first gate needs to be deleted
        if (head->gateId == id)
        {
            Node* temp = head;
            head = head->next;

            delete temp;

            cout << "Gate deleted successfully." << endl;
            return;
        }

        Node* current = head;

        // Find the gate
        while (current->next != nullptr &&
               current->next->gateId != id)
        {
            current = current->next;
        }

        // Gate not found
        if (current->next == nullptr)
        {
            cout << "Gate not found." << endl;
            return;
        }

        // Remove the gate
        Node* temp = current->next;
        current->next = temp->next;

        delete temp;

        cout << "Gate deleted successfully." << endl;
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

                cout << "Gate updated successfully." << endl;
                return;
            }

            current = current->next;
        }

        cout << "Gate not found." << endl;
    }

    // Display all gates
    void display()
    {
        Node* current = head;

        cout << "Circuit Gates: ";

        while (current != nullptr)
        {
            cout << "[" << current->gateId
                 << ": " << current->gateType << "]";

            if (current->next != nullptr)
            {
                cout << " -> ";
            }

            current = current->next;
        }

        cout << " -> NULL" << endl;
    }
};

int main()
{
    GateList circuit;

    cout << "===== LogicCraft CO2 - Linked List =====" << endl;

    // Insert gates
    cout << "\nAdding gates:" << endl;

    circuit.insertGate(1, "AND");
    circuit.insertGate(2, "OR");
    circuit.insertGate(3, "XOR");

    circuit.display();

    // Update gate
    cout << "\nUpdating Gate 2 from OR to NOT:" << endl;

    circuit.updateGate(2, "NOT");

    circuit.display();

    // Delete gate
    cout << "\nDeleting Gate 2:" << endl;

    circuit.deleteGate(2);

    circuit.display();

    return 0;
}