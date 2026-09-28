package gates;

/**
 * Concrete NOT Gate (Inverter) implementation demonstrating Inheritance and Overriding (CO2).
 */
public class NOTGate extends LogicGate {

    public NOTGate() {
        this("G_NOT", "Default_NOT");
    }

    public NOTGate(String id, String name) {
        super(id, name, 1);
        evaluate();
    }

    @Override
    public boolean evaluate() {
        output = !inputs[0];
        return output;
    }

    @Override
    public boolean evaluate(boolean[] inputValues) {
        setInputs(inputValues);
        return evaluate();
    }

    @Override
    public String getGateType() {
        return "NOT";
    }
}
