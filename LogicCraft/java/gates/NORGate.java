package gates;

/**
 * Concrete NOR Gate implementation.
 */
public class NORGate extends LogicGate {

    public NORGate(String id, String name, int inputCount) {
        super(id, name, inputCount);
        evaluate();
    }

    @Override
    public boolean evaluate() {
        boolean orResult = false;
        for (int i = 0; i < inputCount; i++) {
            if (inputs[i]) {
                orResult = true;
                break;
            }
        }
        output = !orResult;
        return output;
    }

    @Override
    public boolean evaluate(boolean[] inputValues) {
        setInputs(inputValues);
        return evaluate();
    }

    @Override
    public String getGateType() {
        return "NOR";
    }
}
