#include "BeetleMove.h"
#include "pch.h"

CBeetleMove::CBeetleMove() {
	_BeetleCnt = 0;
	_Row = 0;
	_Passed = 0;
	_Runtime = 0;
	_ShowMove = false;
	_Tick = 0;
	_Map = nullptr;
	_TimeStamp = nullptr;

	Walk = nullptr;
	BeetleLoc = nullptr;
}

CBeetleMove::~CBeetleMove() {
	for (LLINT row = 0; row < _Row; row++) delete[] _Map[row];
	delete[] _Map;
	delete[] _TimeStamp;
}

void CBeetleMove::Run() {
	ShowInfo();
	SetMap();
	SetBeetle();
	SetMovement();
	SetShowMovement();

	system("pause");
	system("cls");

	BeetleRun();

	PrintTimeStamp();
}

void CBeetleMove::SetMap() {
	SetMapSize();
	InitMap();
}

void CBeetleMove::SetMapSize() {
	cout << "방의 크기는 10의 제곱입니다. 지수를 입력해주세요. >> ";
	do {
		scanf_s("%lld", &_Row);
		if (_Row <= 0) cout << "옳지 않은 값입니다. 1 이상의 값을 적어주세요 >> ";
	} while (_Row <= 0);
	_Row = ROOM_SIZE(_Row);
	cout << "방 크기가 " << _Row << "*" << _Row << "로 설정되었습니다." << endl;
}

void CBeetleMove::InitMap() {
	_Map = new BYTE*[_Row];
	for (LLINT row = 0; row < _Row; row++) {
		_Map[row] = new BYTE[_Row];
		memset(_Map[row], 0, _Row * sizeof(BYTE));
	}
}

void CBeetleMove::SetBeetle() {
	SetBeetleCnt();
	(_BeetleCnt == 1) ? SetBeetleLocCond_One() : SetBeetleLocCond();
	SetBeetleVector();
}

void CBeetleMove::SetBeetleCnt() {
	cout << "딱정벌레의 수를 입력해주세요(0 이상). >> ";
	do {
		scanf_s("%d", &_BeetleCnt);
		if (_BeetleCnt <= 0) cout << "옳지 않은 값입니다. 1 이상의 값을 적어주세요 >> ";
	} while (_BeetleCnt <= 0);

	printf("%d\n", _BeetleCnt);

	_TimeStamp = new double[LOOP];
	memset(_TimeStamp, 0, sizeof(double) * LOOP);
}

void CBeetleMove::SetBeetleLocCond() {
	string str;

	cout << "딱정벌레의 초기 위치를 서로 겹치게 하겠습니까?(T/F) >> ";
	do {
		cin >> str;
		if (!_Strcmp.CompareStrTF(str)) cout << "옳지 않은 값입니다. T/F 둘 중 하나로 적어주세요 >> ";
	} while (!_Strcmp.CompareStrTF(str));

	BeetleLoc = (_Strcmp.CompareStrF(str) ? &CBeetleMove::BeetleLocUniform : &CBeetleMove::BeetleLocComb);
}

void CBeetleMove::BeetleLocComb(vector<BEETLE>::iterator iter, int row, int col) {
	(*iter)._row = row; (*iter)._col = col;
	if (_Map[(*iter)._row][(*iter)._col] == 0) {
		_Map[(*iter)._row][(*iter)._col] = 1;
		_Passed += 1;
	}
}

void CBeetleMove::SetMovement() {
	string str;
	cout << "술 취한것처럼, 벽에 닿아도 딱정벌레가 움직이는 규칙을 추가하시겠습니까? (추가 : T, 유지 : F) >> ";
	do { 
		cin >> str; 
		if (!_Strcmp.CompareStrTF(str)) cout << "옳지 않은 값입니다. T/F 둘 중 하나로 적어주세요 >> ";
	} while (!_Strcmp.CompareStrTF(str));
	Walk = (_Strcmp.CompareStrT(str) ? &CBeetleMove::BeetleWallWalk : &CBeetleMove::BeetleWalk);
}

