#ifndef EXIT_HEADER
#define EXIT_HEADER
#include<string>
#include<vector>
using namespace std;

class exit {
public:
	vector<string> static exitNameList;			//四个可能出口
												//Four possible exits
	string static converseExit(string exit);	//对面出口
												//Opposite exit
};



#endif // !EXIT_HEADER
