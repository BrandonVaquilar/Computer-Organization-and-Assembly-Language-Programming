//Write the assembly equivalent of this C++ code:
//int baz(int x, int y, int z) { return 5*x + 5*x*y + 5*x*y*z; }
//Par: 11 strokes

.global baz
baz:
//Your code here

	MLA R1, R1, R2, R1
	ADD R1, #1
	ADD R0, R0, R0, LSL #2
	MUL R0, R1

	BX LR
