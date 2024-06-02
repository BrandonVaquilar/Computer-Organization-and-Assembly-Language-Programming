#define cimg_display 0
#include <iostream>
#include <ctime>
#include "CImg.h"
#include <cstdlib>
using namespace cimg_library;
using namespace std;

extern "C" {
	int function_one(unsigned char *in, unsigned char *out, size_t width, size_t height);
	int function_two(int *arr, size_t n, unsigned int stride);
}

   //UNCOMMENT TO TEST IMAGE
//	int print_image (unsigned char *in, unsigned char *out, int width, int height) { //Loads into the blank image
//	for (int i = 0; i < width*height*3; i++) {
//		out[i] = in[i] / 2;									//Darken to test everything is being passed in correctly			
//	}
//	return 0;
//}
 

	//UNCOMMENT TO TEST 
void test (int *arr, size_t n, unsigned int stride) {
	for (int i = 0; i < stride; i++) {
		cout << arr[i] << endl;
	}

}
//void test2(int retval) {
//	for (int i = 0; i < retval; i++) {
//		arr[i] = 100;
//	}
//}
int main(int argc, char **argv) {
	int retval;
	cout << "We have " << argc << " command line parameter(s)\n";
	cout << "Parameters are:\n";
	for (int i = 0; i < argc; i++) {
		cout << "Argument " << i << ": " << argv[i] << endl;
	}

	//YOU: Create an array here
	//ASK user to test their functions
	cout << "\n\nWARNING: If the data for the array in function_two is not changed, it will show the hidden data I put inside\n\n";
	cout << "Enter 1 to test function_one: \n" << "Enter 2 to test function_two: \n" << "Enter 3 to skip\n";
	int option = 0;
	while (true)	{
		cin >> option;
		if (option == 1) break;
		if (option == 2) break;
		if (option == 3) break;
	}
	//PHASE 1 - Loading image
	CImg<unsigned char> image("secretShhh");						//Save the image
	CImg<unsigned char> visu(image.width(), image.height(),1,3,0);	//Create a blank image
	//PHASE 2 - Image Processing
	//	print_image(image,visu,image.width(),image.height()); 		//UNCOMMENT TO TEST IMAGE
	//YOU: Create second array here
	size_t n = 2; 
	unsigned int stride = 46; //width * height
	int *arr = new int[stride]{1,1};
	for (int i = n; i < stride; i++) {
		arr[i] = arr[i-1] + arr[i-2];
	}
	//fibby(arr, n, stride);										//UNCOMMENT TO TEST function_two
	//OR... use a highres timer
	//Calling the assembly function like this
	clock_t start_time = clock();
	retval = function_one(image,visu,image.width(),image.height());	//Load array into the assembly function
	clock_t end_time = clock();
	cout << "Running time for function_one: " << end_time - start_time << " ticks\n";
	cout << "function_one returned: " << retval << endl;

	start_time = clock();
	retval = function_two(arr, n, stride);
	end_time = clock();
	cout << "Running time for function_one: " << end_time - start_time << " ticks\n";
	cout << "function_one returned: " << retval << endl;

	//PHASE 3 - Write the image
	
	//retval = print_image(image, visu, image.width(), image.height());

	if (option == 1) {
		visu.save_jpeg("output.jpg",50); //UNCOMMENT to Output the image
		cout << "Using function_one to test code...\n";
		cout << "Check your directory for \"output.jpg\"\n";
	}
	if (option == 2) {
		cout << "Using function_two to test code...\n";
		//function_two(arr, n, stride);
		//test2(arr, n, stride);
		test(arr, n, stride);
			}

	delete []arr;
}
