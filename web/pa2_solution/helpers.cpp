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

    // Persist the dimensions read from the file so callers don't rely on user input.
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
 * TODO: print the path (its coordinates) recursively
 */
// path: a 2D array storing the path coordinates; pathLength: length of the path; index: current index to print
void printPath(int path[][2], int pathLength, int index = 0){
    if (pathLength == 0) {
        cout << "empty path" << endl;
        return;
    }
    // Base case: reached the end of the path
    if (index >= pathLength) {
        cout << endl;
        return;
    }
    // Print the current coordinate
    cout << "(" << path[index][0] << "," << path[index][1] << ")";
    if (index < pathLength - 1) {
        cout << " -> ";
    }
    // Recursive call for the next coordinate
    printPath(path, pathLength, index + 1);
}

/**
 * Mark the path indices on an integer grid
 */
// indexMap: a 2D array to mark the indices of the path coordinates
void markPathIndex(int path[][2], int pathLength, int indexMap[][MAX_MAP_SIZE]){
    // Initialize indexMap with -1 (chasm)
    for (int i = 0; i < MAX_MAP_SIZE; i++) {
        for (int j = 0; j < MAX_MAP_SIZE; j++) {
            indexMap[i][j] = -1;
        }
    }
    // Mark the path indices
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
char directionSymbol(int path[][2], int pathLength, int index){
    // out of bounds
    if (index < 0 || index >= pathLength) {
        return '.';
    }
    // exit point
    if (index == pathLength - 1) {
        return 'E';
    }
    // determine direction to next point
    int dr = path[index + 1][0] - path[index][0];
    int dc = path[index + 1][1] - path[index][1];
    if (dr == -1 && dc == 0) {
        return '^';
    }
    if (dr == 1 && dc == 0) {
        return 'v';
    }
    if (dr == 0 && dc == -1) {
        return '<';
    }
    if (dr == 0 && dc == 1) {
        return '>';
    }
    // unknown direction
    return '*';
}

/**
 * Print the level 1 map with the path annotated
 */
// startR, startC: starting coordinates; targetR, targetC: exit's coordinates
void printLevel1MapWithPath(int path[][2], int pathLength,
                            int map[][MAX_MAP_SIZE], int rows, int cols,
                            int startR, int startC, int targetR, int targetC) {
    int indexMap[MAX_MAP_SIZE][MAX_MAP_SIZE];
    markPathIndex(path, pathLength, indexMap);
    cout << "\nPath annotated map:" << endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++){
            // Starting point
            if (i == startR && j == startC) {
                cout << " S ";
            }
            // Target point
            else if (i == targetR && j == targetC) {
                cout << " E ";
            }
            // Chasm
            else if (map[i][j] == -1) {
                cout << " # ";
            }
            // Path direction
            else if (indexMap[i][j] >= 0) {
                char sym = directionSymbol(path, pathLength, indexMap[i][j]);
                cout << " " << sym << " ";
            }
            // not in path
            else {
                cout << " . ";
            }
        }
        cout << endl;
    }
}

/**
 * TODO: Copy path from source recursively
 */
// source: source path; copy: destination path; pathLength: length of the path; index: current index to copy
void copyPathRecursive(int source[][2], int copy[][2], int pathLength, int index = 0){
    // Base case: reached the end of the path
    if (index >= pathLength) {
        return;
    }
    // Copy the current coordinate
    copy[index][0] = source[index][0];
    copy[index][1] = source[index][1];
    // Recursive call for the next coordinate
    copyPathRecursive(source, copy, pathLength, index + 1);
}

/**
 * TODO: Level 1: Simple path finding (find a way out without revisiting nodes)
 */
