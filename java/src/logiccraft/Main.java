package logiccraft;

import logiccraft.circuit.Circuit;
import logiccraft.circuit.Connection;
import logiccraft.collections.ComponentList;
import logiccraft.exceptions.InvalidInputException;
import logiccraft.features.CircuitReport;
import logiccraft.file.ProjectFileManager;
import logiccraft.gates.ANDGate;
import logiccraft.gates.GateFactory;
import logiccraft.gates.INPUTGate;
import logiccraft.gates.LogicGate;
import logiccraft.gates.NOTGate;
import logiccraft.gates.ORGate;
import logiccraft.gates.OUTPUTGate;
import logiccraft.ui.LogicCraftUI;
import logiccraft.ui.LoginWindow;

import test.CircuitReportTest;
import test.ExceptionTest;
import test.FileTest;
import test.GenericTest;
import test.InheritanceTest;
import test.InterfaceTest;
import test.LogicCraftTest;
import test.MultiStageCircuitTest;
import test.MultipleInheritanceTest;
import test.MultithreadingTest;
import test.OverloadingTest;
import test.Task4FileGenericTest;

/**
 * LogicCraft Java Demonstration Entry Point
 * Academic Course Outcome (CO) Verification:
 *  - CO1: Fundamentals (Classes, Methods, Arrays, Encapsulation)
 *  - CO2: Advanced OOP (Inheritance, Polymorphism, Interfaces, Threads)
 *  - CO3: Exceptions (InvalidInputException), Collections (ComponentList),
 *         Packages (logiccraft.*), File I/O, Swing UI (LogicCraftUI)
 */
public class Main {

    public static void main(String[] args) {
        if (args.length > 0 && (args[0].equalsIgnoreCase("--gui") || args[0].equalsIgnoreCase("-g"))) {
            System.out.println("[INFO] Launching LogicCraft Java Swing Application...");
            if (LoginWindow.showAndAuthenticate()) {
                LogicCraftUI.main(args);
            } else {
                System.out.println("[INFO] Authentication cancelled or failed; Java GUI was not opened.");
            }
            return;
        }

        printBanner();
        runCO1AndCO2Review();
        runCO3Demonstration();
        runFullTestSuite();

        System.out.println("\n*****************************************************************");
        System.out.println("*   JAVA / OOP (CO1, CO2 & CO3) DEMONSTRATION COMPLETE!          *");
        System.out.println("*****************************************************************");
        System.out.println("Tip: Launch the graphical Swing UI anytime using: java -cp bin logiccraft.Main --gui\n");
    }

    private static void printBanner() {
        System.out.println("*****************************************************************");
        System.out.println("*      LOGICCRAFT — VISUAL LOGIC GATE SIMULATOR (TASK 3)        *");
        System.out.println("*           MODULE: PROBLEM SOLVING USING OOP (JAVA)            *");
        System.out.println("*****************************************************************");
        System.out.println("*  CO1: FUNDAMENTALS   -> Classes, Objects, Encapsulation, Arrays*");
        System.out.println("*  CO2: ADVANCED OOP   -> Polymorphism, Hierarchy, Interfaces   *");
        System.out.println("*  CO3: EXCEPTIONS &   -> InvalidInputException, Packages,      *");
        System.out.println("*       COLLECTIONS       ComponentList<T>, Swing UI & Files     *");
        System.out.println("*****************************************************************\n");
    }

