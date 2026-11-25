package test;

import logiccraft.collections.ComponentList;
import logiccraft.gates.ANDGate;
import logiccraft.gates.ORGate;
import logiccraft.gates.LogicGate;

public class GenericTest {

    public static void main(String[] args) {

        ComponentList<LogicGate> gates =
                new ComponentList<>();

        ANDGate andGate = new ANDGate(2);
        ORGate orGate = new ORGate(2);

        gates.add(andGate);
        gates.add(orGate);

        System.out.println(
                "Number of gates: " + gates.size()
        );

        System.out.println(
                "First gate: "
                + gates.get(0).getName()
        );

        System.out.println(
                "Second gate: "
                + gates.get(1).getName()
        );
    }
}