// r: current row; c: current column; visited: visited marks
// currentPath: path has been explored; currentPathLength: length of current path
// returns: true if path found, false otherwise
bool findPathSimple(int map[][MAX_MAP_SIZE], int rows, int cols, int path[][2], int& pathLength, int targetRow, int targetCol,
                    int r, int c, bool visited[][MAX_MAP_SIZE],
                    int currentPath[][2], int currentPathLength){
    // out of map range
    if (r < 0 || r >= rows || c < 0 || c >= cols) {
        return false;
    }
    // encountered chasm
    if (map[r][c] == -1) {
        return false;
    }
    // already visited
    if (visited[r][c]) {
        return false;
    }
    // path array full
    if (currentPathLength >= MAX_PATH_LENGTH) {
        return false;
    }
    
    // Mark as visited
    visited[r][c] = true;
    currentPath[currentPathLength][0] = r;
    currentPath[currentPathLength][1] = c;
    int newPathLength = currentPathLength + 1;

    // reach the exit
    if (r == targetRow && c == targetCol) {
        pathLength = newPathLength;
        copyPathRecursive(currentPath, path, newPathLength, 0);
        visited[r][c] = false;
        return true;
    }

    // try all four directions
    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    for (int i = 0; i < 4; i++) {
        int newR = r + directions[i][0];
        int newC = c + directions[i][1];
        // Recursive attempt for new position
        if (findPathSimple(map, rows, cols, path, pathLength, targetRow, targetCol,
                           newR, newC, visited,
                           currentPath, newPathLength)) {
            // Backtrack: unmark visited
            visited[r][c] = false;
            // path found
            return true;
        }
    }

    // path not found
    visited[r][c] = false;
    return false;
}


// Level 2
/**
 * Print the level 2 map with the path annotated
 */
void printLevel2MapWithPath(int map[][MAX_MAP_SIZE], int rows, int cols, int path[][2], int pathLength, int startR, int startC, int targetR, int targetC) {
    int indexMap[MAX_MAP_SIZE][MAX_MAP_SIZE];
    markPathIndex(path, pathLength, indexMap);
    cout << "\nPath annotated map (survivor counts preserved):" << endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (i == startR && j == startC) {
                cout << " S ";
            }
            else if (i == targetR && j == targetC) {
                cout << " D ";
            }
            else if (map[i][j] == -1) {
                cout << " # ";
            }
            else if (indexMap[i][j] >= 0) {
                char sym = directionSymbol(path, pathLength, indexMap[i][j]);
                cout << " " << sym << " ";
            }
            // not in path, print survivor count
            else {
                cout << setw(2) << map[i][j] << " ";
            }
        }
        cout << endl;
    }
}

/**
 * TODO: Calculate total survivors along the path recursively
 */
int calculate_num_survivors(int map[][MAX_MAP_SIZE], int rows, int cols, int path[][2], int pathLength, int index = 0) {
    if (index >= pathLength) {
        return 0;
    }
    int r = path[index][0];
    int c = path[index][1];
    return map[r][c] + calculate_num_survivors(map, rows, cols, path, pathLength, index + 1);
}

/**
 * Helper function to calculate bouncing position within boundaries
 */
int bouncePosition(int pos, int maxIndex) {
    if (maxIndex <= 0) return 0;
    
    // Handle negative positions by mirroring
    if (pos < 0) {
        pos = -pos;
    }
    
    // Create a cycle that bounces at boundaries: 0->max->0->max...
    int cycle = maxIndex * 2;
    pos = pos % cycle;
    
    // First half: moving forward (0 to maxIndex)
    if (pos <= maxIndex) {
        return pos;
    }
    // Second half: bouncing back (maxIndex to 0)
    else {
        return cycle - pos;
    }
}

/**
 * Monster position calculation
 * The monster moves back and forth along a given row or column.
 */
// type: 0 for row movement, 1 for column movement; row, col: baseline position; direction: 0 moves forward, 1 moves backward; timeStep: current time step; period: movement range (cycles through period*2-2 without dwelling at endpoints); rows, cols: map dimensions for boundary checking
int getMonsterPosition(int type, int row, int col, int direction, int timeStep, int period, int rows, int cols){
    int fullCycle = (period - 1) * 2;  // does not stop at endpoints
    int cyclePos = timeStep % fullCycle;
    
    int offset;
    if (cyclePos < period - 1) {
        // first half of the cycle: from 0 to period-1
        offset = cyclePos;
    }
    else {
        // second half of the cycle: from period-1 back to 0
        offset = (2 * (period - 1)) - cyclePos;
    }
    
    // reverse movement direction
    if (direction == 1) {
        offset = -offset;
    }
    
    // moves along the row
    if (type == 0) {
        int pos = col + offset;
        // boundary check: bounce at map edges
        return bouncePosition(pos, cols - 1);
    }
    // moves along the column
    else {
        int pos = row + offset;
        // boundary check: bounce at map edges
        return bouncePosition(pos, rows - 1);
    }
}

