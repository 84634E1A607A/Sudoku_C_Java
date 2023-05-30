#include <cstdlib>
#include <ctime>
#include <cstring>
#include <queue>
#include <algorithm>
#include "map.h"
#include "main.h"
#include "dancingLinksAlgorithm.h"

int *newMap()
{
    srand((unsigned int)time(NULL));
    int *map = (int *)malloc(81 * sizeof(int));
    memset(map, 0, 81 * sizeof(int));
    return map;
}

// Reference: https://kns.cnki.net/kcms/detail/detail.aspx?dbcode=CJFD&dbname=CJFD2009&filename=SSJS200921000
void generate(int *nodes, int *answer, int level)
{
    clear(nodes, answer);

    // 0 is an empty map
    if (level == 0)
        return;

    generateFullMap(nodes, answer);

#ifdef DEBUG_MAP_GENERATION
    outputMapData(nodes);
#endif

    generateLevelMap(nodes, level);

#ifdef DEBUG_MAP_GENERATION
    outputMapData(nodes);
#endif

    generateShuffledMap(nodes, answer);

#ifdef DEBUG_MAP_GENERATION
    outputMapData(nodes);
#endif
}

bool checkCorrect(int *nodes)
{
    bool good = true;

    for (int i = 0; i < 81; ++i)
    {
        if (nodes[i] == 0)
            continue;

        bool error = isInvalidPlacement(nodes, i);

        if (error)
            good = false;
    }

    return good;
}

bool checkComplete(int *nodes)
{
    for (int i = 0; i < 81; ++i)
        if (nodes[i] == 0)
            return false;

    return true;
}

void clear(int *nodes, int *answer)
{
    memset(nodes, 0, 81 * sizeof(int));
    memset(answer, 0, 81 * sizeof(int));
}

void generateFullMap(int *nodes, int *answer)
{
    // Generate first row
    bool used[9] = {};
    for (int i = 0; i < 9; ++i)
    {
        int step = rand() % 9 + 1;
        int v = 0;
        for (int j = 0; j < step; ++j)
        {
            do
                v = (v + 1) % 9;
            while (used[v]);
        }

        used[v] = true;

        nodes[i] = v + 1;
    }

    // Generate first column
    memset(used, 0, sizeof(used));
    used[nodes[0] - 1] = true;
    for (int i = 1; i < 9; ++i)
    {
        int step = rand() % 9 + 1;
        int v = 0;
        for (int j = 0; j < step; ++j)
        {
            do
                v = (v + 1) % 9;
            while (used[v] || isInvalidPlacement(nodes, 0, i, v + 1));
        }

        used[v] = true;

        nodes[9 * i] = v + 1;
    }

    // Solve the map
    solve(nodes, false, true);

    // Generate answer sheet
    for (int i = 0; i < 81; ++i)
        answer[i] = nodes[i];
}

void generateLevelMap(int *nodes, int level)
{
    if (level > 10)
        generateHighLevelMap(nodes, level);
    else
        generateLowLevelMap(nodes, level);
}

void generateLowLevelMap(int *nodes, int level)
{
    int attemptSequence[81];
    for (int i = 0; i < 81; ++i)
        attemptSequence[i] = i;

    int randSequence[81];
    for (int i = 0; i < 81; ++i)
        randSequence[i] = rand();

    std::sort(&attemptSequence[0], &attemptSequence[80], [&randSequence](int a, int b)
              { return randSequence[a] < randSequence[b]; });

    int attempts = level * 4 + 6;

    // Try to remove a number but ensure the solution is unique
    for (int i = 0; i < attempts; ++i)
    {
        int p = attemptSequence[i];
        int v = nodes[p];
        nodes[p] = 0;

        // If multiple solutions found, restore this
        if (solve(nodes, true, false) > 1)
            nodes[p] = v;
    }
}

void generateHighLevelMap(int *nodes, int level)
{
    int attempts = level * 4;

    bool attemptsMap[81] = {};
    std::queue<int> attemptsQueue;

    // Add initial attempts position
    for (int i = 0; i < 9; ++i)
        attemptsQueue.push(rand() % 81);

    // Try to remove a number but ensure the solution is unique
    for (int i = 0; i < attempts; ++i)
    {
        int p, maximumAttempt = 100;

        do
        {
            if (attemptsQueue.empty())
                attemptsQueue.push(rand() % 81);
            p = attemptsQueue.front();
            attemptsQueue.pop();
        } while ((p < 0 || p >= 81 || attemptsMap[p]) && --maximumAttempt >= 0);

        if (maximumAttempt == 0)
        {
            int attempted = 0;

            for (int j = 0; j < 81; ++j)
                if (attemptsMap[j])
                    ++attempted;

            // If all the positions have been tried, the sudoku is in its final state.
            if (attempted == 81)
                break;

            continue;
        }

        attemptsMap[p] = true;

        int v = nodes[p];
        nodes[p] = 0;

        // If multiple solutions found, restore this
        if (solve(nodes, true, false) > 1)
            nodes[p] = v;

        // Add its adjacent to the queue
        attemptsQueue.push(p - 1);
        attemptsQueue.push(p + 1);
        attemptsQueue.push(p - 9);
        attemptsQueue.push(p + 9);
    }
}

