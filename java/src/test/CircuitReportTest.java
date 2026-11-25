package test;

import logiccraft.circuit.Circuit;
import logiccraft.features.CircuitReport;

public class CircuitReportTest {

    public static void main(String[] args) {

        Circuit circuit = new Circuit();
        CircuitReport report = new CircuitReport();

        // Call the first overloaded method
        System.out.println(report.generateReport(circuit));

        System.out.println();

        // Call the second overloaded method
        System.out.println(
                report.generateReport(circuit, "My Custom Report")
        );
    }
}
