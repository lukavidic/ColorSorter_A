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