/**
 * Check whether the player encounters the monster
 */
// playerR, playerC: player's current position; monsterRow, monsterCol: monster's baseline position
bool isMonsterHere(int playerR, int playerC, 
                   int monsterRow, int monsterCol,
                   int type, int direction, int timeStep, int period,
                   int rows, int cols) {
    if (type == 0) {
        if (playerR != monsterRow) return false;
        int monsterC = getMonsterPosition(type, monsterRow, monsterCol, direction, timeStep, period, rows, cols);
        return playerC == monsterC;
    }
    else {
        if (playerC != monsterCol) return false;
        int monsterR = getMonsterPosition(type, monsterRow, monsterCol, direction, timeStep, period, rows, cols);
        return playerR == monsterR;
    }
}

// forward declaration
bool rescueSurvivorsWithMonster(int map[][MAX_MAP_SIZE], int rows, int cols, int targetRow, int targetCol, int r, int c, bool visited[][MAX_MAP_SIZE], int currentPath[][2], int currentPathLength,
                                int timeLeft, int timeElapsed,
                                int outPath[][2], int& outPathLength,
                                int currentSurvivors,
                                int unlockThreshold,
                                int monsterRow, int monsterCol, int type,
                                int direction, int period);

/**
 * TODO: Try all four directions recursively (monster version)
 */
// timeLeft: remaining time; timeElapsed: time elapsed so far
// dirIndex: current direction index to try
// outPath: output path if found; outPathLength: length of output path
// currentSurvivors: survivors rescued so far;
// unlockThreshold: survivors needed to unlock shelter
bool tryAllDirectionsWithMonster(int map[][MAX_MAP_SIZE], int rows, int cols, int targetRow, int targetCol, int r, int c, bool visited[][MAX_MAP_SIZE], int currentPath[][2], int currentPathLength,
                                 int timeLeft, int timeElapsed,
                                 int dirIndex,
                                 int outPath[][2], int& outPathLength,
                                 int currentSurvivors,
                                 int unlockThreshold,
                                 int monsterRow, int monsterCol, int type,
                                 int direction, int period) {
    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    if (dirIndex >= 4) {
        return false;
    }

    int newR = r + directions[dirIndex][0];
    int newC = c + directions[dirIndex][1];
    int timeCost = 1;

    // Check if monster is at the target position at current time
    // If so, cannot move there to avoid collision
    if (newR >= 0 && newR < rows && newC >= 0 && newC < cols) {
        if (isMonsterHere(newR, newC, monsterRow, monsterCol, type, direction, timeElapsed, period, rows, cols)) {
            // Monster is at target position, skip this direction
            return tryAllDirectionsWithMonster(map, rows, cols, targetRow, targetCol,
                                               r, c, visited,
                                               currentPath, currentPathLength,
                                               timeLeft, timeElapsed,
                                               dirIndex + 1,
                                               outPath, outPathLength,
                                               currentSurvivors,
                                               unlockThreshold,
                                               monsterRow, monsterCol, type, direction, period);
        }
    }

    // Recursive attempt for the new direction (monster version)
    bool found = rescueSurvivorsWithMonster(map, rows, cols, targetRow, targetCol,
                                            newR, newC, visited,
                                            currentPath, currentPathLength,
                                            timeLeft - timeCost, timeElapsed + timeCost,
                                            outPath, outPathLength,
                                            currentSurvivors,
                                            unlockThreshold,
                                            monsterRow, monsterCol, type, direction, period);

    if (found){
        return true;
    }

    // Try next direction (monster version)
    return tryAllDirectionsWithMonster(map, rows, cols, targetRow, targetCol,
                                       r, c, visited,
                                       currentPath, currentPathLength,
                                       timeLeft, timeElapsed,
                                       dirIndex + 1,
                                       outPath, outPathLength,
                                       currentSurvivors,
                                       unlockThreshold,
                                       monsterRow, monsterCol, type, direction, period);
}

