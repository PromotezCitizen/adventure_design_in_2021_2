#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <tuple>
#include <random>
#include <thread>
#include <Windows.h>
#include <conio.h>

typedef unsigned char BYTE;
typedef unsigned int UINT;

#ifndef __POSITION
#define __POSITION

#define UP 0
#define LEFT 1
#define DOWN 2
#define RIGHT 3

struct Position {
	int _row = 0;
	int _col = 0;
};

#endif

#ifndef __FIGHT_FLAG
#define __FIGHT_FLAG

#define __VS_PLAYER 49
#define __VS_AI		50
#define __SOLOPLAY	51

#endif

#ifndef __VKEY_SET
#define __VKEY_SET

#define __VKEY 224
#define __VKEY_UP 72
#define __VKEY_DOWN 80
#define __VKEY_LEFT 75
#define __VKEY_RIGHT 77

#endif
