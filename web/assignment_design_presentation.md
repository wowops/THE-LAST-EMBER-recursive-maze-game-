# COMP2011 PA2 Presentation Notes

## 1. What this assignment is

This assignment is a text-based maze game called **The Last Ember**. Its main teaching goal is not the game itself. The real teaching goal is to force students to practice:

- recursive thinking
- function decomposition
- DFS-style backtracking
- state management without loops inside the required functions
- handling constraints that change the search objective

The assignment is appropriate for a Year 1-2 student because it starts from very small recursive tasks, then builds up into full recursive search.

The progression is intentional:

1. `printPath`
2. `copyPathRecursive`
3. `calculate_num_survivors`
4. Level 1 shortest path
5. Level 2 shortest valid path under extra constraints
6. Level 3 best path under optimization constraints

So the assignment scaffolds students from simple recursion on arrays to recursive graph search with backtracking.

## 2. HTML flow of the assignment page

The main spec is in [index.html](/Users/andy/Desktop/code/comp2011pa2/THE-LAST-EMBER-recursive-maze-game-/web/index.html).

The page flow is:

1. **Header / setup**
2. **Introduction**
3. **Skeleton code rules**
4. **Complete examples**
5. **Assumptions**
6. **Coding tasks**
7. **Compilation / debugging reminders**
8. **Grading / academic integrity / submission / FAQs**

### 2.1 Header / setup

At the top of the HTML, there is a small JavaScript block that fills in:

- assignment title
- TA contacts
- compile command
- run command
- submission filename
- deadline placeholder

This part is mostly administrative. It helps students know how to compile and what filename matters for submission.

### 2.2 Introduction

The introduction explains the game in three levels:

- **Level 1**: simple shortest path to the beacon
- **Level 2**: shortest valid path with time limit and survivor threshold
- **Level 3**: maximize rescued survivors with dynamic movement cost

This is good educational design because each new level changes only part of the problem:

- Level 1 adds pure pathfinding
- Level 2 adds feasibility constraints
- Level 3 changes the objective from shortest path to optimization

So students can reuse the same recursive pattern, but must adapt the stopping conditions and update logic.

### 2.3 Skeleton code section

This section tells students:

- do not start from scratch
- search for `TODO`
- do not add extra headers
- do not create extra global variables
- do not add extra `cin` / `cout`
- do not use loops in the required functions

This is important for the design because it narrows the solution space. The assignment is not only testing correctness. It is also testing whether students can express traversal and search **recursively**, not iteratively.

### 2.4 Complete examples

The examples show exactly how the game behaves for one sample of each level.

Pedagogically, the examples do several things:

- define the exact output format
- show what a path looks like
- show how maps are printed
- show how the path is visualized
- show the difference between shortest path and maximum rescue

This is important because students often misunderstand path length, path printing, or when a path is considered valid.

### 2.5 Assumptions

The assumptions make the recursive search manageable:

- map size is at most `16 x 16`
- path length is at most `256`
- no loops allowed in required functions
- at most one valid path in all 3 levels

The line "at most one valid path" is a major simplification. It means the marker does not need to test complex tie-breaking among multiple equally valid solutions.

### 2.6 Coding tasks

This is the heart of the spec. The TODO functions in `pa2.cpp` are listed in increasing difficulty.

The structure is strong because the tasks are grouped into:

- recursion warm-up
- Level 1 DFS foundation
- Level 2 advanced DFS with penalty tiles
- Level 3 DFS optimization

### 2.7 Compilation and debugging reminders

The HTML also warns students about:

- uninitialized variables
- exact output formatting
- `cerr`
- non-zero return codes
- running sample tests

This is useful because beginner students often lose marks from formatting or uninitialized state rather than the main recursion itself.

## 3. Core program structure

The runtime entry is [testcases.cpp](/Users/andy/Desktop/code/comp2011pa2/THE-LAST-EMBER-recursive-maze-game-/web/pa2_solution/testcases.cpp).

The player chooses:

- `1` for Level 1
- `2` for Level 2
- `3` for Level 3
- `q` to quit

Then the corresponding level file runs:

