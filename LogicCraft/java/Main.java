import gates.*;
import circuit.Circuit;
import circuit.Connection;
import project.Project;
import reports.ReportGenerator;
import simulation.SimulationEngine;

import java.util.List;

/**
 * LogicCraft Review-2 Demonstration Entry Point for Java / OOP
 *
 * Subject: Problem Solving Using OOP — Java
 * Focus:
 *  - CO1: Java Fundamentals (Classes/objects, arrays, loops, conditionals, StringBuilder, encapsulation)
 *  - CO2: Advanced OOP (Abstract class, inheritance, method overriding, runtime polymorphism, static/final, lambdas)
 */
public class Main {

    public static void printBanner() {
        System.out.println();
        System.out.println("*****************************************************************");
        System.out.println("*     LOGICCRAFT — VISUAL LOGIC GATE SIMULATOR (REVIEW 2)       *");
        System.out.println("*             MODULE: PROBLEM SOLVING USING OOP (JAVA)          *");
        System.out.println("*****************************************************************");
        System.out.println("*  CO1: JAVA FUNDAMENTALS -> Classes, Objects, Arrays, Loops,   *");
        System.out.println("*                            Conditionals, StringBuilder Reports *");
        System.out.println("*  CO2: ADVANCED OOP      -> Abstract Class Hierarchy, Dynamic  *");
        System.out.println("*                            Polymorphism, static/final, Lambdas*");
        System.out.println("*****************************************************************\n");
    }

    public static void demonstrateCO1_Fundamentals() {
        System.out.println("=================================================================");
        System.out.println(" [DEMONSTRATION 1 / CO1]: JAVA FUNDAMENTALS & LOGIC EVALUATION   ");
        System.out.println("=================================================================");
        System.out.println("1. Instantiating concrete gates using Encapsulation and Arrays:");

        // Direct instantiation
        ANDGate andGate = new ANDGate("G10", "Manual_AND", 2);
        andGate.setInput(0, true);
        andGate.setInput(1, false);

        boolean andResult = andGate.evaluate();
        System.out.println(" -> Gate: " + andGate.getName() + " | Inputs: [1, 0] -> Output: " + (andResult ? "1" : "0"));

        ORGate orGate = new ORGate("G20", "Manual_OR", 2);
        orGate.setInput(0, true);
        orGate.setInput(1, false);
        boolean orResult = orGate.evaluate();
        System.out.println(" -> Gate: " + orGate.getName() + " | Inputs: [1, 0] -> Output: " + (orResult ? "1" : "0"));

        NOTGate notGate = new NOTGate("G30", "Manual_NOT");
        notGate.setInput(0, true);
        boolean notResult = notGate.evaluate();
        System.out.println(" -> Gate: " + notGate.getName() + " | Inputs: [1] -> Output: " + (notResult ? "1" : "0"));

        // Generate textual report using StringBuilder (CO1)
        System.out.println("\n2. Generating textual circuit report using StringBuilder:");
        Circuit testCircuit = new Circuit("CO1 Fundamentals Demo Circuit");
        testCircuit.addGate(andGate);
        testCircuit.addGate(orGate);
        testCircuit.addGate(notGate);

        String report = ReportGenerator.generateCircuitReport(testCircuit);
        System.out.println(report);
    }

