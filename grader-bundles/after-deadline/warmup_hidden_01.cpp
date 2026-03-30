#include <sstream>
#include <string>

#include "pa2.cpp"

int main() {
    int source[MAX_PATH_LENGTH][2] = {
        {2, 3},
        {2, 4},
        {3, 4},
        {4, 4},
        {4, 5},
        {5, 5}
    };
    int copied[MAX_PATH_LENGTH][2] = {};
    copyPathRecursive(source, copied, 6);

    for (int i = 0; i < 6; i++) {
        if (copied[i][0] != source[i][0] || copied[i][1] != source[i][1]) {
            return 1;
        }
    }

    int map[MAX_MAP_SIZE][MAX_MAP_SIZE] = {};
    map[2][3] = 4;
    map[2][4] = -2;
    map[3][4] = 7;
    map[4][4] = 0;
    map[4][5] = 5;
    map[5][5] = -1;

    int total = calculate_num_survivors(map, 6, 6, copied, 6);
    if (total != 13) {
        return 2;
    }

    return 0;
}
