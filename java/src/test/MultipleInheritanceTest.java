
package test;

import logiccraft.gates.ANDGate;
import logiccraft.gates.GateInfo;
import logiccraft.gates.InputValidator;

public class MultipleInheritanceTest {

    public static void main(String[] args) {

        ANDGate gate = new ANDGate(2);

        GateInfo info = gate;
        InputValidator validator = gate;

        System.out.println(info.getGateDescription());

        System.out.println(
                "Valid input count: "
                + validator.isValidInputCount(2)
        );

        System.out.println(
                "Invalid input count: "
                + validator.isValidInputCount(0)
        );
    }
}
