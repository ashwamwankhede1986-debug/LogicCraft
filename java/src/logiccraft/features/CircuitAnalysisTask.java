
package logiccraft.features;

public class CircuitAnalysisTask implements Runnable {

    private final String taskName;

    public CircuitAnalysisTask(String taskName) {
        this.taskName = taskName;
    }

    @Override
    public void run() {
        System.out.println(
            "Analysis started: " + taskName
        );

        // Simulate a small analysis task
        for (int i = 1; i <= 3; i++) {
            System.out.println(
                taskName + " - Step " + i
            );
        }

        System.out.println(
            "Analysis completed: " + taskName
        );
    }
}
