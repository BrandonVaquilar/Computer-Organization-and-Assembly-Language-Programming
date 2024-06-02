#You are going to write two functions here:
# function_one
# function_two

.global function_one
function_one:
    PUSH {LR}
    PUSH {R4-R12}        @This preserves R4 through R12 

	MUL R5, R2, R3
	ADD R5, R5, LSL #1
	MOV R6, #0
	
loop:
	CMP R6, R5
	BGE quit
	LDRB R4, [R0, R6]
	STRB R4, [R1, R6]
	ADD R6, #1
	BAL loop

quit:
	MOV R0,#0			 @Return value of 0
    POP {R4-R12}         @Restore R4 through R12 for the calling function
	POP {PC}             @Return from a function



.global function_two
function_two:
    PUSH {LR}
    PUSH {R4-R12}        @This preserves R4 through R12 

quit2:
	MOV R0,#0			 @Return value of 0
    POP {R4-R12}         @Restore R4 through R12 for the calling function
	POP {PC}             @Return from a function
