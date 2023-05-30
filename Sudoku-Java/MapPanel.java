import javax.swing.*;
import java.awt.*;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

public class MapPanel extends JPanel {
    public StepListener StepListener;
    public final Map map = new Map();
    public final NumPad numPad = new NumPad();

    private static final int mapFontSize = 32;
    private final Font mapFont = new Font(null, Font.PLAIN, mapFontSize);
    private final Font markFont = new Font(null, Font.PLAIN, mapFontSize / 3);
    private int chosen = -1;
    private Dimension size;
    private int offset;
    private boolean markingMode = false;

    public MapPanel() {
        super();

        setFocusable(true);
        setBackground(Colors.LightGray);

        addMouseListener(new MouseAdapter() {
            @Override
            public void mousePressed(MouseEvent e) {
                grabFocus();
                chosen = e.getX() / offset + e.getY() / offset * 9;
                repaint();
            }

            @Override
            public void mouseClicked(MouseEvent e) {
                // Double click
                if (e.getClickCount() == 2) {
                    numPad.setVisible(true);
                }
            }
        });

        addKeyListener(new KeyAdapter() {
            @Override
            public void keyTyped(KeyEvent e) {
                if (e.getKeyChar() == KeyEvent.VK_DELETE) {
                    setNodeValue(0);
                }

                int v = e.getKeyChar() - '0';
                if (1 <= v && v <= 9) {
                    setNodeValue(v);
                }
            }

            @Override
            public void keyPressed(KeyEvent e) {
                switch (e.getKeyCode()) {
                    case KeyEvent.VK_LEFT: {
                        if (chosen % 9 != 0) chosen--;
                        break;
                    }
                    case KeyEvent.VK_RIGHT: {
                        if (chosen % 9 != 8) chosen++;
                        break;
                    }
                    case KeyEvent.VK_UP: {
                        if (chosen >= 9) chosen -= 9;
                        break;
                    }
                    case KeyEvent.VK_DOWN: {
                        if (chosen < 72) chosen += 9;
                        break;
                    }
                    default: {
                        return;
                    }
                }

                repaint();
            }
        });

        numPad.addListener(this::setNodeValue);
    }

    @Override
    public void paint(Graphics g) {
        super.paint(g);

        // Init offset
        if (offset == 0) {
            size = getSize();
            offset = size.width / 9;
        }

        g.setColor(Colors.AlmostLightGray);
        g.fillRect(chosen % 9 * offset, chosen / 9 * offset, offset, offset);

        g.setColor(Colors.Black);
        for (int i = 1; i < 9; i++) {
            g.drawLine(0, i * offset, size.width, i * offset);
            g.drawLine(i * offset, 0, i * offset, size.height);

            if (i % 3 == 0) {
                g.fillRect(0, i * offset - 1, size.width, 3);
                g.fillRect(i * offset - 1, 0, 3, size.height);
            }
        }

        // Draw border
        g.fillRect(0, 0, size.width, 2);
        g.fillRect(0, 0, 2, size.height);
        g.fillRect(0, size.height - 2, size.width, 2);
        g.fillRect(size.width - 2, 0, 2, size.height);

        for (int i = 0; i < 81; i++) {
            if (map.types[i] < NodeType.Marked) {
                switch (map.types[i]) {
                    case NodeType.Unfilled: {
                        continue;
                    }

                    case NodeType.Generated: {
                        g.setColor(Colors.Black);
                        break;
                    }
                    case NodeType.Filled: {
                        g.setColor(Colors.Blue);
                        break;
                    }
                    case NodeType.Error: {
                        g.setColor(Colors.Red);
                        break;
                    }
                    case NodeType.Hinted: {
                        g.setColor(Colors.Green);
                        break;
                    }
                }

                g.setFont(mapFont);
                g.drawString(String.valueOf(map.nodes[i]), offset * (i % 9) + 10, offset * (i / 9) + mapFontSize);
            } else {
                g.setFont(markFont);
                g.setColor(Color.blue);
                for (int v = 1; v <= 9; ++v) {
                    if ((map.types[i] & NodeType.MarkedNum[v]) != 0) {
                        g.drawString(String.valueOf(v), offset * (i % 9) + (offset / 3 - 1) * ((v - 1) % 3) + 5,
                                offset * (i / 9) + (offset / 3 - 1) * ((v - 1) / 3) + mapFontSize / 3 + 3);
                    }
                }
            }
        }
    }

    private void setNodeValue(int i) {
        if (map.types[chosen] == NodeType.Generated
                || map.types[chosen] == NodeType.Hinted)
            return;

        assert (i >= 0 && i <= 9);

        if (i == 0) {
            map.types[chosen] = NodeType.Unfilled;
        } else if (markingMode && map.types[chosen] != NodeType.Unfilled) {
            if (map.types[chosen] == NodeType.Filled) {
                map.types[chosen] = NodeType.Marked | NodeType.MarkedNum[map.nodes[chosen]];
                map.nodes[chosen] = 0;
            }

            if ((map.types[chosen] & NodeType.MarkedNum[i]) != 0) {
                map.types[chosen] &= ~NodeType.MarkedNum[i];
            } else {
                map.types[chosen] |= NodeType.MarkedNum[i];
            }

            int count = 0, lastValue = 0;
            for (int v = 1; v <= 9; ++v) {
                if ((map.types[chosen] & NodeType.MarkedNum[v]) != 0) {
                    ++count;
                    lastValue = v;
                }
            }

            if (count == 1) {
                map.types[chosen] = NodeType.Filled;
                map.nodes[chosen] = lastValue;
            }
        } else {
            map.types[chosen] = NodeType.Filled;
            map.nodes[chosen] = i;
        }

        repaint();

        StepListener.onStep();
    }

    public void setMarkingMode(boolean markingMode) {
        this.markingMode = markingMode;
    }

    public interface StepListener {
        void onStep();
    }
}
