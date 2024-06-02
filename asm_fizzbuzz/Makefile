a.out: main.o fizzbuzz.o
	gcc main.o fizzbuzz.o

fizzbuzz.o: fizzbuzz.s
	as -g -o fizzbuzz.o fizzbuzz.s

main.o: main.c
	gcc -g -c main.c

clean:
	rm -f a.out core *.o