/**
 * TODO: Rescue survivors to unlock shelter (with dynamic load, time cost, and monster threat)
 */
bool rescueSurvivorsWithMonster(int map[][MAX_MAP_SIZE], int rows, int cols, int targetRow, int targetCol, int r, int c, bool visited[][MAX_MAP_SIZE], int currentPath[][2], int currentPathLength,
                                int timeLeft, int timeElapsed,
                                int outPath[][2], int& outPathLength,
                                int currentSurvivors,
                                int unlockThreshold,
                                int monsterRow, int monsterCol, int type,
                                int direction, int period) {
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

    // encounter monster
    if (isMonsterHere(r, c, monsterRow, monsterCol, type, direction, timeElapsed, period, rows, cols)) {
        // First collect survivors on this tile, then apply monster penalty so accounting matches what we display.
        int survivorsHere = map[r][c];
        int collectedSurvivors = currentSurvivors + survivorsHere;

        int potentialSurvivors = collectedSurvivors - MONSTER_DAMAGE_SURVIVORS;
        int potentialTimeLeft = timeLeft - MONSTER_DAMAGE_TIME;
        
        // If either survivors or time would go non-positive, game over
        if (potentialSurvivors <= 0 || potentialTimeLeft <= 0) {
            return false;
        }

        // Take the hit after collection
        int newCurrentSurvivors = potentialSurvivors;

        visited[r][c] = true;
        currentPath[currentPathLength][0] = r;
        currentPath[currentPathLength][1] = c;
        int newPathLength = currentPathLength + 1;

        bool found = tryAllDirectionsWithMonster(map, rows, cols, targetRow, targetCol, r, c, visited, currentPath, newPathLength,
                                                 potentialTimeLeft, timeElapsed + MONSTER_DAMAGE_TIME,
                                                 0,
                                                 outPath, outPathLength,
                                                 newCurrentSurvivors,
                                                 unlockThreshold,
                                                 monsterRow, monsterCol, type, direction, period);

        visited[r][c] = false;
        return found;
    }

    // mark as visited
    visited[r][c] = true;
    currentPath[currentPathLength][0] = r;
    currentPath[currentPathLength][1] = c;
    int newPathLength = currentPathLength + 1;

    int survivor = map[r][c];
    int totalSurvivors = currentSurvivors + survivor;

    // reach the shelter door
    if (r == targetRow && c == targetCol) {
        if (totalSurvivors >= unlockThreshold) {
            outPathLength = newPathLength;
            copyPathRecursive(currentPath, outPath, newPathLength, 0);
            visited[r][c] = false;
            return true;
        }
        else {
            visited[r][c] = false;
            return false;
        }
    }

    // try all four directions
    bool found = tryAllDirectionsWithMonster(map, rows, cols, targetRow, targetCol, r, c, visited, currentPath, newPathLength,
                                             timeLeft, timeElapsed,
                                             0,
                                             outPath, outPathLength,
                                             totalSurvivors,
                                             unlockThreshold,
                                             monsterRow, monsterCol, type, direction, period);

    visited[r][c] = false;
    return found;
}


// Level 3
/**
 * Calculate dynamic time cost
 * Base cost is 1, plus 1 additional time unit for every 4 survivors rescued
 */
int calculateTimeCost(int num_rescued) {
    return 1 + num_rescued / 4;
}

// forward declaration
int rescueSurvivors(int map[][MAX_MAP_SIZE], int rows, int cols, int targetRow, int targetCol, int r, int c, bool visited[][MAX_MAP_SIZE], int currentPath[][2], int currentPathLength,
                    int timeLeft,
                    int bestPath[][2], int& bestPathLength,
                    int& maxSurvivors, int currentSurvivors,
                    int unlockThreshold);

/**
 * TODO: Rescue survivors to unlock shelter (with dynamic load and time cost)
 */
