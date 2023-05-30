public class NodeType {
    public static final int Unfilled = 0;
    public static final int Generated = 1;
    public static final int Filled = 2;
    public static final int Error = 3;
    public static final int Hinted = 4;
    public static final int Marked = 5;
    public static final int[] MarkedNum = {0, 1 << 3, 1 << 4, 1 << 5, 1 << 6, 1 << 7, 1 << 8, 1 << 9, 1 << 10, 1 << 11};
}