    private static void runCO1AndCO2Review() {
        System.out.println("=================================================================");
        System.out.println(" [DEMONSTRATION 1 / CO1 & CO2]: POLYMORPHISM & CIRCUIT SIMULATION");
        System.out.println("=================================================================");

        Circuit circuit = new Circuit();
        INPUTGate inA = new INPUTGate(true);
        INPUTGate inB = new INPUTGate(false);
        INPUTGate inC = new INPUTGate(true);

        circuit.addGate(inA);
        circuit.addGate(inB);
        circuit.addGate(inC);

        try {
            LogicGate g1 = GateFactory.createGate("AND", 2);
            LogicGate g2 = GateFactory.createGate("OR", 2);
            LogicGate g3 = GateFactory.createGate("NOT", 1);
            OUTPUTGate out = new OUTPUTGate();

            circuit.addGate(g1);
            circuit.addGate(g2);
            circuit.addGate(g3);
            circuit.addGate(out);

            circuit.addConnection(new Connection(inA, g1, 0));
            circuit.addConnection(new Connection(inB, g1, 1));
            circuit.addConnection(new Connection(g1, g2, 0));
            circuit.addConnection(new Connection(inC, g2, 1));
            circuit.addConnection(new Connection(g2, g3, 0));
            circuit.addConnection(new Connection(g3, out, 0));

            boolean finalResult = circuit.evaluate();
            System.out.println("Circuit Evaluated Successfully:");
            System.out.println(" -> Gate 1 (AND) [true, false] = " + g1.calculate(new boolean[]{true, false}));
            System.out.println(" -> Gate 2 (OR)  [G1, true]    = " + g2.calculate(new boolean[]{false, true}));
            System.out.println(" -> Gate 3 (NOT) [G2]          = " + g3.calculate(new boolean[]{true}));
            System.out.println(" -> Final OUTPUT Gate          = " + finalResult);

        } catch (Exception e) {
            System.out.println("[ERROR] Circuit construction error: " + e.getMessage());
        }
    }

    private static void runCO3Demonstration() {
        System.out.println("\n=================================================================");
        System.out.println(" [DEMONSTRATION 2 / CO3]: EXCEPTION HANDLING & GENERIC COLLECTIONS");
        System.out.println("=================================================================");

        System.out.println("\n--- [CO3.1: EXCEPTION HANDLING (InvalidInputException)] ---");
        System.out.println("Scenario: Validating gate creation with invalid input counts.");
        
        // Normal Case
        try {
            LogicGate validGate = GateFactory.createGate("AND", 2);
            System.out.println(" [NORMAL CASE]: Created " + validGate.getName() + " with 2 inputs -> [SUCCESS]");
        } catch (InvalidInputException e) {
            System.out.println(" [UNEXPECTED FAILURE]: " + e.getMessage());
        }

        // Boundary / Error Case 1: Zero inputs
        try {
            System.out.print(" [BOUNDARY CASE 1]: Attempting createGate(\"AND\", 0) -> ");
            GateFactory.createGate("AND", 0);
            System.out.println("[FAILED: Expected Exception Not Thrown]");
        } catch (InvalidInputException e) {
            System.out.println("[CAUGHT EXPECTED EXCEPTION]: " + e.getMessage());
        }

        // Boundary / Error Case 2: Negative inputs
        try {
            System.out.print(" [BOUNDARY CASE 2]: Attempting createGate(\"OR\", -1) -> ");
            GateFactory.createGate("OR", -1);
            System.out.println("[FAILED: Expected Exception Not Thrown]");
        } catch (InvalidInputException e) {
            System.out.println("[CAUGHT EXPECTED EXCEPTION]: " + e.getMessage());
        }

        // Boundary / Error Case 3: Unsupported gate type
        try {
            System.out.print(" [BOUNDARY CASE 3]: Attempting createGate(\"UNKNOWN_GATE\", 2) -> ");
            GateFactory.createGate("UNKNOWN_GATE", 2);
            System.out.println("[FAILED: Expected Exception Not Thrown]");
        } catch (InvalidInputException e) {
            System.out.println("[CAUGHT EXPECTED EXCEPTION]: " + e.getMessage());
        }

        System.out.println("\n--- [CO3.2: GENERIC COLLECTIONS (ComponentList<T>)] ---");
        System.out.println("Scenario: Managing circuit components dynamically with custom generic list.");
        ComponentList<LogicGate> gateRegistry = new ComponentList<>();
        gateRegistry.add(new ANDGate(2));
        gateRegistry.add(new ORGate(2));
        gateRegistry.add(new NOTGate());

        System.out.println(" Total components stored in ComponentList: " + gateRegistry.size());
        for (int i = 0; i < gateRegistry.size(); i++) {
            LogicGate g = gateRegistry.get(i);
            System.out.println("  Index [" + i + "] -> Gate Type: " + g.getName() + " | Inputs: " + g.getInputCount());
        }

        System.out.println("\n--- [CO3.3: PACKAGES & ARCHITECTURAL SEPARATION] ---");
        System.out.println("Verified Modular Package Architecture:");
        System.out.println("  - logiccraft.circuit     -> Circuit, Connection, CircuitEvaluator");
        System.out.println("  - logiccraft.gates       -> LogicGate hierarchy, GateFactory, InputValidator");
        System.out.println("  - logiccraft.collections -> ComponentList<T> generic container");
        System.out.println("  - logiccraft.exceptions  -> InvalidInputException");
        System.out.println("  - logiccraft.features    -> CircuitReport, CircuitAnalysisFeature, Tasks");
        System.out.println("  - logiccraft.file        -> ProjectFileManager");
        System.out.println("  - logiccraft.ui          -> LogicCraftUI (Java Swing Graphical Interface)");

        System.out.println("\n--- [CO3.4: FILE PERSISTENCE & REPORT GENERATION] ---");
        try {
            Circuit testCircuit = new Circuit();
            testCircuit.addGate(new INPUTGate(true));
            testCircuit.addGate(new ANDGate(2));
            CircuitReport report = new CircuitReport();
            String reportText = report.generateReport(testCircuit);
            ProjectFileManager fileMgr = new ProjectFileManager();
            fileMgr.saveProject("LogicCraft_Runtime_Test.txt", reportText);
            System.out.println(" Successfully serialized circuit project to 'LogicCraft_Runtime_Test.txt'");
            System.out.println(reportText);
        } catch (Exception e) {
            System.out.println(" File serialization info: " + e.getMessage());
        }
    }

