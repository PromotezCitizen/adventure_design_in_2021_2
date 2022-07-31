#pragma once

#include "pch.h"
#include "StrCmp.h"

class CBeetleMove {
public:
	CBeetleMove();
	~CBeetleMove();

	void Run();

protected:
	void (CBeetleMove::*BeetleLoc)(vector<BEETLE>::iterator iter, int row, int col);
	void (CBeetleMove::*Walk)(BEETLE &beetle, int movement);

	void ShowInfo() { cout << LOOP << "회동안 반복되는 술취한 딱정벌레입니다." << endl; }

	void SetMap();
	void SetMapSize();
	void InitMap();

	void SetBeetle();
	void SetBeetleCnt();
	void SetBeetleLocCond();
	void SetBeetleLocCond_One() { BeetleLoc = &CBeetleMove::BeetleLocComb; }
	void SetBeetleVector() { for (char cnt = 0; cnt < _BeetleCnt; cnt++) _Beetle.push_back({ 0,0 }); }
	void BeetleLocUniform(vector<BEETLE>::iterator iter, int row, int col) {
		do { BeetleLocComb(iter, row, col); } while (_Map[(*iter)._row][(*iter)._col] == 0);
	}
	void BeetleLocComb(vector<BEETLE>::iterator iter, int row, int col);

	void SetMovement();
	void BeetleWalk(BEETLE &beetle, int movement);
	void BeetleWallWalk(BEETLE &beetle, int movement);

	void BeetleRun();
	void InitBeetlesLoc();
	LLINT BeetleMove(system_clock::time_point start_time, int loop);
	LLINT ShowMap(system_clock::time_point start_time, int loop);
	LLINT CalcTick(system_clock::time_point start, bool *flag);
	void ClearMap() { for (int row = 0; row < _Row; row++) memset(_Map[row], 0, sizeof(BYTE) * _Row); }

	void SetShowMovement();

	void PrintTimeStamp();

	//bool CompareStr(string str);
	//bool CompareStrT(string str);
	//bool CompareStrF(string str);

	void IsBeetlePassed(BEETLE beetle);

private:
	int _BeetleCnt = 0;
	BYTE _Runtime = 0;
	bool _ShowMove = false;
	LLINT _Row = 0, _Passed = 0;
	int _Tick = 0;
	vector<BEETLE> _Beetle;
	double *_TimeStamp = nullptr;
	BYTE **_Map = nullptr;

	CStrCmp _Strcmp;
};

