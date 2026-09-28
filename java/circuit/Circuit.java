package circuit;

import gates.LogicGate;
import java.util.ArrayList;
import java.util.List;
import java.util.function.Predicate;

/**
 * Circuit class managing components, connections, and evaluation.
 * Demonstrates:
 *  - Array storage of components (CO1)
 *  - Loops & Conditionals (CO1)
 *  - Polymorphic evaluation of gate references (CO2)
 *  - Lambda expression support for gate querying/filtering (CO2)
 */
public class Circuit {
    public static final int MAX_GATES = 32;
    public static final int MAX_CONNECTIONS = 32;

    private String name;
    private LogicGate[] gates;         // Array storage (CO1)
    private int gateCount;
    private Connection[] connections; // Array storage (CO1)
    private int connectionCount;

    public Circuit(String name) {
        this.name = name;
        this.gates = new LogicGate[MAX_GATES];
        this.gateCount = 0;
        this.connections = new Connection[MAX_CONNECTIONS];
        this.connectionCount = 0;
    }

    public boolean addGate(LogicGate gate) {
        if (gateCount >= MAX_GATES) {
            System.err.println("[Circuit] Maximum gate capacity reached.");
            return false;
        }
        gates[gateCount++] = gate;
        return true;
    }

    public LogicGate findGate(String id) {
        for (int i = 0; i < gateCount; i++) {
            if (gates[i].getId().equals(id)) {
                return gates[i];
            }
        }
        return null;
    }

    public boolean addConnection(Connection conn) {
        if (connectionCount >= MAX_CONNECTIONS) {
            System.err.println("[Circuit] Maximum connection capacity reached.");
            return false;
        }
        connections[connectionCount++] = conn;
        return true;
    }

    /**
     * Propagate wire signals and evaluate entire circuit.
     * Demonstrates Polymorphism: evaluate() called on abstract LogicGate reference.
     */
    public void evaluateCircuit() {
        // Step 1: Initial evaluation
        for (int i = 0; i < gateCount; i++) {
            gates[i].evaluate();
        }

        // Step 2: Signal propagation through wire connections
        for (int c = 0; c < connectionCount; c++) {
            Connection wire = connections[c];
            LogicGate src = findGate(wire.getSourceGateId());
            LogicGate dst = findGate(wire.getDestinationGateId());

            if (src != null && dst != null) {
                boolean signal = src.getOutput();
                dst.setInput(wire.getDestinationInputIndex(), signal);
                dst.evaluate(); // Polymorphic method call (CO2)
            }
        }
    }

    /**
     * Filter gates using Functional Interface / Lambda Expression (CO2).
     */
    public List<LogicGate> filterGates(Predicate<LogicGate> predicate) {
        List<LogicGate> result = new ArrayList<>();
        for (int i = 0; i < gateCount; i++) {
            if (predicate.test(gates[i])) { // Lambda predicate execution
                result.add(gates[i]);
            }
        }
        return result;
    }

    public String getName() {
        return name;
    }

    public int getGateCount() {
        return gateCount;
    }

    public LogicGate[] getGates() {
        LogicGate[] copy = new LogicGate[gateCount];
        System.arraycopy(gates, 0, copy, 0, gateCount);
        return copy;
    }

    public int getConnectionCount() {
        return connectionCount;
    }

    public Connection[] getConnections() {
        Connection[] copy = new Connection[connectionCount];
        System.arraycopy(connections, 0, copy, 0, connectionCount);
        return copy;
    }
}
