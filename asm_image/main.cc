#define cimg_display 0
#include "CImg.h"
#include <iostream>
#include <cstdlib>
using namespace cimg_library;
using namespace std;

//This will dump the contents of an image file to the screen
void print_image (unsigned char *in, int width, int height) {
	for (int i = 0; i < width*height*3; i++)
		cout << (unsigned int)in[i] << endl;
}

#ifdef student_darken
extern "C" {
	void sdarken(unsigned char *in,unsigned char *out, int width, int height);
}
#elif sepia
extern "C" {
	void ssepia(unsigned char *in,unsigned char *out, int width, int height);
}

#elif student_filter
extern "C" {
	void sfilter2(unsigned char *in,unsigned char *out, int width, int height);
}
#elif student_filter2
extern "C" {
	void sfilter2(unsigned char *in,unsigned char *out, int width, int height);
}
#else 
extern "C" {
	//This function will reduce the brightness of all colors in the image by half, and write the results to out

	// Option 1
	//You will need to implement this loop here in ARM32 assembly 
	void darken(unsigned char *in,unsigned char *out, int width, int height) {
		for (int i = 0; i < width*height*3; i++)
			out[i] = in[i] / 2; //Divide red by 2
		return;
	}

	/* Option 2
	void darken(unsigned char *in,unsigned char *out, int width, int height) {
		int stride = width*height;
		for (int i = 0; i < width*height; i++)
			//The first 2D array of width*height pixels is red
			out[i] = in[i] / 2; //Divide red by 2
			//The second 2D array of width*height pixels is green
			out[i+stride] = in[i+stride] / 2; //Divide green by 2
			//The third 2D array of width*height pixels is blue
			out[i+stride+stride] = in[i+stride+stride] / 2; //Divide blue by 2
		return;
	}
	*/
	/*
	//Option 3 - This seg faults
	void darken(unsigned char *in2,unsigned char *out2, int width, int height) {
		//Cast from a pointer to a 1D array to a pointer to a 3D array
		unsigned char*** in = (unsigned char***)in2;
		unsigned char*** out = (unsigned char***)out2;
		for (int color = 0; color < 3; color++) {
			for (int i = 0; i < height; i++) {
				for (int j = 0; i < width; i++) {
					out[color][i][j] = in[color][i][j] / 2;
				}
			}
		}
		return;
	}
	*/
}
#endif

void usage() {
	cout << "Error: this program needs to be called with a command line parameter indicating what file to open.\n";
	cout << "For example, a.out kyoto.jpg\n";
	exit(1);
}

int main(int argc, char **argv) {
	if (argc != 2) usage(); //Check command line parameters

	//PHASE 1 - Load the image
	clock_t start_time = clock();
	CImg<unsigned char> image(argv[1]);
	CImg<unsigned char> darkimage(image.width(),image.height(),1,3,0);
	clock_t end_time = clock();
	cerr << "Image load time: " << double(end_time - start_time)/CLOCKS_PER_SEC << " secs\n";

	//PHASE 2 - Do the image processing operation
	start_time = clock();
#ifdef student_darken
	sdarken(image,darkimage,image.width(),image.height());
	end_time = clock();
	cerr << "Darken time: " << double(end_time - start_time)/CLOCKS_PER_SEC << " secs\n";
#elif sepia
	ssepia(image,darkimage,image.width(),image.height());
	end_time = clock();
	cerr << "Student Sepia time: " << double(end_time - start_time)/CLOCKS_PER_SEC << " secs\n";
#elif student_filter
	sfilter(image,darkimage,image.width(),image.height());
	end_time = clock();
	cerr << "Student Filter time: " << double(end_time - start_time)/CLOCKS_PER_SEC << " secs\n";
#elif student_filter2
	sfilter2(image,darkimage,image.width(),image.height());
	end_time = clock();
	cerr << "Student Filter 2 time: " << double(end_time - start_time)/CLOCKS_PER_SEC << " secs\n";
#else
	darken(image,darkimage,image.width(),image.height());
	end_time = clock();
	cerr << "Reference Darken time: " << double(end_time - start_time)/CLOCKS_PER_SEC << " secs\n";
#endif

	//PHASE 3 - Write the image
	start_time = clock();
	darkimage.save_jpeg("output.jpg",50);
	//darkimage.save_png("output.png");
	end_time = clock();
	cerr << "Image write time: " << double(end_time - start_time)/CLOCKS_PER_SEC << " secs\n";
}
