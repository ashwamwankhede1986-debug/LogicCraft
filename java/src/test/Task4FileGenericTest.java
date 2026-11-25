package test;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;

import logiccraft.circuit.Circuit;
import logiccraft.circuit.Connection;
import logiccraft.collections.ComponentList;
import logiccraft.exceptions.InvalidInputException;
import logiccraft.file.ProjectFileManager;
import logiccraft.gates.ANDGate;
import logiccraft.gates.INPUTGate;
import logiccraft.gates.LogicGate;
import logiccraft.gates.NOTGate;
import logiccraft.gates.ORGate;
import logiccraft.gates.OUTPUTGate;

/**
 * Task 4 Unit Tests for Java (CO4: File Handling & Java Generics)
 */
public class Task4FileGenericTest {

    public static void main(String[] args) {
        System.out.println("======================================================");
        System.out.println("   JAVA TASK 4 TEST SUITE (CO4 FILE HANDLING & GENERICS)");
        System.out.println("======================================================");

        testGenericComponentList();
        testFileSaveAndLoad();
        testCorruptedAndMissingFileHandling();

        System.out.println("======================================================");
        System.out.println(" ALL JAVA TASK 4 TESTS PASSED SUCCESSFULLY! (CO4)     ");
        System.out.println("======================================================");
    }

    public static void testGenericComponentList() {
        System.out.print("[TEST 1/3] Java Generics ComponentList<T> Operations: ");

        ComponentList<LogicGate> list = new ComponentList<>();
        ANDGate and1 = new ANDGate(2);
        ORGate or1 = new ORGate(2);
        NOTGate not1 = new NOTGate();

        list.add(and1);
        list.add(or1);
        list.add(not1);

        if (list.size() != 3) throw new AssertionError("Expected size 3");
        if (!list.contains(or1)) throw new AssertionError("Expected list to contain ORGate");

        // Test Generic Predicate Filtering
        ComponentList<LogicGate> twoInputGates = list.filter(g -> g.getInputCount() == 2);
        if (twoInputGates.size() != 2) throw new AssertionError("Expected 2 two-input gates");

        // Test Generic Removal
        list.remove(1); // removes or1
        if (list.size() != 2) throw new AssertionError("Expected size 2 after removal");
        if (list.get(1) != not1) throw new AssertionError("Expected not1 shifted to index 1");

        System.out.println("PASS!");
    }

    public static void testFileSaveAndLoad() {
        System.out.print("[TEST 2/3] Circuit File Persistence (Save & Load):    ");

        try {
            Circuit original = new Circuit();
            INPUTGate inA = new INPUTGate(true);
            INPUTGate inB = new INPUTGate(false);
            ANDGate andG = new ANDGate(2);
            OUTPUTGate outG = new OUTPUTGate();

            original.addGate(inA);
            original.addGate(inB);
            original.addGate(andG);
            original.addGate(outG);

            original.addConnection(new Connection(inA, andG, 0));
            original.addConnection(new Connection(inB, andG, 1));
            original.addConnection(new Connection(andG, outG, 0));

            File tempFile = new File("Test_Circuit_Export.logiccraft");
            ProjectFileManager mgr = new ProjectFileManager();
            mgr.saveCircuitToFile(tempFile, original);

            if (!tempFile.exists() || tempFile.length() == 0) {
                throw new AssertionError("Saved file does not exist or is empty");
            }

            // Load back into fresh circuit
            Circuit restored = new Circuit();
            mgr.loadCircuitFromFile(tempFile, restored);

            if (restored.getGateCount() != 4) {
                throw new AssertionError("Expected 4 restored gates, got " + restored.getGateCount());
            }
            if (restored.getConnectionCount() != 3) {
                throw new AssertionError("Expected 3 restored connections, got " + restored.getConnectionCount());
            }

            // Clean up temporary test file
            tempFile.delete();
            System.out.println("PASS!");

        } catch (Exception e) {
            throw new RuntimeException("Save/Load test failed: " + e.getMessage(), e);
        }
    }

    public static void testCorruptedAndMissingFileHandling() {
        System.out.print("[TEST 3/3] Missing and Corrupted File Exception Safety: ");

        ProjectFileManager mgr = new ProjectFileManager();
        Circuit circuit = new Circuit();

        // 1. Missing File -> FileNotFoundException
        boolean caughtMissing = false;
        try {
            mgr.loadCircuitFromFile(new File("NonExistent_File_12345.logiccraft"), circuit);
        } catch (FileNotFoundException e) {
            caughtMissing = true;
        } catch (Exception e) {
            throw new AssertionError("Unexpected exception type: " + e.getClass().getName());
        }
        if (!caughtMissing) throw new AssertionError("Did not catch expected FileNotFoundException");

        // 2. Corrupted File -> IOException
        boolean caughtCorrupted = false;
        File corruptFile = new File("Corrupt_Test_Circuit.logiccraft");
        try {
            try (FileWriter fw = new FileWriter(corruptFile)) {
                fw.write("GATE|0|MALFORMED_LINE_WITHOUT_ENOUGH_TOKENS\n");
            }
            mgr.loadCircuitFromFile(corruptFile, circuit);
        } catch (IOException e) {
            caughtCorrupted = true;
        } catch (Exception e) {
            throw new AssertionError("Unexpected exception type for corrupted file: " + e.getClass().getName());
        } finally {
            if (corruptFile.exists()) corruptFile.delete();
        }
        if (!caughtCorrupted) throw new AssertionError("Did not catch expected IOException for corrupted file");

        System.out.println("PASS!");
    }
}