    public static void demonstrateCO2_AdvancedOOP() {
        System.out.println("=================================================================");
        System.out.println(" [DEMONSTRATION 2 / CO2]: ADVANCED OOP & RUNTIME POLYMORPHISM    ");
        System.out.println("=================================================================");
        System.out.println("ACADEMIC VIVA CONCEPTS:");
        System.out.println(" - Polymorphism allows a generic 'LogicGate' reference to point to");
        System.out.println("   any subclass instance and invoke its overridden evaluate() method.");
        System.out.println("-----------------------------------------------------------------");

        // Runtime Polymorphism: Same base reference executes different overridden logic
        LogicGate polymorphicRef;
        boolean[] testInputs = { true, false };

        System.out.println("\n[Polymorphism Test 1: Binding reference to ANDGate]");
        polymorphicRef = new ANDGate("P1", "PolyAND", 2);
        System.out.println(" Type: " + polymorphicRef.getGateType() 
                         + " | Evaluating [1, 0] -> " + (polymorphicRef.evaluate(testInputs) ? "1" : "0"));

        System.out.println("\n[Polymorphism Test 2: Re-binding reference to ORGate]");
        polymorphicRef = new ORGate("P2", "PolyOR", 2);
        System.out.println(" Type: " + polymorphicRef.getGateType() 
                         + " | Evaluating [1, 0] -> " + (polymorphicRef.evaluate(testInputs) ? "1" : "0"));

        System.out.println("\n[Polymorphism Test 3: Re-binding reference to XORGate]");
        polymorphicRef = new XORGate("P3", "PolyXOR", 2);
        System.out.println(" Type: " + polymorphicRef.getGateType() 
                         + " | Evaluating [1, 0] -> " + (polymorphicRef.evaluate(testInputs) ? "1" : "0"));

        System.out.println("\n[Polymorphism Test 4: Re-binding reference to NANDGate]");
        polymorphicRef = new NANDGate("P4", "PolyNAND", 2);
        System.out.println(" Type: " + polymorphicRef.getGateType() 
                         + " | Evaluating [1, 0] -> " + (polymorphicRef.evaluate(testInputs) ? "1" : "0"));

        System.out.println("\n[Polymorphism Test 5: Re-binding reference to NOTGate]");
        polymorphicRef = new NOTGate("P5", "PolyNOT");
        System.out.println(" Type: " + polymorphicRef.getGateType() 
                         + " | Evaluating [1] -> " + (polymorphicRef.evaluate(new boolean[]{true}) ? "1" : "0"));

        // Demonstration of static and final keywords (CO2)
        System.out.println("\n-----------------------------------------------------------------");
        System.out.println("[CO2 Keyword Audit: static and final]");
        System.out.println(" -> LogicGate.MAX_INPUTS (static final constant)    : " + LogicGate.MAX_INPUTS);
        System.out.println(" -> LogicGate.getTotalGatesInstantiated() (static) : " + LogicGate.getTotalGatesInstantiated());
        System.out.println(" -> polymorphicRef.getId() (final method access)    : " + polymorphicRef.getId());

        // Demonstration of Functional Interface & Lambda Expressions (CO2)
        System.out.println("\n-----------------------------------------------------------------");
        System.out.println("[CO2 Functional Interface & Lambda Expression]");
        System.out.println("Implementing a custom 3-input Majority Vote gate using a Lambda expression:");

        GateEvaluator majorityVote = (inputs) -> {
            int highCount = 0;
            for (boolean in : inputs) {
                if (in) highCount++;
            }
            return highCount >= 2; // High if at least 2 out of 3 inputs are HIGH
        };

        boolean[] trio1 = { true, false, true };  // 2 out of 3 -> TRUE
        boolean[] trio2 = { false, true, false }; // 1 out of 3 -> FALSE

        System.out.println(" Lambda MajorityVote([1, 0, 1]) -> " + (majorityVote.compute(trio1) ? "1" : "0") + " (Expected: 1)");
        System.out.println(" Lambda MajorityVote([0, 1, 0]) -> " + (majorityVote.compute(trio2) ? "1" : "0") + " (Expected: 0)");
    }

    public static void demonstrateIntegratedReferenceCircuit() {
        System.out.println("\n=================================================================");
        System.out.println(" [DEMONSTRATION 3]: INTEGRATED 3-GATE REFERENCE CIRCUIT          ");
        System.out.println("=================================================================");
        System.out.println("Building the reference circuit matching shared/circuit_data.json:");
        System.out.println(" Gate G1: AND  (Inputs: [1, 0]) -> Output G1.out (0)");
        System.out.println(" Gate G2: OR   (Inputs: [G1.out, C=1]) -> Output G2.out (1)");
        System.out.println(" Gate G3: NOT  (Input:  [G2.out]) -> Output G3.out (0)");

        Project project = new Project("LogicCraft Review-2 System", "LogicCraft Academic Team");
        Circuit circuit = new Circuit("Review-2 Integrated Reference Circuit");

        LogicGate g1 = new ANDGate("G1", "Primary_AND_Gate", 2);
        g1.setInput(0, true);
        g1.setInput(1, false);

        LogicGate g2 = new ORGate("G2", "Secondary_OR_Gate", 2);
        g2.setInput(0, false);
        g2.setInput(1, true); // Pin C = 1

        LogicGate g3 = new NOTGate("G3", "Output_Inverter");
        g3.setInput(0, false);

        circuit.addGate(g1);
        circuit.addGate(g2);
        circuit.addGate(g3);

        // Connections
        circuit.addConnection(new Connection("G1", "G2", 0));
        circuit.addConnection(new Connection("G2", "G3", 0));

        project.setActiveCircuit(circuit);

        SimulationEngine sim = new SimulationEngine(circuit);
        sim.runSimulation();

        // Display formatted report
        String circuitReport = ReportGenerator.generateCircuitReport(circuit);
        System.out.println(circuitReport);

        // Demonstrate Lambda filtering on circuit (CO2)
        System.out.println("Filtering Circuit Gates with Output == HIGH using Lambda predicate:");
        List<LogicGate> highGates = circuit.filterGates(g -> g.getOutput() == true);
        for (LogicGate hg : highGates) {
            System.out.println("  -> Active HIGH Gate: " + hg.getId() + " (" + hg.getGateType() + ")");
        }
    }

    public static void main(String[] args) {
        printBanner();
        demonstrateCO1_Fundamentals();
        demonstrateCO2_AdvancedOOP();
        demonstrateIntegratedReferenceCircuit();

        System.out.println("\n*****************************************************************");
        System.out.println("*  JAVA / OOP REVIEW-2 DEMONSTRATION COMPLETE!                  *");
        System.out.println("*****************************************************************\n");
    }
}
