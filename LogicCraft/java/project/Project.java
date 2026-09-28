package project;

import circuit.Circuit;

/**
 * Encapsulates a LogicCraft Project workspace and circuit domain (CO1 Encapsulation).
 */
public class Project {
    private String projectName;
    private String author;
    private final String version;
    private Circuit activeCircuit;

    public Project(String projectName, String author) {
        this.projectName = projectName;
        this.author = author;
        this.version = "Review-2 (v1.0)";
    }

    public String getProjectName() {
        return projectName;
    }

    public void setProjectName(String projectName) {
        this.projectName = projectName;
    }

    public String getAuthor() {
        return author;
    }

    public void setAuthor(String author) {
        this.author = author;
    }

    public String getVersion() {
        return version;
    }

    public Circuit getActiveCircuit() {
        return activeCircuit;
    }

    public void setActiveCircuit(Circuit activeCircuit) {
        this.activeCircuit = activeCircuit;
    }
}
