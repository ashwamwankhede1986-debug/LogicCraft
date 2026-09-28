package gates;

/**
 * Concrete XOR Gate (Exclusive OR) implementation.
 */
public class XORGate extends LogicGate {

    public XORGate() {
        this("G_XOR", "Default_XOR", 2);
    }

    public XORGate(String id, String name, int inputCount) {
        super(id, name, inputCount);
        evaluate();
    }

    @Override
    public boolean evaluate() {
        int trueCount = 0;
        for (int i = 0; i < inputCount; i++) {
            if (inputs[i]) trueCount++;
        }
        output = (trueCount % 2 != 0);
        return output;
    }

    @Override
    public boolean evaluate(boolean[] inputValues) {
        setInputs(inputValues);
        return evaluate();
    }

    @Override
    public String getGateType() {
        return "XOR";
    }
}
