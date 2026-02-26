// Helper routines for path calculation, printing, and recursive search.

#ifndef HELPERS_H
#define HELPERS_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

#include "globals.cpp"

using namespace std;

/**
 * Helper function to read map from map file
 */
// mapName: name of the map file; map: 2D array to store the map; rows, cols: dimensions of the map (output)
bool readMapFromFile(const string& mapName, int map[MAX_MAP_SIZE][MAX_MAP_SIZE], int& rows, int& cols) {
    string filename = mapName;
    if (filename.size() < 4 || filename.substr(filename.size() - 4) != ".txt") {
        filename += ".txt";
    }

    string filepath = filename;
    ifstream mapFile(filepath);
    if (!mapFile.is_open()) {
        filepath = "given_testcases/" + filename; // fallback for old layout
        mapFile.open(filepath);
    }

    if (!mapFile.is_open()) {
        cout << "Error: Cannot open " << filepath << endl;
        rows = 0;
        cols = 0;
        return false;
    }

    int fileRows, fileCols;
    mapFile >> fileRows >> fileCols;

    // Persist dimensions from file so callers do not rely on user input dimensions.
    rows = fileRows;
    cols = fileCols;

    if (rows > MAX_MAP_SIZE || cols > MAX_MAP_SIZE) {
        cout << "Error: Map dimensions exceed MAX_MAP_SIZE" << endl;
        mapFile.close();
        rows = 0;
        cols = 0;
        return false;
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            mapFile >> map[i][j];
        }
    }

    mapFile.close();
    return true;
}

/**
 * Mark the path indices on an integer grid
 */
// indexMap: a 2D array to mark the indices of the path coordinates
void markPathIndex(int path[][2], int pathLength, int indexMap[][MAX_MAP_SIZE]) {
    // Initialize indexMap with -1.
    for (int i = 0; i < MAX_MAP_SIZE; i++) {
        for (int j = 0; j < MAX_MAP_SIZE; j++) {
            indexMap[i][j] = -1;
        }
    }

    // Mark indices for coordinates in the path.
    for (int i = 0; i < pathLength; i++) {
        int r = path[i][0];
        int c = path[i][1];
        if (r >= 0 && r < MAX_MAP_SIZE && c >= 0 && c < MAX_MAP_SIZE) {
            indexMap[r][c] = i;
        }
    }
}

/**
 * Get direction symbol for path visualization
 */
char directionSymbol(int path[][2], int pathLength, int index) {
    if (index < 0 || index >= pathLength) {
        return '.';
    }
    if (index == pathLength - 1) {
        return 'E';
    }

    int dr = path[index + 1][0] - path[index][0];
    int dc = path[index + 1][1] - path[index][1];

    if (dr == -1 && dc == 0) return '^';
    if (dr == 1 && dc == 0) return 'v';
    if (dr == 0 && dc == -1) return '<';
    if (dr == 0 && dc == 1) return '>';

    return '*';
}

/**
 * Print the level 1 map with the path annotated
 */
// startR, startC: starting coordinates; targetR, targetC: exit coordinates
void printLevel1MapWithPath(int path[][2], int pathLength,
                            int map[][MAX_MAP_SIZE], int rows, int cols,
                            int startR, int startC, int targetR, int targetC) {
    int indexMap[MAX_MAP_SIZE][MAX_MAP_SIZE];
    markPathIndex(path, pathLength, indexMap);

    cout << "\nPath annotated map:" << endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (i == startR && j == startC) {
                cout << " S ";
            } else if (i == targetR && j == targetC) {
                cout << " E ";
            } else if (map[i][j] == -1) {
                cout << " # ";
            } else if (indexMap[i][j] >= 0) {
                char sym = directionSymbol(path, pathLength, indexMap[i][j]);
                cout << " " << sym << " ";
            } else {
                cout << " . ";
            }
        }
        cout << endl;
    }
}

/**
 * Print the level 2 map with the path annotated
 */
void printLevel2MapWithPath(int map[][MAX_MAP_SIZE], int rows, int cols,
                            int path[][2], int pathLength,
                            int startR, int startC, int targetR, int targetC) {
    int indexMap[MAX_MAP_SIZE][MAX_MAP_SIZE];
    markPathIndex(path, pathLength, indexMap);

    cout << "\nPath annotated map (survivor counts preserved):" << endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (i == startR && j == startC) {
                cout << " S ";
            } else if (i == targetR && j == targetC) {
                cout << " D ";
            } else if (map[i][j] == -1) {
                cout << " # ";
            } else if (indexMap[i][j] >= 0) {
                char sym = directionSymbol(path, pathLength, indexMap[i][j]);
                cout << " " << sym << " ";
            } else {
                cout << setw(2) << map[i][j] << " ";
            }
        }
        cout << endl;
    }
}

/**
 * Helper function to calculate bouncing position within boundaries
 */
int bouncePosition(int pos, int maxIndex) {
    if (maxIndex <= 0) return 0;

    if (pos < 0) {
        pos = -pos;
    }

    int cycle = maxIndex * 2;
    pos = pos % cycle;

    if (pos <= maxIndex) {
        return pos;
    }
    return cycle - pos;
}

