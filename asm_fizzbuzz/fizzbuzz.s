/* Homework 2 - Assembly Fizzbuzz
 Your program must print the numbers from 1 to 100, except when:
 1) If the number is divisible by 15, print FizzBuzz instead, if not:
   2) If the number is divisible by 3, print Fizz instead
   3) If the number is divisible by 5, print Buzz instead
 I have provided you functions that will do the printing, you just need
 to do the control logic.

 Don't modify the C file at all, or your house will lose 50 points.

 Don't use R0 through R3 in this function (except in the one example below)
 They get overwritten every time you call a function.
 So use R4 through R10 for your logic

Any line marked with DM ("Don't Modify") should probably be left alone.
The rest of the source code is just example source code and can be deleted or changed.
*/

.global fizzbuzz 	     @DM - Don't modify
fizzbuzz:                @DM - Don't modify
	PUSH {LR}            @DM: Preserve LR for the calling function
	PUSH {R4-R12}        @DM: This preserves the R4 through R12 from the calling function

	MOV R4, #0

	top:
	CMP R4, #100
	BEQ return

	ADD R4, R4, #1
	MOV R0, R4			 @Copy value of R4 into R0
	
	MOV R5, R4 			//copying R4 into R5, R6, and R7
	MOV R6, R4
	MOV R7, R4

	loop:
	SUBS R5, #15 		//15 first
	BLEQ print_fizzbuzz
	BEQ top

	SUBS R6, #5
	BLEQ print_buzz 	//5 second
	BEQ top

	SUBS R7, #3 		//3 last
	BLEQ print_fizz
	BEQ top

	BGT loop
	BL print_number      @How to print "3" or whatevs
	B top

return:
	POP {R4-R12}         @DM: Restore R4 through R12 for the calling function
	POP {PC}             @DM: Return from a function
	//BX LR               @This is another way to return from a function, but it is commented out

@You shouldn't need this function for this assignment, but hey it's there for you
quit:
	MOV R7,#1
	SWI 0

