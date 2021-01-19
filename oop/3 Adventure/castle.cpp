#include<string>
#include<cstdlib>
#include<iostream>
#include<vector>
#include"castle.h"
#include"exit.h"
#include"const.h"
using namespace std;

castle::castle() {
	room * lobby = new room("lobby", exit::exitNameList, true);
	roomList.push_back(lobby);
}

castle::~castle() {
	while (!roomList.empty()) {
		vector<room*>::iterator tempRoom = roomList.end();
		roomList.pop_back();
		delete(*tempRoom);
	}
	cout << "leave the castle!" << endl;
}



room* castle::newRoom(string room_name) {
	room * newRoom = new room(room_name, exit::exitNameList);
	roomList.push_back(newRoom);
	return newRoom;
}

void castle::begin() {
	room * tmpRoom = roomList[0];
	string exitName, roomName, go;
	cout << "Welcome to my castle, enjoy your time!" << endl;
	while (1) {
		tmpRoom->printInfo();
		cin >> go >> exitName;
		if (go != "go") {
			cout << "please type in the standard way like: go up/down/east/west" << endl;
			continue;
		}
		if (!tmpRoom->judgeExit(exitName)) {
			cout << "we don't have an exit named " << exitName << endl;
			continue;
		}

		room * nextRoom = tmpRoom->nextRoom(exitName);
		if (!nextRoom) {
			cout << "please give a name to the new room: ";
			cin >> roomName;
			nextRoom = newRoom(roomName);
			tmpRoom->setExit(exitName, nextRoom);
			nextRoom->setExit(exit::converseExit(exitName), tmpRoom);
		}
		
		if (nextRoom->judgeMonster()) {
			cout << "Dangerous! You meet the monster and game is over!" << endl;
			printFailure();
			return;
		}
		if (nextRoom->judgePrincess()&& getPrincess == false) {
			getPrincess = true;
			cout << "Luckily! You meet the princess, please return to Lobby soon!" << endl;
		}

		if (getPrincess&&nextRoom == roomList[0]) {
			cout << "Congratulations! You win!" << endl;
			printWin();
			return;
		}
		tmpRoom = nextRoom;
	}
}



