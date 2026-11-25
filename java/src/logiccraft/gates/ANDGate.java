
package logiccraft.gates;

public class ANDGate extends MultiInputGate
        implements GateInfo, InputValidator {

    public ANDGate(int inputCount) {
        super("AND", inputCount);
    }

    @Override
    public boolean calculate(boolean[] inputs) {

        boolean result = true;

        for (boolean input : inputs) {
            result = result && input;
        }

        return result;
    }

    @Override
    public String getGateDescription() {
        return "AND gate returns true when all inputs are true.";
    }

    @Override
    public boolean isValidInputCount(int inputCount) {
        return inputCount >= 1;
    }
}
