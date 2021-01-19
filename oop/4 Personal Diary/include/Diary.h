#ifndef DIARY_H
#define DIARY_H
#pragma warning(disable:4996)
#include<iostream>
#include<vector>
#include<stdlib.h>
#include<string>
#include"Date.h"
#include <fstream>
#include <regex>
#include <sstream>
#include<iomanip>
using namespace std;


class Diary
{
public:
	Diary();
	~Diary();
	//readFile		read the diary from cache
	void readFile(ifstream &);
	//writeFile		write the diary into cache
	void writeFile(ofstream &);
	//readStdin		get the diary from stdin
	void readStdin();
	//writeStdin	show the diary in the stdin
	void writeStdin();
	//cmpFunc		for the sort of diary based on date
	static const bool cmpFunc(Diary*p1, Diary*p2);
	Date date;

private:
	//store the text in line order of each diary
	vector<string>lineList;
};



#endif // !DIARY_H

