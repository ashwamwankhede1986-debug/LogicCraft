package test;

import logiccraft.circuit.Circuit;
import logiccraft.circuit.Connection;
import logiccraft.gates.ANDGate;
import logiccraft.gates.INPUTGate;
import logiccraft.gates.OUTPUTGate;
import logiccraft.utils.GateOperation;

public class LogicCraftTest {

    public static void main(String[] args) {

        // -------------------------------
        // INPUT GATES
        // -------------------------------

        INPUTGate inputA = new INPUTGate(true);
        INPUTGate inputB = new INPUTGate(true);
        INPUTGate inputC = new INPUTGate(true);


        // -------------------------------
        // AND GATE
        // -------------------------------

        ANDGate andGate = new ANDGate(3);


        // -------------------------------
        // OUTPUT GATE
        // -------------------------------

        OUTPUTGate output = new OUTPUTGate();


        // -------------------------------
        // CREATE CIRCUIT
        // -------------------------------

        Circuit circuit = new Circuit();

        circuit.addGate(inputA);
        circuit.addGate(inputB);
        circuit.addGate(inputC);
        circuit.addGate(andGate);
        circuit.addGate(output);


        // -------------------------------
        // CREATE CONNECTIONS
        // -------------------------------

        Connection connectionA =
                new Connection(inputA, andGate, 0);

        Connection connectionB =
                new Connection(inputB, andGate, 1);

        Connection connectionC =
                new Connection(inputC, andGate, 2);


        // -------------------------------
        // ADD CONNECTIONS TO CIRCUIT
        // -------------------------------

        circuit.addConnection(connectionA);
        circuit.addConnection(connectionB);
        circuit.addConnection(connectionC);


        // -------------------------------
        // DISPLAY CIRCUIT
        // -------------------------------

        System.out.println("Gates in circuit:");

        circuit.showGates();

        System.out.println();

        System.out.println("Connections in circuit:");

        circuit.showConnections();

        System.out.println();


        // -------------------------------
        // EVALUATE CIRCUIT
        // -------------------------------

        boolean result = circuit.evaluate();


        // -------------------------------
        // SEND RESULT TO OUTPUT
        // -------------------------------

        output.calculate(
                new boolean[]{result}
        );


        // -------------------------------
        // DISPLAY INPUT VALUES
        // -------------------------------

        System.out.println(
                "Input A: "
                + inputA.calculate(new boolean[0])
        );

        System.out.println(
                "Input B: "
                + inputB.calculate(new boolean[0])
        );

        System.out.println(
                "Input C: "
                + inputC.calculate(new boolean[0])
        );


        // -------------------------------
        // DISPLAY RESULTS
        // -------------------------------

        System.out.println(
                "AND Output: "
                + result
        );

        System.out.println(
                "Final Output: "
                + output.getValue()
        );


        // -------------------------------
        // LAMBDA EXPRESSION
        // -------------------------------

        GateOperation andOperation =
                (a, b) -> a && b;

        boolean lambdaResult =
                andOperation.operate(true, true);

        System.out.println();

        System.out.println(
                "Lambda AND Result: "
                + lambdaResult
        );
    }
}