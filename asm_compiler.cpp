#include <cstdlib>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <unistd.h>
#include <vector>
using namespace std;

int line_no = 0;
void die(int line_no = 0) {
    cout << "Syntax Error on line " << line_no << endl;
    exit(1);
}

string read_reg(string reg) {
    if (reg == "A" ) return ("R4");
    else if (reg == "B" ) return ("R5");
    else if (reg == "C" ) return ("R6");
    else if (reg == "I" ) return ("R7");
    else if (reg == "J" ) return ("R8");
    else if (reg == "X" ) return ("R9");
    else if (reg == "Y" ) return ("R10");
    else if (reg == "Z" ) return ("R12");
    else die(line_no);
    return reg;
}

int main(int argc, char **argv) {
    vector<string> vec_str = {};
    //If we pass any parameters, we'll just generate an assembly file
    //Otherwise we will generate an assembly file, assemble it, and run it
    bool assemble_only = false;
    if (argc > 1) assemble_only = true;
    ofstream outs("main.s"); //This is the assembly file we're creating
    if (!outs) {
        cout << "Error opening file.\n";
        return -1;
    }
    outs << ".global main\nmain:\n"; //Get the file ready to rock
    outs << "\tPUSH {LR}\n\tPUSH {R4-R12}\n\n";

    //Initialize all variables to 0
    for (int i = 0; i < 13; i++) {
        outs << "\tMOV R" << i << ",#0\n";
    }

    int num_str = 0;
    //int line_no = 0;
 while (cin) {   //We have so far: REM, GOTO, EXIT, IF, LET, END, PRINT "STRING", PRINT VARIABLE     Need:  INPUT
        string s;
        getline(cin,s);
        line_no++;
        if (!cin) break;
        transform(s.begin(), s.end(), s.begin(), ::toupper); //Uppercaseify
        auto it = s.find("QUIT"); //TERMINATE COMPILER
        if (it != string::npos) break;
    //  auto it2 = s.find("END"); //TERMINATE COMPILER
    //  if (it2 != string::npos) break;
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
        else if (command == "IF") { //use code from lecture and learn :pepoG:
            string v1,oper,v2,then,_goto,line,_else; //example:  X < Y THEN GOTO 100
            int line1 = -1, line2 = -1;
            ss >> v1 >>  oper >> v2 >> then >> _goto >> line; //read commands in order
            if (!ss) die(line_no);
            if (then != "THEN") die(line_no);
            if (_goto != "GOTO") die(line_no);
            try {line1 = stoi(line);} catch (...) {die(line_no); } //Use stoi to convert first line into integers and use default exception
            bool has_else = true;
            ss >> _else >> _goto >> line;
            try { line2 = stoi(line); } catch (...) {die(line_no); }
            if (!ss) has_else = false;
            v1 = read_reg(v1);  //Convert registers ex: "X" to "R9"
            v2 = read_reg(v2);  //Convert register again
            //now turn it into assembly
            outs << "\tCMP " << v1 << ", " << v2 << endl;   //ex: "tab CMP X, Y endline"
            if (oper == "<" )           outs << "\tBLT line_" << line1 << endl;
            else if (oper == "<=" )     outs << "\tBLE line_" << line1 << endl;
            else if (oper == ">" )      outs << "\tBGT line_" << line1 << endl;
            else if (oper == ">=" )     outs << "\tBGE line_" << line1 << endl;
            else if (oper == "==" )     outs << "\tBEQ line_" << line1 << endl;
            else if (oper == "!=" )     outs << "\tBNE line_" << line1 << endl;
            else die(line_no);  //invalid operator
            if (has_else) outs << "\tBAL line_" << line2 << endl;   //if it has an ELSE, branch to designated line
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
                if (oper == "+")        outs << "\tADD " << v1 << ", " << v2 << ", " << v3 << endl;
                else if (oper == "-")       outs << "\tSUB " << v1 << ", " << v2 << ", " << v3 << endl;
                else if (oper == "*")       outs << "\tMUL " << v1 << ", " << v2 << ", " << v3 << endl;
                else die(line_no);
            }
            else if (has_oper == false) {
                outs << "\tMOV " << v1 << ", #" << int_v2 << endl;
            }
            else die(line_no);

        }
        else if (command == "PRINT") {  //first let's do strings, then we'll work on variables
            string string, temp, temp2, temp3, var;
            bool is_first = true;
            bool is_variable = false;
            while (ss) {
                temp3 = temp2;
                ss >> temp2;
                if (temp2 == temp3) break;
                if (is_first) {
                    if (temp2.front() != '"') {
                        is_variable = true;
                        break;
                    }
                    string = string + temp2;
                    is_first = false;
                    continue;
                }
                string = string + " " + temp2;
            }

            if (is_variable) {
                //try {var = stoi(temp2);} catch (...) {die(); }
                var = read_reg(temp2);
                outs << "\tMOV R0, " << var << endl << "\tBL print_number" << endl;
            }
            else {
                for (int i = 0; i < string.size(); i++) {
                    if (string.at(i) == 'N' and string.at(i-1) == '\'') string.at(i) = tolower(string.at(i));
                    temp = temp + string.at(i);

                }
                string = temp;
                if (string.at(0) != '"' or string.back() != '"') die(line_no);
                //cerr << endl << "Front: " << string.front() << endl << "Back: " << string.back() << endl;
                //cerr << endl << endl << string << endl << endl;

                vec_str.push_back(string);
                //cerr << endl << endl << vec_str.at(0) << endl << endl;
                num_str++;
                outs << "\tLDR R0,=string_" << num_str << endl << "\tBL print_string" << endl;

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
            outs << "\tMOV R0, " << v << endl << "\tBL read_number" << endl;
        }
        else die(line_no);


        //YOU: Put all of your code here, interpreting the different commands in BB8


    }

    //Clean up the file at the bottom
    outs << "\nquit:\n\tMOV R0, #42\n\tPOP {R4-R12}\n\tPOP {PC}\n"; //Finish the code and return

    if (vec_str.size() > 0) {
        outs << ".data" << endl;
        for (int i = 0; i < vec_str.size(); i++) {
            outs << "string_" << i+1 << ": .asciz " << vec_str.at(i) << endl;
        }
    }

    outs.close(); //Close the file

    if (assemble_only) return 0; //When you're debugging you should run bb8 with a parameter

    //print.o is made from the Makefile, so make sure you make your code
    if (system("g++ main.s print.o")) { //Compile your assembler code and check for errors
        cout << "Assembling failed, which means your compiler screwed up.\n";
        return 1;
    }
    //We've got an a.out now, so let's run it!
    cout << "Compilation successful. Executing program now." << endl;
    execve("a.out",NULL,NULL);
}
                                                          