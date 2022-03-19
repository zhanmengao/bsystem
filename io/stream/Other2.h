#pragma once
//Program shows how to rewind a file. It writes a text file and opens it for reading, then rewinds
// it to the beginning and reads it again.
#include <iostream>
#include <fstream>
using namespace std;

int mainfs()
{
	// Variables needed to read or write file one character at a time char ch;
	fstream ioFile("rewind.txt", ios::out);
	// Open file.
	if (!ioFile)
	{
		cout << "Error in trying to create file";
		return 0;
	}
	// Write to file and close
	ioFile << "All good dogs" << endl << "growl, bark, and eat." << endl;
	ioFile.close();
	//Open the file
	ioFile.open("rewind.txt", ios::in);
	if (!ioFile)
	{
		cout << "Error in trying to open file";
		return 0;
	}
	// Read the file and echo to screen
	char ch;
	ioFile.get(ch);
	while (!ioFile.fail())
	{
		cout.put(ch);
		ioFile.get(ch);
	}
	//Rewind the file
	ioFile.clear();
	ioFile.seekg(0, ios::beg);
	//Read file again and echo to screen
	ioFile.get(ch);
	while (!ioFile.fail())
	{
		cout.put(ch);
		ioFile.get(ch);
	}
	return 0;
}

