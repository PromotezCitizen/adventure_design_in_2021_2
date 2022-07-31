// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

// 여기에 미리 컴파일하려는 헤더 추가
#include "framework.h"
#include <queue>
#include <map>
#include <string>
#include <cmath>
#include <bitset>
#include <locale.h>

#endif //PCH_H

#ifndef HUFFMAN_CODING_A
#define HUFFMAN_CODING_A

#define MAX_ASCII 256

using namespace std;

typedef struct tree_a {
	struct tree_a* right_node = NULL;
	struct tree_a* left_node = NULL;
	USHORT data = 256;
	UINT freq = 0;
} TREE_A;

typedef struct decode_a {
	struct decode_a* left_node = NULL;
	struct decode_a* right_node = NULL;
	USHORT data = 256;
	string huffman = "";
} DECODE_A;

#endif

#ifndef COMP_A
#define COMP_A

class comp_a {
public:
	bool operator()(const TREE_A& a, const TREE_A& b) {
		return (a.freq > b.freq);
	}
};

#endif

/* encoding */
void appendPq(priority_queue<TREE_A, vector<TREE_A>, comp_a>& pq, map<USHORT, UINT> huffman);
void appendFreqTree(priority_queue<TREE_A, vector<TREE_A>, comp_a>& pq, TREE_A* parent);
void deleteFreqTree(TREE_A* tree);

void makeHuffmanMap(TREE_A*, map<BYTE, string>&);
void makeHuffmanMaprec(TREE_A*, string, map<BYTE, string>&);

string encodeFile2Huffman(FILE*, int&, map<BYTE, string>);

/* decoding */

void decodingTREE(DECODE_A*, map<BYTE, string>);
void appendDECODETREE(DECODE_A*, BYTE, string);

void initTREE(DECODE_A*);

WCHAR UTF_16(BYTE* data, UINT i);
WCHAR UTF_16_BE(BYTE* data, UINT i);
WCHAR UTF_16_LE(BYTE* data, UINT i);
WCHAR UTF_8_3BYTE(BYTE* data, UINT i);
WCHAR UTF_8_2BYTE(BYTE* data, UINT i);
