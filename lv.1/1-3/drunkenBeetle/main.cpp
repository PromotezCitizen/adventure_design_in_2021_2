#include "BeetleMove.h"
#include "StrCmp.h"

int main() {
	CBeetleMove *beetle;
	CStrCmp strcmp;
	string end_cond;

	do {
		beetle = new CBeetleMove;
		beetle->Run();
		delete beetle;

		// 방 크기, 마리수 별 구한 데이터를
		// 파일로 저장하는 코드 구현 필요

		//

		cout << "종료하시겠습니까?(e , exit : 종료) >> ";
		cin >> end_cond;
		system("cls");
	} while (!strcmp.CompareStrExit(end_cond));
}