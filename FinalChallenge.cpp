// FinalChallenge.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <chrono>
#include <array>
using namespace std;
using namespace std::chrono;

#define MAX_SIZE 1000002
struct Point {
	int x;
	int y;
	Point(int _x, int _y) {
		x = _x;
		y = _y;
	}
};

Point* filler = new Point(NULL, NULL);
vector<Point*> points(MAX_SIZE, filler);
int filled[MAX_SIZE];

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
	int x = stoi(inp[2]);
	int y = stoi(inp[3]);
	int i = stoi(inp[1]);
	//points.insert(points.begin() + i, &Point(x, y));
	points[i] = new Point(x, y);
	return;
}

void deletePoint(vector<string>& inp)
{
	int i = stoi(inp[1]);
	points[i] = NULL;
	return;
}

void queryPoint(vector<string>& inp)
{
	int cent_x = stoi(inp[1]);
	int cent_y = stoi(inp[2]);
	int rad = stoi(inp[3]);

	int distance = pow(rad, 2) - pow(cent_x;
}
int main()
{
    string line;
    ifstream in("pin.txt");
    ofstream outfile;
    outfile.open("pout.txt");
    vector<string> cur_line;
	const char delim = ' ';

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



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
