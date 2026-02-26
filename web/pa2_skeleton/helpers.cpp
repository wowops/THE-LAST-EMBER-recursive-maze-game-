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
 * TODO: print the path (only the coordinates) recursively
 * 
 * Hints: 
 * - Figure out the base case(s).
 * - Print the current coordinate.
 * - Remember to add " -> ".
 * - Recur for the next iteration.
 */
// path: a 2D array storing the path coordinates; pathLength: length of the path; index: current index to print
void printPath(int path[][2], int pathLength, int index = 0){
    // TODO1: Implement this function
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
 * 
 * Hints: 
 * - Figure out the base case.
 * - Copy the current row and column.
 * - Recur for the next iteration.
 */
// source: source path; copy: destination path; pathLength: length of the path; index: current index to copy
void copyPathRecursive(int source[][2], int copy[][2], int pathLength, int index = 0){
    // TODO2: Implement this function
}

/**
 * TODO: Level 1: Simple path finding using recursion
 * 
 * Hints: 
 * - Check boundaries, chasms, visited, and path length limits and use them as base cases.
 * - Mark current cell as visited and add to currentPath.
 * - If at target, copy path and return true.
 * - Try four directions recursively (fixed order for grading: up, down, left, right).
 * - Remember to backtrack.
 */
// r: current row; c: current column; visited: visited marks
// currentPath: path has been explored; currentPathLength: length of current path
// returns: true if path found, false otherwise
bool findPathSimple(int map[][MAX_MAP_SIZE], int rows, int cols, int path[][2], int& pathLength, int targetRow, int targetCol,
                    int r, int c, bool visited[][MAX_MAP_SIZE],
                    int currentPath[][2], int currentPathLength){
    // TODO3: Implement this function
}


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
 * 
 * Hints: 
 * - Figure out the base case.
 * - Get current survivor count.
 * - Recur for the next iteration.
 */
// index: current index to process
int calculate_num_survivors(int map[][MAX_MAP_SIZE], int rows, int cols, int path[][2], int pathLength, int index = 0) {
    // TODO4: Implement this function
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

// Level 2: Horror Unleashed
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
 * 
 * Hints: 
 * - Use an array of direction (fixed order for grading: up, down, left, right).
 * - Base case: all directions tried.
 * - Calculate new position and time cost.
 * - Avoid the monster if needed.
 * - Recur into rescueSurvivorsWithMonster for the new position.
 * - Make use of the return value.
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
    // TODO5: Implement this function
}

/**
 * TODO: Rescue survivors to unlock shelter (with monster threat)
 * 
 * Hints: 
 * - Check boundaries, time, chasms, visited, path length and use them as base cases.
 * - Handle monster encounters.
 * - Mark as visited and add to path.
 * - Add current survivors. Also check if at target and enough to unlock.
 * - Call tryAllDirectionsWithMonster to explore moves.
 * - Remember to backtrack.
 */
bool rescueSurvivorsWithMonster(int map[][MAX_MAP_SIZE], int rows, int cols, int targetRow, int targetCol, int r, int c, bool visited[][MAX_MAP_SIZE], int currentPath[][2], int currentPathLength,
                                int timeLeft, int timeElapsed,
                                int outPath[][2], int& outPathLength,
                                int currentSurvivors,
                                int unlockThreshold,
                                int monsterRow, int monsterCol, int type,
                                int direction, int period) {
    // TODO6: Implement this function
}


// Level 3: Maximum Rescue
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
 * TODO: Try all four directions recursively (maximum rescue version)
 * 
 * Hints: 
 * - Use direction deltas array.
 * - Base case: all directions tried.
 * - For each direction, calculate new position and time cost.
 * - Recur into rescueSurvivors, then try next direction.
 * - Compare the results
 * - Use the const int variable UNREACHABLE to indicate a path cannot satisfy the constraints.
 */
// bestPath: best path found so far; bestPathLength: length of the best path
// maxSurvivors: maximum survivors rescued along the best path; currentSurvivors: survivors rescued so far
int tryAllDirections(int map[][MAX_MAP_SIZE], int rows, int cols, int targetRow, int targetCol, int r, int c, bool visited[][MAX_MAP_SIZE], int currentPath[][2], int currentPathLength,
                     int timeLeft,
                     int bestPath[][2], int& bestPathLength,
                     int dirIndex,
                     int& maxSurvivors, int currentSurvivors,
                     int unlockThreshold) {
    // TODO7: Implement this function
}


/**
 * TODO: Rescue survivors recursively (maximum rescue version)
 * 
 * Hints: 
 * - there's no need for monster checks.
 * - Remember to update best path and max survivors when reaching the target.
 */
int rescueSurvivors(int map[][MAX_MAP_SIZE], int rows, int cols, int targetRow, int targetCol, int r, int c, bool visited[][MAX_MAP_SIZE], int currentPath[][2], int currentPathLength,
                     int timeLeft,
                     int bestPath[][2], int& bestPathLength,
                     int& maxSurvivors, int currentSurvivors,
                     int unlockThreshold){
    // TODO8: Implement this function
}

#endif // HELPERS_H