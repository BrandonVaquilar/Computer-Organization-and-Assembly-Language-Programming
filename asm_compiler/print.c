#include <stdio.h>

void print_number(int x) {
	printf("%i\n",x);
}

void print_string(char *str) {
	printf("%s",str);
}

int read_number() {
	int temp = 0;
	scanf("%i",&temp);
	return temp;
}
