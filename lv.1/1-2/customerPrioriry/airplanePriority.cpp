#include "airplanePriority.h"

void nowTimeSet(time_t& nowTime) {
	struct tm* timeinfo;

	time(&nowTime);

	timeinfo = localtime(&nowTime);
	timeinfo->tm_hour = 0;
	timeinfo->tm_min = 0;
	timeinfo->tm_sec = 0;
	timeinfo->tm_isdst = 0;

	nowTime = mktime(timeinfo);
}

void getUserOrder(int& order, ifstream& fin) {
	char data[20];
	int arySize = 0;
	fin.getline(data, 20, '\t');
	for (int j = 0; j < 20; j++) {
		if (data[j] == 0) { break; }
		++arySize;
	}
	setUserOrder(order, arySize, data);
}
void setUserOrder(int& order, int arySize, char* data) {
	order = 0;
	for (int i = 1; i <= arySize; i++) {
		order += (static_cast<char>(data[i - 1]) - static_cast <char>('0')) * pow(10, arySize - i);
	}
}

void setUserName(string& name, ifstream& fin) {
	char data[20];
	fin.getline(data, 20, '\t');
	name = data;
}

void getUserMile(int& mile, double& maxMile, ifstream& fin) {
	char data[20];
	int mileCalc = 0, arySize = 0;

	fin.getline(data, 20, '\t');
	getArySize(data, arySize);

	mileCalc = mileCalculation(data, arySize);

	if (mileCalc > maxMile) { maxMile = mileCalc; }
	mile = mileCalc;
}
void getArySize(char* data, int& arySize) {
	for (arySize; arySize < 20; arySize++) {
		if (data[arySize] == 0) { return; }
	}
}
int mileCalculation(char* data, int arySize) {
	int temp = 0;
	for (int loc = 1; loc <= arySize; loc++) {
		temp += (static_cast<char>(data[loc - 1]) - static_cast <char>('0')) * pow(10, arySize - loc);
	}
	return temp;
}


void getUserTime(string& useDate, string& tempDateStr, ifstream& fin) {
	char data[20];

	fin.getline(data, 20, '\n');
	tempDateStr = data;
	for (int i = 0; i < tempDateStr.size(); i++) {
		if (tempDateStr[i] == '-') { tempDateStr[i] = ' '; }
	}
	useDate = data;
}
void userTimeSet(time_t nowTime, time_t& userSigninTime, string tempDateStr, int& tempdata) {
	struct tm* timeinfo;
	int year, month, day;

	istringstream(tempDateStr) >> year >> month >> day;

	time(&userSigninTime);
	timeinfo = localtime(&userSigninTime);
	timeinfo->tm_year = year - 1900;
	timeinfo->tm_mon = month - 1;
	timeinfo->tm_mday = day;
	timeinfo->tm_hour = 0;
	timeinfo->tm_min = 0;
	timeinfo->tm_sec = 0;
	timeinfo->tm_isdst = 0;

	userSigninTime = mktime(timeinfo);

	tempdata = (static_cast<int>(difftime(nowTime, userSigninTime)) / SEC_TIME);
}
void getMaxDate(double& maxDate, int tempdata) {
	if (tempdata > maxDate) { maxDate = tempdata; }
}

void userPrioritySet(customerData& person, double maxMile, double maxDate, int tempdata) {
	person.priority = person.order - (person.mile / maxMile) * 10 - (tempdata / maxDate) * 5;
}

void proritySort(customerData* person, int dataLine) {
	customerData tempUser;
	for (int i = 0; i < dataLine - 1; i++) {
		for (int j = i; j < dataLine; j++) {
			if (person[i].priority > person[j].priority) {
				tempUser = person[i];
				person[i] = person[j];
				person[j] = tempUser;
			}
		}
	}
}

void printResult(customerData* person, int dataLine) {
	ofstream fout("C:\\Users\\gkstk\\OneDrive\\πŸ≈¡ »≠∏È\\test1.txt");

	cout << "No." << "\t" << "name" << "\t" << "mile" << "\t" << "signin" << "\t" << "priority" << "\r\n";
	for (int i = 0; i < dataLine - 1; i++) {
		//fout << person[i].order << "\t" << person[i].name << "\t" << person[i].mile << "\t" << person[i].useDate << "\n";
		cout << person[i].order << "\t" << person[i].name << "\t" << person[i].mile << "\t" << person[i].useDate << "\t" << person[i].priority << endl;
	}

	//fout << person[dataLine - 1].order << "\t" << person[dataLine - 1].name << "\t" << person[dataLine - 1].mile << "\t" << person[dataLine - 1].useDate;
	cout << person[dataLine - 1].order << "\t" << person[dataLine - 1].name << "\t" << person[dataLine - 1].mile << "\t" << person[dataLine - 1].useDate << "\t" << person[dataLine - 1].priority << endl;

	fout.close();
}