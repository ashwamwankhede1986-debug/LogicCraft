package logiccraft.gates;

public class NORGate extends LogicGate {

    public NORGate(int inputCount) {
        super("NOR", inputCount);
    }

    @Override
    public boolean calculate(boolean[] inputs) {

        boolean result = false;

        for (boolean input : inputs) {
            result = result || input;
        }

        return !result;
    }
}