package logiccraft.gates;

public class XORGate extends LogicGate {

    public XORGate(int inputCount) {
        super("XOR", inputCount);
    }

    @Override
    public boolean calculate(boolean[] inputs) {

        boolean result = false;

        for (boolean input : inputs) {
            result = result ^ input;
        }

        return result;
    }
}