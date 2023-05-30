import javax.swing.*;
import javax.swing.border.LineBorder;
import javax.swing.plaf.basic.BasicButtonUI;
import java.awt.*;
import java.awt.event.*;

class ColoredButton extends JButton {
    public ColoredButton(String text) {
        super(text);

        setBackground(Colors.LightGray);
        setBorder(new LineBorder(Colors.AlmostLightGray));
        setUI(ColoredButtonUI.Get());
    }
}

class ColoredToggleButton extends JToggleButton {
    public ColoredToggleButton(String text) {
        super(text);

        setBackground(Color.lightGray);
        setBorder(new LineBorder(Colors.AlmostLightGray));
        setUI(ColoredButtonUI.Get());
    }
}

class ColoredButtonUI extends BasicButtonUI {
    private static final ColoredButtonUI ui = new ColoredButtonUI();

    private static final MouseAdapter mouseAdapter = new MouseAdapter() {
        @Override
        public void mousePressed(MouseEvent e) {
            setPressedState((JComponent) e.getComponent());
        }

        @Override
        public void mouseReleased(MouseEvent e) {
            if (e.getComponent() instanceof JToggleButton) {
                if (((JToggleButton) e.getComponent()).isSelected()) {
                    setPressedState((JComponent) e.getComponent());
                } else {
                    setNormalState((JComponent) e.getComponent());
                }
            } else {
                setNormalState((JComponent) e.getComponent());
            }
        }
    };

    private static final KeyAdapter keyAdapter = new KeyAdapter() {
        @Override
        public void keyPressed(KeyEvent e) {
            if (e.getKeyCode() == KeyEvent.VK_SPACE)
                setPressedState((JComponent) e.getComponent());
        }

        @Override
        public void keyReleased(KeyEvent e) {
            if (e.getKeyCode() == KeyEvent.VK_SPACE)
                setNormalState((JComponent) e.getComponent());
        }
    };

    public static ColoredButtonUI Get() {
        return ui;
    }

    private static void setNormalState(JComponent c) {
        c.setBackground(Colors.LightGray);
    }

    private static void setPressedState(JComponent c) {
        c.setBackground(Colors.AlmostLightGray);
    }

    @Override
    public void installUI(JComponent c) {
        super.installUI(c);
        c.addMouseListener(mouseAdapter);
        c.addKeyListener(keyAdapter);
        setNormalState(c);
    }

    @Override
    public void uninstallUI(JComponent c) {
        super.uninstallUI(c);
        c.removeMouseListener(mouseAdapter);
        c.removeKeyListener(keyAdapter);
    }
}