#pragma once
#include <iostream>
using namespace std;

struct position
{
	int x;
	int y;
	double rotation;
	friend ostream& operator<<(ostream& out, const position& pos);
};

ostream& operator<<(ostream& out, const position& pos)
{
	cout << "x = " << pos.x << "   y = " << pos.y << "  rotation = " << pos.rotation << endl;
}