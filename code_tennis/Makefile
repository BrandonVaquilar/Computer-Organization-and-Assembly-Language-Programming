a.out: teama.o teamb.o
	g++ teama.o teamb.o -pthread

teama.o: teama.cc
	g++ -O3 -c -Dcimg_display=0 -Dteamb -fexceptions -pthread teama.cc 

teamb.o: teamb.s
	g++ -c teamb.s -pthread


