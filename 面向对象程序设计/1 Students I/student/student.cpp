#include "pch.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace std;
int lines(string thefile)//use lines to confirm how many students
{
	ifstream fin(thefile);
	if (!fin)
	{
		cout << "do not find the txt file" << endl;
		system("pause");
		return 0;
	}
	char c;
	int lineCnt = 0;
	while (fin.get(c))
	{
		if (c == '\n')//use \n to confirm how many lines
			lineCnt++;
	}
	fin.close();
	return lineCnt;
}
int main()
{
	struct Student {
		int Number;
		string name;
		int score1;
		int score2;
		int score3;
		double average;
	}student[10000];//define the struct to store the data, max can contain 10000 students
	int counter;
	double line=double(lines("test.txt"));
	ifstream testfile("test.txt");
    if (!testfile.is_open())//if the file is not opened correctly
	{
		cout << "do not find the txt file" << endl;
		system("pause");
		return 0;
	}
	cout << "no  name            score1          score2          score3          average\n"; //output the title
	string title;
	int score1sum = 0, score2sum = 0, score3sum = 0, score1min = 6, score2min = 6, score3min = 6, score1max = 0, score2max = 0, score3max = 0;//the averages,maxs and mins;
	for (counter = 0; counter <= line; counter++)//data input and data analyze
	{
		testfile >> student[counter].Number >> student[counter].name >> student[counter].score1 >> student[counter].score2 >> student[counter].score3;//datainput
		student[counter].average = (student[counter].score1 + student[counter].score2 + student[counter].score3) / 3.0;//calculate the average,max and min
		score1sum += student[counter].score1;
		score2sum += student[counter].score2;
		score3sum += student[counter].score3;
		if (student[counter].score1 < score1min)
		{
			score1min = student[counter].score1;
		}
		if (student[counter].score2 < score2min)
		{
			score2min = student[counter].score2;
		}
		if (student[counter].score3 < score3min)
		{
			score3min = student[counter].score3;
		}
		if (student[counter].score1 > score1max)
		{
			score1max = student[counter].score1;
		}
		if (student[counter].score2 > score2max)
		{
			score2max = student[counter].score2;
		}
		if (student[counter].score3 > score3max)
		{
			score3max = student[counter].score3;
		}
	};
	for (counter = 0; counter <= line; counter++)
	{
		cout <<left<<setw(4)<< student[counter].Number << setw(16) << student[counter].name << setw(16) << student[counter].score1 << setw(16) << student[counter].score2 << setw(16) << student[counter].score3 << setw(16) << student[counter].average << endl;
	};//output the data
	double score1ave = score1sum / line;
	double score2ave = score2sum / line;
	double score3ave = score3sum / line;
	cout << left << "    " << setw(16) << "average" << setw(16) << score1ave << setw(16) << score2ave << setw(16) << score3ave << endl;
	cout << left << "    " << setw(16) << "min" << setw(16) << score1min << setw(16) << score2min << setw(16) << score3min << endl;
	cout << left << "    " << setw(16) << "max" << setw(16) << score1max << setw(16) << score2max << setw(16) << score3max << endl;//output the averages,maxs and mins.
	system("pause");
	return 0;
}



