
package logiccraft.gates;

import logiccraft.exceptions.InvalidInputException;

public class GateFactory {

    // Compile-time polymorphism: Method 1
    // Creates a gate with 2 inputs by default
    public static LogicGate createGate(String gateType)
            throws InvalidInputException {

        return createGate(gateType, 2);
    }

    // Method 2: Creates a gate with a specified input count
    public static LogicGate createGate(
            String gateType,
            int inputCount) throws InvalidInputException {

        if (inputCount < 1) {
            throw new InvalidInputException(
                    "Input count must be at least 1"
            );
        }

        switch (gateType.toUpperCase()) {

            case "AND":
                return new ANDGate(inputCount);

            case "OR":
                return new ORGate(inputCount);

            case "NOT":
                if (inputCount != 1) {
                    throw new InvalidInputException(
                            "NOT gate requires exactly 1 input"
                    );
                }
                return new NOTGate();

            case "NAND":
                return new NANDGate(inputCount);

            case "NOR":
                return new NORGate(inputCount);

            case "XOR":
                return new XORGate(inputCount);

            case "XNOR":
                return new XNORGate(inputCount);

            default:
                throw new InvalidInputException(
                        "Unknown gate type: " + gateType
                );
        }
    }
}