void CBeetleMove::BeetleWalk(BEETLE &beetle, int movement) {
	switch (movement) {
	case TO_12:
		if (--beetle._row < 0) { ++beetle._row; }
		break;
	case TO_2:
		if (--beetle._row < 0 || ++beetle._col > MAX_ROOM(_Row)) { ++beetle._row; --beetle._col; }
		break;
	case TO_3:
		if (++beetle._col > MAX_ROOM(_Row)) { --beetle._col; }
		break;
	case TO_5:
		if (++beetle._row > MAX_ROOM(_Row) || ++beetle._col > MAX_ROOM(_Row)) { --beetle._row; --beetle._col; }
		break;
	case TO_6:
		if (++beetle._row > MAX_ROOM(_Row)) { --beetle._row; }
		break;
	case TO_8:
		if (++beetle._row > MAX_ROOM(_Row) || --beetle._col < 0) { --beetle._row;  ++beetle._col; }
		break;
	case TO_9:
		if (--beetle._col < 0) { ++beetle._col; }
		break;
	case TO_11:
		if (--beetle._row < 0 || --beetle._col < 0) { ++beetle._row; ++beetle._col; }
		break;
	}
}

void CBeetleMove::BeetleWallWalk(BEETLE &beetle, int movement) {
	switch (movement) {
	case TO_2: case TO_3: case TO_5:
		if (++beetle._col > MAX_ROOM(_Row)) { --beetle._col; }
		break;
	case TO_8: case TO_9: case TO_11:
		if (--beetle._col < 0) { ++beetle._col; }
		break;
	}

	switch (movement) {
	case TO_11: case TO_12: case TO_2:
		if (++beetle._row > MAX_ROOM(_Row)) { --beetle._row; }
		break;
	case TO_5: case TO_6: case TO_8:
		if (--beetle._row < 0) { ++beetle._row; }
		break;
	}
}

void CBeetleMove::SetShowMovement() {
	string str;
	cout << "딱정벌레의 이동과정을 보겠습니까?(T/F) >> ";
	do {
		cin >> str;
		if (!_Strcmp.CompareStrTF(str)) cout << "옳지 않은 값입니다. T/F 둘 중 하나로 적어주세요 >> ";
	} while (!_Strcmp.CompareStrTF(str));

	_ShowMove = (_Strcmp.CompareStrT(str) ? true : false);
}

void CBeetleMove::BeetleRun() {
	LLINT print_time;
	system_clock::time_point start_time;

	for (int loop = 0; loop < LOOP; loop++) {
		InitBeetlesLoc();

		start_time = system_clock::now();
		print_time = BeetleMove(start_time, loop);
		_TimeStamp[loop] = (double)(duration_cast<microseconds>(system_clock::now() - start_time).count() - print_time) / MICRO_SEC;

		ClearMap();
	}
}

void CBeetleMove::InitBeetlesLoc() {
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(0, _Row - 1);

	_Passed = 0;

	for (vector<BEETLE>::iterator iter = _Beetle.begin(); iter != _Beetle.end(); ++iter)
		(this->*BeetleLoc)(iter, dis(gen), dis(gen));
}

LLINT CBeetleMove::BeetleMove(system_clock::time_point start_time, int loop) {
	random_device rd;
	mt19937 gen(rd());
	bool ret = false;
	LLINT show_time = 0;

	cout << loop + 1 << "회 시작" << endl;

	while ((_Passed != (_Row * _Row)) && (ret != true)) {
		for (BYTE beetle_num = 0; beetle_num < _BeetleCnt; beetle_num++) {
			uniform_int_distribution<> beetleMovement(0, 7);

			(this->*Walk)(_Beetle[beetle_num], beetleMovement(gen));
			IsBeetlePassed(_Beetle[beetle_num]);

			if (_ShowMove)	show_time += ShowMap(start_time, loop);
			else			show_time += CalcTick(start_time, &ret);

			if (_Passed >= _Row * _Row) break;
			if (ret == true) break; // 특정 시간이 되면 timeout 발생시켜 강제로 종료
		}
	}

	return show_time;
}

