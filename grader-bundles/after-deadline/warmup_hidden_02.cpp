#include <sstream>
#include <string>

#include "pa2.cpp"

int main() {
    int path[MAX_PATH_LENGTH][2] = {
        {0, 0},
        {0, 1},
        {1, 1},
        {1, 2},
        {2, 2}
    };

    std::ostringstream capture;
    std::streambuf* old = std::cout.rdbuf(capture.rdbuf());
    printPath(path, 5);
    std::cout.rdbuf(old);

    if (capture.str() != "(0,0) -> (0,1) -> (1,1) -> (1,2) -> (2,2)\n") {
        return 1;
    }

    int map[MAX_MAP_SIZE][MAX_MAP_SIZE] = {};
    map[0][0] = 3;
    map[0][1] = 0;
    map[1][1] = 6;
    map[1][2] = -4;
    map[2][2] = 9;

    int total = calculate_num_survivors(map, 3, 3, path, 5);
    if (total != 14) {
        return 2;
    }

    return 0;
}
