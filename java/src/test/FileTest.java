package test;

import java.io.IOException;

import logiccraft.file.ProjectFileManager;

public class FileTest {

    public static void main(String[] args) {

        ProjectFileManager manager =
                new ProjectFileManager();

        String projectData =
                "Project Name: LogicCraft\n"
                + "Gate: AND\n"
                + "Inputs: 3\n";

        try {

            manager.saveProject(
                    "LogicCraftProject.txt",
                    projectData
            );

            System.out.println(
                    "Project saved successfully."
            );

            System.out.println();
            System.out.println("Loading project...");
            System.out.println();

            String loadedData =
                    manager.loadProject(
                            "LogicCraftProject.txt"
                    );

            System.out.println(
                    "Saved Project Data:"
            );

            System.out.println(loadedData);

        }
        catch (IOException e) {

            System.out.println(
                    "File error: "
                    + e.getMessage()
            );
        }
    }
}