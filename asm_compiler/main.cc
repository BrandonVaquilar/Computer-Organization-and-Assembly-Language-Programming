//Assisted Tourangeau
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <unistd.h>
#include <vector>
#include <cctype>
using namespace std;

int line_no = 0;
void die(int line_no = 0) {
	cout << "Syntax Error on line " << line_no << endl;
	exit(1);
}

//YOINK FROM KERNEY >:)
string read_reg(string reg) {
	if (reg == "A" ) return ("R4");
	else if (reg == "B" ) return ("R5");
	else if (reg == "C" ) return ("R6");
	else if (reg == "I" ) return ("R7");
	else if (reg == "J" ) return ("R8");
	else if (reg == "X" ) return ("R9");
	else if (reg == "Y" ) return ("R10");
	else if (reg == "Z" ) return ("R11");
	else die(line_no);
	return reg;
}

int main(int argc, char **argv) {
	vector<string> vec_str = {};
	// Emit only: assembly and execution are separate, explicit build steps.
	if (argc > 2) {
		cerr << "Usage: bb8 [output.s] < program.bas\n";
		return 1;
	}
	ofstream outs(argc == 2 ? argv[1] : "main.s");
	if (!outs) {
		cout << "Error opening file.\n";
		return -1;
	}
	outs << ".syntax unified\n.arm\n.text\n.global main\nmain:\n";
	outs << "\tPUSH {LR}\n\tPUSH {R4-R12}\n\n";

	//Initialize all variables to 0
	for (int i = 0; i < 13; i++) {
		outs << "\tMOV R" << i << ",#0\n";
	}

	int num_str = 0;
	while (cin) {
		string s;
		getline(cin,s);
		line_no++;
		if (!cin) break;
		// Normalize keywords without changing quoted text or escape sequences.
		bool quoted = false, escaped = false;
		for (char& ch : s) {
			if (ch == '"' && !escaped) quoted = !quoted;
			if (!quoted) ch = static_cast<char>(toupper(static_cast<unsigned char>(ch)));
			escaped = ch == '\\' && !escaped;
		}
		if (s.find_first_not_of(" \t\r") == string::npos) continue;
		if (s == "QUIT") break;
		stringstream ss(s); //Turn s into a stringstream
		int label;
		ss >> label;
		if (!ss) die(line_no);
		outs << "line_" << label << ":\n"; //Write each line number to the file ("line_20:")
		string command;
		ss >> command;
		if (!ss) die(line_no);

		if (command == "REM") {
			continue;
		}
		else if (command == "GOTO") {
			int target;
			ss >> target;
			if (!ss) die(line_no);
			outs << "\tBAL line_" << target << endl;
			continue;
		}
		else if (command == "EXIT") {
			outs << "\tBAL quit\n";
			continue;
		}
		else if (command == "IF") { //YOINK CODE FROM KERNEY
			string v1,oper,v2,then,_goto,line,_else; //X < Y THEN GOTO 100
			int line1 = -1, line2 = -1;
			ss >> v1 >>  oper >> v2 >> then >> _goto >> line; //ss in this order
			if (!ss) die(line_no);
			if (then != "THEN") die(line_no); //vet inputs
			if (_goto != "GOTO") die(line_no);
			try {line1 = stoi(line);} catch (...) {die(line_no); } //Convert to interger formats
			bool has_else = static_cast<bool>(ss >> _else);
			if (has_else) {
				if (_else != "ELSE" || !(ss >> _goto >> line) || _goto != "GOTO") die(line_no);
				try { line2 = stoi(line); } catch (...) {die(line_no); }
			}
			v1 = read_reg(v1);	//Convert registers "X" to "R9"
			v2 = read_reg(v2);	//Convert again
			//now turn it into assembly
			outs << "\tCMP " << v1 << ", " << v2 << endl;	//CMP X, Y endline
			if (oper == "<" )			outs << "\tBLT line_" << line1 << endl;
			else if (oper == "<=" )		outs << "\tBLE line_" << line1 << endl;
			else if (oper == ">" )		outs << "\tBGT line_" << line1 << endl;
			else if (oper == ">=" )		outs << "\tBGE line_" << line1 << endl;
			else if (oper == "==" )		outs << "\tBEQ line_" << line1 << endl;
			else if (oper == "!=" )		outs << "\tBNE line_" << line1 << endl;
			else die(line_no);	//invalid oper
			if (has_else) outs << "\tBAL line_" << line2 << endl;	//ELSE branch
		}

		else if (command == "LET") {
			string v1, equal, v2, oper, v3;
			int int_v2 = -1;
			ss >> v1 >> equal >> v2;
			if (!ss) die(line_no);
			bool has_oper = true;
			if (equal != "=") die(line_no);
			ss >> oper >> v3;
			if (!ss) has_oper = false;
			if (has_oper) {
				v2 = read_reg(v2);
				v3 = read_reg(v3);
			}
			else {
				try {int_v2 = stoi(v2);} catch (...) {die(line_no); }
			}
			v1 = read_reg(v1);
			if (has_oper) {
				if (oper == "+")		outs << "\tADD " << v1 << ", " << v2 << ", " << v3 << endl;
				else if (oper == "-")		outs << "\tSUB " << v1 << ", " << v2 << ", " << v3 << endl;
				else if (oper == "*")		outs << "\tMUL " << v1 << ", " << v2 << ", " << v3 << endl;
				else die(line_no);
			}
			else if (has_oper == false) {
				outs << "\tLDR " << v1 << ", =" << int_v2 << endl;
			}
			else die(line_no);

		}
		else if (command == "PRINT") {
            string value;
            getline(ss >> ws, value);
            const auto end = value.find_last_not_of(" \t\r");
            if (end == string::npos) die(line_no);
            value.erase(end + 1);
            if (value.front() == '"') {
                if (value.size() < 2 || value.back() != '"') die(line_no);
                vec_str.push_back(value);
                ++num_str;
                outs << "\tLDR R0,=string_" << num_str << "\n\tBL print_string\n";
            } else {
                outs << "\tMOV R0, " << read_reg(value) << "\n\tBL print_number\n";
            }
        }

		else if (command == "END") {
			outs << "\tBAL quit" << endl;
		}
		else if (command == "INPUT") {
			string v;
			ss >> v;
			if (!ss) die(line_no);
			v = read_reg(v);
			outs << "\tBL read_number\n\tMOV " << v << ", R0\n";
		}
		else die(line_no);


		//YOU: Put all of your code here, interpreting the different commands in BB8


	}

	//Clean up the file at the bottom
	outs << "\nquit:\n\tMOV R0, #0\n\tPOP {R4-R12}\n\tPOP {PC}\n"; //Finish the code and return

	if (vec_str.size() > 0) {
		outs << ".data" << endl;
		for (size_t i = 0; i < vec_str.size(); i++) {
			outs << "string_" << i+1 << ": .asciz " << vec_str.at(i) << endl;
		}
	}


    outs << ".section .note.GNU-stack,\"\",%progbits\n";
    if (!outs) {
        cerr << "Failed to write assembly output\n";
        return 1;
    }
    return 0;
}
