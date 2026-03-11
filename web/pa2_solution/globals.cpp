// Shared constants.

#ifndef GLOBALS_H
#define GLOBALS_H

const int MAX_PATH_LENGTH = 256;
const int MAX_MAP_SIZE = 16;
const int UNREACHABLE = -100; // sentinel for impossible states (path not found, etc.)
const int WALL_CELL = -7777;
const char WALL_SYMBOL = '#';

// Direction deltas in grading order: up, down, left, right
const int DIRECTIONS[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

#endif // GLOBALS_H
