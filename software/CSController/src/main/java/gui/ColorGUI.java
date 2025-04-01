package gui;

import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.layout.AnchorPane;
import javafx.scene.input.MouseEvent;
import java.util.ArrayList;

public class ColorGUI {

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

	int xaxis = -1, yinc = 28, currentLeft, currentRight;

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
			refreshColors();
		});

		GreenLabel = new Label();
		GreenLabel.setStyle("-fx-background-color: green;-fx-border-color: black");
		GreenLabel.setPrefWidth(38);
		GreenLabel.setPrefHeight(28);
		GreenLabel.setOnMouseClicked(e -> {
			positions[1] = !positions[1];
			refreshColors();
		});

		BlueLabel = new Label();
		BlueLabel.setStyle("-fx-background-color: blue;-fx-border-color: black");
		BlueLabel.setPrefWidth(38);
		BlueLabel.setPrefHeight(28);
		BlueLabel.setOnMouseClicked(e -> {
			positions[2] = !positions[2];
			refreshColors();
		});

		YellowLabel = new Label();
		YellowLabel.setStyle("-fx-background-color: yellow;-fx-border-color: black");
		YellowLabel.setPrefWidth(38);
		YellowLabel.setPrefHeight(28);
		YellowLabel.setOnMouseClicked(e -> {
			positions[3] = !positions[3];
			refreshColors();
		});

		OrangeLabel = new Label();
		OrangeLabel.setStyle("-fx-background-color: orange;-fx-border-color: black");
		OrangeLabel.setPrefWidth(38);
		OrangeLabel.setPrefHeight(28);
		OrangeLabel.setOnMouseClicked(e -> {
			positions[4] = !positions[4];
			refreshColors();
		});

		PinkLabel = new Label();
		PinkLabel.setStyle("-fx-background-color: pink;-fx-border-color: black");
		PinkLabel.setPrefWidth(38);
		PinkLabel.setPrefHeight(28);
		PinkLabel.setOnMouseClicked(e -> {
			positions[5] = !positions[5];
			refreshColors();
		});

		WhiteLabel = new Label();
		WhiteLabel.setStyle("-fx-background-color: white;-fx-border-color: black");
		WhiteLabel.setPrefWidth(38);
		WhiteLabel.setPrefHeight(28);
		WhiteLabel.setOnMouseClicked(e -> {
			positions[6] = !positions[6];
			refreshColors();
		});

		BlackLabel = new Label();
		BlackLabel.setStyle("-fx-background-color: black;-fx-border-color: black");
		BlackLabel.setPrefWidth(38);
		BlackLabel.setPrefHeight(28);
		BlackLabel.setOnMouseClicked(e -> {
			positions[7] = !positions[7];
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
	}

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

	@FXML
    void MouseClicked(MouseEvent event) {
		initialize();
    }

	@FXML
    void LeftAllClicked(MouseEvent event) {
		for(int i=0;i<8;i++)
			positions[i] = false;
		refreshColors();
    }

    @FXML
    void RightAllClicked(MouseEvent event) {
		for(int i=0;i<8;i++)
			positions[i] = true;
		refreshColors();
    }
}

