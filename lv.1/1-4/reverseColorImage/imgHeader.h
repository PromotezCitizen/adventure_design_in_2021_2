#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <tuple>
#include <iostream>
#include <Windows.h>

using namespace std;

#ifndef _PGM_PPM
#define _PGM_PPM

typedef unsigned int UINT;
typedef unsigned char UCHAR;
typedef unsigned char BYTE;

#ifndef _MENU
#define _MENU

#define _EXIT_FLAG			0
#define _DRAW_SQUARE		1
#define _TURN_RIGHT			2
#define _TURN_LEFT			3
#define _IMG_COLOR_REVERSE	4
#define _CONVERT_EXTENSION	5
#define _UNDO				6
#define _REWIND				7
#define _SAVE_IMG			8
#define _SAVE_2_BMP			9

#endif

#ifndef _IMG_EXPRESSION
#define _IMG_EXPRESSION

#define INT_TO_CHAR(a) ((a)+0x30)
#define CHAR_TO_INT(a) ((a)-0x30)
#define SPACE 0x20
#define ENTER 0x0A
#define FILE_PATH "C:\\Users\\Han\\Desktop\\"

#define REVERSE_IMG_COLOR(row, col) (((row) * (_Undo->_ImgRow) + (col)) * (_Undo->_ColorBytes))
#define _GET_COLOR_AVG(arr) (((*((arr) + 0)) * 0.11 + (*((arr) + 1)) * 0.59 + (*((arr) + 2))) * 0.3)

#endif

#ifndef _IMG_COLOR_DAT
#define _IMG_COLOR_DAT

#define	COLOR_R		1
#define	COLOR_B		2
#define	COLOR_G		3
#define	COLOR_RG	4
#define	COLOR_RB	5
#define	COLOR_GB	6
#define	COLOR_RGB	7

#define _PPM_COLOR_BYTES 3
#define _PGM_COLOR_BYTES 1
#define _BIT (USHORT)8

#define _RGB 3

#endif

#ifndef _INTEGRATE_IMG
#define _INTEGRATE_IMG
typedef struct integrateImage {
	char _BOM[2] = { 0,0 }; // BOM[1] - 0x36 : ppm, 0x35 : pgm
	UCHAR _MaxBright = 0;
	UINT _ImgRow = 0, _ImgCol = 0;
	BYTE *_Color = nullptr; // rgb : 용량 3인 배열, wb : 용량 1인 배열
} IMGDAT;
#endif

#ifndef _UNDO_IMG
#define _UNDO_IMG
typedef struct undoImage : IMGDAT {
	BYTE _ColorBytes = 0;
	struct undoImage *_Before = nullptr;
	struct undoImage *_Next = nullptr;
} UNDO;
#endif

#ifndef _CONVERT_PNM_2_BMP
#define _CONVERT_PNM_2_BMP
typedef struct convert2BMP{
	BITMAPFILEHEADER bmpheader = { 0, };
	BITMAPINFOHEADER bmpinfo = { 0, };
	BYTE *bmp_color = nullptr;
} CONV2BMP;
#endif

#endif

//#ifndef __BEFORE_CODE
//
//#define __BEFORE_CODE
//
//#ifndef INTEGRATE_HEADER
//
//#define INTEGRATE_HEADER
//
//typedef struct integrateHeader {
//	char BOM[2] = { 0,0 }; // BOM[1] - 0x36 : ppm, 0x35 : pgm
//	UCHAR max_bright = 0;
//	UINT row = 0, col = 0;
//	BYTE*** rgb_bw = NULL; // rgb : 용량 3인 배열, wb : 용량 1인 배열
//} IMGHEADER;
//
//#endif
//
//#ifndef UNDO_CHANGED
//
//#define UNDO_CHANGED
//
//typedef struct imgUndoChain {
//	char BOM = 0;
//	UINT row = 0, col = 0, chain_idx = 0;
//	BYTE*** rgb_bw = NULL;
//	struct imgUndoChain* before = NULL;
//	struct imgUndoChain* next = NULL;
//} UNDO;
//
//#endif
//
//#endif