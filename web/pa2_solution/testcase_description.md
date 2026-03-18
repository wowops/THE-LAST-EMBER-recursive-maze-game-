The table below lists the full-program testcase set in `pa2_solution`. These Level 1-3 cases are part of the counted testcase suite. The separate Phase A warm-up hidden checks are direct helper-function tests, so they are not listed here.

| Input | Output | Map | Level | Description |
|---|---|---|---|---|
| input01.txt | output01.txt | map01.txt | Level 1 | shortest out of 3 |
| input02.txt | output02.txt | map02.txt | Level 2 | shortest with not enough survivors, choose the 2-nd |
| input03.txt | output03.txt | map03.txt | Level 3 | maximum with not enough time, choose the 2-nd |
| input05.txt | output05.txt | map02.txt | Level 2 | 3-rd shortest |
| input09.txt | output09.txt | map03.txt | Level 3 | maximum survivors |
| input04.txt | output04.txt | map02.txt | Level 1 | shortest out of 3 |
| input06.txt | output06.txt | map04.txt | Level 2 | shortest with just enough survivors |
| input07.txt | output07.txt | map04.txt | Level 2 | must pass through dangerous region |
| input08.txt | output08.txt | map04.txt | Level 2 | no valid path |
| input10.txt | output10.txt | map05.txt | Level 3 | 3-rd maximum survivors |
| input11.txt | output11.txt | map05.txt | Level 3 | 2-nd maximum survivors |
| input12.txt | output12.txt | map05.txt | Level 3 | maximum survivors with just enough time |
| input13.txt | output13.txt | map05.txt | Level 3 | no valid path |
| input14.txt | output14.txt | map06.txt | Level 1 | unique shortest route with several misleading branches and dead ends |
| input15.txt | output15.txt | map07.txt | Level 2 | equal-length options, but only survivor-rich corridor unlocks the door |
| input16.txt | output16.txt | map07.txt | Level 2 | same maze as input15, but one less move of time makes the valid route impossible |
| input17.txt | output17.txt | map07.txt | Level 2 | no valid path because the visible high-value regions cannot be collected on any valid route to the door |
| input18.txt | output18.txt | map08.txt | Level 3 | dynamic-cost cliff: medium-reward corridor is best under the time limit |
| input19.txt | output19.txt | map08.txt | Level 3 | dynamic-cost boundary case: the 73-survivor route is just reachable at time 125 |
| input20.txt | output20.txt | map08.txt | Level 3 | one unit less time than input19 makes the high-reward route impossible, so threshold 70 fails |
| input21.txt | output21.txt | map09.txt | Level 1 | long unique shortest path through a maze with many misleading branches |
| input22.txt | output22.txt | map10.txt | Level 1 | nontrivial no-path case where the beacon is visually close but unreachable |
| input23.txt | output23.txt | map07.txt | Level 2 | exact-boundary success: shortest valid route meets both time 19 and threshold 32 exactly |
| input24.txt | output24.txt | map07.txt | Level 2 | one survivor above the reachable total on this map; no valid route unlocks the shelter |
| input25.txt | output25.txt | map08.txt | Level 3 | threshold 33 with time 124: medium route is one survivor short and high-reward route is too slow |