// bestPath: best path found so far; bestPathLength: length of the best path
// maxSurvivors: maximum survivors rescued along the best path; currentSurvivors: survivors rescued so far
int tryAllDirections(int map[][MAX_MAP_SIZE], int rows, int cols, int targetRow, int targetCol, int r, int c, bool visited[][MAX_MAP_SIZE], int currentPath[][2], int currentPathLength,
                     int timeLeft,
                     int bestPath[][2], int& bestPathLength,
                     int dirIndex,
                     int& maxSurvivors, int currentSurvivors,
                     int unlockThreshold) {
    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    // base case: all directions tried
    if (dirIndex >= 4) {
        return UNREACHABLE;
    }

    // calculate new position and time cost
    int newR = r + directions[dirIndex][0];
    int newC = c + directions[dirIndex][1];
    int timeCost = calculateTimeCost(currentSurvivors);

    // Recursive attempt for the new direction
    int result = rescueSurvivors(map, rows, cols, targetRow, targetCol,
                                 newR, newC, visited,
                                 currentPath, currentPathLength,
                                 timeLeft - timeCost,
                                 bestPath, bestPathLength,
                                 maxSurvivors,
                                 currentSurvivors,
                                 unlockThreshold);

    // Try next direction
    int nextResult = tryAllDirections(map, rows, cols, targetRow, targetCol,
                                      r, c, visited,
                                      currentPath, currentPathLength,
                                      timeLeft,
                                      bestPath, bestPathLength,
                                      dirIndex + 1,
                                      maxSurvivors, currentSurvivors,
                                      unlockThreshold);

    // all four directions failed
    if (result == UNREACHABLE && nextResult == UNREACHABLE){
        return UNREACHABLE;
    }
    // current direction failed, return nextdirection result
    else if (result == UNREACHABLE) {
        return nextResult;
    }
    // next direction failed, return current direction result
    else if (nextResult == UNREACHABLE) {
        return result;
    }
    // both succeeded, return the maximum of the two
    else {
        return (result > nextResult) ? result : nextResult;
    }
}


/**
 * TODO: Rescue survivors recursively
 */
int rescueSurvivors(int map[][MAX_MAP_SIZE], int rows, int cols, int targetRow, int targetCol, int r, int c, bool visited[][MAX_MAP_SIZE], int currentPath[][2], int currentPathLength,
                     int timeLeft,
                     int bestPath[][2], int& bestPathLength,
                     int& maxSurvivors, int currentSurvivors,
                     int unlockThreshold){
    if (r < 0 || r >= rows || c < 0 || c >= cols) {
        return UNREACHABLE;
    }
    // out of time
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

    // Mark as visited
    visited[r][c] = true;
    currentPath[currentPathLength][0] = r;
    currentPath[currentPathLength][1] = c;
    int newPathLength = currentPathLength + 1;

    // Collect resources at current position
    int survivor = map[r][c];
    int totalSurvivors = currentSurvivors + survivor;

    // reach the shelter door
    if (r == targetRow && c == targetCol){
        // able to unlock the shelter
        if (totalSurvivors >= unlockThreshold) {
            // update best path if better
            if (bestPathLength == 0 || totalSurvivors > maxSurvivors || maxSurvivors == 0) {
                maxSurvivors = totalSurvivors;
                bestPathLength = newPathLength;
                copyPathRecursive(currentPath, bestPath, newPathLength, 0);
            }

            // Backtrack: unmark visited
            visited[r][c] = false;
            // record total survivors
            return totalSurvivors;
        }
        // cannot unlock the shelter
        else {
            visited[r][c] = false;
            return UNREACHABLE;
        }
    }

    // try all four directions
    int maxFromHere = tryAllDirections(map, rows, cols, targetRow, targetCol, r, c, visited,
                                       currentPath, newPathLength,
                                       timeLeft,
                                       bestPath, bestPathLength,
                                       0,
                                       maxSurvivors, totalSurvivors, unlockThreshold);

    // Backtrack: unmark visited
    visited[r][c] = false;
    
    return maxFromHere;
}

#endif // HELPERS_H
