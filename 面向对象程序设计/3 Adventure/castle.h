#ifndef CASTLE_HEADER
#define CASTLE_HEADER
#include<string>
#include<vector>
#include"room.h"
#include"exit.h"
using namespace std;

class castle {
public:
	castle();
	~castle();

private:
	bool getPrincess = false;			//是否已经遇到princess
										//Whether you have encountered princess
	vector<room*> roomList;				//记录已经遇到的房间
										//Record rooms that have been encountered

public:
	room* newRoom(string room_name);	//添加探索的新房间
										//Add new room explored
	void begin();						//开始游戏，进入城堡
										//Begin the game, enter the castle

};


#endif