// Level 2 test cases.

#include "helpers.cpp"

void level2() {
    cout << "\n========================================" << endl;
    cout << "Level 2: The Last Ember — Horror Unleashed" << endl;
    cout << "\n========================================" << endl;
    cout << "Goal: Rescue enough survivors amidst blizzard and monster threats" << endl;
    cout << "Settings:" << endl;
    cout << "  1. The shelter door is initially locked and opens only after rescuing enough survivors" << endl;
    cout << "  2. Your time is limited. Speed up!" << endl;
    cout << "  3. The blizzard has buried the path behind you, no turning back!" << endl;
    cout << "  4. A terrifying monster moves periodically. Watch out!" << endl;

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

    cout << "Enter monsterRow monsterCol monsterType monsterDirection monsterPeriod: ";
    int monsterRow, monsterCol, monsterType, monsterDirection, monsterPeriod;
    if (!(cin >> monsterRow >> monsterCol >> monsterType >> monsterDirection >> monsterPeriod)) {
        cout << "Error: Invalid monster parameters" << endl;
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
    cout << "Monster: row=" << monsterRow << " col=" << monsterCol << " type=" << monsterType << " dir=" << monsterDirection << " period=" << monsterPeriod << endl;

    cout << "\nSurvivor map (numbers = survivors, # = snow wall/chasm, M = monster start, D = locked shelter door):" << endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (i == startR && j == startC) {
                cout << " S ";
            }
            else if (i == targetR && j == targetC) {
                cout << " D ";
            }
            else if (i == monsterRow && j == monsterCol) {
                cout << " M ";
            }
            else if (resourceMap[i][j] == -1) {
                cout << " # ";
            }
            else {
                cout << setw(2) << resourceMap[i][j] << " ";
            }
        }
        cout << endl;
    }

    cout << "\nMonster info:" << endl;
    cout << "  Location: Row " << monsterRow << ", starting from column " << monsterCol << endl;
    cout << "  Period: " << monsterPeriod << " cells" << endl;
    cout << "  Damage: Lose " << MONSTER_DAMAGE_TIME << " time units and " << MONSTER_DAMAGE_SURVIVORS << " survivors upon encounter" << endl;

    cout << "\nSurvivors needed to unlock shelter: " << unlockThreshold << endl;
    cout << "Available time: " << timeLeft << " units" << endl;

    bool visited[MAX_MAP_SIZE][MAX_MAP_SIZE] = {false};
    int path[MAX_PATH_LENGTH][2];
    int pathLength = 0;
    int currentPath[MAX_PATH_LENGTH][2];

    // Use monster-aware search
    bool found = rescueSurvivorsWithMonster(resourceMap, rows, cols, targetR, targetC,
                                            startR, startC, visited,
                                            currentPath, 0,
                                            timeLeft, 0,
                                            path, pathLength,
                                            0,
                                            unlockThreshold,
                                            monsterRow, monsterCol, monsterType, monsterDirection, monsterPeriod);

    if (found && pathLength > 0) {
        cout << "\nValid path found!" << endl;
        cout << "Path length: " << pathLength << endl;
        
        // Calculate survivors with monster damage
        int rescued = calculate_num_survivors(resourceMap, rows, cols, path, pathLength);
        int totalTime = 0;
        int survivorsLostToMonster = 0;
        
        // Check for monster encounters along the path
        for (int step = 0; step < pathLength; step++) {
            int r = path[step][0];
            int c = path[step][1];
            
            bool monsterPresent = isMonsterHere(r, c, monsterRow, monsterCol, monsterType, monsterDirection, totalTime, monsterPeriod, rows, cols);
            
            if (monsterPresent) {
                survivorsLostToMonster += MONSTER_DAMAGE_SURVIVORS;
            }
            
            totalTime++;
        }
        
        rescued -= survivorsLostToMonster;
        
        cout << "Survivors rescued: " << rescued << endl;
        cout << "Path: ";
        printPath(path, pathLength);
        printLevel2MapWithPath(resourceMap, rows, cols, path, pathLength, startR, startC, targetR, targetC);
    }
    else {
        cout << "\nNo valid path found!" << endl;
    }
}