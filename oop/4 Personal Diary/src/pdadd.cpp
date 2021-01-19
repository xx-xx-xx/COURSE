#include"Diarybook.h"
#include"Diary.h"
#include"Date.h"

int main() {
	cout << " pdadd reads lines of the diary from the stdin, line by line, until a line with a single '.' character "<<endl<<"or the EOF character (Ctrl-D in Unix and Ctrl-Z in Windows"<<endl;
	Diarybook myDiary;
	myDiary.addDiary();
	myDiary.~Diarybook();
	system("pause");
	return 0;
}