#ifndef ROOM_HEADER
#define ROOM_HEADER
#include<map>
#include<string>
#include"exit.h"
using namespace std;

class room {
public:
	room(string room_name, vector<string>exit, bool notExistNPC = false);
	~room();

private:
	string room_name;							//房间名称，来源输入
												//Room name, source CLI input
	map<string, room*> room_exit;				//所在房间的可行出口
												//Viable exit in the room
	bool princess;								//Whether have get the princess
	bool monster;								//Whether have met the monster

public:
	bool judgePrincess();						//判断是否有princess
												//Determine if there is princess
	bool judgeMonster();						//判断是否有monster
												//Determine if there is monster
	bool judgeExit(string exit);				//判断输入的出口是否存在
												//Determine if there is exit
	room * nextRoom(string exit);				//返回下一个room
												//Back to the next room
	void setExit(string exit,room* room);		//随机在四个方向生成出口
												//Generate exits randomly in four directions
	void printInfo();							//打印房间信息
												//Print room information

};



#endif // !ROOM_HEADER