/**
 * Monster position calculation.
 * The monster moves back and forth along a given row or column.
 */
// type: 0 for row movement, 1 for column movement; row, col: baseline position
// direction: 0 moves forward, 1 moves backward; timeStep: current time step
// period: movement range (cycles through period*2-2 without dwelling at endpoints)
// rows, cols: map dimensions for boundary checks
int getMonsterPosition(int type, int row, int col, int direction, int timeStep,
                       int period, int rows, int cols) {
    int fullCycle = (period - 1) * 2;
    int cyclePos = timeStep % fullCycle;

    int offset;
    if (cyclePos < period - 1) {
        offset = cyclePos;
    } else {
        offset = (2 * (period - 1)) - cyclePos;
    }

    if (direction == 1) {
        offset = -offset;
    }

    if (type == 0) {
        int pos = col + offset;
        return bouncePosition(pos, cols - 1);
    }

    int pos = row + offset;
    return bouncePosition(pos, rows - 1);
}

/**
 * Check whether the player encounters the monster
 */
// playerR, playerC: player's current position; monsterRow, monsterCol: monster baseline
bool isMonsterHere(int playerR, int playerC,
                   int monsterRow, int monsterCol,
                   int type, int direction, int timeStep, int period,
                   int rows, int cols) {
    if (type == 0) {
        if (playerR != monsterRow) return false;
        int monsterC = getMonsterPosition(type, monsterRow, monsterCol, direction,
                                          timeStep, period, rows, cols);
        return playerC == monsterC;
    }

    if (playerC != monsterCol) return false;
    int monsterR = getMonsterPosition(type, monsterRow, monsterCol, direction,
                                      timeStep, period, rows, cols);
    return playerR == monsterR;
}

// Level 2 utility (non-TODO)
int calculateTimeCost(int num_rescued) {
    return 1 + num_rescued / 4;
}

// Forward declarations for mutually recursive TODO functions.
int tryAllDirections(int map[][MAX_MAP_SIZE], int rows, int cols,
                     int targetRow, int targetCol,
                     int r, int c, bool visited[][MAX_MAP_SIZE],
                     int currentPath[][2], int currentPathLength,
                     int timeLeft,
                     int bestPath[][2], int& bestPathLength,
                     int dirIndex,
                     int& maxSurvivors, int currentSurvivors,
                     int unlockThreshold);

int rescueSurvivors(int map[][MAX_MAP_SIZE], int rows, int cols,
                    int targetRow, int targetCol,
                    int r, int c, bool visited[][MAX_MAP_SIZE],
                    int currentPath[][2], int currentPathLength,
                    int timeLeft,
                    int bestPath[][2], int& bestPathLength,
                    int& maxSurvivors, int currentSurvivors,
                    int unlockThreshold);

bool tryAllDirectionsWithMonster(int map[][MAX_MAP_SIZE], int rows, int cols,
                                 int targetRow, int targetCol,
                                 int r, int c, bool visited[][MAX_MAP_SIZE],
                                 int currentPath[][2], int currentPathLength,
                                 int timeLeft, int timeElapsed,
                                 int dirIndex,
                                 int outPath[][2], int& outPathLength,
                                 int currentSurvivors,
                                 int unlockThreshold,
                                 int monsterRow, int monsterCol, int type,
                                 int direction, int period);

bool rescueSurvivorsWithMonster(int map[][MAX_MAP_SIZE], int rows, int cols,
                                int targetRow, int targetCol,
                                int r, int c, bool visited[][MAX_MAP_SIZE],
                                int currentPath[][2], int currentPathLength,
                                int timeLeft, int timeElapsed,
                                int outPath[][2], int& outPathLength,
                                int currentSurvivors,
                                int unlockThreshold,
                                int monsterRow, int monsterCol, int type,
                                int direction, int period);

/**
 * Phase A (Recursion Warm-up)
 * TODO A1: Print the path coordinates recursively.
 */
// path: 2D array storing path coordinates; pathLength: path length; index: current print index
void printPath(int path[][2], int pathLength, int index = 0) {
    // Hint 1: Handle empty path first.
    // Hint 2: Base case when index reaches pathLength.
    // Hint 3: Print current coordinate, then recurse to index + 1.
    // TODO: Implement this function
}

/**
 * Phase A (Recursion Warm-up)
 * TODO A2: Copy path coordinates recursively.
 */
// source: source path; copy: destination path; pathLength: length; index: current index
void copyPathRecursive(int source[][2], int copy[][2], int pathLength, int index = 0) {
    // Hint 1: Stop when index reaches pathLength.
    // Hint 2: Copy one (row, col) pair each step.
    // Hint 3: Recurse with index + 1.
    // TODO: Implement this function
}

/**
 * Phase A (Recursion Warm-up)
 * TODO A3: Recursively sum survivors along a path.
 */
int calculate_num_survivors(int map[][MAX_MAP_SIZE], int rows, int cols,
                            int path[][2], int pathLength, int index = 0) {
    // Hint 1: Base case returns 0 when index reaches pathLength.
    // Hint 2: Add current cell survivors and recurse to next index.
    // TODO: Implement this function
}

