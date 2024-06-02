.global sfilter

sfilter:
	PUSH {LR}
	PUSH {R4-R12}
    MOV R4, #0           //int i = 0;
    MUL R6, R2, R3       //Stride = Width * Height;
width:
    CMP R4, R2           //i < width
    BGE return
    MOV R5, #0           //int j = 0, declare it in width so we can reset height after every iteration
height:
    CMP R5, R3           //j < height
    ADDGE R4, #1         //i++
    BGE width            //if j >= height, continue width for loop
    MUL R8, R5, R2       // j*width
    ADD R8, R4           // (j*width) + i
widen:
    LDRB R9, [R0, R8]    // R[i,j] value
    ADD R7, R6, R8       //stride + (j*width+i)
    LDRB R10, [R0, R7]   //G[i,j] value
    ADD R7, R6           //add stride again
    LDRB R11, [R0, R7]   //B[i,j] value
red:
	//unsigned short r = in[R(i,j)]*0.393 + in[G(i,j)]*0.769 + in[B(i,j)]*0.189;
	LDR R12, =rRED
	VLDR.F32 S0, [R12] //save rRED into S0 which is 0.393
	VCVT.F64.F32 D3, S0 //turn it into 64 bits
	//VLDR.F64 D0, [R9] //TODO BUS Error when a.out 
	VMUL.F64 D3, D3, D0 //multiply in[R(i,j)]*0.393 into D3

	LDR R12, =rGREEN
	//VLDR.F64 S0, [R12] //save rBLUE into S0 which is 0.769
	//VCVT.F64.F32 D3, S0 //turn it into 64 bits
	//VLDR.F64 D1, [R10]	//save R10 to D1
	VMUL.F64 D4, D3, D1


	LDR R12, =rBLUE
	//VLDR.F32 S0, [R12] //save rGREEN into S0
	//VCVT.F64.F32 D3, S0 //turn it into 64 bits
	//VLDR.F64 D2, [R11]  //save R11 to D2
	VMUL.F64 D5, D3, D2

	VADD.F64 D3, D4
	VADD.F64 D3, D5

	//VSTR D3, [R7]		//store the addition of the 3 into R7
	CMP R7, #255
	MOVGT R7, #255		//clamp
    //STRB R7, [R1, R8]       //Store
/*	
green:
	LDR R12, =gRED
	VLDR.F32 S0, [R12]
	VCVT.F64.F32 D3, S0
	VMUL.F64 D3, D3, D0

	LDR R12, =gGREEN
	VLDR.F32 S0, [R12]
	VCVT.F64.F32 D3, S0
	VMUL.F64 D4, D3, D1

	LDR R12, =gBLUE
	VLDR.F32 S0, [R12]
	VCVT.F64.F32 D3, S0
	VMUL.F64 D5, D3, D2

	VADD.F64 D3, D4
	VADD.F64 D3, D5

	
	VSTR D3, [R7]
	CMP R7, #255
	MOVGT R7, #255		//clamp
	STRB R7, [R1, R12]
blue:
	 LDR R12, =bRED
    VLDR.F32 S0, [R12]
    VCVT.F64.F32 D3, S0
    VMUL.F64 D3, D3, D0

    LDR R12, =bGREEN
    VLDR.F32 S0, [R12]
    VCVT.F64.F32 D3, S0
    VMUL.F64 D4, D3, D1

    LDR R12, =bBLUE
    VLDR.F32 S0, [R12]
    VCVT.F64.F32 D3, S0
    VMUL.F64 D5, D3, D2

    VADD.F64 D3, D4
    VADD.F64 D3, D5


    VADD.F64 D3, D4
    VADD.F64 D3, D5

    VSTR D3, [R7]
    CMP R7, #255
    MOVGT R7, #255      //clamp
    ADD R12, R8, R6     //add stride to location
    ADD R12, R6     //add stride to location
    STRB R7, [R1, R12]
*/


    ADD R5, #1           //j++
    B height





return:
	POP {R4-R12}
	POP {PC}

.data
THREE: .double 3
rRED: .float 0.393
rGREEN: .float 0.769
rBLUE: .float 0.189
gRED: .float 0.349
gGREEN: .float 0.686
gBLUE: .float 0.168
bRED: .float 0.272
bGREEN: .float 0.534
bBLUE: .float 0.131

