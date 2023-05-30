#include <cstring>
#include "dancingLinksAlgorithm.h"

static int left[10000], right[10000], up[10000], down[10000];
static int row[10000], col[10000];
static int head;
static int rows, cols, nodeCount;
static int count[10000];
static int ans[10000];
static int mustSelectRowCount;

void initDancingLinksAlgorithm()
{
    up[0] = 0;
    down[0] = 0;
    right[0] = 1;
    left[0] = 324;

    for (register int i = 1; i <= 324; ++i)
    {
        up[i] = i;
        down[i] = i;
        left[i] = i - 1;
        right[i] = i + 1;
        col[i] = i;
        row[i] = 0;
        count[i] = 0;
    }

    right[324] = 0;

    rows = 0;
    cols = 324;
    nodeCount = 324;
    head = 0;
    mustSelectRowCount = 0;
}

bool appendDancingLinksLineByIndex(int a, int b, int c, int d)
{
    int arr[] = {a, b, c, d};
    return appendDancingLinksLineByIndex(arr, 4);
}

bool appendDancingLinksLineByIndex(int index[], int len)
{
    int k = 0;

    if (mustSelectRowCount)
    {
        if (!len)
        {
            ++rows;
            return true;
        }
        else
        {
            for (register int i = 0; i < len; ++i)
            {
                if (count[index[i]] == -1)
                    return false;
            }
        }
    }
    else
    {
        if (!len)
            return false;
    }

    ++rows;

    for (register int i = 0; i < len; ++i)
    {
        ++nodeCount;

        if (i)
        {
            left[nodeCount] = nodeCount - 1;
            right[nodeCount] = right[nodeCount - 1];
            left[right[nodeCount - 1]] = nodeCount;
            right[nodeCount - 1] = nodeCount;
        }
        else
        {
            left[nodeCount] = nodeCount;
            right[nodeCount] = nodeCount;
        }

        down[nodeCount] = index[i];
        up[nodeCount] = up[index[i]];
        down[up[index[i]]] = nodeCount;
        up[index[i]] = nodeCount;

        row[nodeCount] = rows;
        col[nodeCount] = index[i];

        if (mustSelectRowCount)
        {
            ++count[index[i]];
        }
        else
        {
            count[index[i]] = -1;
        }
    }
    return true;
}

void completeDancingLinksInsertionOfMustSelectRow()
{
    for (register int i = 1; i <= cols; ++i)
    {
        if (count[i] == -1)
        {
            left[right[i]] = left[i];
            right[left[i]] = right[i];
        }
    }

    for (register int i = 1; i <= rows; ++i)
    {
        ans[i - 1] = i;
    }

    mustSelectRowCount = rows;
}

int *runDancingLinksAlgorithm(bool multiSolutionSearch)
{
    switch (runDancingLinksAlgorithm(mustSelectRowCount, multiSolutionSearch))
    {
    case 0:
        return NULL;
    case 1:
        return ans;
    default:
        return (int *)-1;
    }
}

int runDancingLinksAlgorithm(int k, bool multiSolutionSearch)
{
    if (right[head] == head)
    {
        return 1;
    }

    int p, c1;

    p = right[head];
    c1 = p;

    while (p != head)
    {
        if (count[p] < count[c1])
        {
            c1 = p;
        }
        p = right[p];
    }

    if (count[c1] < 1)
    {
        return false;
    }

    removeCol(c1);

    int i, j, s = 0;

    i = down[c1];
    while (i != c1)
    {
        ans[k] = row[i];

        j = right[i];
        while (j != i)
        {
            removeCol(col[j]);
            j = right[j];
        }

        if (s += runDancingLinksAlgorithm(k + 1, multiSolutionSearch))
        {
            if (!multiSolutionSearch || s > 1)
                return s;
        }

        j = left[i];
        while (j != i)
        {
            resumeCol(col[j]);
            j = left[j];
        }

        i = down[i];
    }

    resumeCol(c1);
    return s;
}

static void removeCol(int c)
{
    left[right[c]] = left[c];
    right[left[c]] = right[c];

    int i = down[c], j;
    while (i != c)
    {
        j = right[i];
        while (j != i)
        {
            up[down[j]] = up[j];
            down[up[j]] = down[j];

            --count[col[j]];

            j = right[j];
        }

        i = down[i];
    }
}

static void resumeCol(int c)
{
    left[right[c]] = c;
    right[left[c]] = c;

    int i = up[c], j;
    while (i != c)
    {
        j = right[i];
        while (j != i)
        {
            up[down[j]] = j;
            down[up[j]] = j;

            ++count[col[j]];

            j = right[j];
        }

        i = up[i];
    }
}