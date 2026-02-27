# THE LAST EMBER - Recursive Maze Game

## Level 1: Simple Pathfinding

Objective:
- Find the shortest valid path (fewest steps) from start to exit.

Map Elements:
- 'S' = Starting position
- 'E' = Exit position
- '.' = Passable terrain
- '#' = Chasm (impassable)

Rules:
- Move in 4 directions: up, down, left, right
- Cannot revisit cells
- Cannot pass through chasms
- Must stay within map boundaries

Success:
- Shortest valid path (fewest steps) from start to exit

## Level 2: Shortest Path With Penalty Tiles

Objective:
- Rescue enough survivors to unlock the exit and find the shortest valid path.

Map Elements:
- 'S' = Starting position
- 'D' = Locked exit (requires survivors to unlock)
- '#' = Impassable
- Numbers = Survivor count at location (can be negative for hazard penalties)

Movement:
- 4 directions: up, down, left, right
- Cannot revisit cells
- Each step costs 1 time unit
- Must stay within boundaries

Survivor System:
- Collect survivors as you traverse cells
- Hazard tiles (negative values) reduce the rescued survivor total
- Need >= unlock threshold to open exit

Success Conditions:
- rescued_survivors >= unlockThreshold
- Reach exit before time runs out
- Among valid paths, pick the shortest path

## Level 3: Optimization With Dynamic Time Cost

Objective:
- Maximize survivors rescued while meeting unlock threshold and time constraint.

Map Elements:
- 'S' = Starting position
- 'D' = Locked exit
- '#' = Impassable
- Numbers = Survivor count at location

Movement:
- 4 directions: up, down, left, right
- Cannot revisit cells
- Must stay within boundaries

Dynamic Time Cost:
- Base cost per step: 1 time unit
- Additional cost: +1 time unit per 4 survivors rescued
- Formula: time_cost = 1 + (total_survivors_rescued / 4)
- Carrying more survivors slows movement

Survivor System:
- Collect survivors as you traverse cells
- Need >= unlock threshold to open exit
- Goal: maximize total survivors (not just meet threshold)

Success Conditions:
- rescued_survivors >= unlockThreshold
- Reach exit before time runs out
- Among valid paths, maximize survivor count

Strategy:
- Balance between collecting survivors vs reaching exit in time
- More survivors = slower movement = less time to explore
