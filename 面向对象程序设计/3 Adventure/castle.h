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
	bool getPrincess = false;			//�Ƿ��Ѿ�����princess
										//Whether you have encountered princess
	vector<room*> roomList;				//��¼�Ѿ������ķ���
										//Record rooms that have been encountered

public:
	room* newRoom(string room_name);	//���̽�����·���
										//Add new room explored
	void begin();						//��ʼ��Ϸ������Ǳ�
										//Begin the game, enter the castle

};


#endif