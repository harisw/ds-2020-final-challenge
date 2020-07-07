// FinalChallenge.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <chrono>
#include <array>
#include "BTree.h"

using namespace std;
using namespace std::chrono;

#define MAX_SIZE 1000002

//RBTree tree;
BTree tree(4);
ofstream outfile;
ofstream outInsertFile;
ofstream outDeleteFile;
ofstream outQueryFile;

const vector<string> explode(string& s, const char& c)
{
	string buff{ "" };
	vector<string> v;

	for (auto n : s)
	{
		if (n != c) buff += n; else
			if (n == c && buff != "") { v.push_back(buff); buff = ""; }
	}
	if (buff != "") v.push_back(buff);

	return v;
}

void insertPoint(vector<string>& inp)
{
	long int x = stoi(inp[2]);
	long int y = stoi(inp[3]);
	int i = stoi(inp[1]);
	tree.insert(i, x, y);
	return;
}

void deletePoint(vector<string>& inp)
{
	int i = stoi(inp[1]);
	//rb tree
	//tree.deleteNode(i);
	//b tree
	tree.deletion(i);
	return;
}

void queryPoint(vector<string>& inp)
{
	long double cent_x = stod(inp[1]);
	long double cent_y = stod(inp[2]);
	long double rad = stod(inp[3]);
	int farthest = 0;
	//rb tree
	//int result = tree.queryinorder(cent_x, cent_y, rad, farthest);
	//b tree
	int result = tree.queryTraverse(cent_x, cent_y, rad, farthest);


	if (!result)
		outfile << "0" << endl;
	else
		outfile << result << " " << farthest << endl;
	return;
}

void preparation()
{
	outfile.open("pout.txt");
	outInsertFile.open("time_ins_order4.txt");
	outDeleteFile.open("time_del_order4.txt");
	outQueryFile.open("time_que_order4.txt");
}
void closing()
{
	outfile.close();
	outInsertFile.close();
	outDeleteFile.close();
	outQueryFile.close();
}


int main()
{
    string line;
	ifstream in("pin_2.txt");
    vector<string> cur_line;
	const char delim = ' ';
	preparation();
	
	//TIME MEASUREMENT VARIABLES
	int insDur = 0, delDur = 0, queDur = 0;
	int insCount = 1, delCount = 1, queCount = 1;
	auto start = high_resolution_clock::now();
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start).count();
    while (getline(in, line)) {
        cur_line = explode(line, delim);
		switch (cur_line[0][0])
		{
		case '+':
			start = high_resolution_clock::now();
			insertPoint(cur_line);
			stop = high_resolution_clock::now();
			duration = duration_cast<microseconds>(stop - start).count();
			insDur += duration;
			if (insCount % 5000 == 0) {
				outInsertFile << insCount << ". TIME AVG : " << insDur / 5000 << " microseconds" << endl;
				insDur = 0;
			}
			insCount++;
			break;
		case '-':
			start = high_resolution_clock::now();
			deletePoint(cur_line);
			stop = high_resolution_clock::now();
			duration = duration_cast<microseconds>(stop - start).count();
			delDur += duration;
			if (delCount % 2000 == 0) {
				outDeleteFile << delCount << ". TIME AVG : " << delDur / 3000 << " microseconds" << endl;
				delDur = 0;
			}
			delCount++;
			break;
		case '?':
			start = high_resolution_clock::now();
			queryPoint(cur_line);
			stop = high_resolution_clock::now();
			duration = duration_cast<microseconds>(stop - start).count();
			queDur += duration;
			if (queCount % 1400 == 0) {
				outQueryFile << queCount << ". TIME AVG : " << queDur / 1400 << " microseconds" << endl;
				queDur = 0;
			}
			queCount++;
			break;
		default:
			break;
		}

    }
	closing();
	return 1;
}