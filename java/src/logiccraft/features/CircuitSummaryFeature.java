
package logiccraft.features;

public class CircuitSummaryFeature extends CircuitAnalysisFeature {

    public CircuitSummaryFeature(String featureName) {
        super(featureName);
    }

    public String createSummary() {
        return analyze() + "\nSummary generated successfully.";
    }
}
