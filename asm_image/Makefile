a.out: main.o student_filter2.o #sepia.o student_darken.o
	#g++ -mfpu=neon main.o student_darken.o
	#g++ -mfpu=neon main.o sepia.o
	#g++ -mfpu=neon main.o student_filter.o
	g++ -mfpu=neon main.o student_filter2.o

#student_darken.o: student_darken.s
#	as -mfpu=neon -o student_darken.o student_darken.s

#sepia.o: sepia.s
#	as -mfpu=neon -o sepia.o sepia.s -g

#student_filter.o: student_filter.s
#	as -mfpu=neon -o student_filter.o student_filter.s -g

student_filter2.o: student_filter2.s
	as -mfpu=neon -o student_filter2.o student_filter2.s -g
main.o: main.cc
	#g++ -c -Dcimg_display=0 -fexceptions main.cc
	#g++ -O1 -c -Dcimg_display=0 -fexceptions main.cc
	#g++ -O3 -c -Dcimg_display=0 -fexceptions main.cc
	#g++ -O3 -mfpu=neon -c -Dcimg_display=0 -Dstudent_darken -fexceptions main.cc
	#g++ -O3 -mfpu=neon -c -Dcimg_display=0 -Dsepia -fexceptions main.cc
	#g++ -O3 -mfpu=neon -c -Dcimg_display=0 -Dstudent_filter -fexceptions main.cc
	g++ -O3 -mfpu=neon -c -Dcimg_display=0 -Dstudent_filter2 -fexceptions main.cc

clean:
	rm -rf *.o a.out output.*
