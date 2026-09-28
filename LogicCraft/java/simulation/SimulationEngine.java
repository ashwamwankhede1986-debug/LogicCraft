package simulation;

import circuit.Circuit;
import gates.LogicGate;

/**
 * SimulationEngine orchestrates step-by-step logic propagation and verification.
 */
public class SimulationEngine {
    private Circuit circuit;

    public SimulationEngine(Circuit circuit) {
        this.circuit = circuit;
    }

    public void runSimulation() {
        if (circuit != null) {
            circuit.evaluateCircuit();
        }
    }

    public void setCircuit(Circuit circuit) {
        this.circuit = circuit;
    }

    public Circuit getCircuit() {
        return circuit;
    }
}