- [level1.cpp](/Users/andy/Desktop/code/comp2011pa2/THE-LAST-EMBER-recursive-maze-game-/web/pa2_solution/level1.cpp)
- [level2.cpp](/Users/andy/Desktop/code/comp2011pa2/THE-LAST-EMBER-recursive-maze-game-/web/pa2_solution/level2.cpp)
- [level3.cpp](/Users/andy/Desktop/code/comp2011pa2/THE-LAST-EMBER-recursive-maze-game-/web/pa2_solution/level3.cpp)

All TODO logic lives in:

- [pa2.cpp](/Users/andy/Desktop/code/comp2011pa2/THE-LAST-EMBER-recursive-maze-game-/web/pa2_solution/pa2.cpp)

Shared constants are in:

- [globals.cpp](/Users/andy/Desktop/code/comp2011pa2/THE-LAST-EMBER-recursive-maze-game-/web/pa2_solution/globals.cpp)

Important constants:

- `MAX_MAP_SIZE = 16`
- `MAX_PATH_LENGTH = 256`
- `UNREACHABLE = -100`
- `DIRECTIONS = {{-1,0}, {1,0}, {0,-1}, {0,1}}`

The direction order is:

1. up
2. down
3. left
4. right

That order matters for deterministic grading.

## 4. High-level teaching design

The assignment is designed around one repeated pattern:

1. check whether the current state is invalid
2. record current state
3. check whether current state satisfies the goal
4. recursively explore next choices
5. undo the current state before returning

This is classic recursive backtracking.

What changes between levels is:

- what counts as an invalid state
- what counts as success
- what value is being optimized
- what state variables must be carried down the recursion

## 5. Function-by-function explanation

Below is the presentation-ready explanation of every TODO function from `pa2.cpp`.

---

## 5.1 `printPath`

Function:

- `void printPath(int path[][2], int pathLength, int index = 0)`

### What this function is doing

This prints the coordinates stored in the `path` array recursively.

Example output:

`(0,0) -> (0,1) -> (1,1)`

### What we want to test

This is the simplest recursion task. It checks whether students understand:

- base case
- recursive step
- processing one array element at a time
- formatting while recursing

### Conditions / cases inside

- If `pathLength == 0`, the path is empty.
- If `index >= pathLength`, recursion should stop.
- Otherwise print `path[index]`, then recurse to `index + 1`.
- Only print `" -> "` if this is not the last coordinate.

### Recursive pattern

- base case: stop when index reaches the end
- recursive step: print current coordinate and move forward by 1

### Common student mistakes

- printing an extra arrow at the end
- forgetting the base case
- forgetting to increase `index`
- printing the whole path with a loop, which violates the recursion requirement

---

## 5.2 `copyPathRecursive`

Function:

- `void copyPathRecursive(int source[][2], int copy[][2], int pathLength, int index = 0)`

### What this function is doing

This copies path coordinates from one 2D array to another recursively.

### What we want to test

This checks whether students can:

- recursively traverse a structured array
- copy state from one recursive search buffer into a best-answer buffer
- understand why DFS often needs a temporary path and a saved best path

### Conditions / cases inside

- If `index >= pathLength`, stop copying.
- Otherwise copy:
  - `copy[index][0] = source[index][0]`
  - `copy[index][1] = source[index][1]`
- Then recurse with `index + 1`.

### Why this matters in the assignment

During DFS, `currentPath` changes constantly. When a valid solution is found, it must be saved immediately into another array. Otherwise later recursion will overwrite it.

### Common student mistakes

- copying only row or only column
- not stopping at `pathLength`
- trying to use loops

---

## 5.3 `calculate_num_survivors`

Function:

- `int calculate_num_survivors(int map[][MAX_MAP_SIZE], int rows, int cols, int path[][2], int pathLength, int index = 0)`

### What this function is doing

This sums the values of the map cells visited along a path.

### What we want to test

This tests recursive accumulation:

- take one item
- combine it with the result of the smaller subproblem

### Conditions / cases inside

- If `index >= pathLength`, return `0`.
- Otherwise:
  - get `r = path[index][0]`
  - get `c = path[index][1]`
  - return `map[r][c] + recursive_result`

### Why it is educationally useful

It is a direct bridge from simple array recursion to state-based search. It also prepares students to think of the path as meaningful data, not just positions.

