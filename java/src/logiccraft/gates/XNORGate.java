package logiccraft.gates;

public class XNORGate extends LogicGate {

    public XNORGate(int inputCount) {
        super("XNOR", inputCount);
    }

    @Override
    public boolean calculate(boolean[] inputs) {

        boolean result = false;

        for (boolean input : inputs) {
            result = result ^ input;
        }

        return !result;
    }
} 