import javax.swing.*;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.util.Objects;
import java.util.Random;

public class Map {
    public final int[] nodes = new int[81];
    private final int[] answer = new int[81];
    public final int[] types = new int[81];
    private final Random random = new Random();

    // Load JNI library
    static {
        try {
            InputStream dllStream = Objects.requireNonNull(Main.class.getResourceAsStream("map.dll"));
            FileOutputStream dll = new FileOutputStream("map.dll");
            byte[] buf = new byte[4096];
            int len;

            while ((len = dllStream.read(buf)) != -1) {
                dll.write(buf, 0, len);
            }

            dllStream.close();
            dll.close();

            System.loadLibrary("map");

            // Init random
            init();
        } catch (Exception e) {
            JOptionPane.showMessageDialog(null, "Error setting up dll:\n" + e.getLocalizedMessage(),
                    "Fatal Error", JOptionPane.ERROR_MESSAGE);

            System.exit(1);
        }
    }

    private static native void init();

    private static native void generate(int level, int[] map, int[] ans);

    private static native int solve(int[] map, boolean multiSolutionSearch, boolean keepOutcome);

    public Map() {
        for (int i = 0; i < 81; ++i) {
            types[i] = NodeType.Unfilled;
        }
    }

    public void generate(int level) {
        generate(level, nodes, answer);
        updateNodeTypes();
    }

    public boolean checkCorrect() {
        boolean good = true;

        for (int i = 0; i < 81; ++i) {
            if (nodes[i] == 0)
                continue;

            boolean error = isInvalidPlacement(nodes, i % 9, i / 9, nodes[i]);

            if (error) {
                good = false;
                if (types[i] == NodeType.Filled)
                    types[i] = NodeType.Error;
            }
        }

        return good;
    }


    private boolean isInvalidPlacement(int[] nodes, int x, int y, int v) {
        for (int j = 0; j < 9; ++j) {
            if ((j != y && nodes[x + 9 * j] == v) || (j != x && nodes[j + 9 * y] == v) ||
                    ((j % 3 != x % 3 || y % 3 != j / 3) && (nodes[x / 3 * 3 + j % 3 + 9 * (y / 3 * 3 + j / 3)] == v))) {
                return true;
            }
        }

        return false;
    }

    public boolean checkComplete() {
        for (int i = 0; i < 81; ++i)
            if (types[i] == NodeType.Unfilled)
                return false;

        return true;
    }

    public boolean hint() {
        int p = 0;

        if (checkComplete()) {
            if (checkCorrect())
                return false;

            while (types[p] != NodeType.Error)
                p++;
        } else {
            int r = random.nextInt(81) + 1;
            for (int i = 0; i < r; i++) {
                do
                    p = (p + 1) % 81;
                while (types[p] != NodeType.Unfilled);
            }
        }

        nodes[p] = answer[p];
        types[p] = NodeType.Hinted;
        return true;
    }

    public int solve(boolean multiSolutionSearch, boolean keepOutcome) {
        int r = solve(nodes, multiSolutionSearch, keepOutcome);
        if (keepOutcome) {
            updateNodeTypes();
        }

        return r;
    }

    private void updateNodeTypes() {
        for (int i = 0; i < 81; ++i) {
            if (nodes[i] != 0) {
                types[i] = NodeType.Generated;
            } else {
                types[i] = NodeType.Unfilled;
            }
        }
    }
}
