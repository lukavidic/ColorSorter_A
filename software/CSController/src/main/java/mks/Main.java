/**
 * @file Main.java
 * @brief Entry point of the JavaFX desktop application for controlling microcontroller.
 *
 * This class serves as the main entry point for the JavaFX application.
 * It loads the FXML layout file, initializes the primary window (stage)
 * and sets up the graphical user interface.
 * The `start` method uses the FXMLLoader to
 * parse the `color_gui.fxml` file and obtain an instance of the `ColorGUI` controller, which handles all UI logic.
 * 
 * The static reference to the controller (`GuiInstance`) allows other parts of the application to access the UI logic if needed.
 */

package mks;

import javafx.application.*;
import javafx.fxml.*;
import javafx.scene.*;
import javafx.stage.*;
import java.io.*;
import gui.*;


public class Main extends Application{
	public static ColorGUI GuiInstance;

	public static void main(String[] args) {
		launch(args);
	}

	/**
	 * The `start` method uses the FXMLLoader to
	 * parse the `color_gui.fxml` file and obtain an
	 * instance of the `ColorGUI` controller,
	 * which handles all UI logic.
	 */
	@Override
	public void start(Stage primaryStage) throws Exception {
		String pathToFXML = "src" + File.separator + "main" + File.separator + "java" + File.separator + "gui" + File.separator + "color_gui.fxml";
		FXMLLoader loader = new FXMLLoader(new File(pathToFXML).toURI().toURL());
		Parent root = loader.load();

		Scene scene = new Scene(root);
		primaryStage.setScene(scene);
		primaryStage.setTitle("ColorSorterController");
		primaryStage.show();

		GuiInstance = loader.getController();
	}
}
