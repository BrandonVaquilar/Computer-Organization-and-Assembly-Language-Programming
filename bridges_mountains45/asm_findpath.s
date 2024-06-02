.global asm_findpath
.type	asm_findpath,%function
//findPath(int *elev_data, int imgWidth, int imgHeight, int startRow, int maxVal)


asm_findpath:
	PUSH {LR}
	PUSH {R4-R12}


B check
compare:
CMP R2, R1			//compare topright and middle values
BGT compare2		//if R1 is greater than, go to compare2
CMP R2, R0			//compare topright and bottom right values
BLT resume			//if R2 is the smallest, go back to resume

compare2:
CMP R1, R0			//compare middleright with bottom right
BLT middle
B bottom

check:				
	CMP R3, #0
	BLT return
	CMP R1, #0
	BGE return
	
	MOV R10,#0		//column
	//MOV R8, R4		//maxVal
	MOV R7, R3		//startRow
	MOV R6, R2		//imgHeight
	MOV R5, R1 		//imgWidth
	MOV R4, R0 		//elev_data
	MUL R3, R7, R5  //pixel address

	MOV R9, R5
	SUB R9, #1
top: 
	//MOV R9, R5 		
	//SUB R9, #1			//imgWidth - 1
	CMP R10, R9		
	BGE return			//if (Column >= (imgwidth - 1)) return

	LDR R8, [R4, R3] 	//Current
	MOV R8, #-1		 	//set current path to -1
	STR R8,	[R4, R3]
	
Row0://check if it's at the top of the graph

	
inrow:
	ADD R3, #4			//one right of pixel address
	LDR R1, [R4, R3]	
	LSL R11, R5, #2		//multiply imgWidth by 4
	SUB R3, R3, R11		//subtract pixaddr with imgWidth
	LDR R0, [R4, R3]
	ADD R3, R3, R11		//add pixaddr with imgWidth
	ADD R3, R3, R11		//do it again because we're at curr and we want the one under it
	LDR R2, [R4, R3]
	B compare

middle:
	SUB R3, R3, R11
	B resume
bottom:
	SUB R3, R3, R11
	SUB R3, R3, R11
	B resume
resume:
//	STR R8, [R4, R3]
endrow:

	ADD R10, #1		//add 1 to column
	B top

return:
	POP {R4-R12}
	POP {PC}