void generateShuffledMap(int *nodes, int *answer)
{
    // Shuffle units
    {
        int r = rand() % 5;
        switch (r)
        {
        case 0:
            generateShuffledRowUnit(nodes, answer, 0, 1);
            break;
        case 1:
            generateShuffledRowUnit(nodes, answer, 0, 2);
            break;
        case 2:
            generateShuffledRowUnit(nodes, answer, 1, 2);
            break;
        case 3:
            generateShuffledRowUnit(nodes, answer, 0, 1);
            generateShuffledRowUnit(nodes, answer, 1, 2);
            break;
        case 4:
            generateShuffledRowUnit(nodes, answer, 1, 2);
            generateShuffledRowUnit(nodes, answer, 0, 1);
            break;
        }
    }
    {
        int r = rand() % 5;
        switch (r)
        {
        case 0:
            generateShuffledColUnit(nodes, answer, 0, 1);
            break;
        case 1:
            generateShuffledColUnit(nodes, answer, 0, 2);
            break;
        case 2:
            generateShuffledColUnit(nodes, answer, 1, 2);
            break;
        case 3:
            generateShuffledColUnit(nodes, answer, 0, 1);
            generateShuffledColUnit(nodes, answer, 1, 2);
            break;
        case 4:
            generateShuffledColUnit(nodes, answer, 1, 2);
            generateShuffledColUnit(nodes, answer, 0, 1);
            break;
        }
    }

    // Shuffle rows
    for (int i = 0; i < 3; ++i)
    {
        int r = rand() % 6;
        switch (r)
        {
        case 0:
            generateShuffledRow(nodes, answer, 3 * i, 3 * i + 1);
            break;
        case 1:
            generateShuffledRow(nodes, answer, 3 * i, 3 * i + 2);
            break;
        case 2:
            generateShuffledRow(nodes, answer, 3 * i + 1, 3 * i + 2);
            break;
        case 3:
            generateShuffledRow(nodes, answer, 3 * i, 3 * i + 1);
            generateShuffledRow(nodes, answer, 3 * i + 1, 3 * i + 2);
            break;
        case 4:
            generateShuffledRow(nodes, answer, 3 * i + 1, 3 * i + 2);
            generateShuffledRow(nodes, answer, 3 * i, 3 * i + 1);
            break;
        case 5:
            break;
        }
    }

    // Shuffle columns
    for (int i = 0; i < 3; ++i)
    {
        int r = rand() % 6;
        switch (r)
        {
        case 0:
            generateShuffledCol(nodes, answer, 3 * i, 3 * i + 1);
            break;
        case 1:
            generateShuffledCol(nodes, answer, 3 * i, 3 * i + 2);
            break;
        case 2:
            generateShuffledCol(nodes, answer, 3 * i + 1, 3 * i + 2);
            break;
        case 3:
            generateShuffledCol(nodes, answer, 3 * i, 3 * i + 1);
            generateShuffledCol(nodes, answer, 3 * i + 1, 3 * i + 2);
            break;
        case 4:
            generateShuffledCol(nodes, answer, 3 * i + 1, 3 * i + 2);
            generateShuffledCol(nodes, answer, 3 * i, 3 * i + 1);
            break;
        case 5:
            break;
        }
    }
}

void generateShuffledRowUnit(int *nodes, int *answer, int a, int b)
{
    for (int i = 0; i < 3; ++i)
        generateShuffledRow(nodes, answer, 3 * a + i, 3 * b + i);
}

void generateShuffledColUnit(int *nodes, int *answer, int a, int b)
{
    for (int i = 0; i < 3; ++i)
        generateShuffledCol(nodes, answer, 3 * a + i, 3 * b + i);
}

// Nodes in a row are continuous in memory, so memcpy is used
void generateShuffledRow(int *nodes, int *answer, int a, int b)
{
    int mapSwap[9];
    int answerSwap[9];

    memcpy(mapSwap, nodes + 9 * a, sizeof(mapSwap));
    memcpy(nodes + 9 * a, nodes + 9 * b, sizeof(mapSwap));
    memcpy(nodes + 9 * b, mapSwap, sizeof(mapSwap));

    memcpy(answerSwap, answer + 9 * a, sizeof(answerSwap));
    memcpy(answer + 9 * a, answer + 9 * b, sizeof(answerSwap));
    memcpy(answer + 9 * b, answerSwap, sizeof(answerSwap));
}

