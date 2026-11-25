package logiccraft.gates;

public class NOTGate extends LogicGate {

    public NOTGate() {
        super("NOT", 1);
    }

    @Override
    public boolean calculate(boolean[] inputs) {
        return !inputs[0];
    }
}