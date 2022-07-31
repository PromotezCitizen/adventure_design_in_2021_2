#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <sstream>

#define SEC_TIME 86400
#define FILE_PATH "C:\\Users\\Han\\Desktop\\"

using namespace std;

struct customerData {
	int order;
	string name;
	int mile;
	string useDate;
	double priority;
};

struct chain {
	customerData data;
	chain* left;
	chain* right;
};

void nowTimeSet(time_t& nowTime);

void getUserOrder(int& order, ifstream& fin);
void setUserOrder(int& order, int arySize, char* data);

void setUserName(string& name, ifstream& fin);

void getUserMile(int& mile, double& maxMile, ifstream& fin);
void getArySize(char* data, int& arySize);
int mileCalculation(char* data, int arySize);

void getUserTime(string& useDate, string& tempDateStr, ifstream& fin);
void userTimeSet(time_t nowTime, time_t& userSigninTime, string tempDateStr, int& tempdata);
void getMaxDate(double& maxDate, int tempdata);

void userPrioritySet(customerData& person, double maxMile, double maxDate, int tempdata);

void proritySort(customerData* person, int dataLine);

void printResult(customerData* person, int dataLine);