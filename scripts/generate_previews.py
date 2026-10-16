"""Generate gallery assets by running the actual ARM32 programs.

Pillow decodes/encodes image files and draws captions. The filters and path
selection run exclusively in the compiled ARM assembly through the runner.
"""
import argparse
import json
import shlex
import subprocess
from pathlib import Path

from PIL import Image, ImageDraw, ImageFont

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build/previews"
ASSETS = ROOT / "docs/assets"
parser = argparse.ArgumentParser()
parser.add_argument("--runner", default="qemu-arm -L /usr/arm-linux-gnueabihf")
args = parser.parse_args()
runner = shlex.split(args.runner)
BUILD.mkdir(parents=True, exist_ok=True)
ASSETS.mkdir(parents=True, exist_ok=True)
commands = []


def run(command, text=None):
    result = subprocess.run(command, input=text, text=True, capture_output=True, cwd=ROOT, timeout=30, check=True)
    commands.append({"command": shlex.join(command), "stdout": result.stdout})
    return result.stdout


def font(size, mono=False):
    # Use a redistributable system font when installed, or Pillow's default.
    path = Path("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf" if mono else "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")
    return ImageFont.truetype(str(path), size) if path.exists() else ImageFont.load_default(size=size)


image = Image.open(ROOT / "asm_image/kyoto.jpg").convert("RGB")
image.thumbnail((960, 540), Image.Resampling.LANCZOS)
image.save(BUILD / "input.ppm")
image.save(ASSETS / "original.webp", quality=88)
outputs = [("Original", image.copy())]
for name, label in [("darken", "Darken / bit shift"), ("sepia", "Sepia / fixed-point RGB"), ("warm", "Warm / custom color matrix")]:
    run(runner + ["build/image_filters", name, "build/previews/input.ppm", f"build/previews/{name}.ppm"])
    result = Image.open(BUILD / f"{name}.ppm").convert("RGB")
    result.save(ASSETS / f"{name}.webp", quality=88)
    outputs.append((label, result))

sheet = Image.new("RGB", (1664, 1140), "#101c2b")
draw = ImageDraw.Draw(sheet)
draw.text((32, 24), "Pixels, transformed in ARM32", font=font(36), fill="#f3f5f7")
draw.text((32, 77), "One input. Three assembly routines. Actual program output.", font=font(21), fill="#a7b8cb")
for index, (label, result) in enumerate(outputs):
    x = 32 + index % 2 * 816
    y = 136 + index // 2 * 490
    draw.text((x, y), label, font=font(24), fill="#f3f5f7")
    result = result.resize((784, 441), Image.Resampling.LANCZOS)
    sheet.paste(result, (x, y+38))
sheet.save(ASSETS / "filters-comparison.webp", quality=90)

for dataset in ["tahoe", "san_joaquin"]:
    source = ROOT / f"bridges_mountains45/{dataset}.dat"
    dimensions = list(map(int, source.read_text().split()[:2]))
    row = dimensions[1] // 2
    run(runner + ["build/terrain", f"bridges_mountains45/{dataset}.dat", str(row), f"build/previews/{dataset}.ppm"])
    result = Image.open(BUILD / f"{dataset}.ppm").convert("RGB")
    # Nearest-neighbor keeps each actual terrain cell and route marker visible.
    scale = max(1, min(6, 900 // max(result.size)))
    result.resize((result.width*scale, result.height*scale), Image.Resampling.NEAREST).save(ASSETS / f"{dataset}.webp", lossless=True)

run(["build/bb8", "build/previews/countdown.s"], (ROOT / "examples/countdown.bas").read_text())
compiler = "arm-linux-gnueabihf-g++" if runner else "g++"
run([compiler, "-marm", "-Wl,-z,noexecstack", "build/previews/countdown.s", "build/print.o", "-o", "build/previews/countdown"])
countdown = run(runner + ["build/previews/countdown"])
fizz = run(runner + ["build/fizzbuzz"])
sponge = run(runner + ["build/spongebob"], "42\nexamples/message.txt\n")
(ASSETS / "console-output.txt").write_text("BASIC TO ARM32\n" + countdown + "\nFIZZBUZZ\n" + fizz + "\nSEEDED TEXT TRANSFORM\n" + sponge)

terminal = Image.new("RGB", (1440, 760), "#101c2b")
draw = ImageDraw.Draw(terminal)
draw.rounded_rectangle((24, 24, 1416, 736), radius=18, fill="#162438", outline="#33465e", width=2)
draw.text((60, 52), "BASIC source to ARM32 instructions", font=font(30), fill="#f3f5f7")
draw.text((60, 116), "01  Source", font=font(21), fill="#79c6ff")
source_lines = (ROOT / "examples/countdown.bas").read_text().splitlines()[1:]
draw.multiline_text((60, 165), "\n".join(source_lines), font=font(19, mono=True), spacing=13, fill="#c7d6e6")
draw.line((860, 120, 860, 660), fill="#33465e", width=2)
draw.text((906, 116), "02  Actual ARM output", font=font(21), fill="#79c6ff")
draw.multiline_text((906, 165), countdown, font=font(22, mono=True), spacing=15, fill="#f3f5f7")
draw.text((60, 681), "Compiled with GNU ARM tools. Executed with QEMU user mode.", font=font(18), fill="#93a9bf")
terminal.save(ASSETS / "compiler-output.webp", quality=94)
(ASSETS / "provenance.json").write_text(json.dumps({
    "execution": "Actual ARM32 binaries, invoked with the configured runner",
    "image_input": "asm_image/kyoto.jpg, bundled in the original coursework repository",
    "processing": "Pillow decodes/encodes files, resizes previews and draws captions; ARM assembly transforms pixels and selects terrain paths",
    "terrain_palette": "C++ preview driver maps original elevations to colors; cyan pixels are ARM -1 path markers",
    "commands": commands,
}, indent=2) + "\n")
print("Generated docs/assets from actual ARM32 program runs.")
