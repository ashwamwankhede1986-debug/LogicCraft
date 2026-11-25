package logiccraft.gates;

public class NANDGate extends LogicGate {

    public NANDGate(int inputCount) {
        super("NAND", inputCount);
    }

    @Override
    public boolean calculate(boolean[] inputs) {

        boolean result = true;

        for (boolean input : inputs) {
            result = result && input;
        }

        return !result;
    }
}