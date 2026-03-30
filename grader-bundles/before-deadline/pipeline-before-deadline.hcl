spec_version = "2.0"

defaults {
  timeout = "30"
  given_case_count = 5
}

pipeline "2011-pa2-before-deadline" {
  stage "compile-main" {
    exec {
      command = "g++"
      args = ["-std=c++11", "-Wall", "-o", "pa2", "testcases.cpp"]
      assert { exit_code = 0 }
    }
    scoring {
      mode = "pass_fail"
      points = "0"
    }
  }

  stage "run-given" {
    depends_on = ["compile-main"]

    exec {
      script = <<-EOF
        #!/bin/bash
        ./pa2
      EOF

      test "given-01" {
        points = "1"
        stdin = file("given_testcases/input01.txt")
        assert { stdout = file("given_testcases/output01.txt") }
      }

      test "given-02" {
        points = "1"
        stdin = file("given_testcases/input02.txt")
        assert { stdout = file("given_testcases/output02.txt") }
      }

      test "given-03" {
        points = "1"
        stdin = file("given_testcases/input03.txt")
        assert { stdout = file("given_testcases/output03.txt") }
      }

      test "given-05" {
        points = "1"
        stdin = file("given_testcases/input05.txt")
        assert { stdout = file("given_testcases/output05.txt") }
      }

      test "given-09" {
        points = "1"
        stdin = file("given_testcases/input09.txt")
        assert { stdout = file("given_testcases/output09.txt") }
      }
    }
  }
}
