/**
 * @file ColorGUI.java
 * @brief Controller class for the JavaFX desktop application used to interact with microcontroller
 *
 * This class contains the core logic of the JavaFX-based desktop application.
 * It defines and manages all UI components described in the FXML layout and
 * handles user interactions, input validation, and communication with the microcontroller
 * over a WiFi connection. The class includes event handlers, updates to the UI based on 
 * system state, and the sending/receiving of messages to/from the embedded system.
 * It acts as the main bridge between the graphical interface and the backend system logic.
 *
 * The application entry point and basic setup are handled separately in the Main.java file.
 */

package gui;

import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.layout.AnchorPane;
import javafx.scene.input.MouseEvent;
import java.util.ArrayList;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Objects;
import java.util.Optional;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class ColorGUI {

	/* server stuff */
    public Socket clientSocket;
    public PrintWriter out;
    public BufferedReader in;
    public ExecutorService executorService;

	/* flags */
    private volatile boolean appRunning = false;
    private volatile boolean resetActivated = false;
    private volatile boolean startActivated = false;
    private volatile boolean applyActivated = false;
    private volatile boolean serverRunning = true;

	/* ESP8266EX address on local network: 10.99.146.44 */
	private static final String espAddress = "10.99.146.44";
	private static final int espPort = 8084;
    @FXML
    private Label BlackLabel;

    @FXML
    private Label BlueLabel;

    @FXML
    private Button ButtonLeft;

    @FXML
    private Button ButtonLeftAll;

    @FXML
    private Button ButtonRight;

    @FXML
    private Button ButtonRightAll;

    @FXML
    private Button ButtonStart;

    @FXML
    private Button ButtonStop;

    @FXML
    private Label GreenLabel;

    @FXML
    private AnchorPane PaneLeft;

    @FXML
    private AnchorPane PaneRight;

    @FXML
    private Label RedLabel;

    @FXML
    private Label OrangeLabel;

    @FXML
    private Label PinkLabel;

    @FXML
    private Label WhiteLabel;

    @FXML
    private Label YellowLabel;

	public ArrayList<Label> listLeft;
	public ArrayList<Label> listRight;

	public boolean positions[];

	/**
	 * Starts server that will communicate with microcontroller
	 */
	public void startServer(){
        executorService = Executors.newSingleThreadExecutor();
        executorService.execute(() -> {
            try (ServerSocket serverSocket = new ServerSocket(espPort)) {
                System.out.println("Server started on port " + espPort);

                while (serverRunning) {
                    try {
                        clientSocket = serverSocket.accept();
                        System.out.println("Client connected: " + clientSocket.getInetAddress());

                        // Set up communication streams
                        out = new PrintWriter(clientSocket.getOutputStream(), true);
                        in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));

                        // Listen for incoming messages
                        String inputLine;
                        while ((inputLine = in.readLine()) != null && serverRunning) {
                            System.out.println("Received: " + inputLine);
                            if(!inputLine.startsWith("AT+")) {
                                // Process each character in the inputLine
                                for (int i = 0; i < inputLine.length(); i++) {
                                    char c = inputLine.charAt(i);
                                    // Send or process each character
                                    processIncomingMessage(String.valueOf(c));
                                }
                            }
                        }

                        clearInputBuffer();

                    } catch (IOException e) {
                        System.err.println("Connection error: " + e.getMessage());
                    } finally {
                        cleanupConnection();
                    }
                }
            } catch (IOException e) {
                System.err.println("Server error: " + e.getMessage());
            }
        });
    }

	int xaxis = -1, yinc = 28, currentLeft, currentRight;

	/**
	 * Function that processes received message from microcontroller
	 */
    private void processIncomingMessage(String message) {
        // Filter out AT commands and empty messages
        if (message.isEmpty() || message.startsWith("AT+")) {
            return;
        }
        if(message.trim().equals("S")) // Microcontroller sent START, so button has been pressed
        {
            serverRunning = true;
            appRunning = true;
            startActivated = true;
            resetActivated = false;
        } else if (message.trim().equals("R")) { // Microcontroller sent RESET, so button has been pressed again
            serverRunning = true;
            appRunning = false;
            startActivated = false;
            resetActivated = true;
        }
        else {
            // Update GUI based on received color code if none of above is true
			System.out.println("TODO");
        }
    }

	/**
	 * Cleans interrupt buffer
	 */
    private void clearInputBuffer() throws IOException {
        char[] buffer = new char[1024];
        while (in.ready()) {
            int bytesRead = in.read(buffer, 0, buffer.length);
            if (bytesRead == -1) break;
        }
    }

	/**
	 * Closes connection when communication is finished
	 */
    private void cleanupConnection() {
        try {
            if (out != null) out.close();
            if (in != null) in.close();
            if (clientSocket != null && !clientSocket.isClosed()) {
                clientSocket.close();
            }
        } catch (IOException e) {
            System.err.println("Error closing connection: " + e.getMessage());
        }
    }


	/**
	 * @brief Function called first time program is started
	 *
	 * Styles gui, sets up events for clicking on each color
	 * Also starts server using startServer() function
	 */
	@FXML 
	public void initialize() {
		PaneRight.getChildren().clear();
		PaneLeft.getChildren().clear();

		RedLabel = new Label();
		RedLabel.setStyle("-fx-background-color: red; -fx-border-color: black");
		RedLabel.setPrefWidth(38);
		RedLabel.setPrefHeight(28);
		RedLabel.setOnMouseClicked(e -> {
			positions[0] = !positions[0];
			sendData("000\r\n");
			refreshColors();
		});

		GreenLabel = new Label();
		GreenLabel.setStyle("-fx-background-color: green;-fx-border-color: black");
		GreenLabel.setPrefWidth(38);
		GreenLabel.setPrefHeight(28);
		GreenLabel.setOnMouseClicked(e -> {
			positions[1] = !positions[1];
			sendData("111\r\n");
			refreshColors();
		});

		BlueLabel = new Label();
		BlueLabel.setStyle("-fx-background-color: blue;-fx-border-color: black");
		BlueLabel.setPrefWidth(38);
		BlueLabel.setPrefHeight(28);
		BlueLabel.setOnMouseClicked(e -> {
			positions[2] = !positions[2];
			sendData("222\r\n");
			refreshColors();
		});

		YellowLabel = new Label();
		YellowLabel.setStyle("-fx-background-color: yellow;-fx-border-color: black");
		YellowLabel.setPrefWidth(38);
		YellowLabel.setPrefHeight(28);
		YellowLabel.setOnMouseClicked(e -> {
			positions[3] = !positions[3];
			sendData("333\r\n");
			refreshColors();
		});

		OrangeLabel = new Label();
		OrangeLabel.setStyle("-fx-background-color: orange;-fx-border-color: black");
		OrangeLabel.setPrefWidth(38);
		OrangeLabel.setPrefHeight(28);
		OrangeLabel.setOnMouseClicked(e -> {
			positions[4] = !positions[4];
			sendData("444\r\n");
			refreshColors();
		});

		PinkLabel = new Label();
		PinkLabel.setStyle("-fx-background-color: pink;-fx-border-color: black");
		PinkLabel.setPrefWidth(38);
		PinkLabel.setPrefHeight(28);
		PinkLabel.setOnMouseClicked(e -> {
			positions[5] = !positions[5];
			sendData("555\r\n");
			refreshColors();
		});

		WhiteLabel = new Label();
		WhiteLabel.setStyle("-fx-background-color: white;-fx-border-color: black");
		WhiteLabel.setPrefWidth(38);
		WhiteLabel.setPrefHeight(28);
		WhiteLabel.setOnMouseClicked(e -> {
			positions[6] = !positions[6];
			sendData("666\r\n");
			refreshColors();
		});

		BlackLabel = new Label();
		BlackLabel.setStyle("-fx-background-color: black;-fx-border-color: black");
		BlackLabel.setPrefWidth(38);
		BlackLabel.setPrefHeight(28);
		BlackLabel.setOnMouseClicked(e -> {
			positions[7] = !positions[7];
			sendData("777\r\n");
			refreshColors();
		});

		listLeft = new ArrayList<>();
		listRight = new ArrayList<>();

		positions = new boolean[8];
		positions[4] = true;
		positions[5] = true;
		positions[6] = true;
		positions[7] = true;

		refreshColors();

       	new Thread(() -> {
            try {
                Thread.sleep(500);
				startServer();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }).start();
	}

	/**
	 * Redraws colors after a color has been clicked or
	 * when all colors have been shifted to right or left
	 */
	void refreshColors() {
		int i=0;
		listLeft.clear();
		listRight.clear();
		if(positions[0] == false)
			listLeft.add(RedLabel);
		else listRight.add(RedLabel);
		if(positions[1] == false)
			listLeft.add(GreenLabel);
		else listRight.add(GreenLabel);
		if(positions[2] == false)
			listLeft.add(BlueLabel);
		else listRight.add(BlueLabel);
		if(positions[3] == false)
			listLeft.add(YellowLabel);
		else listRight.add(YellowLabel);
		if(positions[4] == false)
			listLeft.add(OrangeLabel);
		else listRight.add(OrangeLabel);
		if(positions[5] == false)
			listLeft.add(PinkLabel);
		else listRight.add(PinkLabel);
		if(positions[6] == false)
			listLeft.add(WhiteLabel);
		else listRight.add(WhiteLabel);
		if(positions[7] == false)
			listLeft.add(BlackLabel);
		else listRight.add(BlackLabel);

		PaneLeft.getChildren().clear();
		for(Label l : listLeft) {
			l.setLayoutX(xaxis);
			l.setLayoutY(yinc*i);
			i++;
			PaneLeft.getChildren().add(l);			
		}
		i=0;
		PaneRight.getChildren().clear();
		for(Label l : listRight) {
			l.setLayoutX(xaxis);
			l.setLayoutY(yinc*i);
			i++;
			PaneRight.getChildren().add(l);			
		}
	}

	/**
	 * Function used to send data to microcontroller
	 *
	 * @param[in] data we want to send
	 */
    public void sendData(String data) {
        if (out != null && !clientSocket.isClosed()) {
            out.println(data);
            System.out.println("Sent to ESP8266: " + data);
        } else {
            System.err.println("Cannot send data - no active connection");
        }
    }

	/**
	 * Handler for LeftAll button
	 */
	@FXML
    void LeftAllClicked(MouseEvent event) {
		for(int i=0;i<8;i++)
			positions[i] = false;
		refreshColors();
		sendData("LA\r\n");
    }

	/**
	 * Handler for RightAll button
	 */
    @FXML
    void RightAllClicked(MouseEvent event) {
		for(int i=0;i<8;i++)
			positions[i] = true;
		refreshColors();
		sendData("RA\r\n");
    }

	/**
	 * Sends start command to microcontroller
	 */
	@FXML
    void StartClicked(MouseEvent event) {
		ButtonStart.setStyle("-fx-background-color: green");
		ButtonStop.setStyle("-fx-background-color: ");
		sendData("START\r\n");
    }

	/**
	 * Sends stop command to microcontroller
	 */
    @FXML
    void StopClicked(MouseEvent event) {
		ButtonStart.setStyle("-fx-background-color: ");
		ButtonStop.setStyle("-fx-background-color: red");
		sendData("STOP\r\n");
    }
}
