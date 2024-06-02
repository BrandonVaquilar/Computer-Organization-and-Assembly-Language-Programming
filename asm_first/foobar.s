//This file has two functions in it, foo and bar
//The other files have one each. 
//Par: 6 strokes

//Foo should be the assembly version of this function:
//int foo(int x) { return 4*x; }
.global foo
foo:
    //Your code here
	MOV R0, R0, LSL #2
	BX LR

//Bar should be the assembly version of this function:
//int bar(int x, int y) { return xy - pow(y,2); }
.global bar
bar:
    //Your code here
	SUB R0, R0, R1
	MUL R0, R0, R1
	BX LR

