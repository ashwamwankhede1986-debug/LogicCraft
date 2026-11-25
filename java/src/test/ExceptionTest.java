package test;

import logiccraft.gates.GateFactory;
import logiccraft.gates.LogicGate;
import logiccraft.exceptions.InvalidInputException;

public class ExceptionTest {

    public static void main(String[] args) {

        try {

            LogicGate gate =
                    GateFactory.createGate("AND", 0);

            System.out.println(
                    "Gate created: " + gate.getName()
            );

        }
        catch (InvalidInputException e) {

            System.out.println(
                    "Error: " + e.getMessage()
            );
        }
    }
}