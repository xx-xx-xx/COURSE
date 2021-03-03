#include "Diary.h"



Diary::Diary()
{
}


Diary::~Diary()
{
}

void Diary::readFile(ifstream &infile) {
	int lineNum;
	string tmpDate;
	infile >> lineNum >> tmpDate;
	infile.ignore();
	date.fromStr(tmpDate);
	for (int i = 0; i < lineNum; i++) {
		string line;
		getline(infile, line);
		lineList.push_back(line);
	}
}

void Diary::writeFile(ofstream &outfile) {
	outfile << lineList.size() << endl << date.toStr() << endl;
	for (auto line : lineList)outfile << line << endl;
}

void Diary::readStdin() {
	string text;
	cout << ">>> ";
	while (getline(cin, text) && text != ".") {
		lineList.push_back(text);
		cout << ">>>";
	}
}

void Diary::writeStdin() {
	cout << string(80, '-') << endl;
	cout << string(35, ' ') << date.toStr() << endl;
	for (auto line : lineList)cout << line << endl;
}

const bool Diary::cmpFunc(Diary *p1, Diary *p2) {
	Date d;
	if (d.op_leq(p1->date, p2->date))return true;
}

