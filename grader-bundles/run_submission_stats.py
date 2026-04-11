#!/usr/bin/env python3

import csv
import os
import shutil
import subprocess
import tempfile
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path


ROOT = Path(__file__).resolve().parent
TEMPLATE_DIR = ROOT / "after-deadline"
SUBMISSIONS_DIR = ROOT / "2530-COMP2011-PA2_submissions"
OUT_CSV = ROOT / "2530-COMP2011-PA2_results.csv"

COMPILE_TIMEOUT = 12
RUN_TIMEOUT = 2
WARMUP_TIMEOUT = 5
SUBMISSION_TIMEOUT_CASE_BUDGET = 8
MAX_WORKERS = min(8, max(1, (os.cpu_count() or 4) - 1))


def testcase_names(case_dir: Path, prefix: str) -> list[str]:
    return sorted(
        path.name
        for path in case_dir.glob(f"{prefix}*.txt")
        if path.is_file()
    )


GIVEN_INPUTS = testcase_names(TEMPLATE_DIR / "given_testcases", "input")
HIDDEN_INPUTS = testcase_names(TEMPLATE_DIR / "hidden_testcases", "input")
WARMUP_FILES = ["warmup_hidden_01.cpp", "warmup_hidden_02.cpp"]


def setup_workspace(pa2_source: Path) -> Path:
    workdir = Path(tempfile.mkdtemp(prefix="pa2_batch_"))
    for item in TEMPLATE_DIR.iterdir():
        target = workdir / item.name
        if item.name == "pa2.cpp":
            shutil.copy2(pa2_source, target)
        elif item.is_dir():
            shutil.copytree(item, target)
        else:
            shutil.copy2(item, target)
    return workdir


def compile_main(workdir: Path) -> tuple[bool, str]:
    proc = subprocess.run(
        ["g++", "-std=c++11", "-Wall", "-o", "pa2", "testcases.cpp"],
        cwd=workdir,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        timeout=COMPILE_TIMEOUT,
    )
    output = (proc.stdout + proc.stderr).decode(errors="replace")
    return proc.returncode == 0, output


def run_case(workdir: Path, case_path: Path, expected_path: Path) -> tuple[bool, str]:
    proc = subprocess.run(
        ["./pa2"],
        cwd=workdir,
        input=case_path.read_bytes(),
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        timeout=RUN_TIMEOUT,
    )
    actual = proc.stdout.replace(b"\r\n", b"\n")
    expected = expected_path.read_bytes().replace(b"\r\n", b"\n")
    if proc.returncode != 0:
        return False, f"runtime_exit_{proc.returncode}"
    if actual != expected:
        return False, "wrong_answer"
    return True, ""


def compile_and_run_warmup(workdir: Path, warmup_name: str) -> tuple[bool, str]:
    if warmup_name == "warmup_hidden_02.cpp":
        return run_relaxed_warmup_02(workdir)

    exe_name = warmup_name.replace(".cpp", "")
    compile_proc = subprocess.run(
        ["g++", "-std=c++11", "-Wall", warmup_name, "-o", exe_name],
        cwd=workdir,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        timeout=COMPILE_TIMEOUT,
    )
    if compile_proc.returncode != 0:
        return False, "warmup_compile_error"

    run_proc = subprocess.run(
        [f"./{exe_name}"],
        cwd=workdir,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        timeout=WARMUP_TIMEOUT,
    )
    if run_proc.returncode != 0:
        return False, f"warmup_exit_{run_proc.returncode}"
    return True, ""


def run_relaxed_warmup_02(workdir: Path) -> tuple[bool, str]:
    diag = workdir / "warmup_hidden_02_relaxed.cpp"
    diag.write_text(
        """#include <sstream>
#include <string>
#include <iostream>

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

    std::string got = capture.str();
    if (got != "(0,0) -> (0,1) -> (1,1) -> (1,2) -> (2,2)\\n" &&
        got != "(0,0) -> (0,1) -> (1,1) -> (1,2) -> (2,2)") {
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
""",
        encoding="ascii",
    )

    exe_name = "warmup_hidden_02_relaxed"
    compile_proc = subprocess.run(
        ["g++", "-std=c++11", "-Wall", diag.name, "-o", exe_name],
        cwd=workdir,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        timeout=COMPILE_TIMEOUT,
    )
    if compile_proc.returncode != 0:
        return False, "warmup_compile_error"

    run_proc = subprocess.run(
        [f"./{exe_name}"],
        cwd=workdir,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        timeout=WARMUP_TIMEOUT,
    )
    if run_proc.returncode != 0:
        return False, f"warmup_exit_{run_proc.returncode}"
    return True, ""