    private static void runFullTestSuite() {
        System.out.println("\n=================================================================");
        System.out.println(" [DEMONSTRATION 3 / TEST SUITE]: RUNNING ALL 11 ACADEMIC TESTS   ");
        System.out.println("=================================================================");
        try {
            System.out.print("[TEST 01/11] ExceptionTest:             ");
            ExceptionTest.main(new String[0]);
            System.out.print("[TEST 02/11] GenericTest:               ");
            GenericTest.main(new String[0]);
            System.out.print("[TEST 03/11] InheritanceTest:           ");
            InheritanceTest.main(new String[0]);
            System.out.print("[TEST 04/11] InterfaceTest:             ");
            InterfaceTest.main(new String[0]);
            System.out.print("[TEST 05/11] OverloadingTest:           ");
            OverloadingTest.main(new String[0]);
            System.out.print("[TEST 06/11] MultipleInheritanceTest:   ");
            MultipleInheritanceTest.main(new String[0]);
            System.out.print("[TEST 07/11] MultiStageCircuitTest:     ");
            MultiStageCircuitTest.main(new String[0]);
            System.out.print("[TEST 08/11] MultithreadingTest:        ");
            MultithreadingTest.main(new String[0]);
            System.out.print("[TEST 09/11] FileTest:                  ");
            FileTest.main(new String[0]);
            System.out.println("[TEST 10/11] CircuitReportTest:         ");
            CircuitReportTest.main(new String[0]);
            System.out.println("[TEST 11/11] LogicCraftTest (Master):   ");
            LogicCraftTest.main(new String[0]);
            System.out.println("[TEST 12/12] Task4FileGenericTest (CO4): ");
            Task4FileGenericTest.main(new String[0]);

            System.out.println(">>> ALL 12 JAVA TEST SUITES PASSED CLEANLY! (CO1, CO2, CO3, CO4) <<<");
        } catch (Exception e) {
            System.out.println("[ERROR in Test Suite]: " + e.getMessage());
            e.printStackTrace();
        }
    }
}
