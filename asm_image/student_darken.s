#Write your image darkening code here in assembly
.global sdarken

//void sdarken(unsigned char *in,unsigned char *out, int width, int height);


sdarken:
	PUSH {LR}
	PUSH {R4-R12}

	MOV R8, #0			//counter
	
	MUL R9, R2, R3		//width * height
	MOV R10, #3	
	MUL R9, R10, R9		//width * height * 3

top:
						//R8 is our counter	
	CMP R8, R9			//compare counter with width * height * 3
	BGE return			//if (i < width*height*3) return
	LDRB R11, [R0, R8]	//Access the *in at whatever number the counter is at


	LSR R11, #1			//divides the images value by 2
	STRB R11, [R1, R8]	//stores it back into *outs

	ADD R8, #1			//adds 1 to the counter

	B top	

return:
	POP {R4-R12}
	POP {PC}

