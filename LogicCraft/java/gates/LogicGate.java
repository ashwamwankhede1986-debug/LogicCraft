package gates;

import java.util.Arrays;

/**
 * Abstract Base Class representing a Logic Gate in LogicCraft.
 * Demonstrates:
 *  - Abstract Class & Encapsulation (CO1 & CO2)
 *  - static and final keywords (CO2)
 *  - Array storage for Boolean inputs (CO1)
 *  - Template for Polymorphic evaluation (CO2)
 */
public abstract class LogicGate {
    // static final constant (CO2)
    public static final int MAX_INPUTS = 8;

    // static field tracking global gate instantiation count (CO2)
    private static int totalGatesInstantiated = 0;

    // final identifier (CO2)
    protected final String id;
    protected String name;
    protected int inputCount;
    protected boolean[] inputs; // Boolean inputs stored in array (CO1)
    protected boolean output;

    /**
     * Parameterized Constructor
     */
    public LogicGate(String id, String name, int inputCount) {
        this.id = id;
        this.name = name;
        this.inputCount = Math.min(inputCount, MAX_INPUTS);
        this.inputs = new boolean[this.inputCount];
        this.output = false;
        totalGatesInstantiated++;
    }

    /**
     * Abstract method to be overridden by concrete gate subclasses (CO2 Polymorphism).
     */
    public abstract boolean evaluate();

    /**
     * Overloaded evaluate accepting an array of inputs (CO1 Arrays & Method Overloading).
     */
    public abstract boolean evaluate(boolean[] inputValues);

    /**
     * Returns the standardized gate type name (e.g. "AND", "OR", "NOT").
     */
    public abstract String getGateType();

    // Encapsulation: Getters and Setters (CO1)
    public final String getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getInputCount() {
        return inputCount;
    }

    public boolean getInput(int index) {
        if (index >= 0 && index < inputCount) {
            return inputs[index];
        }
        return false;
    }

    public boolean[] getInputs() {
        return Arrays.copyOf(inputs, inputCount);
    }

    public void setInput(int index, boolean value) {
        if (index >= 0 && index < inputCount) {
            inputs[index] = value;
            evaluate();
        }
    }

    public void setInputs(boolean[] values) {
        if (values != null) {
            int len = Math.min(values.length, inputCount);
            for (int i = 0; i < len; i++) {
                inputs[i] = values[i];
            }
            evaluate();
        }
    }

    public boolean getOutput() {
        return output;
    }

    public static int getTotalGatesInstantiated() {
        return totalGatesInstantiated;
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("[").append(id).append("] ")
          .append(getGateType()).append(" Gate ('").append(name).append("') ")
          .append("Inputs: [");
        for (int i = 0; i < inputCount; i++) {
            sb.append(inputs[i] ? "1" : "0");
            if (i < inputCount - 1) sb.append(", ");
        }
        sb.append("] -> Output: ").append(output ? "1" : "0");
        return sb.toString();
    }
}