def evaluate_submission(student_dir: Path) -> dict[str, str]:
    result = {
        "student": student_dir.name,
        "pa2_found": "0",
        "compile_ok": "0",
        "given_pass": "0",
        "given_total": str(len(GIVEN_INPUTS)),
        "hidden_pass": "0",
        "hidden_total": str(len(HIDDEN_INPUTS)),
        "warmup_pass": "0",
        "warmup_total": str(len(WARMUP_FILES)),
        "total_pass": "0",
        "total_tests": str(len(GIVEN_INPUTS) + len(HIDDEN_INPUTS) + len(WARMUP_FILES)),
        "failed_cases": "",
        "compile_note": "",
    }

    pa2_source = student_dir / "pa2.cpp"
    if not pa2_source.exists():
        result["failed_cases"] = "missing_pa2.cpp"
        return result

    result["pa2_found"] = "1"
    workdir = setup_workspace(pa2_source)
    failures: list[str] = []
    timeout_cases = 0

    try:
        try:
            compile_ok, compile_note = compile_main(workdir)
        except subprocess.TimeoutExpired:
            result["compile_note"] = "compile_timeout"
            result["failed_cases"] = "compile_timeout"
            return result

        result["compile_ok"] = "1" if compile_ok else "0"
        result["compile_note"] = compile_note.strip().replace("\n", " ")[:500]
        if not compile_ok:
            result["failed_cases"] = "compile_error"
            return result

        given_pass = 0
        for name in GIVEN_INPUTS:
            try:
                ok, note = run_case(
                    workdir,
                    workdir / "given_testcases" / name,
                    workdir / "given_testcases" / name.replace("input", "output"),
                )
            except subprocess.TimeoutExpired:
                ok, note = False, "timeout"
            if ok:
                given_pass += 1
            else:
                failures.append(f"{name}:{note}")
                if note == "timeout":
                    timeout_cases += 1
            if timeout_cases >= SUBMISSION_TIMEOUT_CASE_BUDGET:
                failures.append("submission:too_many_timeouts")
                result["given_pass"] = str(given_pass)
                result["hidden_pass"] = "0"
                result["warmup_pass"] = "0"
                result["total_pass"] = str(given_pass)
                result["failed_cases"] = ";".join(failures)
                return result

        hidden_pass = 0
        for name in HIDDEN_INPUTS:
            try:
                ok, note = run_case(
                    workdir,
                    workdir / "hidden_testcases" / name,
                    workdir / "hidden_testcases" / name.replace("input", "output"),
                )
            except subprocess.TimeoutExpired:
                ok, note = False, "timeout"
            if ok:
                hidden_pass += 1
            else:
                failures.append(f"{name}:{note}")
                if note == "timeout":
                    timeout_cases += 1
            if timeout_cases >= SUBMISSION_TIMEOUT_CASE_BUDGET:
                failures.append("submission:too_many_timeouts")
                result["given_pass"] = str(given_pass)
                result["hidden_pass"] = str(hidden_pass)
                result["warmup_pass"] = "0"
                result["total_pass"] = str(given_pass + hidden_pass)
                result["failed_cases"] = ";".join(failures)
                return result

        warmup_pass = 0
        for name in WARMUP_FILES:
            try:
                ok, note = compile_and_run_warmup(workdir, name)
            except subprocess.TimeoutExpired:
                ok, note = False, "timeout"
            if ok:
                warmup_pass += 1
            else:
                failures.append(f"{name}:{note}")

        result["given_pass"] = str(given_pass)
        result["hidden_pass"] = str(hidden_pass)
        result["warmup_pass"] = str(warmup_pass)
        result["total_pass"] = str(given_pass + hidden_pass + warmup_pass)
        result["failed_cases"] = ";".join(failures)
        return result

    except subprocess.TimeoutExpired as exc:
        failures.append(f"timeout:{exc.cmd[0] if exc.cmd else 'unknown'}")
        result["failed_cases"] = ";".join(failures)
        return result
    finally:
        shutil.rmtree(workdir, ignore_errors=True)


def main() -> None:
    students = sorted(path for path in SUBMISSIONS_DIR.iterdir() if path.is_dir())
    with ThreadPoolExecutor(max_workers=MAX_WORKERS) as pool:
        results = list(pool.map(evaluate_submission, students))

    with OUT_CSV.open("w", newline="") as csvfile:
        writer = csv.DictWriter(
            csvfile,
            fieldnames=[
                "student",
                "pa2_found",
                "compile_ok",
                "given_pass",
                "given_total",
                "hidden_pass",
                "hidden_total",
                "warmup_pass",
                "warmup_total",
                "total_pass",
                "total_tests",
                "failed_cases",
                "compile_note",
            ],
        )
        writer.writeheader()
        writer.writerows(results)

    compile_ok = sum(1 for row in results if row["compile_ok"] == "1")
    full_pass = sum(1 for row in results if row["total_pass"] == row["total_tests"])
    print(f"students={len(results)}")
    print(f"compile_ok={compile_ok}")
    print(f"full_pass={full_pass}")
    print(f"csv={OUT_CSV}")


if __name__ == "__main__":
    main()
