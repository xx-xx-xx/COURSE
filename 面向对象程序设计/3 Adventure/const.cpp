#include"const.h"
#include<iostream>

using namespace std;

int level;
int exitPercent;

void getConst() {
	printTitle();
	cout << "Which level(2-10) do you want to choose?" << endl;
	cout << "a HIGHER level means a LOWER probability to meet MONSTER and PRINCESS" << endl;
	cout << "your level: ";
	cin >> level;

	cout << "What percent(10-100) do you expect 4 exits to appear in each room?" << endl;
	cout << "a HIGHER percent means a HIGHER probability to meet exits in each room" << endl;
	cout << "exit percent: ";
	cin >> exitPercent;
}

void printTitle() {
	cout << "             _                 _                  " << endl;
	cout << "    /\\      | |               | |                 " << endl;
	cout << "   /  \\   __| |_   _____ _ __ | |_ _   _ _ __ ___ " << endl;
	cout << "  / /\\ \\ / _` \\ \\ / / _ \\ '_ \\| __| | | | '__/ _ \\" << endl;
	cout << " / ____ \\ (_| |\\ V /  __/ | | | |_| |_| | | |  __/" << endl;
	cout << "/_/    \\_\\__,_| \\_/ \\___|_| |_|\\__|\\__,_|_|  \\___|" << endl;
	cout << "***********************************************************************************" << endl;
}

void printFailure() {
	cout << "***********************************************************************************" << endl;
	cout << "  _____                         ____                 " << endl;
	cout << " / ____|                       / __ \\                " << endl;
	cout << "| |  __  __ _ _ __ ___   ___  | |  | |_   _____ _ __ " << endl;
	cout << "| | |_ |/ _` | '_ ` _ \\ / _ \\ | |  | \\ \\ / / _ \\ '__|" << endl;
	cout << "| |__| | (_| | | | | | |  __/ | |__| |\\ V /  __/ |   " << endl;
	cout << " \\_____|\\__,_|_| |_| |_|\\___|  \\____/  \\_/ \\___|_|   " << endl;
}

void printWin() {
	cout << "***********************************************************************************" << endl;
	cout << "  _____                            _         _       _   _                 _ " << endl;
	cout << " / ____|                          | |       | |     | | (_)               | |" << endl;
	cout << "| |     ___  _ __   __ _ _ __ __ _| |_ _   _| | __ _| |_ _  ___  _ __  ___| |" << endl;
	cout << "| |    / _ \\| '_ \\ / _` | '__/ _` | __| | | | |/ _` | __| |/ _ \\| '_ \\/ __| |" << endl;
	cout << "| |___| (_) | | | | (_| | | | (_| | |_| |_| | | (_| | |_| | (_) | | | \\__ \\_|" << endl;
	cout << " \\_____\\___/|_| |_|\\__, |_|  \\__,_|\\__|\\__,_|_|\\__,_|\\__|_|\\___/|_| |_|___(_)" << endl;
	cout << "                    __/ |                                                    " << endl;
	cout << "                   |___/                                                     " << endl;
}