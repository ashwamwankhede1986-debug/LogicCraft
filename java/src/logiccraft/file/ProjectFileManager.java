package logiccraft.file;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

import logiccraft.circuit.Circuit;
import logiccraft.circuit.Connection;
import logiccraft.exceptions.InvalidInputException;
import logiccraft.gates.GateFactory;
import logiccraft.gates.INPUTGate;
import logiccraft.gates.LogicGate;
import logiccraft.gates.OUTPUTGate;

/**
 * Robust Circuit File Management System (CO4 - File Handling & Generics Integration)
 *
 * Capabilities:
 * - Serialize complete circuit state (gates, types, connections) to disk
 * - Deserialize and reconstruct active circuit instances
 * - Handle missing files (FileNotFoundException)
 * - Handle corrupted / malformed files (IOException with syntax details)
 * - Export human-readable circuit evaluation reports
 */
public class ProjectFileManager {

    public void saveProject(String fileName, String projectData) throws IOException {
        if (fileName == null || fileName.trim().isEmpty()) {
            throw new IllegalArgumentException("File name cannot be empty.");
        }
        try (FileWriter writer = new FileWriter(fileName)) {
            writer.write(projectData != null ? projectData : "");
        }
    }

    public String loadProject(String fileName) throws IOException {
        if (fileName == null || fileName.trim().isEmpty()) {
            throw new IllegalArgumentException("File name cannot be empty.");
        }
        File f = new File(fileName);
        if (!f.exists()) {
            throw new FileNotFoundException("Project file not found: " + fileName);
        }

        StringBuilder projectData = new StringBuilder();
        try (BufferedReader bufferedReader = new BufferedReader(new FileReader(f))) {
            String line;
            while ((line = bufferedReader.readLine()) != null) {
                projectData.append(line).append("\n");
            }
        }
        return projectData.toString();
    }

    /**
     * Serializes complete Circuit topology to a structured file.
     */
    public void saveCircuitToFile(File file, Circuit circuit) throws IOException {
        if (file == null) {
            throw new IllegalArgumentException("Target file cannot be null.");
        }
        if (circuit == null) {
            throw new IllegalArgumentException("Circuit cannot be null.");
        }

        StringBuilder sb = new StringBuilder();
        sb.append("# LOGICCRAFT CIRCUIT TOPOLOGY FILE v1.0\n");
        sb.append("GATES: ").append(circuit.getGateCount()).append("\n");

        for (int i = 0; i < circuit.getGateCount(); i++) {
            LogicGate gate = circuit.getGate(i);
            String typeName = getGateTypeName(gate);
            sb.append("GATE|").append(i).append("|").append(typeName).append("|").append(gate.getInputCount());

            if (gate instanceof INPUTGate) {
                boolean val = gate.calculate(new boolean[0]);
                sb.append("|").append(val);
            }
            sb.append("\n");
        }

        sb.append("CONNECTIONS: ").append(circuit.getConnectionCount()).append("\n");
        for (int i = 0; i < circuit.getConnectionCount(); i++) {
            Connection c = circuit.getConnection(i);
            int srcIdx = findGateIndex(circuit, c.getSourceGate());
            int dstIdx = findGateIndex(circuit, c.getDestinationGate());
            sb.append("CONN|").append(srcIdx).append("|").append(dstIdx).append("|").append(c.getDestinationInput()).append("\n");
        }

        try (FileWriter writer = new FileWriter(file)) {
            writer.write(sb.toString());
        }
    }

    /**
     * Deserializes and restores a Circuit from a structured file.
     */
    public void loadCircuitFromFile(File file, Circuit targetCircuit) throws IOException, InvalidInputException {
        if (file == null) {
            throw new IllegalArgumentException("Source file cannot be null.");
        }
        if (!file.exists()) {
            throw new FileNotFoundException("Circuit file does not exist: " + file.getAbsolutePath());
        }
        if (file.length() == 0) {
            throw new IOException("Circuit file is corrupted or empty: " + file.getName());
        }
        if (targetCircuit == null) {
            throw new IllegalArgumentException("Target circuit cannot be null.");
        }

        targetCircuit.clear();

        try (BufferedReader reader = new BufferedReader(new FileReader(file))) {
            String line;
            int lineNumber = 0;
            boolean headerVerified = false;

            while ((line = reader.readLine()) != null) {
                lineNumber++;
                line = line.trim();
                if (line.isEmpty() || line.startsWith("#")) {
                    if (line.contains("LOGICCRAFT")) headerVerified = true;
                    continue;
                }

                if (line.startsWith("GATES:") || line.startsWith("CONNECTIONS:")) {
                    continue;
                }

                String[] parts = line.split("\\|");
                if (parts[0].equals("GATE")) {
                    if (parts.length < 4) {
                        throw new IOException("Malformed GATE definition at line " + lineNumber + ": " + line);
                    }
                    String type = parts[2].trim().toUpperCase();
                    int inCount = Integer.parseInt(parts[3].trim());

                    if (type.equals("INPUT")) {
                        boolean val = (parts.length >= 5) && Boolean.parseBoolean(parts[4].trim());
                        targetCircuit.addGate(new INPUTGate(val));
                    } else if (type.equals("OUTPUT")) {
                        targetCircuit.addGate(new OUTPUTGate());
                    } else {
                        LogicGate gate = GateFactory.createGate(type, inCount);
                        targetCircuit.addGate(gate);
                    }

                } else if (parts[0].equals("CONN")) {
                    if (parts.length < 4) {
                        throw new IOException("Malformed CONN definition at line " + lineNumber + ": " + line);
                    }
                    int srcIdx = Integer.parseInt(parts[1].trim());
                    int dstIdx = Integer.parseInt(parts[2].trim());
                    int pinIdx = Integer.parseInt(parts[3].trim());

                    if (srcIdx < 0 || srcIdx >= targetCircuit.getGateCount() ||
                        dstIdx < 0 || dstIdx >= targetCircuit.getGateCount()) {
                        throw new IOException("Connection references out-of-bounds gate index at line " + lineNumber);
                    }

                    LogicGate src = targetCircuit.getGate(srcIdx);
                    LogicGate dst = targetCircuit.getGate(dstIdx);
                    targetCircuit.addConnection(new Connection(src, dst, pinIdx));
                }
            }
        }
    }

    private String getGateTypeName(LogicGate gate) {
        if (gate instanceof INPUTGate) return "INPUT";
        if (gate instanceof OUTPUTGate) return "OUTPUT";
        String name = gate.getName();
        if (name == null) return "AND";
        // Extract base type e.g. "AND", "OR"
        for (String t : new String[]{"AND", "OR", "NOT", "XOR", "NAND", "NOR", "XNOR"}) {
            if (name.toUpperCase().contains(t)) return t;
        }
        return "AND";
    }

    private int findGateIndex(Circuit circuit, LogicGate gate) {
        for (int i = 0; i < circuit.getGateCount(); i++) {
            if (circuit.getGate(i) == gate) return i;
        }
        return -1;
    }
}
