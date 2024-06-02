//Write the C++ equivalent of this code:
//int quz(int x, int y, int z, int w) { return (x-y)*(z+w) - (x-y)*(-1*z + -1*w); }
//Par: 7 strokes

.global quz
quz:
	//Your code here
	SUB R0, R0, R1 	//X - Y
	ADD R1, R2, R3 	//Z + W
	LSL R1, #1 		// 2(Z+W)
	MUL R0, R0, R1 	// (X-Y) (2(Z+W))
	
	


	BX LR
