/*********************************************************************************
  *FileName:	MyAdventure
  *Author:		LU
  *Date:		2020/3/28
**********************************************************************************/

/*
1����һ������lobby��֤û�й��������
2����������޲�������ͬһ������
3��ÿ�ν����·���ʱ�������һ�����䣬һ�����ʳ��ֹ�������ޣ����ʴ�С����ȡ������Ϸ�Ѷ�level
4��������һ������ʱ������������ĳ���
5����ʱ�ĳ��ڵĶ�����ڱ�Ȼ�Ƿ���ĳ���
6������������Ϸ����
7�����������轫��������lobby
*/

/*
1.The first room lobby guarantees no princesses or monsters
2.The princess and the monster do not appear in the same room
3. Each time you enter a new room, a room is randomly generated. 
   A princess or monster will appear with a certain probability. The probability can depend on the difficulty level
4. When a new room is generated, randomly generate its exit
5. The exit opposite to the exit when coming must be the exit of the room
6. Meet monsters game over
7. Meet the princess and bring the princess back to the lobby
*/



#include"castle.h"
#include"const.h"
#include<iostream>
using namespace std;

int main() {
	getConst();	
	castle gameCastle;
	gameCastle.begin();
	system("pause");
	return 0;
}