### Common student mistakes

- using `rows` and `cols` incorrectly
- forgetting that path entries are coordinates
- returning the wrong base value

---

## 5.4 `shortestPath`

Function:

- `bool shortestPath(...)`

This is the main Level 1 DFS state function.

### What this function is doing

It processes the current cell:

- checks whether the current state is invalid
- marks the cell as visited
- appends the cell to the current path
- checks whether the target is reached
- if not, delegates direction exploration to `shortestPathTryDirections`
- backtracks before returning

### What we want to test

This is the first full backtracking problem. It tests:

- DFS state design
- pruning invalid states
- visited marking
- backtracking
- path recording
- updating the shortest answer

### Conditions / cases inside

The important conditions are:

- `r < 0 || r >= rows || c < 0 || c >= cols`
  - outside the map, invalid
- `map[r][c] == -1`
  - wall or chasm, invalid
- `visited[r][c]`
  - revisit not allowed
- `currentPathLength >= MAX_PATH_LENGTH`
  - path buffer overflow, invalid

After passing those checks:

- mark visited
- save `(r, c)` into `currentPath`
- increase path length

Then success condition:

- `r == targetRow && c == targetCol`

If target is reached:

- compare current path length with best path length
- if shorter, copy current path into output path

If target is not reached:

- recursively try all directions

Before returning:

- `visited[r][c] = false`

This last step is the key backtracking step.

### Why this design is good

The function cleanly separates:

- **cell-state logic** in `shortestPath`
- **direction iteration logic** in `shortestPathTryDirections`

That is a strong design choice because the spec forbids loops. So instead of a `for` loop over directions, the assignment forces a second recursive helper.

### Common student mistakes

- not unmarking `visited`
- returning immediately after the first path instead of checking all branches
- forgetting to add current cell into `currentPath`
- not updating the shortest path correctly

---

## 5.5 `shortestPathTryDirections`

Function:

- `bool shortestPathTryDirections(...)`

This is the recursive direction enumerator for Level 1.

### What this function is doing

It tries the 4 possible moves one by one recursively:

1. compute next cell from `DIRECTIONS[dirIndex]`
2. call `shortestPath` on that next cell
3. recurse again with `dirIndex + 1`
4. combine both results with `||`

### What we want to test

This tests whether students can replace a normal loop like:

```cpp
for (int i = 0; i < 4; i++)
```

with recursion.

### Conditions / cases inside

- Base case: `dirIndex >= 4`
  - all directions have been tried
  - return `false`

Otherwise:

- compute `newR`, `newC`
- try current direction
- try remaining directions
- return `foundThis || foundNext`

### Why this function exists separately

Without this separation, students would usually hide the direction loop inside `shortestPath`, which would defeat the assignment's recursion objective.

---

## 5.6 `rescueSurvivorsWithPenalty`

Function:

- `bool rescueSurvivorsWithPenalty(...)`

This is the main Level 2 DFS state function.

### What this function is doing

It extends Level 1 by adding:

- time limit
- survivor accumulation
- penalty tiles with negative values
- unlock threshold at the target

The function:

1. prunes invalid states
2. records the current cell
3. updates current survivor total using the current tile value
4. if at target, checks whether survivor threshold is met
5. if valid, updates the shortest valid path
6. otherwise explores all directions recursively
7. backtracks before returning

### What we want to test

This level tests whether students can adapt an existing DFS design when more state variables are introduced.

New reasoning being tested:

- not every path to the target is valid
- a path can be geometrically reachable but logically invalid
- success now depends on both position and resource state

### Conditions / cases inside

Invalid-state pruning:

- out of bounds
- `timeLeft <= 0`
- wall `-1`
- visited already
- path length overflow

State update:

- add current cell value to `currentSurvivors`
- values may be positive or negative

Target condition:

- if current cell is the target:
  - only succeed if `totalSurvivors >= unlockThreshold`
  - if successful, save path only if it is shorter than the previous best

Backtracking:

- unmark visited before return

### Important teaching point

Level 1 success condition is:

- "Did I reach the destination?"

Level 2 success condition is:

- "Did I reach the destination with enough survivors and within time?"

