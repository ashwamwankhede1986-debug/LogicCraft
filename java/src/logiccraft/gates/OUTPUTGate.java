package logiccraft.gates;

public class OUTPUTGate extends LogicGate {

    private boolean value;

    public OUTPUTGate() {
        super("OUTPUT", 1);
    }

    @Override
    public boolean calculate(boolean[] inputs) {
        value = inputs[0];
        return value;
    }

    public boolean getValue() {
        return value;
    }
}