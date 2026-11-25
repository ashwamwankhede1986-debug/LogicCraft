package logiccraft.gates;

public abstract class LogicGate {

    protected String name;
    protected int inputCount;

    public LogicGate(String name, int inputCount) {
        this.name = name;
        this.inputCount = inputCount;
    }

    public abstract boolean calculate(boolean[] inputs);

    public String getName() {
        return name;
    }

    public int getInputCount() {
        return inputCount;
    }
}