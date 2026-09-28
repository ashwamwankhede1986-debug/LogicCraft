package gates;

/**
 * Concrete AND Gate implementation demonstrating Inheritance and Overriding (CO2).
 */
public class ANDGate extends LogicGate {

    public ANDGate() {
        this("G_AND", "Default_AND", 2);
    }

    public ANDGate(String id, String name, int inputCount) {
        super(id, name, inputCount);
        evaluate();
    }

    @Override
    public boolean evaluate() {
        output = true;
        // Loops and conditional statements (CO1)
        for (int i = 0; i < inputCount; i++) {
            if (!inputs[i]) {
                output = false;
                break;
            }
        }
        return output;
    }

    @Override
    public boolean evaluate(boolean[] inputValues) {
        setInputs(inputValues);
        return evaluate();
    }

    @Override
    public String getGateType() {
        return "AND";
    }
}
