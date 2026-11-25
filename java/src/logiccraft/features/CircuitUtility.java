
package logiccraft.features;

public class CircuitUtility
        implements CircuitPrintable, CircuitValidatable {

    private String circuitName;

    public CircuitUtility(String circuitName) {
        this.circuitName = circuitName;
    }

    @Override
    public String printDetails() {
        return "Circuit Name: " + circuitName;
    }

    @Override
    public boolean validate() {
        return circuitName != null && !circuitName.trim().isEmpty();
    }
}
