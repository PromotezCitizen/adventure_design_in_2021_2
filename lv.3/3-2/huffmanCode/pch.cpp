// pch.cpp: 미리 컴파일된 헤더에 해당하는 소스 파일

#define _CRT_SECURE_NO_WARNINGS

#include "pch.h"
#include "huffmanCodeDlg.h"

// 미리 컴파일된 헤더를 사용하는 경우 컴파일이 성공하려면 이 소스 파일이 필요합니다.


void appendPq(priority_queue<TREE_A, vector<TREE_A>, comp_a>& pq, map<USHORT, UINT> huffman) {
	TREE_A datas;
	for (auto e : huffman) {
		datas.right_node = NULL;
		datas.left_node = NULL;
		datas.data = e.first;
		datas.freq = e.second;
		pq.push(datas);
	}
}

void appendFreqTree(priority_queue<TREE_A, vector<TREE_A>, comp_a>& pq, TREE_A* parent) {

	TREE_A* right = new TREE_A;
	*right = pq.top();
	pq.pop();

	TREE_A* left = new TREE_A;
	*left = pq.top();
	pq.pop();

	parent->left_node = left;
	parent->right_node = right;

	parent->data = 256;
	parent->freq = right->freq + left->freq;

	pq.push(*parent);
}

void deleteFreqTree(TREE_A* tree) {
	if (tree != NULL) {
		deleteFreqTree(tree->left_node);
		deleteFreqTree(tree->right_node);
		delete tree;
	}
}

void makeHuffmanMap(TREE_A* node, map<BYTE, string>& huffman_code) {
	makeHuffmanMaprec(node, "", huffman_code);
}

void makeHuffmanMaprec(TREE_A* node, string ss, map<BYTE, string>& huffman_code) {
	if (node != NULL) {
		makeHuffmanMaprec(node->left_node, ss + "0", huffman_code);
		makeHuffmanMaprec(node->right_node, ss + "1", huffman_code);

		if (node->data < MAX_ASCII) huffman_code[node->data] = ss.c_str();
	}
	return;
}

string encodeFile2Huffman(FILE* file, int& cnt, map<BYTE, string> huffman_code) {
	BYTE index;
	string ss = "";

	while (fscanf(file, "%c", &index) != EOF) {
		ss += huffman_code[index];
		cnt++;
	}

	return ss;
}

/* ===================================================== */

void decodingTREE(DECODE_A* decode, map<BYTE, string> huffman_code) {
	decode->left_node = NULL;
	decode->right_node = NULL;

	for (auto e : huffman_code) {
		appendDECODETREE(decode, e.first, e.second);
	}
}

void appendDECODETREE(DECODE_A* decode, BYTE f, string s) {
	DECODE_A* temp = decode;

	for (UINT cnt = 0; cnt < s.size(); cnt++) {
		if (s[cnt] == '0') {
			if (temp->left_node == NULL) {
				temp->left_node = new DECODE_A;
				temp = temp->left_node;
				initTREE(temp);
			}
			else {
				temp = temp->left_node;
			}
		}
		else {
			if (temp->right_node == NULL) {
				temp->right_node = new DECODE_A;
				temp = temp->right_node;
				initTREE(temp);
			}
			else {
				temp = temp->right_node;
			}
		}
	}
	temp->data = f;
	temp->huffman = s;
}

void initTREE(DECODE_A* tree) {
	tree->left_node = NULL;
	tree->right_node = NULL;
	tree->data = -1;
	tree->huffman = "";
}

WCHAR UTF_16(BYTE* data, UINT i) {
	if (data[0] == 0xFF) {
		return UTF_16_LE(data, i);
	}
	else {
		return UTF_16_BE(data, i);
	}
}

WCHAR UTF_16_BE(BYTE* data, UINT i) {
	WCHAR unicode = 0;
	unicode = data[i];
	return (unicode << 8) + data[i + 1];
}

WCHAR UTF_16_LE(BYTE* data, UINT i) {
	WCHAR unicode = 0;
	unicode = data[i + 1];
	return data[i] + (unicode << 8);
}

WCHAR UTF_8_3BYTE(BYTE* data, UINT i) {
	WCHAR temp;
	WCHAR unicode = 0;

	data[i] = data[i] << 4;
	data[i + 1] = data[i + 1] << 2;
	data[i + 2] = data[i + 2] << 2;

	temp = data[i];
	unicode = temp << 8;
	temp = data[i + 1];
	unicode += temp << 4;
	return unicode + (data[i + 2] >> 2);
}

WCHAR UTF_8_2BYTE(BYTE* data, UINT i) {
	WCHAR temp;
	WCHAR unicode = 0;

	data[i] = data[i] << 5;
	data[i + 1] = data[i + 1] << 6;

	temp = data[i];
	unicode = temp << 1;
	return unicode + (data[i + 1] >> 6);
}