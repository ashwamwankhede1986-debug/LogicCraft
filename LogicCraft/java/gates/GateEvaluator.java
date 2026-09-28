package gates;

/**
 * Functional Interface for custom gate logic evaluation using Lambda expressions (CO2).
 */
@FunctionalInterface
public interface GateEvaluator {
    boolean compute(boolean[] inputs);
}
