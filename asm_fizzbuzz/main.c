
#include <stdio.h>

void fizzbuzz(); //This is how you call an assembly function

//To call this from assembly, write "BL print_fizz"
//When you call this from assembly, registers R0 through R3 GET WIPED
void print_fizz() { //These will be called by your assembly function
	printf("Fizz\n");
	return;
}

void print_buzz() {
	printf("Buzz\n");
	return;
}

void print_fizzbuzz() {
	printf("FizzBuzz\n");
	return;
}

void print_number(int x) {
	printf("%i\n",x);
	return;
}

int main() {
	printf("Calling your assembly fizzbuzz program...\n");
	fizzbuzz();
	printf("Your assembly fizzbuzz successfully returned control.\n");
}
