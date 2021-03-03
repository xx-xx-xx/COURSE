#ifndef DIARYBOOK_H
#define DIARYBOOK_H
#include<iostream>
#include<vector>
#include<stdlib.h>
#include<string>
#include"Date.h"
#include <fstream>
#include <regex>
#include <sstream>
#include<iomanip>
#include"Diary.h"
using namespace std;
class Diarybook :private Diary {
public:
	Diarybook();
	~Diarybook();
	//addDiary		add a diary into the cache
	void addDiary();
	//listAllDiary	list all the diaries stored in the cache
	void listAllDiary();
	//listSoloDiary	list specific diaries between the ordered time
	void listSoloDiary(string, string);
	//showDiary		show the diary in the specific date
	void showDiary(string);
	//removeDiary	remove the diary in the specific date
	bool removeDiary(string);

private:
	//findDiary		find the diary in the ordered date
	Diary * findDiary(Date &);
	//path			store the path of the cache
	const string path = "./assets/cache.txt";
	//store all diaries in the cache
	vector<Diary*>diaryList;
};
#endif // !DIARYBOOK_H
