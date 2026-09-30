#include <iostream>
#include <string>
using namespace std;

class BooleanTable
{
private:
    // Maximum:
    // 8 rows for 3 inputs (2^3 = 8)
    // 3 input columns + 1 output column
    bool data[8][4];

    int rows;
    int inputs;
    string gate;

public:

    // Constructor
    BooleanTable()
    {
        rows = 0;
        inputs = 0;
        gate = "";
    }

    // ----------------------------------------------------
    // Set number of inputs
    // ----------------------------------------------------
    void setInputs()
    {
        do
        {
            cout << "Enter number of gate inputs (2 or 3): ";
            cin >> inputs;

            if (inputs != 2 && inputs != 3)
            {
                cout << "Please enter only 2 or 3.\n";
            }

        } while (inputs != 2 && inputs != 3);
    }

    // ----------------------------------------------------
    // Select Logic Gate
    // ----------------------------------------------------
    void selectGate()
    {
        int choice;

        cout << "\n===== SELECT LOGIC GATE =====\n";
        cout << "1. AND\n";
        cout << "2. OR\n";
        cout << "3. NAND\n";
        cout << "4. NOR\n";
        cout << "5. XOR\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            gate = "AND";
            break;

        case 2:
            gate = "OR";
            break;

        case 3:
            gate = "NAND";
            break;

        case 4:
            gate = "NOR";
            break;

        case 5:
            gate = "XOR";
            break;

        default:
            cout << "Invalid choice. AND gate selected.\n";
            gate = "AND";
        }
    }

    // ----------------------------------------------------
    // Calculate output for one row
    // ----------------------------------------------------
    bool calculateOutput(int row)
    {
        bool result;

        if (gate == "AND")
        {
            result = true;

            for (int j = 0; j < inputs; j++)
            {
                result = result && data[row][j];
            }
        }

        else if (gate == "OR")
        {
            result = false;

            for (int j = 0; j < inputs; j++)
            {
                result = result || data[row][j];
            }
        }

        else if (gate == "NAND")
        {
            result = true;

            for (int j = 0; j < inputs; j++)
            {
                result = result && data[row][j];
            }

            result = !result;
        }

        else if (gate == "NOR")
        {
            result = false;

            for (int j = 0; j < inputs; j++)
            {
                result = result || data[row][j];
            }

            result = !result;
        }

        else if (gate == "XOR")
        {
            result = false;

            for (int j = 0; j < inputs; j++)
            {
                result = result ^ data[row][j];
            }
        }

        return result;
    }

    // ----------------------------------------------------
    // Generate complete truth table
    // ----------------------------------------------------
    void generateTruthTable()
    {
        rows = 1 << inputs;   // 2^inputs

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < inputs; j++)
            {
                data[i][j] =
                    (i >> (inputs - j - 1)) & 1;
            }

            data[i][inputs] = calculateOutput(i);
        }

        cout << "\nTruth table generated successfully.\n";
    }

    // ----------------------------------------------------
    // INSERT operation
    // ----------------------------------------------------
    void insertRow()
    {
        if (rows >= 8)
        {
            cout << "\nArray is full! Maximum 8 rows allowed.\n";
            return;
        }

        cout << "\nEnter values for new row:\n";

        for (int j = 0; j < inputs; j++)
        {
            int value;

            do
            {
                cout << "Input " << j + 1 << " (0/1): ";
                cin >> value;

                if (value != 0 && value != 1)
                {
                    cout << "Please enter only 0 or 1.\n";
                }

            } while (value != 0 && value != 1);

            data[rows][j] = value;
        }

        data[rows][inputs] = calculateOutput(rows);

        rows++;

        cout << "Row inserted successfully.\n";
    }

    // ----------------------------------------------------
    // DELETE operation
    // ----------------------------------------------------
    void deleteRow()
    {
        if (rows == 0)
        {
            cout << "\nTable is empty.\n";
            return;
        }

        int row;

        cout << "\nEnter row number to delete (1-" << rows << "): ";
        cin >> row;

        if (row < 1 || row > rows)
        {
            cout << "Invalid row number.\n";
            return;
        }

        row--;

        // Shift rows upward
        for (int i = row; i < rows - 1; i++)
        {
            for (int j = 0; j <= inputs; j++)
            {
                data[i][j] = data[i + 1][j];
            }
        }

        rows--;

        cout << "Row deleted successfully.\n";
    }

    // ----------------------------------------------------
    // UPDATE operation
    // ----------------------------------------------------
    void updateRow()
    {
        if (rows == 0)
        {
            cout << "\nTable is empty.\n";
            return;
        }

        int row;

        cout << "\nEnter row number to update (1-" << rows << "): ";
        cin >> row;

        if (row < 1 || row > rows)
        {
            cout << "Invalid row number.\n";
            return;
        }

        row--;

        cout << "\nEnter new values:\n";

        for (int j = 0; j < inputs; j++)
        {
            int value;

            do
            {
                cout << "Input " << j + 1 << " (0/1): ";
                cin >> value;

                if (value != 0 && value != 1)
                {
                    cout << "Please enter only 0 or 1.\n";
                }

            } while (value != 0 && value != 1);

            data[row][j] = value;
        }

        // Recalculate output
        data[row][inputs] = calculateOutput(row);

        cout << "Row updated successfully.\n";
    }

    // ----------------------------------------------------
    // SEARCH operation
    // ----------------------------------------------------
    void searchRow()
    {
        if (rows == 0)
        {
            cout << "\nTable is empty.\n";
            return;
        }

        bool searchValues[3];

        cout << "\nEnter input combination to search:\n";

        for (int j = 0; j < inputs; j++)
        {
            int value;

            do
            {
                cout << "Input " << j + 1 << " (0/1): ";
                cin >> value;

                if (value != 0 && value != 1)
                {
                    cout << "Please enter only 0 or 1.\n";
                }

            } while (value != 0 && value != 1);

            searchValues[j] = value;
        }

        bool found = false;

        for (int i = 0; i < rows; i++)
        {
            bool match = true;

            for (int j = 0; j < inputs; j++)
            {
                if (data[i][j] != searchValues[j])
                {
                    match = false;
                    break;
                }
            }

            if (match)
            {
                cout << "\nCombination found at row "
                     << i + 1 << "." << endl;

                cout << "Output = "
                     << data[i][inputs] << endl;

                found = true;
            }
        }

        if (!found)
        {
            cout << "\nCombination not found.\n";
        }
    }

    // ----------------------------------------------------
    // DISPLAY operation
    // ----------------------------------------------------
    void display()
    {
        if (rows == 0)
        {
            cout << "\nTable is empty.\n";
            return;
        }

        cout << "\n=====================================\n";
        cout << "        " << gate << " TRUTH TABLE\n";
        cout << "=====================================\n";

        for (int j = 0; j < inputs; j++)
        {
            cout << "Input" << j + 1 << "\t";
        }

        cout << "Output\n";

        cout << "-------------------------------------\n";

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j <= inputs; j++)
            {
                cout << data[i][j] << "\t";
            }

            cout << endl;
        }

        cout << "=====================================\n";
    }
};


// ========================================================
// MAIN FUNCTION
// ========================================================

int main()
{
    BooleanTable table;

    int choice;

    cout << "========================================\n";
    cout << " DIGITAL LOGIC CIRCUIT VISUAL WORKBENCH\n";
    cout << "        CO1 - 2D ARRAY MODULE\n";
    cout << "========================================\n";

    // User selects number of inputs
    table.setInputs();

    // User selects gate
    table.selectGate();

    do
    {
        cout << "\n\n========== MENU ==========\n";
        cout << "1. Generate Truth Table\n";
        cout << "2. Insert Row\n";
        cout << "3. Delete Row\n";
        cout << "4. Update Row\n";
        cout << "5. Search Row\n";
        cout << "6. Display Truth Table\n";
        cout << "7. Exit\n";
        cout << "==========================\n";

        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            table.generateTruthTable();
            break;

        case 2:
            table.insertRow();
            break;

        case 3:
            table.deleteRow();
            break;

        case 4:
            table.updateRow();
            break;

        case 5:
            table.searchRow();
            break;

        case 6:
            table.display();
            break;

        case 7:
            cout << "\nExiting program...\n";
            break;

        default:
            cout << "\nInvalid choice. Try again.\n";
        }

    } while (choice != 7);

    return 0;
}