import javax.swing.*;
import java.awt.*;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

public class MainPanel extends JPanel {
    // UI Components
    private MapPanel MapPanel;
    private JLabel LevelLabel;
    private JLabel TimeLabel;
    private JLabel StepsLabel;
    private ColoredButton NewGameButton;
    private ColoredButton NewMapButton;
    private ColoredButton HintButton;
    private ColoredButton CheckButton;
    private ColoredButton SolveButton;
    private ColoredToggleButton MarkingButton;
    private final Timer Timer;

    // Local variables
    private int level = 1;
    private boolean emptyMap = false;
    private int timeUsed = 0;
    private int stepsUsed = 0;
    private boolean markingMode = false;

    public MainPanel() {
        super();

        Timer = new Timer(1000, e -> {
            ++timeUsed;
            UpdateTimeLabel();
        });

        Timer.setRepeats(true);
        Timer.start();

        setupUIComponents();

        // Adjust level
        LevelLabel.addMouseListener(new MouseAdapter() {
            @Override
            public void mousePressed(MouseEvent e) {
                LevelLabel.grabFocus();
            }
        });

        LevelLabel.addKeyListener(new KeyAdapter() {
            @Override
            public void keyTyped(KeyEvent e) {
                switch (e.getKeyChar()) {
                    case '+': {
                        if (level < 20) {
                            level++;
                            UpdateLevelLabel();
                        }

                        break;
                    }
                    case '-': {
                        if (level > 1) {
                            level--;
                            UpdateLevelLabel();
                        }

                        break;
                    }
                }
            }
        });

        NewGameButton.addActionListener(e -> {
            emptyMap = false;
            UpdateLevelLabel();
            MapPanel.map.generate(level);
            MapPanel.repaint();
            timeUsed = 0;
            stepsUsed = 0;
            UpdateTimeLabel();
            UpdateStepsLabel();
            Timer.restart();
        });

        NewMapButton.addActionListener(e -> {
            emptyMap = true;
            MapPanel.map.generate(0);
            UpdateLevelLabel();
            MapPanel.repaint();
            timeUsed = 0;
            stepsUsed = 0;
            UpdateTimeLabel();
            UpdateStepsLabel();
            Timer.restart();
        });

        HintButton.addActionListener(e -> {
            if (emptyMap) {
                JOptionPane.showMessageDialog(this, "No hints available", "Oops", JOptionPane.INFORMATION_MESSAGE);
                return;
            }

            if (MapPanel.map.hint()) {
                stepsUsed += 50;
                UpdateStepsLabel();
                MapPanel.repaint();
            } else
                JOptionPane.showMessageDialog(this, "Nothing to hint", "Oops", JOptionPane.INFORMATION_MESSAGE);
        });

        CheckButton.addActionListener(e -> {
            if (MapPanel.map.checkCorrect()) {
                if (MapPanel.map.checkComplete()) {
                    Timer.stop();
                    JOptionPane.showMessageDialog(this, String.format("You have worked this out!\nTime used: %ds", timeUsed), "Congratulations", JOptionPane.INFORMATION_MESSAGE);
                    if (!emptyMap) {
                        if (level >= 20) {
                            level = 19;
                            JOptionPane.showMessageDialog(this, "No harder game for you!", "Oops", JOptionPane.INFORMATION_MESSAGE);
                        }

                        MapPanel.map.generate(++level);
                        timeUsed = 0;
                        stepsUsed = 0;
                        UpdateLevelLabel();
                        UpdateTimeLabel();
                        UpdateStepsLabel();
                    } else {
                        MapPanel.map.generate(0);
                    }
                } else {
                    JOptionPane.showMessageDialog(this, "So far so good!", "Map correct", JOptionPane.INFORMATION_MESSAGE);
                }
            } else {
                JOptionPane.showMessageDialog(this, "Something goes wrong!", "Map incorrect", JOptionPane.ERROR_MESSAGE);
            }

            MapPanel.repaint();
        });

        SolveButton.addActionListener(e -> {
            int i = MapPanel.map.solve(false, true);

            if (i == 0)
                JOptionPane.showMessageDialog(this, "Invalid map!", "Cannot solve", JOptionPane.ERROR_MESSAGE);

            MapPanel.repaint();
        });

        MarkingButton.addActionListener(e -> {
            markingMode = MarkingButton.isSelected();
            MapPanel.setMarkingMode(markingMode);
        });

        MapPanel.StepListener = () -> {
            ++stepsUsed;
            UpdateStepsLabel();
        };
    }

    private void setupUIComponents() {
        // This
        this.setBackground(Colors.White);

        // Main hBox
        Box hBox = Box.createHorizontalBox();
        add(hBox);

        // Map panel
        MapPanel = new MapPanel();
        MapPanel.setPreferredSize(new Dimension(360, 360));
        MapPanel.map.generate(level);
        hBox.add(MapPanel);

        // Space between map and controls
        hBox.add(Box.createHorizontalStrut(20));

        // Control panel box
        Box vBox = Box.createVerticalBox();
        vBox.setPreferredSize(new Dimension(115, 0));
        hBox.add(vBox);
        hBox.add(Box.createHorizontalStrut(15));

        // Upper space (to make controls vertically centered)
        vBox.add(Box.createVerticalGlue());

        // Control panel
        JPanel grid = new JPanel(new GridLayout(9, 1, 5, 8));
        grid.setBackground(Colors.White);
        vBox.add(grid);

        // Lower space
        vBox.add(Box.createVerticalGlue());

        LevelLabel = new JLabel("Level 1");
        LevelLabel.setHorizontalAlignment(SwingConstants.CENTER);
        LevelLabel.setFont(new Font(null, Font.PLAIN, 22));
        grid.add(LevelLabel);

        TimeLabel = new JLabel("Time: 0s");
        TimeLabel.setHorizontalAlignment(SwingConstants.CENTER);
        TimeLabel.setFont(new Font(null, Font.PLAIN, 18));
        grid.add(TimeLabel);

        StepsLabel = new JLabel("Steps: 0");
        StepsLabel.setHorizontalAlignment(SwingConstants.CENTER);
        StepsLabel.setFont(new Font(null, Font.PLAIN, 18));
        grid.add(StepsLabel);

        NewGameButton = new ColoredButton("New Game");
        grid.add(NewGameButton);

        NewMapButton = new ColoredButton("Empty Map");
        grid.add(NewMapButton);

        HintButton = new ColoredButton("Hint");
        grid.add(HintButton);

        CheckButton = new ColoredButton("Check");
        grid.add(CheckButton);

        SolveButton = new ColoredButton("Solve");
        grid.add(SolveButton);

        MarkingButton = new ColoredToggleButton("Marking");
        grid.add(MarkingButton);
    }

    private void UpdateLevelLabel() {
        LevelLabel.setText(emptyMap ? "Empty" : String.format("Level %d", level));
    }

    private void UpdateTimeLabel() {
        TimeLabel.setText(String.format("Time: %ds", timeUsed));
    }

    private void UpdateStepsLabel() {
        StepsLabel.setText(String.format("Steps: %d", stepsUsed));
    }
}