That change is conceptually important. It teaches that recursive search often depends on more than location.

### Common student mistakes

- forgetting to add the current tile value
- forgetting that negative tiles reduce the total
- checking threshold too early instead of at the target
- not comparing path lengths when multiple valid paths are explored

---

## 5.7 `tryAllDirectionsWithPenalty`

Function:

- `bool tryAllDirectionsWithPenalty(...)`

This is the Level 2 direction enumerator.

### What this function is doing

It recursively tries all four moves from the current cell, just like Level 1, but now each move also reduces `timeLeft`.

In the provided solution:

- each move costs `1` time unit

### What we want to test

This tests whether students can:

- carry extra state through recursive branch calls
- reduce time correctly per move
- continue searching all branches even after finding one valid answer, because another branch may be shorter

### Conditions / cases inside

- Base case: `dirIndex >= 4`
- For current direction:
  - compute next row/col
  - reduce time by move cost
  - recurse into `rescueSurvivorsWithPenalty`
- Then recurse with `dirIndex + 1`
- Return logical OR of the two results

### Design point worth mentioning in presentation

This pair of functions (`rescueSurvivorsWithPenalty` and `tryAllDirectionsWithPenalty`) mirrors the Level 1 design almost exactly. That is good assignment design because students see that the recursive framework stays stable while the constraints evolve.

---

## 5.8 `rescueSurvivorsMax`

Function:

- `int rescueSurvivorsMax(...)`

This is the main Level 3 DFS state function.

### What this function is doing

It changes the objective from:

- shortest valid path

to:

- maximum rescued survivors among valid paths

The function:

1. prunes impossible states
2. records current cell and path
3. adds survivors from current cell
4. if target is reached, checks unlock threshold
5. if valid, updates `maxSurvivors` and `bestPath`
6. otherwise delegates branch exploration to `tryAllDirectionsMax`
7. backtracks before returning

### What we want to test

This is the most advanced conceptual step in the assignment. It tests:

- recursive optimization
- using a sentinel value (`UNREACHABLE`) for impossible branches
- distinguishing feasibility from objective value
- carrying and updating a global best answer through recursion

### Conditions / cases inside

Invalid-state pruning:

- out of bounds
- `timeLeft <= 0`
- wall `-1`
- revisiting a cell
- path overflow

State update:

- record current cell
- `totalSurvivors = currentSurvivors + map[r][c]`

Target condition:

- if at target and `totalSurvivors >= unlockThreshold`
  - compare with `maxSurvivors`
  - update best result if larger
  - copy best path
  - return survivor total
- otherwise return `UNREACHABLE`

### Major conceptual shift from Level 2

In Level 2, the question is:

- "Is there a shortest valid path?"

In Level 3, the question is:

- "Among all valid paths, which one rescues the most survivors?"

So Level 3 changes from a boolean search problem to an optimization problem returning an integer score.

### Common student mistakes

- still thinking in terms of shortest path
- forgetting to update `maxSurvivors`
- using `false`/`true` style returns instead of a numeric best-result value
- not handling unreachable branches consistently

---

## 5.9 `tryAllDirectionsMax`

Function:

- `int tryAllDirectionsMax(...)`

This is the Level 3 direction enumerator.

### What this function is doing

It recursively tries each direction and returns the best survivor total reachable from those branches.

It also applies **dynamic time cost**:

- `timeCost = 1 + currentSurvivors / 4`

This means the more survivors you already carry, the more expensive the next move becomes.

### What we want to test

This tests whether students can:

- propagate optimization values through recursion
- combine branch results correctly
- use a sentinel such as `UNREACHABLE`
- understand that state variables affect future transition cost

### Conditions / cases inside

- Base case: `dirIndex >= 4`
  - return `UNREACHABLE`

For current direction:

- compute next cell
- compute dynamic time cost from `currentSurvivors`
- recurse into `rescueSurvivorsMax`

Then:

- recurse into remaining directions with `dirIndex + 1`

Combine results:

- if both unreachable, return `UNREACHABLE`
- if only one is reachable, return the reachable one
- if both reachable, return the larger survivor total

### Why this is a strong final task

This task makes students manage:

- recursive branching
- dynamic transition cost
- optimization return values
- sentinel handling

