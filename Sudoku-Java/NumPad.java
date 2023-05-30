import javax.swing.*;
import javax.swing.border.EmptyBorder;
import java.awt.*;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

public class NumPad extends JDialog {
    private final List<InputListener> listeners = new ArrayList<>();

    private final ActionListener numberButtonActionListener = e -> {
        String text = ((JButton) e.getSource()).getText();
        int v = 0;

        if (!Objects.equals(text, "x"))
            v = Integer.parseInt(text);

        for (InputListener listener : listeners) {
            listener.onInput(v);
        }
    };

    private final Font font = new Font(null, Font.PLAIN, 24);

    public NumPad() {
        super((JFrame) null, "NumPad");
        setResizable(false);

        setupUIComponents();
        pack();
    }

    public void addListener(InputListener l) {
        listeners.add(l);
    }

    private void setupUIComponents() {
        JPanel panel = new JPanel(new GridLayout(4, 3, 3, 3));
        panel.setBorder(new EmptyBorder(3, 3, 3, 3));
        getContentPane().add(panel);

        for (int i = 1; i <= 9; i++) {
            JButton button = new ColoredButton(String.valueOf(i));
            button.setFont(font);
            button.addActionListener(numberButtonActionListener);
            panel.add(button);
        }

        panel.add(new JPanel());

        JButton button = new ColoredButton("x");
        button.setFont(font);
        button.addActionListener(numberButtonActionListener);
        panel.add(button);

        panel.add(new JPanel());
    }

    public interface InputListener {
        void onInput(int i);
    }
}
