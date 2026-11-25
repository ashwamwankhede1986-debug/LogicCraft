
package test;

import logiccraft.gates.GateFactory;
import logiccraft.gates.LogicGate;
import logiccraft.exceptions.InvalidInputException;

public class OverloadingTest {

    public static void main(String[] args)
            throws InvalidInputException {

        // Calls the one-parameter method
        LogicGate gate1 =
                GateFactory.createGate("AND");

        // Calls the two-parameter method
        LogicGate gate2 =
                GateFactory.createGate("AND", 4);

        System.out.println(
                gate1.getName() + " inputs: "
                + gate1.getInputCount()
        );

        System.out.println(
                gate2.getName() + " inputs: "
                + gate2.getInputCount()
        );
    }
}
