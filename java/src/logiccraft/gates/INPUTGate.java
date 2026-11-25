package logiccraft.gates;

public class INPUTGate extends LogicGate {

    private boolean value;

    public INPUTGate(boolean value) {
        super("INPUT", 0);
        this.value = value;
    }

    @Override
    public boolean calculate(boolean[] inputs) {
        return value;
    }

    public void setValue(boolean value) {
        this.value = value;
    }
}