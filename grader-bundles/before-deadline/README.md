Public standalone bundle for the before-deadline grader.

Files included:
- public harness and maps
- given testcase inputs and expected outputs
- `pipeline-before-deadline.hcl`

Usage:
- replace `pa2.cpp` with the student submission if your grading environment does not inject it automatically
- run the pipeline from this directory

Compile target:
- `g++ -std=c++11 -Wall -o pa2 testcases.cpp`
