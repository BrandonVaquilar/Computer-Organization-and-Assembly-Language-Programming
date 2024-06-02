#include "/public/read.h"
#include <ctime>
#include <cmath>
#include <cctype>
#include <cstring>
#include <cstdlib>
using namespace std;

extern "C" {
	 void asm_spongebob(char *);
}

void cpp_spongebob(char *str) {
	if (!str) return;
	for (size_t i = 0; str[i]; i++) {
		char c = str[i];
		if (isalpha(c)) {
			bool coinflip = rand() % 2; //AND R0,R0,#7 is mod 8
			if (coinflip) c = toupper(c);
			else c = tolower(c);
		}
		str[i] = c;
	}
}

int main() {
	int seed = read<int>("Please enter a random seed:\n");
	ifstream ins(read<string>("Please enter a filename to spongebob:\n"));
	string whole_file;
	while (ins) {
		string temp = readline(ins);
		if (!ins) break;
		whole_file += temp + '\n';
	}
	whole_file.pop_back();

	//Copy C++ style string to two C style strings
	char *str1 = new char[whole_file.length()+1];
	char *str2 = new char[whole_file.length()+1];
	strcpy(str1,whole_file.c_str());
	strcpy(str2,whole_file.c_str());
	const bool verbose = whole_file.length() <= 1000 ? true : false; //Only print files of 1000 characters or smaller

	//C++ Implementation
	srand(seed);
	if (verbose) cout << "Str1 Before: " << str1 << endl;
	clock_t start = clock();
	cpp_spongebob(str1); //Passes a char * holding the string
	clock_t end = clock();
	if (verbose) cout << "Str1 After: " << str1 << endl;
	cerr << "C++ Implementation: " << end - start << " ticks." << endl; //endl here so we can flush before your code

	//ASM Implementation
	srand(seed);
	if (verbose) cout << "Str2 Before: " << str2 << endl;
	start = clock();
	asm_spongebob(str2); //Passes a char * holding the string
	end = clock();
	if (verbose) cout << "Str2 After: " << str2 << endl;
	cerr << "ASM Implementation: " << end - start << " ticks." << endl; //endl here so we can flush before your code

	if (strcmp(str1,str2)) {
		cout << "Error, the assembly function is making a different string than the C++ implementation." << endl;
	}
	else {
		cout << "Congrats, your program is correct!" << endl;
	}

	delete []str1;
	delete []str2;
}
