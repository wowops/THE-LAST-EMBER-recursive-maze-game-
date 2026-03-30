spec_version = "2.0"

defaults {
  timeout = "30"
  given_case_count = 5
  hidden_case_count = 22
  total_case_count = 27
}

pipeline "2011-pa2-after-deadline" {
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

  stage "compile-hidden-warmup-01" {
    exec {
      command = "g++"
      args = ["-std=c++11", "-Wall", "-o", "warmup_hidden_01", "warmup_hidden_01.cpp"]
      assert { exit_code = 0 }
    }
    scoring {
      mode = "pass_fail"
      points = "0"
    }
  }

  stage "compile-hidden-warmup-02" {
    exec {
      command = "g++"
      args = ["-std=c++11", "-Wall", "-o", "warmup_hidden_02", "warmup_hidden_02.cpp"]
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

  stage "run-hidden-full-program" {
    depends_on = ["compile-main"]

    exec {
      script = <<-EOF
        #!/bin/bash
        ./pa2
      EOF

      test "hidden-04" {
        points = "1"
        stdin = file("hidden_testcases/input04.txt")
        assert { stdout = file("hidden_testcases/output04.txt") }
      }

      test "hidden-06" {
        points = "1"
        stdin = file("hidden_testcases/input06.txt")
        assert { stdout = file("hidden_testcases/output06.txt") }
      }

      test "hidden-07" {
        points = "1"
        stdin = file("hidden_testcases/input07.txt")
        assert { stdout = file("hidden_testcases/output07.txt") }
      }

      test "hidden-08" {
        points = "1"
        stdin = file("hidden_testcases/input08.txt")
        assert { stdout = file("hidden_testcases/output08.txt") }
      }

      test "hidden-10" {
        points = "1"
        stdin = file("hidden_testcases/input10.txt")
        assert { stdout = file("hidden_testcases/output10.txt") }
      }

      test "hidden-11" {
        points = "1"
        stdin = file("hidden_testcases/input11.txt")
        assert { stdout = file("hidden_testcases/output11.txt") }
      }

      test "hidden-12" {
        points = "1"
        stdin = file("hidden_testcases/input12.txt")
        assert { stdout = file("hidden_testcases/output12.txt") }
      }

      test "hidden-13" {
        points = "1"
        stdin = file("hidden_testcases/input13.txt")
        assert { stdout = file("hidden_testcases/output13.txt") }
      }

      test "hidden-14" {
        points = "1"
        stdin = file("hidden_testcases/input14.txt")
        assert { stdout = file("hidden_testcases/output14.txt") }
      }

      test "hidden-15" {
        points = "1"
        stdin = file("hidden_testcases/input15.txt")
        assert { stdout = file("hidden_testcases/output15.txt") }
      }

      test "hidden-16" {
        points = "1"
        stdin = file("hidden_testcases/input16.txt")
        assert { stdout = file("hidden_testcases/output16.txt") }
      }

      test "hidden-17" {
        points = "1"
        stdin = file("hidden_testcases/input17.txt")
        assert { stdout = file("hidden_testcases/output17.txt") }
      }

      test "hidden-18" {
        points = "1"
        stdin = file("hidden_testcases/input18.txt")
        assert { stdout = file("hidden_testcases/output18.txt") }
      }

      test "hidden-19" {
        points = "1"
        stdin = file("hidden_testcases/input19.txt")
        assert { stdout = file("hidden_testcases/output19.txt") }
      }

      test "hidden-20" {
        points = "1"
        stdin = file("hidden_testcases/input20.txt")
        assert { stdout = file("hidden_testcases/output20.txt") }
      }

      test "hidden-21" {
        points = "1"
        stdin = file("hidden_testcases/input21.txt")
        assert { stdout = file("hidden_testcases/output21.txt") }
      }

      test "hidden-22" {
        points = "1"
        stdin = file("hidden_testcases/input22.txt")
        assert { stdout = file("hidden_testcases/output22.txt") }
      }

      test "hidden-23" {
        points = "1"
        stdin = file("hidden_testcases/input23.txt")
        assert { stdout = file("hidden_testcases/output23.txt") }
      }

      test "hidden-24" {
        points = "1"
        stdin = file("hidden_testcases/input24.txt")
        assert { stdout = file("hidden_testcases/output24.txt") }
      }

      test "hidden-25" {
        points = "1"
        stdin = file("hidden_testcases/input25.txt")
        assert { stdout = file("hidden_testcases/output25.txt") }
      }
    }
  }

  stage "run-hidden-warmup-01" {
    depends_on = ["compile-hidden-warmup-01"]

    exec {
      script = <<-EOF
        #!/bin/bash
        ./warmup_hidden_01
      EOF

      test "hidden-phase-a-01" {
        points = "1"
        assert { exit_code = 0 }
      }
    }
  }

  stage "run-hidden-warmup-02" {
    depends_on = ["compile-hidden-warmup-02"]

    exec {
      script = <<-EOF
        #!/bin/bash
        ./warmup_hidden_02
      EOF

      test "hidden-phase-a-02" {
        points = "1"
        assert { exit_code = 0 }
      }
    }
  }
}
