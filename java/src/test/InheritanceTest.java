
package test;

import logiccraft.gates.ANDGate;
import logiccraft.gates.MultiInputGate;
import logiccraft.gates.LogicGate;

public class InheritanceTest {

    public static void main(String[] args) {

        ANDGate andGate = new ANDGate(2);

        System.out.println(
                "Gate: " + andGate.getName()
        );

        System.out.println(
                "Inputs: " + andGate.getInputCount()
        );

        System.out.println(
                "Result: "
                + andGate.calculate(
                    new boolean[]{true, true}
                )
        );

        System.out.println(
                "Multilevel inheritance: "
                + (andGate instanceof MultiInputGate
                && andGate instanceof LogicGate)
        );
    }
}
