// Level 1 test cases.

#include "pa2.cpp"

void level1() {
    cout << "\n========================================" << endl;
    cout << "Level 1: Traverse the Blizzard to Find the Beacon" << endl;
    cout << "\n========================================" << endl;
    cout << "Goal: Reach the distress beacon from the temporary camp at (0,0) by finding the shortest viable path." << endl;

    cout << "\nEnter startR startC targetR targetC: ";
    int rows, cols, startR, startC, targetR, targetC;
    if (!(cin >> startR >> startC >> targetR >> targetC)) {
        cout << "Error: Invalid Level 1 parameters" << endl;
        return;
    }

    cout << "Enter map file name (e.g., map01 or map01.txt): ";
    string mapName;
    cin >> mapName;

    int map[MAX_MAP_SIZE][MAX_MAP_SIZE];
    if (!readMapFromFile(mapName, map, rows, cols)) {
        return;
    }

    cout << "\nTest case: " << rows << " " << cols << " " << startR << " " << startC << " " << targetR << " " << targetC << endl;
    cout << "\nWasteland terrain (. = passable, # = wall):" << endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (i == startR && j == startC) {
                cout << " S ";
            }
            else if (i == targetR && j == targetC) {
                cout << " E ";
            } else if (map[i][j] == WALL_CELL) {
                cout << " # ";
            } else {
                cout << " . ";
            }
        }
        cout << endl;
    }

    bool visited[MAX_MAP_SIZE][MAX_MAP_SIZE] = {false};
    int path[MAX_PATH_LENGTH][2];
    int pathLength = 0;
    int currentPath[MAX_PATH_LENGTH][2];
    int currentPathLength = 0;

    cout << "\nSearching for shortest path..." << endl;
    bool found = shortestPath(map, rows, cols, path, pathLength, targetR, targetC,
                                startR, startC, visited,
                                currentPath, currentPathLength);

    if (found) {
        cout << "\nPath found successfully!" << endl;
        cout << "Path length: " << pathLength << " steps" << endl;
        printPath(path, pathLength, 0);
        printLevel1MapWithPath(path, pathLength,
                               map, rows, cols,
                               startR, startC, targetR, targetC);
    } else {
        cout << "\nNo viable path found" << endl;
    }
}
