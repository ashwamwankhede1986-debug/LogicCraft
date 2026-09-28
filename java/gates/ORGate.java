package gates;

/**
 * Concrete OR Gate implementation demonstrating Inheritance and Overriding (CO2).
 */
public class ORGate extends LogicGate {

    public ORGate() {
        this("G_OR", "Default_OR", 2);
    }

    public ORGate(String id, String name, int inputCount) {
        super(id, name, inputCount);
        evaluate();
    }

    @Override
    public boolean evaluate() {
        output = false;
        for (int i = 0; i < inputCount; i++) {
            if (inputs[i]) {
                output = true;
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
        return "OR";
    }
}
