#ifndef CONST_HEADER
#define CONST_HEADER


extern int level;			//level越高，出现monster概率越低，出现princess概率也越低
							//The higher the level, the lower the probability of a monster and the lower the probability of a princess.

extern int exitPercent;		//exitPercent越高，每个房间出现较多出口的可能性越高
							//The higher the exitPercent, the higher the probability of more exits per room

void getConst();			//通过CLI输入获取上述两个游戏难度值
							//Get the above two game difficulty values through CLI input

void printTitle();			//GUI
void printFailure();
void printWin();

#endif // !CONST_HEADER
