#include "pch.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;
int scoresum[5] = {0}, scoremin[5] = {5, 5, 5, 5, 5}, scoremax[5] = {0}; //store the max average min score
double course_student[5] = {0};											 //store the student of each course which the student choose
int course_sumnum = 0;													 //store the sum of the courses, the max is 5;
string course_name[5];													 //store the names of the courses;
int lines(string thefile)												 //use lines to confirm how many students
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
		if (c == '\n') //use \n to confirm how many lines
			lineCnt++;
	}
	fin.close();
	return lineCnt;
}
class Student //the student class
{
public:
	Student(ifstream &studentfile);
	int stu_num;
	string name;
	int score[5] = {-1, -1, -1, -1, -1}; //the scores of the students;
	void output();

private:
	double course_num;
	double average;
	void get_average();
};
typedef Student *Stupoint;
void Student::output()
{
	cout << left << setw(4) << stu_num << setw(10) << name;
	for (int i = 0; i < course_sumnum; i++)
	{
		if (score[i] == -1) //judge if the student choose this course
		{
			cout << right << setw(10) << "无";
		}
		else
		{
			cout << right << setw(10) << score[i];
			course_student[i]++;
		}
	}
	cout << right << setw(10) << average;
	cout << '\n';
}
Student::Student(ifstream &studentfile) //read the student information from the txt
{
	studentfile >> stu_num >> name;
	int p = 0;
	char ch;
	course_num = 0;
	while (studentfile.peek() != '\n') //one student, one line, use the enter to change to the next student
	{
		studentfile >> course_name[p] >> score[p];
		course_num++;
		p++;
	}
	p = 0;
	course_sumnum = course_num;
	course_num = 0;
	for (int i = 0; i < 5; i++)
	{
		if (score[i] != -1)
		{
			course_num++;
			scoresum[i] += score[i];
			if (score[i] > scoremax[i]) //find the max and min while reading
			{
				scoremax[i] = score[i];
			}
			if (score[i] < scoremin[i])
			{
				scoremin[i] = score[i];
			}
		}
	}
	get_average();
};
void Student::get_average() //count the average
{
	int tempsum;
	tempsum = 0;
	for (int j = 0; j < 5; j++)
	{
		if (score[j] == -1)
		{
			tempsum += 0;
		}
		else
		{
			tempsum += score[j];
		}
	}
	average = tempsum / course_num;
};
int main()
{
	double student_num = double(lines("studentfile.txt"));
	ifstream studentfile("studentfile.txt");
	Stupoint stupoint[20];
	for (int k = 0; k < student_num; k++) //create the objects
	{
		stupoint[k] = new Student{studentfile};
	}
	cout << "              ";
	for (int k = 0; k < course_sumnum; k++) //output the courses name
	{
		cout << right << setw(10) << course_name[k];
	}
	cout << right << setw(10) << "average";
	cout << '\n';
	for (int k = 0; k < student_num; k++) //out put the students' scores
	{
		stupoint[k]->output();
	}
	cout << "average       "; //out put the average,max,min
	for (int i = 0; i < course_sumnum; i++)
	{
		cout << right << setw(10) << scoresum[i] / course_student[i];
	}
	cout << "\n"
		 << "max           ";
	for (int i = 0; i < course_sumnum; i++)
	{
		cout << right << setw(10) << scoremax[i];
	}
	cout << "\n"
		 << "min           ";
	for (int i = 0; i < course_sumnum; i++)
	{
		cout << right << setw(10) << scoremin[i];
	}
}