That is a good capstone for an introductory recursion assignment.

## 6. How the recursive pairs work together

The design intentionally uses two-function recursion pairs.

### Level 1

- `shortestPath`
  - handles the current cell
- `shortestPathTryDirections`
  - handles the 4 possible moves

### Level 2

- `rescueSurvivorsWithPenalty`
  - handles the current cell plus survivor/time logic
- `tryAllDirectionsWithPenalty`
  - handles the 4 possible moves

### Level 3

- `rescueSurvivorsMax`
  - handles current cell plus optimization update
- `tryAllDirectionsMax`
  - handles the 4 possible moves and returns the best score

This is one of the best design features of the assignment. It keeps the recursive structure consistent, which helps students focus on the changing constraints instead of relearning a new framework each time.

## 7. What each level is testing

### Level 1 is testing

- basic DFS
- recursive exploration without loops
- backtracking with `visited`
- shortest-path update logic

### Level 2 is testing

- DFS with additional state
- feasibility under resource constraints
- negative tile handling
- shortest valid path, not merely reachable path

### Level 3 is testing

- DFS optimization
- dynamic move cost
- comparing branch results
- use of sentinel return values

## 8. Important conditions to highlight in the presentation

These are the conditions worth explicitly saying during presentation because they define the assignment's logic.

### Universal pruning conditions

Used in most DFS state functions:

- out of bounds
- wall / blocked cell
- visited already
- path too long

### Level-specific conditions

Level 1:

- no extra resource constraints
- only needs to reach target

Level 2:

- time must remain positive
- total survivors at target must reach threshold
- negative cells can reduce progress

Level 3:

- time must remain positive
- threshold only matters when reaching target
- move cost increases with rescued survivors
- objective is maximum survivors, not minimum path length

## 9. Why this assignment is well designed for recursion

From a teaching perspective, the assignment is strong because:

- it starts small
- it reuses one recursive pattern repeatedly
- it makes recursion necessary by forbidding loops
- it introduces new constraints gradually
- it connects recursion to an interesting game narrative

It also gives students multiple dimensions of reasoning:

- structural recursion on arrays
- DFS recursion on maps
- recursive backtracking
- recursive optimization

## 10. Possible talking points for tomorrow's presentation

You can present the assignment in this order:

1. The assignment teaches recursion through a maze game.
2. The HTML is structured from rules and examples into progressively harder coding tasks.
3. The first 3 TODOs are warm-up tasks on arrays and path data.
4. Level 1 introduces recursive DFS backtracking.
5. Level 2 keeps the same recursive framework but adds validity constraints.
6. Level 3 changes the goal from shortest path to best rescue result.
7. The paired-function design is intentional to replace loops over directions with recursion.
8. The assignment tests not only correctness, but recursive decomposition and state management.

## 11. Notes about files in the repo

For your explanation, the most important files are:

- spec: [index.html](/Users/andy/Desktop/code/comp2011pa2/THE-LAST-EMBER-recursive-maze-game-/web/index.html)
- student template: [pa2.cpp](/Users/andy/Desktop/code/comp2011pa2/THE-LAST-EMBER-recursive-maze-game-/web/pa2_skeleton/pa2.cpp)
- reference solution: [pa2.cpp](/Users/andy/Desktop/code/comp2011pa2/THE-LAST-EMBER-recursive-maze-game-/web/pa2_solution/pa2.cpp)
- level drivers:
  - [level1.cpp](/Users/andy/Desktop/code/comp2011pa2/THE-LAST-EMBER-recursive-maze-game-/web/pa2_solution/level1.cpp)
  - [level2.cpp](/Users/andy/Desktop/code/comp2011pa2/THE-LAST-EMBER-recursive-maze-game-/web/pa2_solution/level2.cpp)
  - [level3.cpp](/Users/andy/Desktop/code/comp2011pa2/THE-LAST-EMBER-recursive-maze-game-/web/pa2_solution/level3.cpp)
- shared constants: [globals.cpp](/Users/andy/Desktop/code/comp2011pa2/THE-LAST-EMBER-recursive-maze-game-/web/pa2_solution/globals.cpp)

I did not modify the student skeleton. This markdown file is separate from the template.
