Full standalone bundle for the after-deadline grader.

Files included:
- public harness and maps
- given testcase inputs and expected outputs
- hidden full-program testcase inputs and expected outputs
- `pipeline-after-deadline.hcl`
- `warmup_hidden_01.cpp`
- `warmup_hidden_02.cpp`

Usage:
- replace `pa2.cpp` with the student submission if your grading environment does not inject it automatically
- run the pipeline from this directory

Notes:
- this bundle contains 27 checks total: 5 given, 20 hidden full-program, 2 hidden Phase A helper checks
- the two Phase A helper checks are custom grader-owned hidden checks created from the repo specification, because the exact official helper-only hidden cases are not present in the repository
