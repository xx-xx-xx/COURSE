#include"Diarybook.h"
#include"Diary.h"
#include"Date.h"
int main() {
	Diarybook myDiary;
	cout << "which date do you want to remove? format[yyy-mm-dd] like 1999-9-9" << endl;
	string time;
	cin >> time;
	bool success;
	success = myDiary.removeDiary(time);
	if (success) {
		cout << "0";
	}
	else {
		cout << "-1";
	}
	myDiary.~Diarybook();
	system("pause");
	return 0;
}