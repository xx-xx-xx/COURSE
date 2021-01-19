#include"Diarybook.h"

Diarybook::Diarybook() {
	ifstream infile;
	infile.open(path);
	if (!infile.is_open())return;
	int num;
	infile >> num;
	diaryList.resize(num);
	for (auto ite = diaryList.begin(); ite < diaryList.end(); ite++) {
		*ite = new Diary;
		(*ite)->readFile(infile);
	}
}

Diarybook::~Diarybook() {
	sort(diaryList.begin(), diaryList.end(), Diary::cmpFunc);
	ofstream outfile;
	outfile.open(path);
	outfile << diaryList.size() << endl;
	for (auto pDiary : diaryList) {
		pDiary->writeFile(outfile);
	}
}

Diary* Diarybook::findDiary(Date& date) {
	for (auto p : diaryList) {
		Date d;
		if (d.op_equal(date,p->date)) {
			delete p;
			p = new Diary;
			return p;
		}
	}
	return nullptr;
}

void Diarybook::addDiary() {
	Date d;
	Diary* tmpDiary = findDiary(d);
	if (tmpDiary == nullptr) {
		diaryList.push_back(new Diary);
		tmpDiary = diaryList.back();
	}
	tmpDiary->readStdin();
	cout << endl << "Successfully added." << endl;
}

void Diarybook::listAllDiary() {
	for (auto tmpDiary : diaryList)tmpDiary->writeStdin();
}

void Diarybook::listSoloDiary(string begin, string end) {
	Date beginD, endD;
	if (!beginD.fromStr(begin) || !endD.fromStr(end)) {
		cout << "format error!" << endl;
		return;
	}
	for (auto pDiary : diaryList) {
		if (beginD.op_leq(beginD, pDiary->date) && beginD.op_leq(pDiary->date, endD)) {
			pDiary->writeStdin();
		}
	}
}

void Diarybook::showDiary(string rawdate) {
	Date d;
	if (!d.fromStr(rawdate)) {
		cout << "Format error" << endl;
		return;
	}
	for (auto pDiary : diaryList) {
		if (d.op_equal(pDiary->date, d)) {
			pDiary->writeStdin();
			return;
		}
	}
	cout << "Cannot find the diary. Please check the date again." << endl;
}

bool Diarybook::removeDiary(string rawdate) {
	Date d;
	if (!d.fromStr(rawdate)) {
		cout << "Format error" << endl;
		return false;
	}
	for (auto pDiary : diaryList) {
		if (d.op_equal(d, pDiary->date)) {
			pDiary->date.fromStr("99999-99-99");
			sort(diaryList.begin(), diaryList.end(), cmpFunc);
			delete diaryList.back();
			diaryList.pop_back();
			cout << "Successfully removed." << endl;
			return true;
		}
	}
	cout << "Cannot find the diary. Please check the date again." << endl;
	return false;
}