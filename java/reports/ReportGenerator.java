package reports;

import circuit.Circuit;
import circuit.Connection;
import gates.LogicGate;

/**
 * ReportGenerator formats circuit metrics and logic states into a textual report
 * using StringBuilder (CO1 - StringBuilder & Loops).
 */
public class ReportGenerator {

    /**
     * Generates comprehensive textual circuit report using StringBuilder.
     */
    public static String generateCircuitReport(Circuit circuit) {
        StringBuilder sb = new StringBuilder();

        sb.append("=================================================================\n");
        sb.append("                 LOGICCRAFT CIRCUIT REPORT                       \n");
        sb.append("=================================================================\n");
        sb.append("Circuit Name : ").append(circuit.getName()).append("\n");
        sb.append("Gate Count   : ").append(circuit.getGateCount()).append("\n");
        sb.append("Wires Count  : ").append(circuit.getConnectionCount()).append("\n");
        sb.append("-----------------------------------------------------------------\n");
        sb.append("GATE INVENTORY & STATE DUMP:\n");

        LogicGate[] gates = circuit.getGates();
        for (int i = 0; i < gates.length; i++) {
            LogicGate g = gates[i];
            sb.append(String.format("  Gate %d: [%s] Type: %-4s | Name: %-18s\n", 
                (i + 1), g.getId(), g.getGateType(), g.getName()));
            
            sb.append("          Inputs : [");
            boolean[] inputs = g.getInputs();
            for (int k = 0; k < inputs.length; k++) {
                sb.append(inputs[k] ? "1" : "0");
                if (k < inputs.length - 1) sb.append(", ");
            }
            sb.append("]  --->  Output: ").append(g.getOutput() ? "1" : "0").append("\n");
        }

        sb.append("-----------------------------------------------------------------\n");
        sb.append("WIRE ROUTING CONNECTIONS:\n");
        Connection[] wires = circuit.getConnections();
        if (wires.length == 0) {
            sb.append("  (No inter-gate wire connections defined)\n");
        } else {
            for (int i = 0; i < wires.length; i++) {
                sb.append("  [").append(i + 1).append("] ").append(wires[i].toString()).append("\n");
            }
        }
        sb.append("=================================================================\n");

        return sb.toString();
    }
}
