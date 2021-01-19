#include<string>
#include<cstdlib>
#include<iostream>
#include<vector>
#include"exit.h"

using namespace std;

vector<string>exit::exitNameList = {
	"up","down","east","west"
};

string exit::converseExit(string exit) {
	if (exit == "up")return "down";
	if (exit == "down")return "up";
	if (exit == "east")return "west";
	if (exit == "west")return "east";
	return "ERROR when looking for the opposite Exit";
}