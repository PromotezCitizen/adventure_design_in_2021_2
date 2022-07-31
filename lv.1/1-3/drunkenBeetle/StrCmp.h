#pragma once
#include <string>

using namespace std;

class CStrCmp {
public:
	bool CompareStrTF(string str) { return ((CompareStrT(str) || CompareStrF(str)) == true) ? true : false; }
	bool CompareStrT(string str) { return (((str.compare("T") == 0) || (str.compare("t") == 0)) == true) ? true : false; }
	bool CompareStrF(string str) { return (((str.compare("F") == 0) || (str.compare("f") == 0)) == true) ? true : false; }

	bool CompareStrExit(string str) { return ((CompareStrE(str) || CompareStrExit_full(str)) == true) ? true : false; }
	bool CompareStrE(string str) { return ((str.compare("E") == 0) || (str.compare("e") == 0) == true) ? true : false; }
	bool CompareStrExit_full(string str) { return (((str.compare("EXIT") == 0) || (str.compare("exit") == 0) || (str.compare("Exit") == 0)) == true) ? true : false; }
};
