package gates;

/**
 * Concrete NAND Gate implementation.
 */
public class NANDGate extends LogicGate {

    public NANDGate(String id, String name, int inputCount) {
        super(id, name, inputCount);
        evaluate();
    }

    @Override
    public boolean evaluate() {
        boolean andResult = true;
        for (int i = 0; i < inputCount; i++) {
            if (!inputs[i]) {
                andResult = false;
                break;
            }
        }
        output = !andResult;
        return output;
    }

    @Override
    public boolean evaluate(boolean[] inputValues) {
        setInputs(inputValues);
        return evaluate();
    }

    @Override
    public String getGateType() {
        return "NAND";
    }
}
