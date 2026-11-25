package logiccraft.circuit;

import logiccraft.gates.LogicGate;

public class Connection {

    private LogicGate sourceGate;
    private LogicGate destinationGate;
    private int destinationInput;

    public Connection(LogicGate sourceGate,
                      LogicGate destinationGate,
                      int destinationInput) {

        this.sourceGate = sourceGate;
        this.destinationGate = destinationGate;
        this.destinationInput = destinationInput;
    }

    public LogicGate getSourceGate() {
        return sourceGate;
    }

    public LogicGate getDestinationGate() {
        return destinationGate;
    }

    public int getDestinationInput() {
        return destinationInput;
    }
}