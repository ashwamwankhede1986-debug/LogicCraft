package logiccraft.gates;

public class ORGate extends LogicGate {

    public ORGate(int inputCount) {
        super("OR", inputCount);
    }

    @Override
    public boolean calculate(boolean[] inputs) {

        boolean result = false;

        for (boolean input : inputs) {
            result = result || input;
        }

        return result;
    }
}