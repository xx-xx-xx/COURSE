#ifndef CONST_HEADER
#define CONST_HEADER


extern int level;			//levelԽ�ߣ�����monster����Խ�ͣ�����princess����ҲԽ��
							//The higher the level, the lower the probability of a monster and the lower the probability of a princess.

extern int exitPercent;		//exitPercentԽ�ߣ�ÿ��������ֽ϶���ڵĿ�����Խ��
							//The higher the exitPercent, the higher the probability of more exits per room

void getConst();			//ͨ��CLI�����ȡ����������Ϸ�Ѷ�ֵ
							//Get the above two game difficulty values through CLI input

void printTitle();			//GUI
void printFailure();
void printWin();

#endif // !CONST_HEADER
