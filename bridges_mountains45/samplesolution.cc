#include <iostream>
#include <fstream>
#include <cmath>
#include <climits>
#include <algorithm>
#include <vector>
using namespace std;

void die() {
	cout << "BAD INPUT!\n";
	exit(EXIT_FAILURE);
}

int main(){
	ifstream data("Colorado_480x480.dat");
	if (!data) die();
	const int ROWS = 480, COLS = 480;
	vector<vector<int>> vec(ROWS, vector<int>(COLS)); //Make a matrix of size ROWSxCOLS
	for (size_t i = 0; i < ROWS; i++) {
		for (size_t j = 0; j < COLS; j++) {
			int x = 0;
			data >> x;
			if (!data) break;
			vec.at(i).at(j) = x;
		}
	}
	//cout << "What row do you want to start with?\n";
	//cin >> row;
	//gg=G vim command to reformat
	//if (row < 0 or row >= ROWS or !cin) die();
	vector<int> total_changes;
	for (int start_row = 0; start_row < ROWS; start_row++) {
		int row = start_row;
		int col = 0;
		int total_change = 0;
		while (col < COLS-1) {
			//cerr << row << "," << col;
			int current_elevation = vec.at(row).at(col); 
			int ne = INT_MAX;
			if (row > 0)
				ne = abs(current_elevation - vec.at(row-1).at(col+1));
			int e = abs(current_elevation - vec.at(row).at(col+1));
			int se = INT_MAX;
			if (row < ROWS-1) 
				se = abs(current_elevation - vec.at(row+1).at(col+1));
			if (ne < e and ne < se) {
				row = row - 1;
				col = col + 1;
			}
			else if (se < e and se < ne) {
				row += 1;
				col += 1;
			}
			else if (e <= se and e <= ne) {
				col++;
			}
			else {
				col++;
				if (rand() % 2) row++;
				else row--;
			}
			int new_elevation = vec.at(row).at(col);
			//cout << " moving up/down " << new_elevation - current_elevation << endl;
			total_change += abs(new_elevation - current_elevation);
		}
		//cout << "Start Row: " << start_row << " Total change: " << total_change << endl;
		total_changes.push_back(total_change);
	}
	int min_row = 0;
	int smallest_change = total_changes.at(0);
	for (size_t i = 0; i < total_changes.size(); i++) {
		if (total_changes.at(i) < smallest_change) {
			min_row = i;
			smallest_change = total_changes.at(i);
		}
	}
	cout << "Row " << min_row << " had the smallest change at " << smallest_change << endl;
}