/**
 * Phase B (Level 1 DFS Foundation)
 * TODO B1: DFS backtracking to find the shortest valid path to the exit.
 */
// r, c: current position; visited: visited marks
// currentPath/currentPathLength: path under exploration
// returns true if at least one valid route to target exists
bool shortestPath(int map[][MAX_MAP_SIZE], int rows, int cols,
                    int path[][2], int& pathLength,
                    int targetRow, int targetCol,
                    int r, int c, bool visited[][MAX_MAP_SIZE],
                    int currentPath[][2], int currentPathLength) {
    // Hint 1: Prune invalid states first (bounds, wall, revisits, path overflow).
    // Hint 2: Record current state before exploring branches.
    // Hint 3: At target, update best path only if current path is shorter.
    // Hint 4: Explore all four directions (do not stop at first success).
    // Hint 5: Backtrack after branch exploration and return whether any route exists.
    // TODO: Implement this function
}

/**
 * Phase C (Level 2 DFS Optimization)
 * TODO C1: DFS rescue with dynamic time cost, threshold check, and best-path update.
 * Note: Kept separate from tryAllDirections intentionally for DFS decomposition learning.
 */
int rescueSurvivors(int map[][MAX_MAP_SIZE], int rows, int cols,
                    int targetRow, int targetCol,
                    int r, int c, bool visited[][MAX_MAP_SIZE],
                    int currentPath[][2], int currentPathLength,
                    int timeLeft,
                    int bestPath[][2], int& bestPathLength,
                    int& maxSurvivors, int currentSurvivors,
                    int unlockThreshold) {
    // Hint 1: Prune invalid states first (bounds, time, wall, visited, path overflow).
    // Hint 2: Record current node, then update rescued survivors.
    // Hint 3: Enforce unlock threshold only when reaching the shelter door.
    // Hint 4: Delegate branch exploration to tryAllDirections, then backtrack.
    // TODO: Implement this function
}

/**
 * Phase C (Level 2 DFS Optimization)
 * TODO C2: Try all four directions and combine branch results.
 * Note: Kept separate from rescueSurvivors intentionally for DFS decomposition learning.
 */
int tryAllDirections(int map[][MAX_MAP_SIZE], int rows, int cols,
                     int targetRow, int targetCol,
                     int r, int c, bool visited[][MAX_MAP_SIZE],
                     int currentPath[][2], int currentPathLength,
                     int timeLeft,
                     int bestPath[][2], int& bestPathLength,
                     int dirIndex,
                     int& maxSurvivors, int currentSurvivors,
                     int unlockThreshold) {
    // Hint 1: Direction-recursion base case is when all 4 directions are attempted.
    // Hint 2: Dynamic time cost depends on currently rescued survivors.
    // Hint 3: Recurse current direction, then recurse next direction index.
    // Hint 4: Combine both branch results; preserve the best reachable total.
    // TODO: Implement this function
}

/**
 * Phase D (Level 3 Advanced DFS with Monster)
 * TODO D1: DFS rescue with time/resource constraints and monster penalties.
 */
bool rescueSurvivorsWithMonster(int map[][MAX_MAP_SIZE], int rows, int cols,
                                int targetRow, int targetCol,
                                int r, int c, bool visited[][MAX_MAP_SIZE],
                                int currentPath[][2], int currentPathLength,
                                int timeLeft, int timeElapsed,
                                int outPath[][2], int& outPathLength,
                                int currentSurvivors,
                                int unlockThreshold,
                                int monsterRow, int monsterCol, int type,
                                int direction, int period) {
    // Hint 1: Prune impossible states early (bounds, time, wall, visited, max path).
    // Hint 2: If monster is on this tile now, apply monster penalty consistently.
    // Hint 3: Otherwise, mark state, collect survivors, and check unlock condition.
    // Hint 4: Recurse to all directions and backtrack visited on return.
    // TODO: Implement this function
}

/**
 * Phase D (Level 3 Advanced DFS with Monster)
 * TODO D2: Recursively try all four directions while avoiding monster collisions.
 */
bool tryAllDirectionsWithMonster(int map[][MAX_MAP_SIZE], int rows, int cols,
                                 int targetRow, int targetCol,
                                 int r, int c, bool visited[][MAX_MAP_SIZE],
                                 int currentPath[][2], int currentPathLength,
                                 int timeLeft, int timeElapsed,
                                 int dirIndex,
                                 int outPath[][2], int& outPathLength,
                                 int currentSurvivors,
                                 int unlockThreshold,
                                 int monsterRow, int monsterCol, int type,
                                 int direction, int period) {
    // Hint 1: Base case for direction recursion is dirIndex >= 4.
    // Hint 2: Before moving, check monster occupancy with current timing model.
    // Hint 3: If safe, recurse into rescueSurvivorsWithMonster for this branch.
    // Hint 4: If branch fails, continue with the next direction.
    // TODO: Implement this function
}

#endif // HELPERS_H
