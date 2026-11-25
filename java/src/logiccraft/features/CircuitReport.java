package logiccraft.features;

import logiccraft.circuit.Circuit;

public class CircuitReport {

    // Method 1: Generate a report with a default title
    public String generateReport(Circuit circuit) {

        return generateReport(circuit, "LogicCraft Circuit Report");
    }

    // Method 2: Generate a report with a custom title
    public String generateReport(Circuit circuit, String title) {

        return title
                + "\n-------------------------"
                + "\nTotal Gates: " + circuit.getGateCount()
                + "\nTotal Connections: " + circuit.getConnectionCount();
    }
}
