#include </public/read.h>
using namespace std;

//extern "C" is how you connect assembly functions and C++
extern "C" {
	int foo(int x); //int foo(int x) { return 4*x; }
	int bar(int x, int y); //int bar(int x, int y) { return xy - pow(y,2); }
	int baz(int x, int y, int z); //int baz(int x, int y, int z) { return 5*x + 5*x*y + 5*x*y*z; }
	int quz(int x, int y, int z, int w); //int quz(int x, int y, int z, int w) { return (x-y)*(z+w) - (x-y)*(-1*z + -1*w); }
}

int main() {
	const int x = read("Please input a value for x:\n");
	const int y = read("Please input a value for y:\n");
	const int z = read("Please input a value for z:\n");
	const int w = read("Please input a value for w:\n");
	const int choice = read("Please choose which function you want to call:\n 1) foo (4*x)\n 2) bar (xy-y^2)\n 3) baz (5x+5xy+5xyz)\n 4) quz (x-y)*(z+w) - (x-y)*(-1*z + -1*w)\n (Anything else) Quit\n");
	if (choice == 1) cout << foo(x) << endl;
	else if (choice == 2) cout << bar(x,y) << endl;
	else if (choice == 3) cout << baz(x,y,z) << endl;
	else if (choice == 4) cout << quz(x,y,z,w) << endl;
}
