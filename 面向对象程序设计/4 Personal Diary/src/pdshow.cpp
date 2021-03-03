#include"Diarybook.h"
#include"Diary.h"
#include"Date.h"
int main() {
	Diarybook myDiary;
	cout << "what date do you want to show? formate[yyyy-mm-dd] like 1999-9-9" << endl;
	string time;
	cin >> time;
	myDiary.showDiary(time);
	myDiary.~Diarybook();
	system("pause");
	return 0;
}