void generateShuffledCol(int *nodes, int *answer, int a, int b)
{
    int t;
    for (int i = 0; i < 9; ++i)
    {
        t = nodes[a + 9 * i];
        nodes[a + 9 * i] = nodes[b + 9 * i];
        nodes[b + 9 * i] = t;

        t = answer[a + 9 * i];
        answer[a + 9 * i] = answer[b + 9 * i];
        answer[b + 9 * i] = t;
    }
}

bool isInvalidPlacement(int *nodes, int x, int y, int v)
{
    for (int j = 0; j < 9; ++j)
    {
        if ((j != y && nodes[x + 9 * j] == v) || (j != x && nodes[j + 9 * y] == v) ||
            ((j % 3 != x % 3 || y % 3 != j / 3) && (nodes[x / 3 * 3 + j % 3 + 9 * (y / 3 * 3 + j / 3)] == v)))
        {
            return true;
        }
    }

    return false;
}

// Use after you have filled in nodes[p]
bool isInvalidPlacement(int *nodes, int p)
{
    return isInvalidPlacement(nodes, p % 9, p / 9, nodes[p]);
}

bool checkNoSolution(int *nodes)
{
    for (int v = 1; v <= 9; ++v)
    {
        int count = 0;
        bool filledInRow[9] = {}, filledInCol[9] = {};
        for (int p = 0; p < 81; ++p)
        {
            if (nodes[p] != v)
                continue;

            filledInRow[p / 9] = true;
            filledInCol[p % 9] = true;
            ++count;
        }

        if (count == 8)
        {
            int x = 0;
            while (filledInCol[x])
                ++x;

            int y = 0;
            while (filledInRow[y])
                ++y;

            // If the last position possible has been filled, the map has no solution.
            if (nodes[x + y * 9])
                return true;
        }

        else if (count == 7)
        {
            int x1 = 0, x2;
            while (filledInCol[x1])
                ++x1;

            x2 = x1 + 1;
            while (filledInCol[x2])
                ++x2;

            int y1 = 0, y2;
            while (filledInRow[y1])
                ++y1;

            y2 = y1 + 1;
            while (filledInRow[y2])
                ++y2;

            // If the only two possible positions have all been filled, the map has no solution.
            if (
                (nodes[x1 + y1 * 9] && nodes[x1 + y2 * 9]) ||
                (nodes[x2 + y1 * 9] && nodes[x2 + y2 * 9]) ||
                (nodes[x1 + y1 * 9] && nodes[x2 + y1 * 9]) ||
                (nodes[x1 + y2 * 9] && nodes[x2 + y2 * 9]))
                return true;
        }

        // Other cases are too complicated to code here and have little chance to be hit.
    }

    return false;
}

// If multiple solution search is on, map will be searched until two solutions are found.
int solve(int *nodes, bool multiSolutionSearch, bool keepOutcome)
{
    int index[10000], value[10000];
    int pIndex = 0, pValue = 0;

    initDancingLinksAlgorithm();

    for (register int i = 0; i < 81; ++i)
    {
        register int x = i / 9;
        register int y = i % 9;

        if (nodes[i])
        {
            int c1 = i + 1;
            int c2 = 9 * x + nodes[i] + 81;
            int c3 = 9 * y + nodes[i] + 162;
            int c4 = (x / 3 * 3 + y / 3) * 9 + nodes[i] + 243;
            appendDancingLinksLineByIndex(c1, c2, c3, c4);
            index[pIndex++] = i;
            value[pValue++] = nodes[i];
        }
    }

    completeDancingLinksInsertionOfMustSelectRow();

    for (register int i = 0; i < 81; ++i)
    {
        register int x = i / 9;
        register int y = i % 9;
        if (!nodes[i])
        {
            int c1 = i + 1;
            int c2 = 9 * x + 1 + 81;
            int c3 = 9 * y + 1 + 162;
            int c4 = (x / 3 * 3 + y / 3) * 9 + 1 + 243;
            if (appendDancingLinksLineByIndex(c1, c2, c3, c4))
            {
                index[pIndex++] = i;
                value[pValue++] = 1;
            }

            for (register int j = 2; j <= 9; ++j)
            {
                if (appendDancingLinksLineByIndex(c1, c2 + j - 1, c3 + j - 1, c4 + j - 1))
                {
                    index[pIndex++] = i;
                    value[pValue++] = j;
                }
            }
        }
    }

    int *p = runDancingLinksAlgorithm(multiSolutionSearch);

    if (!p)
        return 0;

    if (p == (int *)-1)
        return 2;

    if (keepOutcome)
    {
        for (register int i = 0; i < 81; ++i)
        {
            nodes[index[p[i] - 1]] = value[p[i] - 1];
        }
    }

    return true;
}