
package test;

import logiccraft.circuit.Circuit;
import logiccraft.circuit.Connection;
import logiccraft.gates.ANDGate;
import logiccraft.gates.INPUTGate;
import logiccraft.gates.ORGate;
import logiccraft.gates.OUTPUTGate;

public class MultiStageCircuitTest {

    public static void main(String[] args) {

        Circuit circuit = new Circuit();

        // Create input gates.
        INPUTGate inputA = new INPUTGate(true);
        INPUTGate inputB = new INPUTGate(true);
        INPUTGate inputC = new INPUTGate(false);

        // Create logic gates.
        ANDGate andGate = new ANDGate(2);
        ORGate orGate = new ORGate(2);
        OUTPUTGate outputGate = new OUTPUTGate();

        // Add gates to the circuit.
        circuit.addGate(inputA);
        circuit.addGate(inputB);
        circuit.addGate(inputC);
        circuit.addGate(andGate);
        circuit.addGate(orGate);
        circuit.addGate(outputGate);

        // Connect A and B to the AND gate.
        circuit.addConnection(
                new Connection(inputA, andGate, 0));
        circuit.addConnection(
                new Connection(inputB, andGate, 1));

        // Connect AND output and C to the OR gate.
        circuit.addConnection(
                new Connection(andGate, orGate, 0));
        circuit.addConnection(
                new Connection(inputC, orGate, 1));

        // Connect OR output to the OUTPUT gate.
        circuit.addConnection(
                new Connection(orGate, outputGate, 0));

        // Evaluate the complete circuit.
        boolean result = circuit.evaluate();

        System.out.println("Multi-stage circuit result: " + result);
        System.out.println("Expected result: true");

        if (result == true) {
            System.out.println("TEST PASSED");
        } else {
            System.out.println("TEST FAILED");
        }
    }
}
