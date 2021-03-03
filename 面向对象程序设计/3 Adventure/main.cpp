/*********************************************************************************
  *FileName:	MyAdventure
  *Author:		LU
  *Date:		2020/3/28
**********************************************************************************/

/*
1、第一个房间lobby保证没有公主或怪兽
2、公主与怪兽不出现在同一个房间
3、每次进入新房间时随机生成一个房间，一定概率出现公主或怪兽，概率大小可以取决于游戏难度level
4、新生成一个房间时，随机生成它的出口
5、来时的出口的对面出口必然是房间的出口
6、遇见怪兽游戏结束
7、遇见公主需将公主带回lobby
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