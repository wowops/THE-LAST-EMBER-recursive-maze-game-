// Level 3 test cases - Maximize survivors with dynamic time cost.

#include "pa2.cpp"

void level3() {
    cout << "\n========================================" << endl;
    cout << "Level 3: The Last Ember — Rescue Mission" << endl;
    cout << "\n========================================" << endl;
    cout << "Goal: Rescue as many survivors as possible while unlocking the shelter" << endl;
    cout << "Settings:" << endl;
    cout << "  1. The shelter door is initially locked and opens only after rescuing enough survivors" << endl;
    cout << "  2. The more survivors you rescue, the slower you move (dynamic time cost)" << endl;
    cout << "  3. Your time is limited. Plan your route carefully!" << endl;
    cout << "  4. The blizzard has buried the path behind you, no turning back!" << endl;
    cout << "  5. Rescue as many survivors as possible before time runs out" << endl;

    cout << "\nEnter startR startC targetR targetC: ";
    int rows, cols, startR, startC, targetR, targetC;
    if (!(cin >> startR >> startC >> targetR >> targetC)) {
        cout << "Error: Invalid Level 3 basic parameters" << endl;
        return;
    }

    cout << "Enter timeLeft unlockThreshold: ";
    int timeLeft, unlockThreshold;
    if (!(cin >> timeLeft >> unlockThreshold)) {
        cout << "Error: Invalid Level 3 time/threshold parameters" << endl;
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

    cout << "\nSurvivor distribution map (numbers = survivors to rescue, # = wall, D = locked shelter door):" << endl;
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
            else {
                cout << setw(2) << resourceMap[i][j] << " ";
            }
        }
        cout << endl;
    }
    cout << "\nNumber of survivors needed to unlock the shelter: " << unlockThreshold << endl;

    bool visited[MAX_MAP_SIZE][MAX_MAP_SIZE] = {false};
    int bestPath[MAX_PATH_LENGTH][2];
    int bestPathLength = 0;
    int currentPath[MAX_PATH_LENGTH][2];
    int currentPathLength = 0;

    int maxSurvivors = 0;

    cout << "\nPlanning route (dynamic time consumption)..." << endl;
    cout << "Initial remaining survival time: " << timeLeft << " units" << endl;
    
    rescueSurvivorsMax(resourceMap, rows, cols, targetR, targetC,
                    startR, startC, visited,
                    currentPath, currentPathLength,
                    timeLeft,
                    bestPath, bestPathLength,
                    maxSurvivors,
                    0,
                    unlockThreshold);

    if (bestPathLength > 0) {
        cout << "\nPath found successfully!" << endl;
        cout << "Successfully rescued: " << maxSurvivors << " survivors (threshold " << unlockThreshold << ")" << endl;
        cout << "Path length: " << bestPathLength << " steps" << endl;
        cout << "Path coordinates: ";
        printPath(bestPath, bestPathLength, 0);
        printLevel2MapWithPath(resourceMap, rows, cols, bestPath, bestPathLength, startR, startC, targetR, targetC);
    }
    else {
        cout << "\nNo viable path found" << endl;
        cout << "Possible reasons:" << endl;
        cout << "  - Insufficient remaining time to reach and rescue enough survivors" << endl;
        cout << "  - Not enough survivors in accessible areas to meet the threshold of " << unlockThreshold << endl;
        cout << "  - All viable paths are blocked by obstacles" << endl;
        cout << "  - Too many survivors rescued causing excessive movement time" << endl;
    }
}
