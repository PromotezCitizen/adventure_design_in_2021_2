#include "airplanePriority.h"

int main() {
	ifstream fin;
	char temp[100];
	int dataLine = 0;

	customerData* person;

	double maxDate = 0, maxMile = 0;
	int arySize = 0;
	string tempDateStr;
	int tempdata = 0;

	time_t nowTime, userSigninTime;

	//fin.open("C:\\Users\\gkstk\\OneDrive\\바탕 화면\\test.txt");
	fin.open("C:\\Users\\gkstk\\oneDrive\\바탕 화면\\레벨1_data\\레벨1.3 대기자명단.txt");

	if (!fin.is_open()) {
		cout << "파일을 찾을 수 없음";
	}
	else {
		while (!fin.eof()) {
			fin.getline(temp, 100);
			dataLine++;
		}
		fin.seekg(0, ios::beg);

		customerData* person = new customerData[dataLine];

		nowTimeSet(nowTime);

		for (int i = 0; i < dataLine; i++) {
			/*------------------- 순번 --------------------*/
			getUserOrder(person[i].order, fin);
			arySize = 0;
			/*---------------------------------------------*/

			/*------------------- 이름 --------------------*/
			setUserName(person[i].name, fin);
			/*---------------------------------------------*/

			/*--------------- 마일리지 합계 ----------------*/
			getUserMile(person[i].mile, maxMile, fin);
			arySize = 0;
			/*---------------------------------------------*/

			/*------------------ 가입일 -------------------*/
			getUserTime(person[i].useDate, tempDateStr, fin);
			userTimeSet(nowTime, userSigninTime, tempDateStr, tempdata);
			getMaxDate(maxDate, tempdata);
			/*---------------------------------------------*/
		}
		fin.close();

		for (int i = 0; i < dataLine; i++) {
			userPrioritySet(person[i], maxMile, maxDate, tempdata);
		}
		cout << "max Mile = " << maxMile << ", max Signin : " << maxDate << endl;
		proritySort(person, dataLine);

		printResult(person, dataLine);
	}

	fin.close();
}