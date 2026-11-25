package logiccraft.ui;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JButton;
import javax.swing.JOptionPane;
import javax.swing.JTextArea;
import javax.swing.JScrollPane;

import java.awt.GridLayout;
import java.awt.BorderLayout;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;

import javax.swing.JFileChooser;

import logiccraft.gates.GateFactory;
import logiccraft.gates.LogicGate;
import logiccraft.gates.INPUTGate;
import logiccraft.gates.OUTPUTGate;
import logiccraft.circuit.Circuit;
import logiccraft.circuit.Connection;
import logiccraft.collections.ComponentList;
import logiccraft.exceptions.InvalidInputException;
import logiccraft.file.ProjectFileManager;
import logiccraft.features.CircuitReport;

public class LogicCraftUI {

    public static void main(String[] args) {

        javax.swing.SwingUtilities.invokeLater(() -> {

            JFrame window = new JFrame("LogicCraft");
            window.setSize(950, 650);
            window.setDefaultCloseOperation(
                    JFrame.EXIT_ON_CLOSE
            );
            window.setLayout(new BorderLayout());

            Circuit circuit = new Circuit();

            JPanel gatePanel = new JPanel();
            gatePanel.setLayout(new GridLayout(0, 1, 5, 5));

            JButton inputButton = new JButton("Add INPUT");
            JButton andButton = new JButton("AND");
            JButton orButton = new JButton("OR");
            JButton notButton = new JButton("NOT");
            JButton xorButton = new JButton("XOR");
            JButton nandButton = new JButton("NAND");
            JButton norButton = new JButton("NOR");
            JButton xnorButton = new JButton("XNOR");
            JButton outputButton = new JButton("Add OUTPUT");

            JButton connectButton = new JButton("Connect Gates");
            JButton showButton = new JButton("Show Circuit");
            JButton evaluateButton = new JButton("Evaluate Circuit");

            // Task 4: File Handling & Generics Actions
            JButton saveButton = new JButton("Save Circuit File");
            JButton loadButton = new JButton("Load Circuit File");
            JButton exportButton = new JButton("Export Report");
            JButton filterButton = new JButton("Filter Gates (Generics)");
            JButton clearButton = new JButton("Clear Circuit");

            JTextArea circuitArea = new JTextArea();
            circuitArea.setEditable(false);
            circuitArea.setFont(
                    new java.awt.Font(
                            java.awt.Font.MONOSPACED,
                            java.awt.Font.PLAIN,
                            14
                    )
            );

            JScrollPane scrollPane =
                    new JScrollPane(circuitArea);

            inputButton.addActionListener(e -> {

                Object[] choices = {"true", "false"};

                Object selected = JOptionPane.showInputDialog(
                        window,
                        "Choose the input value:",
                        "Add INPUT Gate",
                        JOptionPane.QUESTION_MESSAGE,
                        null,
                        choices,
                        choices[0]
                );

                if (selected == null) {
                    return;
                }

                INPUTGate input = new INPUTGate(
                        Boolean.parseBoolean(selected.toString())
                );

                circuit.addGate(input);

                JOptionPane.showMessageDialog(
                        window,
                        "INPUT gate added.\nGate number: "
                                + circuit.getGateCount()
                                + "\nValue: "
                                + input.calculate(new boolean[0])
                );
            });

            andButton.addActionListener(e ->
                    addGate(circuit, "AND", 2, window));

            orButton.addActionListener(e ->
                    addGate(circuit, "OR", 2, window));

            notButton.addActionListener(e ->
                    addGate(circuit, "NOT", 1, window));

            xorButton.addActionListener(e ->
                    addGate(circuit, "XOR", 2, window));

            nandButton.addActionListener(e ->
                    addGate(circuit, "NAND", 2, window));

            norButton.addActionListener(e ->
                    addGate(circuit, "NOR", 2, window));

            xnorButton.addActionListener(e ->
                    addGate(circuit, "XNOR", 2, window));

            outputButton.addActionListener(e -> {
                circuit.addGate(new OUTPUTGate());

                JOptionPane.showMessageDialog(
                        window,
                        "OUTPUT gate added.\nGate number: "
                                + circuit.getGateCount()
                );
            });

            connectButton.addActionListener(e -> {

                if (circuit.getGateCount() < 2) {
                    JOptionPane.showMessageDialog(
                            window,
                            "Add at least two gates first."
                    );
                    return;
                }

                String sourceText = JOptionPane.showInputDialog(
                        window,
                        "Enter SOURCE gate number:"
                );

                if (sourceText == null) {
                    return;
                }

                String destinationText =
                        JOptionPane.showInputDialog(
                                window,
                                "Enter DESTINATION gate number:"
                        );

                if (destinationText == null) {
                    return;
                }

                try {
                    int sourceNumber =
                            Integer.parseInt(sourceText.trim());

                    int destinationNumber =
                            Integer.parseInt(destinationText.trim());

                    if (sourceNumber < 1
                            || sourceNumber > circuit.getGateCount()
                            || destinationNumber < 1
                            || destinationNumber > circuit.getGateCount()) {

                        throw new IllegalArgumentException(
                                "Gate numbers are outside the valid range."
                        );
                    }

                    LogicGate source =
                            circuit.getGate(sourceNumber - 1);

                    LogicGate destination =
                            circuit.getGate(destinationNumber - 1);

                    if (source == destination) {
                        throw new IllegalArgumentException(
                                "A gate cannot connect to itself."
                        );
                    }

                    if (destination instanceof INPUTGate) {
                        throw new IllegalArgumentException(
                                "An INPUT gate cannot receive a connection."
                        );
                    }

                    String inputText = JOptionPane.showInputDialog(
                            window,
                            "Enter destination input position (starting at 0):"
                    );

                    if (inputText == null) {
                        return;
                    }

                    int inputPosition =
                            Integer.parseInt(inputText.trim());

                    if (inputPosition < 0
                            || inputPosition >= destination.getInputCount()) {

                        throw new IllegalArgumentException(
                                "Invalid destination input position."
                        );
                    }

                    circuit.addConnection(
                            new Connection(
                                    source,
                                    destination,
                                    inputPosition
                            )
                    );

                    JOptionPane.showMessageDialog(
                            window,
                            "Connection added successfully."
                    );

                } catch (NumberFormatException ex) {

                    JOptionPane.showMessageDialog(
                            window,
                            "Please enter valid whole numbers."
                    );

                } catch (IllegalArgumentException ex) {

                    JOptionPane.showMessageDialog(
                            window,
                            "Connection error: " + ex.getMessage()
                    );

                } catch (Exception ex) {

                    JOptionPane.showMessageDialog(
                            window,
                            "Unable to connect gates: "
                                    + ex.getMessage()
                    );
                }
            });

            showButton.addActionListener(e -> {

                StringBuilder display = new StringBuilder();

                display.append("GATES\n");
                display.append("-----\n");

                for (int i = 0; i < circuit.getGateCount(); i++) {
                    LogicGate gate = circuit.getGate(i);

                    display.append(i + 1)
                            .append(". ")
                            .append(gate.getName())
                            .append(" (inputs: ")
                            .append(gate.getInputCount())
                            .append(")\n");
                }

                display.append("\nCONNECTIONS\n");
                display.append("-----------\n");

                circuit.showConnections();

                display.append(
                        "Use the console output to see connection details.\n"
                );

                circuitArea.setText(display.toString());
            });

            evaluateButton.addActionListener(e -> {

                try {
                    boolean result = circuit.evaluate();

                    circuitArea.append(
                            "\nCircuit evaluation result: "
                                    + result
                                    + "\n"
                    );

                    JOptionPane.showMessageDialog(
                            window,
                            "Circuit result: " + result
                    );

                } catch (Exception ex) {

                    JOptionPane.showMessageDialog(
                            window,
                            "Could not evaluate circuit:\n"
                                    + ex.getMessage(),
                            "Evaluation Error",
                            JOptionPane.ERROR_MESSAGE
                    );
                }
            });

            // Action: Save Circuit to File (CO4 File Handling)
            saveButton.addActionListener(e -> {
                JFileChooser chooser = new JFileChooser();
                chooser.setSelectedFile(new File("MyCircuit.logiccraft"));
                int res = chooser.showSaveDialog(window);
                if (res == JFileChooser.APPROVE_OPTION) {
                    try {
                        ProjectFileManager mgr = new ProjectFileManager();
                        mgr.saveCircuitToFile(chooser.getSelectedFile(), circuit);
                        JOptionPane.showMessageDialog(window, "Circuit successfully saved to:\n" + chooser.getSelectedFile().getAbsolutePath());
                    } catch (IOException ex) {
                        JOptionPane.showMessageDialog(window, "File Error: " + ex.getMessage(), "Save Error", JOptionPane.ERROR_MESSAGE);
                    }
                }
            });

            // Action: Load Circuit from File (CO4 File Handling)
            loadButton.addActionListener(e -> {
                JFileChooser chooser = new JFileChooser();
                int res = chooser.showOpenDialog(window);
                if (res == JFileChooser.APPROVE_OPTION) {
                    try {
                        ProjectFileManager mgr = new ProjectFileManager();
                        mgr.loadCircuitFromFile(chooser.getSelectedFile(), circuit);
                        JOptionPane.showMessageDialog(window, "Circuit successfully loaded!\nTotal Gates: " + circuit.getGateCount() + "\nConnections: " + circuit.getConnectionCount());
                        showButton.doClick();
                    } catch (FileNotFoundException ex) {
                        JOptionPane.showMessageDialog(window, "File Not Found:\n" + ex.getMessage(), "Error", JOptionPane.ERROR_MESSAGE);
                    } catch (IOException ex) {
                        JOptionPane.showMessageDialog(window, "Corrupted / Malformed Circuit File:\n" + ex.getMessage(), "File Error", JOptionPane.ERROR_MESSAGE);
                    } catch (InvalidInputException ex) {
                        JOptionPane.showMessageDialog(window, "Invalid Circuit Gate Configuration:\n" + ex.getMessage(), "Input Error", JOptionPane.ERROR_MESSAGE);
                    }
                }
            });

            // Action: Export Report (CO4 File Handling)
            exportButton.addActionListener(e -> {
                JFileChooser chooser = new JFileChooser();
                chooser.setSelectedFile(new File("Circuit_Report.txt"));
                int res = chooser.showSaveDialog(window);
                if (res == JFileChooser.APPROVE_OPTION) {
                    try {
                        CircuitReport report = new CircuitReport();
                        String reportText = report.generateReport(circuit);
                        ProjectFileManager mgr = new ProjectFileManager();
                        mgr.saveProject(chooser.getSelectedFile().getAbsolutePath(), reportText);
                        JOptionPane.showMessageDialog(window, "Report exported successfully!");
                    } catch (IOException ex) {
                        JOptionPane.showMessageDialog(window, "Export Error: " + ex.getMessage(), "Error", JOptionPane.ERROR_MESSAGE);
                    }
                }
            });

            // Action: Filter Gates by Type using Java Generics ComponentList (CO4 Generics)
            filterButton.addActionListener(e -> {
                String typeToFilter = JOptionPane.showInputDialog(window, "Enter gate type to filter (e.g., AND, OR, NOT, INPUT):");
                if (typeToFilter != null && !typeToFilter.trim().isEmpty()) {
                    String query = typeToFilter.trim().toUpperCase();
                    ComponentList<LogicGate> filtered = circuit.getGates().filter(g -> g.getName().toUpperCase().contains(query));
                    StringBuilder sb = new StringBuilder();
                    sb.append("FILTER RESULTS FOR TYPE [").append(query).append("]:\n");
                    sb.append("Total Matches: ").append(filtered.size()).append("\n-------------------------\n");
                    for (int i = 0; i < filtered.size(); i++) {
                        LogicGate g = filtered.get(i);
                        sb.append(i + 1).append(". ").append(g.getName()).append(" (Inputs: ").append(g.getInputCount()).append(")\n");
                    }
                    circuitArea.setText(sb.toString());
                }
            });

            // Action: Clear Circuit
            clearButton.addActionListener(e -> {
                circuit.clear();
                circuitArea.setText("Circuit cleared.\n");
                JOptionPane.showMessageDialog(window, "Circuit reset.");
            });

            gatePanel.add(inputButton);
            gatePanel.add(andButton);
            gatePanel.add(orButton);
            gatePanel.add(notButton);
            gatePanel.add(xorButton);
            gatePanel.add(nandButton);
            gatePanel.add(norButton);
            gatePanel.add(xnorButton);
            gatePanel.add(outputButton);
            gatePanel.add(connectButton);
            gatePanel.add(showButton);
            gatePanel.add(evaluateButton);
            gatePanel.add(saveButton);
            gatePanel.add(loadButton);
            gatePanel.add(exportButton);
            gatePanel.add(filterButton);
            gatePanel.add(clearButton);

            window.add(gatePanel, BorderLayout.WEST);
            window.add(scrollPane, BorderLayout.CENTER);

            window.setLocationRelativeTo(null);
            window.setVisible(true);
        });
    }

    public static void addGate(
            Circuit circuit,
            String gateType,
            int inputCount,
            JFrame window) {

        try {
            LogicGate gate = GateFactory.createGate(
                    gateType,
                    inputCount
            );

            circuit.addGate(gate);

            JOptionPane.showMessageDialog(
                    window,
                    gate.getName()
                            + " added to circuit!\n"
                            + "Gate number: "
                            + circuit.getGateCount()
            );

        } catch (InvalidInputException ex) {

            JOptionPane.showMessageDialog(
                    window,
                    "Error: " + ex.getMessage()
            );
        }
    }
}
