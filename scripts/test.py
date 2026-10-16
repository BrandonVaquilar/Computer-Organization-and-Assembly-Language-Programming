"""Run freshly built ARM programs; every subprocess has a timeout."""
import argparse
import shlex
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
parser = argparse.ArgumentParser()
parser.add_argument("--runner", default="qemu-arm -L /usr/arm-linux-gnueabihf")
parser.add_argument("--compiler", default="arm-linux-gnueabihf-g++")
args = parser.parse_args()
runner = shlex.split(args.runner)


def run(command, text=None, expected=0):
    result = subprocess.run(command, input=text, text=True, capture_output=True, cwd=ROOT, timeout=20)
    if result.returncode != expected:
        raise RuntimeError(f"{command}: exit {result.returncode}\n{result.stdout}\n{result.stderr}")
    return result.stdout


print(run(runner + ["build/arm_tests"]), end="")
lines = run(runner + ["build/fizzbuzz"]).splitlines()[1:-1]
expected = ["FizzBuzz" if i % 15 == 0 else "Fizz" if i % 3 == 0 else "Buzz" if i % 5 == 0 else str(i) for i in range(1, 101)]
assert lines == expected, "FizzBuzz must emit exactly the expected 100 lines"
print("PASS: complete 1–100 FizzBuzz output")

for seed in (0, 1, 42, 2026):
    assert "PASS:" in run(runner + ["build/spongebob"], f"{seed}\nexamples/message.txt\n")
(ROOT / "build/empty.txt").write_text("")
assert "PASS:" in run(runner + ["build/spongebob"], "42\nbuild/empty.txt\n")
run(runner + ["build/spongebob"], "42\nbuild/missing.txt\n", expected=1)
run(runner + ["build/arithmetic"], "invalid\n", expected=1)
print("PASS: seeded text conversion, empty files and invalid input")


def basic(source, expected, stdin=""):
    assembly = ROOT / "build/test_program.s"
    run(["build/bb8", str(assembly)], source)
    run(shlex.split(args.compiler) + ["-marm", "-Wl,-z,noexecstack", str(assembly), "build/print.o", "-o", "build/test_program"])
    actual = run(runner + ["build/test_program"], stdin)
    assert actual == expected, f"BASIC output differs: {actual!r} != {expected!r}"


basic((ROOT / "examples/countdown.bas").read_text(), "Countdown from ARM32:\n5\n4\n3\n2\n1\nDone.\n")
basic('10 INPUT X\n20 PRINT X\n30 END\n', "37\n", "37\n")
basic('10 LET A = 12345\n20 PRINT A\n30 PRINT "go go QUIT quietly\\n"\n40 END\n', "12345\ngo go QUIT quietly\n")
basic('10 LET A = 2\n20 LET B = 1\n30 IF A < B THEN GOTO 60 ELSE GOTO 40\n40 PRINT "else\\n"\n50 END\n60 PRINT "then\\n"\n70 END\n', "else\n")
run(["build/bb8", "build/invalid.s"], "10 UNKNOWN\n", expected=1)
print("PASS: BASIC countdown, INPUT, large literals, quoted text and IF/ELSE")

# The public preview CLIs should fail clearly for malformed inputs.
(ROOT / "build/invalid.ppm").write_bytes(b"P6\n2 2\n255\n\x00")
run(runner + ["build/image_filters", "sepia", "build/invalid.ppm", "build/unused.ppm"], expected=1)
run(runner + ["build/terrain", "bridges_mountains45/test.dat", "-1", "build/unused.ppm"], expected=1)
print("PASS: malformed image and out-of-range terrain input")
