a.out: main.o foobar.o baz.o quz.o
	g++ main.o foobar.o baz.o quz.o

main.o: main.cc
	g++ -c main.cc

foobar.o: foobar.s
	g++ -c foobar.s

baz.o: baz.s
	g++ -c baz.s

quz.o: quz.s
	g++ -c quz.s

clean:
	rm -f *.o a.out core
