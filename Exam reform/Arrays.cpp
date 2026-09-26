#include <iostream>
using namespace std;

class BooleanArray
{
private:
    bool data[100];
    int size;

public:

    // Constructor
    BooleanArray()
    {
        size = 0;
    }

    // Insert a Boolean value
    void insert(bool value)
    {
        if (size >= 100)
        {
            cout << "Array is full!" << endl;
            return;
        }

        data[size] = value;
        size++;

        cout << "Value inserted successfully." << endl;
    }

    // Delete value at a particular index
    void remove(int index)
    {
        if (index < 0 || index >= size)
        {
            cout << "Invalid index!" << endl;
            return;
        }

        // Shift elements to the left
        for (int i = index; i < size - 1; i++)
        {
            data[i] = data[i + 1];
        }

        size--;

        cout << "Value deleted successfully." << endl;
    }

    // Update a Boolean value
    void update(int index, bool value)
    {
        if (index < 0 || index >= size)
        {
            cout << "Invalid index!" << endl;
            return;
        }

        data[index] = value;

        cout << "Value updated successfully." << endl;
    }

    // Search for a Boolean value
    void search(bool value)
    {
        bool found = false;

        for (int i = 0; i < size; i++)
        {
            if (data[i] == value)
            {
                cout << "Value found at index: " << i << endl;
                found = true;
            }
        }

        if (!found)
        {
            cout << "Value not found." << endl;
        }
    }

    // Display the array
    void display()
    {
        cout << "Boolean Array: ";

        for (int i = 0; i < size; i++)
        {
            cout << data[i] << " ";
        }

        cout << endl;
    }

    // Perform AND operation on all values
    bool ANDOperation()
    {
        bool result = true;

        for (int i = 0; i < size; i++)
        {
            result = result && data[i];
        }

        return result;
    }
};

int main()
{
    BooleanArray inputs;

    cout << "===== LogicCraft CO1 - Arrays =====" << endl;

    // Insert Boolean inputs
    inputs.insert(true);
    inputs.insert(false);
    inputs.insert(true);

    // Display array
    cout << "\nAfter insertion:" << endl;
    inputs.display();

    // Search
    cout << "\nSearching for TRUE:" << endl;
    inputs.search(true);

    // Update
    cout << "\nUpdating index 1 to TRUE:" << endl;
    inputs.update(1, true);
    inputs.display();

    // AND operation
    cout << "\nAND Result: "
         << inputs.ANDOperation() << endl;

    // Delete
    cout << "\nDeleting index 1:" << endl;
    inputs.remove(1);
    inputs.display();

    return 0;
}