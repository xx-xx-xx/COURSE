#ifndef EXIT_HEADER
#define EXIT_HEADER
#include<string>
#include<vector>
using namespace std;

class exit {
public:
	vector<string> static exitNameList;			//�ĸ����ܳ���
												//Four possible exits
	string static converseExit(string exit);	//�������
												//Opposite exit
};



#endif // !EXIT_HEADER
