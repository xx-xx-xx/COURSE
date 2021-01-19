#include"Diarybook.h"
#include"Diary.h"
#include"Date.h"

int main() {
	Diarybook myDiary;
	cout << "list all diaries or limit the date?[0/1]" << endl;
	int op;
	cin >> op;
	if (op == 0) {
		myDiary.listAllDiary();
		myDiary.~Diarybook();
	}
	else if (op == 1) {
		cout << "Begin Date:[in format yyyy-mm-dd] like 1999-9-9" << endl;
		string begindate;
		cin >> begindate;
		cout << "End Date:[in format yyyy-mm-dd] like 1999-9-9" << endl;
		string enddate;
		cin >> enddate;
		myDiary.listSoloDiary(begindate, enddate);
		myDiary.~Diarybook();
	}
	else {
		cout << "error format!";
	}
	system("pause");
	return 0;
}