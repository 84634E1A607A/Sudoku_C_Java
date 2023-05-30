import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.image.BufferedImage;
import java.util.Objects;

public class Main {
    public static void main(String[] args) {
        BufferedImage iconImage = null;
        try {
            iconImage = ImageIO.read(Objects.requireNonNull(Main.class.getResourceAsStream("iconSmall.png")));
        } catch (Exception e) {
            JOptionPane.showMessageDialog(null, "Icon load failed", "Error", JOptionPane.ERROR_MESSAGE);
        }

        JFrame frame = new JFrame("Sudoku");
        if (iconImage != null)
            frame.setIconImage(iconImage);

        frame.setResizable(false);
        frame.setContentPane(new MainPanel());
        frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        frame.setVisible(true);
        frame.pack();
    }
}