#pragma once

#ifndef _BEETLE

#define _BEETLE

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <stdlib.h>

#include <random>
#include <ctime>
#include <Windows.h>
#include <cstring>

#include <vector>

#include <chrono>

typedef struct Beetle {
	int _row;
	int _col;
} BEETLE;

#endif

using namespace std;
using namespace chrono;

typedef unsigned char BYTE;
typedef long long int LLINT;

#ifndef _BEETLE_MACRO

#define _BEETLE_MACRO

#define ROOM_SIZE(a) LLINT(pow(10, (a)))
#define MAX_ROOM(a) LLINT((a)-1)
#define LOOP 10							// 몇 번 반복해서 결과를 얻을건지 알아내는 상수

#define TO_12 0
#define TO_2 1
#define TO_3 2
#define TO_5 3
#define TO_6 4
#define TO_8 5
#define TO_9 6
#define TO_11 7

#define SLEEP_TIME 2
#define MAX_TIME 1800.0
#define MAX_TICK 5000000

#define MICRO_SEC 1000000

#endif