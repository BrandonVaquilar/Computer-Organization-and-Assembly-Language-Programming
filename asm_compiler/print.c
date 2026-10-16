#include <stdio.h>
#include <stdlib.h>

void print_number(int x) {
	printf("%i\n",x);
}

void print_string(char *str) {
	printf("%s",str);
}

int read_number() {
	int temp = 0;
	if (scanf("%i",&temp) != 1) {
		fprintf(stderr, "Expected an integer for INPUT\n");
		exit(1);
	}
	return temp;
}
