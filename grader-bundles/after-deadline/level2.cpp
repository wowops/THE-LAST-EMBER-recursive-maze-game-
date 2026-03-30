// Level 2 test cases (Penalty Tiles system).

#include "pa2.cpp"

void level2() {
    cout << "\n========================================" << endl;
    cout << "Level 2: The Last Ember — Hazardous Journey" << endl;
    cout << "\n========================================" << endl;
    cout << "Goal: Rescue enough survivors and find the shortest safe path" << endl;
    cout << "Settings:" << endl;
    cout << "  1. The shelter door is initially locked and can only be opened by rescuing enough survivors" << endl;
    cout << "  2. Your time is limited. Speed up!" << endl;
    cout << "  3. The blizzard has buried the path behind you, no turning back!" << endl;
    cout << "  4. Some tiles contain hazards (negative values) that reduce rescued survivors" << endl;
    cout << "  5. Find the SHORTEST valid path to the shelter while meeting survivor requirements" << endl;

    cout << "\nEnter startR startC targetR targetC: ";
    int rows, cols, startR, startC, targetR, targetC;
    if (!(cin >> startR >> startC >> targetR >> targetC)) {
        cout << "Error: Invalid Level 2 basic parameters" << endl;
        return;
    }

    cout << "Enter timeLeft unlockThreshold: ";
    int timeLeft, unlockThreshold;
    if (!(cin >> timeLeft >> unlockThreshold)) {
        cout << "Error: Invalid Level 2 time/threshold parameters" << endl;
        return;
    }

    cout << "Enter map file name (e.g., map01 or map01.txt): ";
    string mapName;
    cin >> mapName;

    int resourceMap[MAX_MAP_SIZE][MAX_MAP_SIZE];
    if (!readMapFromFile(mapName, resourceMap, rows, cols)) {
        return;
    }

    cout << "\nTest case: " << rows << " " << cols << " " << startR << " " << startC << " " << targetR << " " << targetC << " " << timeLeft << " " << unlockThreshold << endl;

    cout << "\nMap (positive = survivors to rescue, negative = hazard points lost, # = wall, S = start, D = locked shelter door):" << endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (i == startR && j == startC) {
                cout << " S ";
            }
            else if (i == targetR && j == targetC) {
                cout << " D ";
            }
            else if (resourceMap[i][j] == WALL_CELL) {
                cout << " # ";
            }
            else if (resourceMap[i][j] < 0) {
                cout << setw(2) << resourceMap[i][j] << " ";
            }
            else {
                cout << setw(2) << resourceMap[i][j] << " ";
            }
        }
        cout << endl;
    }
    cout << "\nSurvivors needed to unlock the shelter: " << unlockThreshold << endl;
    cout << "Available time: " << timeLeft << " units" << endl;

    bool visited[MAX_MAP_SIZE][MAX_MAP_SIZE] = {false};
    int bestPath[MAX_PATH_LENGTH][2];
    int bestPathLength = 0;
    int currentPath[MAX_PATH_LENGTH][2];
    int currentPathLength = 0;
    // int maxSurvivors = 0;

    cout << "\nPlanning route (must reach unlock threshold, avoid hazards, find shortest path)..." << endl;
    cout << "Initial remaining time: " << timeLeft << " units" << endl;
    
    rescueSurvivorsWithPenalty(resourceMap, rows, cols, targetR, targetC,
                               startR, startC, visited,
                               currentPath, currentPathLength,
                               timeLeft,
                               bestPath, bestPathLength,
                               0,
                               unlockThreshold,
                               bestPathLength);

    if (bestPathLength > 0) {
        cout << "\nPath found successfully!" << endl;
        cout << "Path length: " << bestPathLength << " steps (SHORTEST)" << endl;
        cout << "Path coordinates: ";
        printPath(bestPath, bestPathLength, 0);
        cout << endl;
        printLevel1MapWithPath(bestPath, bestPathLength, resourceMap, rows, cols, startR, startC, targetR, targetC);
    }
    else {
        cout << "\nNo viable path found" << endl;
        cout << "Possible reasons:" << endl;
        cout << "  - Insufficient remaining time to reach the shelter" << endl;
        cout << "  - Not enough accessible survivors to meet the threshold of " << unlockThreshold << endl;
        cout << "  - All viable paths are blocked by obstacles" << endl;
    }
}
