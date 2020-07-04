// FinalChallenge.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <chrono>
#include <array>
#include "BTree.h"
#include "RBTree.h"

using namespace std;
using namespace std::chrono;

#define MAX_SIZE 1000002

RBTree tree;
ofstream outfile;
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
	tree.deleteNode(i);
	return;
}

void queryPoint(vector<string>& inp)
{
	long int cent_x = stoi(inp[1]);
	long int cent_y = stoi(inp[2]);
	long int rad = stoi(inp[3]);
	int farthest = 0;
	int result = tree.queryinorder(cent_x, cent_y, rad, farthest);
	if (!result)
		outfile << "0" << endl;
	else
		outfile << result << " " << farthest << endl;
	return;
}

void testBTree()
{
	string line;
	ifstream in("pin.txt"); 
	vector<string> cur_line;
	const char delim = ' ';
	BTree tree(4);
 	while (getline(in, line))
	{
		cur_line = explode(line, delim);
		if(cur_line[0][0] == '+')
			tree.insert(stoi(cur_line[1]));
	}
	in.close();
}

void testRBTree()
{
	string line;
	ifstream in("pin.txt");
	vector<string> cur_line;
	const char delim = ' ';
	RBTree tree;
	while (getline(in, line))
	{
		cur_line = explode(line, delim);
		if (cur_line[0][0] == '+')
			tree.insert(stoi(cur_line[1]), stoi(cur_line[2]), stoi(cur_line[3]));
	}
	in.close();
}
int main()
{

	//testRBTree();
    string line;
    ifstream in("pin_1.txt");
    outfile.open("pout.txt");
    vector<string> cur_line;
	const char delim = ' ';
	int j = 0;
    while (getline(in, line)) {
        cur_line = explode(line, delim);
		switch (cur_line[0][0])
		{
		case '+':
			insertPoint(cur_line);
			break;
		case '-':
			deletePoint(cur_line);
			break;
		case '?':
			queryPoint(cur_line);
			break;
		default:
			return 1;
		}

    }
    outfile.close();
	return 1;
}