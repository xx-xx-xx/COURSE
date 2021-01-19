#ifndef DATE_H
#define DATE_H
#include<iostream>
#include<vector>
#include<stdlib.h>
using namespace std;
struct Date {
	int yy, mm, dd;
	
	//init()	get the date of today
	void init();
	Date() {
		init();
	}
	//op_euqaul		judge if two date is same
	bool op_equal(const Date&, const Date&);
	//op_leq		judge if date1 is earlier than date2
	bool op_leq(const Date&, const Date&);
	//tostr			transfer the date to standare format like yyyy-mm-dd
	string toStr();
	//fromstr		get the date information from the string like yyyy-mm-dd
	bool fromStr(string);
};
#endif // !DATE_H
