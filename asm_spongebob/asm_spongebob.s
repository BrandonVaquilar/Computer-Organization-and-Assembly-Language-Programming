	.global asm_spongebob
asm_spongebob:
	PUSH {LR}
	PUSH {R4-R12}

	MOV R4, #0 			//Counter of array, start is 0
	MOV R5, R0 			//Save R0 to R5

top:
	LDRB R6, [R5, R4]
	MOV R0, R6			//copy to R0
	CMP R6, #0			
	BEQ return 			//if end of array, return
	BL isalpha 		//call isalpha and compare to 0. 1 = alpha, 0 = not an alphabetical character
	CMP R0, #0
	BGT flip			//if alpha then flip a coin and decide to uppercase letter, else counter += 1
	ADD R4, R4, #1
	BAL top

flip:					//coinflip to decide to uppercase letter
	BL rand				//call rand function
	ANDS R0, R0, #1		//this does modulus 2 and compares to 0
	BNE uppercaseify	//if it's a 1, uppercaseify
	BEQ lowercaseify	//if it's a 0, lowercaseify

uppercaseify:
	MOV R0, R6
	BL toupper			//uppercase it and go next
	B next			

lowercaseify:
	MOV R0, R6
	BL tolower			//lowercase and go next

next:
	STRB R0, [R5, R4]	//save the change
	ADD R4, R4, #1		//increase counter by 1
	BAL top

return:
	POP {R4-R12}
	POP {PC}
