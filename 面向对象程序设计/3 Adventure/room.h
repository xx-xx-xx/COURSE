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
	string room_name;							//�������ƣ���Դ����
												//Room name, source CLI input
	map<string, room*> room_exit;				//���ڷ���Ŀ��г���
												//Viable exit in the room
	bool princess;								//Whether have get the princess
	bool monster;								//Whether have met the monster

public:
	bool judgePrincess();						//�ж��Ƿ���princess
												//Determine if there is princess
	bool judgeMonster();						//�ж��Ƿ���monster
												//Determine if there is monster
	bool judgeExit(string exit);				//�ж�����ĳ����Ƿ����
												//Determine if there is exit
	room * nextRoom(string exit);				//������һ��room
												//Back to the next room
	void setExit(string exit,room* room);		//������ĸ��������ɳ���
												//Generate exits randomly in four directions
	void printInfo();							//��ӡ������Ϣ
												//Print room information

};



#endif // !ROOM_HEADER
