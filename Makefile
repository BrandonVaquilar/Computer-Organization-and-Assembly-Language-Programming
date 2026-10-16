# On 32-bit ARM Linux: make CROSS_COMPILE= RUNNER=
CROSS_COMPILE ?= arm-linux-gnueabihf-
ARM_CXX := $(CROSS_COMPILE)g++
ARM_CC := $(CROSS_COMPILE)gcc
HOST_CXX ?= g++
RUNNER ?= qemu-arm -L /usr/arm-linux-gnueabihf
CXXFLAGS ?= -O2 -g -std=c++17 -Wall -Wextra
ARMFLAGS := -marm -Wl,-z,noexecstack
PROGRAMS := build/arithmetic build/fizzbuzz build/spongebob build/image_filters build/terrain build/bb8 build/print.o

.PHONY: all test previews clean
all: $(PROGRAMS)

build:
	mkdir -p $@

build/arithmetic: asm_first/main.cc asm_first/foobar.s asm_first/baz.s asm_first/quz.s include/input.hpp | build
	$(ARM_CXX) $(CXXFLAGS) $(ARMFLAGS) $(filter %.cc %.s,$^) -o $@

build/fizzbuzz: asm_fizzbuzz/main.c asm_fizzbuzz/fizzbuzz.s | build
	$(ARM_CC) -O2 -g -Wall -Wextra $(ARMFLAGS) $^ -o $@

build/spongebob: asm_spongebob/main.cc asm_spongebob/asm_spongebob.s include/input.hpp | build
	$(ARM_CXX) $(CXXFLAGS) $(ARMFLAGS) $(filter %.cc %.s,$^) -o $@

build/image_filters: asm_image/preview.cc asm_image/student_darken.s asm_image/sepia.s asm_image/student_filter2.s include/ppm.hpp | build
	$(ARM_CXX) $(CXXFLAGS) $(ARMFLAGS) $(filter %.cc %.s,$^) -o $@

build/terrain: bridges_mountains45/preview.cc bridges_mountains45/asm_findpath.s include/ppm.hpp | build
	$(ARM_CXX) $(CXXFLAGS) $(ARMFLAGS) $(filter %.cc %.s,$^) -o $@

# The compiler runs on the host; its output targets ARM32.
build/bb8: asm_compiler/main.cc | build
	$(HOST_CXX) $(CXXFLAGS) $< -o $@

build/print.o: asm_compiler/print.c | build
	$(ARM_CC) -O2 -g -Wall -Wextra -c $< -o $@

build/arm_tests: tests/arm_tests.cc asm_first/foobar.s asm_first/baz.s asm_first/quz.s asm_image/student_darken.s asm_image/sepia.s asm_image/student_filter2.s bridges_mountains45/asm_findpath.s | build
	$(ARM_CXX) $(CXXFLAGS) $(ARMFLAGS) $^ -o $@

test: all build/arm_tests
	python3 scripts/test.py --runner '$(RUNNER)' --compiler '$(ARM_CXX)'

previews: all
	python3 scripts/generate_previews.py --runner '$(RUNNER)'

clean:
	rm -rf build
