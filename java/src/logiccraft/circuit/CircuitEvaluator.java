package logiccraft.circuit;

import logiccraft.gates.INPUTGate;
import logiccraft.gates.LogicGate;

public class CircuitEvaluator {

    public boolean evaluateInput(INPUTGate inputGate) {

        return inputGate.calculate(new boolean[0]);
    }

    public boolean evaluateGate(
            LogicGate gate,
            boolean[] inputs) {

        return gate.calculate(inputs);
    }

    public boolean evaluateConnection(
            Connection connection,
            boolean[] destinationInputs) {

        LogicGate sourceGate =
                connection.getSourceGate();

        LogicGate destinationGate =
                connection.getDestinationGate();

        boolean sourceOutput;

        if (sourceGate instanceof INPUTGate) {

            sourceOutput =
                    sourceGate.calculate(new boolean[0]);

        } else {

            sourceOutput =
                    sourceGate.calculate(destinationInputs);
        }

        destinationInputs[
                connection.getDestinationInput()
        ] = sourceOutput;

        return destinationGate.calculate(
                destinationInputs
        );
    }
}