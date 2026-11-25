
package test;

import logiccraft.features.CircuitUtility;

public class InterfaceTest {
    public static void main(String[] args) {
        CircuitUtility utility =
                new CircuitUtility("LogicCraft Main Circuit");

        System.out.println(utility.printDetails());
        System.out.println("Circuit Valid: " + utility.validate());
    }
}
