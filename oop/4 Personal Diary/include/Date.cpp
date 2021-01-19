#include"Date.h"
#include<time.h>
#include<iomanip>
#include<fstream>
#include<iostream>
#include<sstream>
#include<regex>
#include<list>
#pragma warning(disable:4996)
using namespace std;

void Date::init() {
	time_t tmp = time(nullptr);
	tm *lt = localtime(&tmp);  // Get local time
	yy = lt->tm_year + 1900;
	mm = lt->tm_mon + 1;
	dd = lt->tm_mday;
}

//return true if date1=date2
bool Date::op_equal(const Date &date1,const Date &date2) {
	if (date1.yy == date2.yy&&date1.mm == date2.mm&&date1.dd == date2.dd)return true;
	else return false;
}

//return true if date1<=date2
bool Date::op_leq(const Date &date1, const Date &date2) {
	if (date1.yy < date2.yy)return true;
	else if (date1.yy > date2.yy)return false;
	if (date1.mm < date2.mm)return true;
	else if (date1.mm > date2.mm)return false;
	if (date1.dd < date2.dd)return true;
	else if (date1.dd > date2.dd)return false;
	return true;
}

string Date::toStr() {
	stringstream tmp;
	tmp << setw(4) << setfill('0') << yy << '-' << setw(2) << setfill('0') << mm <<'-'<< setw(2) << setfill('0') << dd;
	return tmp.str();
}

bool Date::fromStr(string str) {
	
	regex sep("\\s*-\\s*");  // Use '-' as seperate, space omitted
	sregex_token_iterator p(str.begin(), str.end(), sep, -1);
	sregex_token_iterator end;
	for (int i = 0; i < 3; i++, p++) {
		if (p == end)
			return false;
		switch (i) {
		case 0:
			yy = stoi(*p);
			break;
		case 1:
			mm = stoi(*p);
			break;
		case 2:
			dd = stoi(*p);
			break;
		default:
			break;
		}
	}
	
	
	
	return true;
}