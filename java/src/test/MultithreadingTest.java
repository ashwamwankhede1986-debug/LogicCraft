
package test;

import logiccraft.features.CircuitAnalysisTask;

public class MultithreadingTest {

    public static void main(String[] args) {

        CircuitAnalysisTask task =
            new CircuitAnalysisTask("Circuit Analysis");

        Thread analysisThread = new Thread(task);

        System.out.println("Main program started.");

        analysisThread.start();

        System.out.println(
            "Main program can continue working."
        );

        try {
            analysisThread.join();
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            System.out.println(
                "Analysis was interrupted."
            );
        }

        System.out.println("Main program completed.");
    }
}
