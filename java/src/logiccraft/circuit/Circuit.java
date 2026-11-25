
package logiccraft.circuit;

import java.util.Collections;
import java.util.IdentityHashMap;
import java.util.Map;
import java.util.Set;

import logiccraft.collections.ComponentList;
import logiccraft.gates.INPUTGate;
import logiccraft.gates.LogicGate;
import logiccraft.gates.OUTPUTGate;

public class Circuit {

    private ComponentList<LogicGate> gates;
    private ComponentList<Connection> connections;

    public Circuit() {
        gates = new ComponentList<>();
        connections = new ComponentList<>();
    }

    public void addGate(LogicGate gate) {
        if (gate == null) {
            throw new IllegalArgumentException("Gate cannot be null.");
        }
        gates.add(gate);
    }

    public void addConnection(Connection connection) {
        if (connection == null) {
            throw new IllegalArgumentException(
                    "Connection cannot be null.");
        }
        connections.add(connection);
    }

    public void showGates() {
        System.out.println("Gates in circuit:");

        for (int i = 0; i < gates.size(); i++) {
            System.out.println(gates.get(i).getName());
        }
    }

    public void showConnections() {
        System.out.println("Connections in circuit:");

        for (int i = 0; i < connections.size(); i++) {
            Connection connection = connections.get(i);

            System.out.println(
                    connection.getSourceGate().getName()
                    + " -> "
                    + connection.getDestinationGate().getName()
                    + " (Input "
                    + connection.getDestinationInput()
                    + ")");
        }
    }

    public int getGateCount() {
        return gates.size();
    }

    public int getConnectionCount() {
        return connections.size();
    }

    public LogicGate getGate(int index) {
        return gates.get(index);
    }

    public void clear() {
        gates.clear();
        connections.clear();
    }

    public ComponentList<LogicGate> getGates() {
        return gates;
    }

    public ComponentList<Connection> getConnections() {
        return connections;
    }

    public Connection getConnection(int index) {
        return connections.get(index);
    }

    public boolean evaluateConnection(
            Connection connection,
            boolean[] destinationInputs) {

        LogicGate sourceGate = connection.getSourceGate();
        LogicGate destinationGate =
                connection.getDestinationGate();

        int inputIndex = connection.getDestinationInput();

        if (inputIndex < 0
                || inputIndex >= destinationInputs.length) {
            throw new IllegalArgumentException(
                    "Invalid destination input index: " + inputIndex);
        }

        boolean sourceOutput;

        if (sourceGate instanceof INPUTGate) {
            sourceOutput = sourceGate.calculate(new boolean[0]);
        } else {
            sourceOutput = sourceGate.calculate(destinationInputs);
        }

        destinationInputs[inputIndex] = sourceOutput;

        return destinationGate.calculate(destinationInputs);
    }

    /**
     * Evaluates the final connected gate in the circuit.
     * Supports acyclic, multi-stage circuits.
     */
    public boolean evaluate() {

        if (gates.size() == 0 || connections.size() == 0) {
            return false;
        }

        Map<LogicGate, Boolean> results = new IdentityHashMap<>();

        Set<LogicGate> visiting =
                Collections.newSetFromMap(
                        new IdentityHashMap<LogicGate, Boolean>());

        LogicGate finalGate = null;

        // Prefer an OUTPUT gate only if a connection enters it.
        for (int i = 0; i < gates.size(); i++) {
            LogicGate gate = gates.get(i);

            if (gate instanceof OUTPUTGate
                    && hasIncomingConnection(gate)) {
                finalGate = gate;
                break;
            }
        }

        // Otherwise find a connected gate that has no outgoing
        // connection to another gate.
        if (finalGate == null) {
            for (int i = 0; i < connections.size(); i++) {
                LogicGate candidate =
                        connections.get(i).getDestinationGate();

                if (!hasOutgoingConnection(candidate)) {
                    finalGate = candidate;
                }
            }
        }

        if (finalGate == null) {
            throw new IllegalStateException(
                    "Could not determine the final circuit gate.");
        }

        return evaluateGateRecursively(
                finalGate, results, visiting);
    }

    private boolean hasIncomingConnection(LogicGate gate) {
        for (int i = 0; i < connections.size(); i++) {
            if (connections.get(i).getDestinationGate() == gate) {
                return true;
            }
        }
        return false;
    }

    private boolean hasOutgoingConnection(LogicGate gate) {
        for (int i = 0; i < connections.size(); i++) {
            if (connections.get(i).getSourceGate() == gate) {
                return true;
            }
        }
        return false;
    }

    private boolean evaluateGateRecursively(
            LogicGate gate,
            Map<LogicGate, Boolean> results,
            Set<LogicGate> visiting) {

        if (results.containsKey(gate)) {
            return results.get(gate);
        }

        if (visiting.contains(gate)) {
            throw new IllegalStateException(
                    "Circuit contains a cycle at gate: "
                    + gate.getName());
        }

        visiting.add(gate);

        // Input gates provide their stored Boolean values.
        if (gate instanceof INPUTGate) {
            boolean value = gate.calculate(new boolean[0]);

            visiting.remove(gate);
            results.put(gate, value);

            return value;
        }

        int inputCount = gate.getInputCount();
        boolean[] inputs = new boolean[inputCount];
        boolean[] connected = new boolean[inputCount];

        // Calculate every source gate connected to this gate.
        for (int i = 0; i < connections.size(); i++) {
            Connection connection = connections.get(i);

            if (connection.getDestinationGate() != gate) {
                continue;
            }

            int inputIndex = connection.getDestinationInput();

            if (inputIndex < 0 || inputIndex >= inputCount) {
                throw new IllegalArgumentException(
                        "Invalid input index " + inputIndex
                        + " for gate " + gate.getName());
            }

            inputs[inputIndex] = evaluateGateRecursively(
                    connection.getSourceGate(),
                    results,
                    visiting);

            connected[inputIndex] = true;
        }

        // Every required input must be connected.
        for (int i = 0; i < inputCount; i++) {
            if (!connected[i]) {
                throw new IllegalStateException(
                        "Input " + i + " of gate "
                        + gate.getName() + " is not connected.");
            }
        }

        boolean output = gate.calculate(inputs);

        visiting.remove(gate);
        results.put(gate, output);

        return output;
    }
}
