package circuit;

/**
 * Represents a directed wire connection between two gates in the circuit.
 */
public class Connection {
    private final String sourceGateId;
    private final String destinationGateId;
    private final int destinationInputIndex;

    public Connection(String sourceGateId, String destinationGateId, int destinationInputIndex) {
        this.sourceGateId = sourceGateId;
        this.destinationGateId = destinationGateId;
        this.destinationInputIndex = destinationInputIndex;
    }

    public String getSourceGateId() {
        return sourceGateId;
    }

    public String getDestinationGateId() {
        return destinationGateId;
    }

    public int getDestinationInputIndex() {
        return destinationInputIndex;
    }

    @Override
    public String toString() {
        return "Wire: " + sourceGateId + " [OUT] ---> " + destinationGateId + " [Pin " + destinationInputIndex + "]";
    }
}
