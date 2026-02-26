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
    if (pathLength == 0) {
        cout << "empty path" << endl;
        return;
    }
    if (index >= pathLength) {
        cout << endl;
        return;
    }
    cout << "(" << path[index][0] << "," << path[index][1] << ")";
    if (index < pathLength - 1) {
        cout << " -> ";
    }
    printPath(path, pathLength, index + 1);
}

/**
 * Phase A (Recursion Warm-up)
 * TODO A2: Copy path coordinates recursively.
 */
// source: source path; copy: destination path; pathLength: length; index: current index
void copyPathRecursive(int source[][2], int copy[][2], int pathLength, int index = 0) {
    if (index >= pathLength) {
        return;
    }
    copy[index][0] = source[index][0];
    copy[index][1] = source[index][1];
    copyPathRecursive(source, copy, pathLength, index + 1);
}

/**
 * Phase A (Recursion Warm-up)
 * TODO A3: Recursively sum survivors along a path.
 */
int calculate_num_survivors(int map[][MAX_MAP_SIZE], int rows, int cols,
                            int path[][2], int pathLength, int index = 0) {
    if (index >= pathLength) {
        return 0;
    }
    int r = path[index][0];
    int c = path[index][1];
    return map[r][c] + calculate_num_survivors(map, rows, cols, path, pathLength, index + 1);
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
    if (r < 0 || r >= rows || c < 0 || c >= cols) {
        return false;
    }
    if (map[r][c] == -1) {
        return false;
    }
    if (visited[r][c]) {
        return false;
    }
    if (currentPathLength >= MAX_PATH_LENGTH) {
        return false;
    }

    // Hint 2: Record current state before exploring branches.
    visited[r][c] = true;
    currentPath[currentPathLength][0] = r;
    currentPath[currentPathLength][1] = c;
    int newPathLength = currentPathLength + 1;

    bool found = false;

    if (r == targetRow && c == targetCol) {
        if (pathLength == 0 || newPathLength < pathLength) {
            pathLength = newPathLength;
            copyPathRecursive(currentPath, path, newPathLength, 0);
        }
        found = true;
    }

    // Hint 4: Explore all four directions (do not stop at first success).
    if (!(r == targetRow && c == targetCol)) {
        for (int i = 0; i < 4; i++) {
            int newR = r + DIRECTIONS[i][0];
            int newC = c + DIRECTIONS[i][1];
            if (shortestPath(map, rows, cols, path, pathLength,
                               targetRow, targetCol,
                               newR, newC, visited,
                               currentPath, newPathLength)) {
                found = true;
            }
        }
    }

    // Hint 5: Backtrack after branch exploration and return whether any route exists.
    visited[r][c] = false;
    return found;
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
    if (r < 0 || r >= rows || c < 0 || c >= cols) {
        return UNREACHABLE;
    }
    if (timeLeft <= 0) {
        return UNREACHABLE;
    }
    if (map[r][c] == -1) {
        return UNREACHABLE;
    }
    if (visited[r][c]) {
        return UNREACHABLE;
    }
    if (currentPathLength >= MAX_PATH_LENGTH) {
        return UNREACHABLE;
    }

    // Hint 2: Record current node, then update rescued survivors.
    visited[r][c] = true;
    currentPath[currentPathLength][0] = r;
    currentPath[currentPathLength][1] = c;
    int newPathLength = currentPathLength + 1;

    int survivor = map[r][c];
    int totalSurvivors = currentSurvivors + survivor;

    // Hint 3: Enforce unlock threshold only when reaching the shelter door.
    if (r == targetRow && c == targetCol) {
        if (totalSurvivors >= unlockThreshold) {
            if (bestPathLength == 0 || totalSurvivors > maxSurvivors || maxSurvivors == 0) {
                maxSurvivors = totalSurvivors;
                bestPathLength = newPathLength;
                copyPathRecursive(currentPath, bestPath, newPathLength, 0);
            }
            visited[r][c] = false;
            return totalSurvivors;
        }

        visited[r][c] = false;
        return UNREACHABLE;
    }

    // Hint 4: Delegate branch exploration to tryAllDirections, then backtrack.
    int maxFromHere = tryAllDirections(map, rows, cols, targetRow, targetCol,
                                       r, c, visited,
                                       currentPath, newPathLength,
                                       timeLeft,
                                       bestPath, bestPathLength,
                                       0,
                                       maxSurvivors, totalSurvivors,
                                       unlockThreshold);

    visited[r][c] = false;
    return maxFromHere;
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
    if (dirIndex >= 4) {
        return UNREACHABLE;
    }

    // Hint 2: Dynamic time cost depends on currently rescued survivors.
    int newR = r + DIRECTIONS[dirIndex][0];
    int newC = c + DIRECTIONS[dirIndex][1];
    int timeCost = calculateTimeCost(currentSurvivors);

    // Hint 3: Recurse current direction, then recurse next direction index.
    int result = rescueSurvivors(map, rows, cols, targetRow, targetCol,
                                 newR, newC, visited,
                                 currentPath, currentPathLength,
                                 timeLeft - timeCost,
                                 bestPath, bestPathLength,
                                 maxSurvivors, currentSurvivors,
                                 unlockThreshold);

    int nextResult = tryAllDirections(map, rows, cols, targetRow, targetCol,
                                      r, c, visited,
                                      currentPath, currentPathLength,
                                      timeLeft,
                                      bestPath, bestPathLength,
                                      dirIndex + 1,
                                      maxSurvivors, currentSurvivors,
                                      unlockThreshold);

    // Hint 4: Combine both branch results; preserve the best reachable total.
    if (result == UNREACHABLE && nextResult == UNREACHABLE) {
        return UNREACHABLE;
    }
    if (result == UNREACHABLE) {
        return nextResult;
    }
    if (nextResult == UNREACHABLE) {
        return result;
    }
    return (result > nextResult) ? result : nextResult;
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
    if (r < 0 || r >= rows || c < 0 || c >= cols) {
        return false;
    }
    if (timeLeft <= 0) {
        return false;
    }
    if (map[r][c] == -1) {
        return false;
    }
    if (visited[r][c]) {
        return false;
    }
    if (currentPathLength >= MAX_PATH_LENGTH) {
        return false;
    }

    // Hint 2: If monster is on this tile now, apply monster penalty consistently.
    if (isMonsterHere(r, c, monsterRow, monsterCol, type, direction, timeElapsed,
                      period, rows, cols)) {
        int survivorsHere = map[r][c];
        int collectedSurvivors = currentSurvivors + survivorsHere;

        int potentialSurvivors = collectedSurvivors - MONSTER_DAMAGE_SURVIVORS;
        int potentialTimeLeft = timeLeft - MONSTER_DAMAGE_TIME;

        if (potentialSurvivors <= 0 || potentialTimeLeft <= 0) {
            return false;
        }

        int newCurrentSurvivors = potentialSurvivors;

        visited[r][c] = true;
        currentPath[currentPathLength][0] = r;
        currentPath[currentPathLength][1] = c;
        int newPathLength = currentPathLength + 1;

        bool found = tryAllDirectionsWithMonster(map, rows, cols, targetRow, targetCol,
                                                 r, c, visited,
                                                 currentPath, newPathLength,
                                                 potentialTimeLeft,
                                                 timeElapsed + MONSTER_DAMAGE_TIME,
                                                 0,
                                                 outPath, outPathLength,
                                                 newCurrentSurvivors,
                                                 unlockThreshold,
                                                 monsterRow, monsterCol, type,
                                                 direction, period);

        visited[r][c] = false;
        return found;
    }

    // Hint 3: Otherwise, mark state, collect survivors, and check unlock condition.
    visited[r][c] = true;
    currentPath[currentPathLength][0] = r;
    currentPath[currentPathLength][1] = c;
    int newPathLength = currentPathLength + 1;

    int survivor = map[r][c];
    int totalSurvivors = currentSurvivors + survivor;

    if (r == targetRow && c == targetCol) {
        if (totalSurvivors >= unlockThreshold) {
            outPathLength = newPathLength;
            copyPathRecursive(currentPath, outPath, newPathLength, 0);
            visited[r][c] = false;
            return true;
        }

        visited[r][c] = false;
        return false;
    }

    // Hint 4: Recurse to all directions and backtrack visited on return.
    bool found = tryAllDirectionsWithMonster(map, rows, cols, targetRow, targetCol,
                                             r, c, visited,
                                             currentPath, newPathLength,
                                             timeLeft, timeElapsed,
                                             0,
                                             outPath, outPathLength,
                                             totalSurvivors,
                                             unlockThreshold,
                                             monsterRow, monsterCol, type,
                                             direction, period);

    visited[r][c] = false;
    return found;
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
    if (dirIndex >= 4) {
        return false;
    }

    int newR = r + DIRECTIONS[dirIndex][0];
    int newC = c + DIRECTIONS[dirIndex][1];
    int timeCost = 1;

    // Hint 2: Before moving, check monster occupancy with current timing model.
    if (newR >= 0 && newR < rows && newC >= 0 && newC < cols) {
        if (isMonsterHere(newR, newC, monsterRow, monsterCol, type, direction,
                          timeElapsed, period, rows, cols)) {
            return tryAllDirectionsWithMonster(map, rows, cols, targetRow, targetCol,
                                               r, c, visited,
                                               currentPath, currentPathLength,
                                               timeLeft, timeElapsed,
                                               dirIndex + 1,
                                               outPath, outPathLength,
                                               currentSurvivors,
                                               unlockThreshold,
                                               monsterRow, monsterCol, type,
                                               direction, period);
        }
    }

    // Hint 3: If safe, recurse into rescueSurvivorsWithMonster for this branch.
    bool found = rescueSurvivorsWithMonster(map, rows, cols, targetRow, targetCol,
                                            newR, newC, visited,
                                            currentPath, currentPathLength,
                                            timeLeft - timeCost,
                                            timeElapsed + timeCost,
                                            outPath, outPathLength,
                                            currentSurvivors,
                                            unlockThreshold,
                                            monsterRow, monsterCol, type,
                                            direction, period);

    if (found) {
        return true;
    }

    // Hint 4: If branch fails, continue with the next direction.
    return tryAllDirectionsWithMonster(map, rows, cols, targetRow, targetCol,
                                       r, c, visited,
                                       currentPath, currentPathLength,
                                       timeLeft, timeElapsed,
                                       dirIndex + 1,
                                       outPath, outPathLength,
                                       currentSurvivors,
                                       unlockThreshold,
                                       monsterRow, monsterCol, type,
                                       direction, period);
}

#endif // HELPERS_H
