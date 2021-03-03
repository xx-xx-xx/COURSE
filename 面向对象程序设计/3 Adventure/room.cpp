#include<string>
#include<iostream>
#include<vector>
#include<cstdlib>
#include<map>
#include<time.h>
#include"room.h"
#include"const.h"

using namespace std;


room::room(string room_name, vector<string>exit, bool notExitNPC) :
	room_name(room_name) {
	if (notExitNPC) {
		//lobby为无NPC房间
		princess = false;
		monster = false;
	}
	else {
		srand(time(NULL));
		monster = (0 == rand() % level);		//新房间随机出现monster概率为1/level
		if (monster) {
			princess = false;					//存在monster则不存在princess
		}
		else {
			princess = (0 == rand() % level);	//新房间随机出现princess概率为1/level
		}
	}

	vector<string>::iterator tempExit;
	for (tempExit = exit.begin(); tempExit != exit.end(); tempExit++) {
		if (rand() % 100 < exitPercent ) {
			room_exit[*tempExit] = NULL;		//为出口概率由exitPercent决定
		}
	}
}

//room析构
room::~room() {
	cout << "leave Room:" << room_name << endl;
}

bool room::judgeMonster() {
	return monster;
}

bool room::judgePrincess() {
	return princess;
}

bool room::judgeExit(string exit) {
	if (room_exit.count(exit))return true;
	else return false;
}

room* room::nextRoom(string exit) {
	return room_exit[exit];
}

void room::printInfo() {
	if (room_name == "lobby") {
		cout << "Welcome to " << room_name << "!" << endl;
	}
	else {
		cout << "Welcome to room " << room_name << "!" << endl;
	}
	if (room_exit.size() > 1) {
		cout << "There are " << room_exit.size() << " exits as: ";
	}
	else {
		cout << "There is " << room_exit.size() << " exit as: ";
	}
	
	//print 出口信息
	map<string, room*>::iterator tempExit;
	for (tempExit = room_exit.begin(); tempExit != room_exit.end(); tempExit++) {
		cout << (*tempExit).first << " ";
	}
	cout << endl;
	cout << "Please enter your command: ";
}

void room::setExit(string exit, room* room) {
	room_exit[exit] = room;						//map形式储存对应方向下下一个房间
}