LLINT CBeetleMove::ShowMap(system_clock::time_point start_time, int loop) {
	system_clock::time_point middle_time, print_time;
	string str, temp("\"10"); // const char을 string변수에 넣을 수 없는 상황이 발생하여 temp string변수 생성
	int str_loc = 0;
	bool isNotLocBeetle = true;

	print_time = system_clock::now();

	system("cls");
	cout << "현재 " << loop + 1 << "번째 진행중, " << "진행률 : " << _Passed << "/" << (_Row * _Row) << endl;
	cout << "걸린 시간  << " << duration_cast<microseconds>(system_clock::now() - start_time).count() / MICRO_SEC << "초 >>" << endl;

	// 2마리 이상일 때 위치가 겹치는 경우, 출력이 이상하게 되는 경우를 방지하기 위한 코드.
	// string 클래스를 이용하여 해결한다.
	for (int row = 0; row < _Row; row++, str_loc = 0) {
		for (int col = 0; col < _Row; col++, isNotLocBeetle = true, str_loc++) {
			for (int beetle_num = 0; beetle_num < _Beetle.size(); beetle_num++) {
				if (_Beetle[beetle_num]._row == row) {
					if (_Beetle[beetle_num]._col == col) {
						// 딱정벌레의 위치가 현재 방의 좌표라면 " 추가.
						str.push_back(temp.at(0));
						isNotLocBeetle = false;
						break;
					}
				}
			}
			if (isNotLocBeetle) {
				if (_Map[row][col] == 1)	str.push_back(temp.at(1)); // 이미 딱정벌레가 지나간 자리라면 1 추가
				else						str.push_back(temp.at(2));
			}
		}
		cout << str << endl;
		str.clear();
	}
	Sleep(SLEEP_TIME);

	return duration_cast<microseconds>(system_clock::now() - print_time).count();
}

LLINT CBeetleMove::CalcTick(system_clock::time_point start_time, bool *flag) {
	LLINT flag_time = 0;
	system_clock::time_point ret = system_clock::now();
	if (++_Tick == MAX_TICK) {
		flag_time = duration_cast<microseconds>(system_clock::now() - start_time).count();
		if ((double)flag_time / MICRO_SEC > MAX_TIME) {
			cout << MAX_TIME << "초가 초과하여 강제로 다음 루프를 진행합니다." << endl;
			*flag = true;
			return 0;
		}
		printf("%llu/%llu 진행중... (%lf초)\n", _Passed, _Row * _Row, (double)flag_time / MICRO_SEC);
		_Tick %= MAX_TICK;
	}
	return duration_cast<microseconds>(system_clock::now() - ret).count();
}

void CBeetleMove::PrintTimeStamp() {
	system("cls");
	for (int idx = 0; idx < LOOP; idx++)
		printf("%d회차 : %lf초\n", idx + 1, _TimeStamp[idx]);
}

//bool CBeetleMove::CompareStr(string str) {
//	return ((CompareStrT(str) ||
//		CompareStrF(str)) == true) ? true : false;
//}
//
//bool CBeetleMove::CompareStrT(string str) {
//	return ((str.compare("T") == 0) ||
//		(str.compare("t") == 0) == true) ? true : false;
//}
//
//bool CBeetleMove::CompareStrF(string str) {
//	return ((str.compare("F") == 0) ||
//		(str.compare("f") == 0) == true) ? true : false;
//}

void CBeetleMove::IsBeetlePassed(BEETLE beetle) {
	if (_Map[beetle._row][beetle._col] == 0) {
		_Map[beetle._row][beetle._col] = 1;
		_Passed += 1;
	}
}