#pragma once
#define _CRT_SECURE_NO_WARNINGS

#ifndef __INCLUDES
#define __INCLUDES
#include <string.h>
#include <stdlib.h>
#include <queue>
#include <map>
#include <bitset>
#include <iostream>
#endif

#ifndef __DATATYPE
#define __DATATYPE
typedef unsigned char BYTE;		// unsigned 1byte
typedef unsigned char UCHAR;	// unsigned 1byte
typedef unsigned short USHRT;	// unsigned 2byte
typedef unsigned int UINT;		// unsigned 4byte?
#endif

#ifndef __ENCODE
#define __ENCODE
typedef struct Encode {
	struct Encode *left = nullptr;
	struct Encode *right = nullptr;
	UINT freq = 0;
	USHRT val = 256;
} ENCODE;
#endif

#ifndef __DECODE
#define __DECODE
typedef struct Decode {
	struct Decode *left = nullptr;
	struct Decode *right = nullptr;
	USHRT val = 256;
} DECODE;
#endif

#ifndef __COMPARE
#define __COMPARE
struct cmp {
	bool operator()(Encode a, Encode b) { return a.freq > b.freq; }
};
#endif

#ifndef __MATH
#define __MATH
#define _MAX_ASCII (0xFF)
#define _SUM_FREQ(a, b) (((a)->freq) + ((b)->freq)) 
#endif

class CHuffmancode {
public:
	CHuffmancode() { 
		InitDatas(_encodetree);
		InitDatas(_decodetree);
	}
	~CHuffmancode() {
		DeleteTree(_encodetree);
		DeleteTree(_decodetree);
	}

	bool LoadEncodeFile();
	bool SaveEncodeFile();
	bool LoadDecodeFIle();
	bool SaveDecodeFile();

	map<BYTE, std::string> GetHuffmanMap() { return _huffmancode; }
	map<BYTE, UINT> GetCharFreqMap() { return _freq; }

	void PrintStrMap() { for (auto e : _huffmancode) printf("0x%02X - %s\n", e.first, e.second.c_str()); }

protected:
	template <typename H>
	void InitDatas(H *tree);

	void EraseEmptyMapVal() { for (UINT idx = 0; idx < 256; idx++) if (_huffmancode[idx] == "") _huffmancode.erase(idx); }

	template <typename H>
	void DeleteTree(H *head) {
		if (head != nullptr) {
			DeleteTree(head->left);
			DeleteTree(head->right);
			delete head;
		}
	}

	FILE *GetFile(const char *rw, const char* opentype);

	void GetOriginFileSize(FILE *file) {
		fseek(file, 0, SEEK_END);
		_filesize = ftell(file);
		fseek(file, 0, SEEK_SET);
	}

	void GetEncodeDataSize(FILE *file) {
		UINT origin = ftell(file);
		fseek(file, 0, SEEK_END);
		_filesize = ftell(file) - origin;
		fseek(file, origin, SEEK_SET);
	}

	void ReadFile(FILE *file) {
		if (_document != nullptr) {
			delete[] _document;
			_document = nullptr;
		}
		_document = new BYTE[_filesize];
		fread(_document, sizeof(char), _filesize, file);
	}

	void MakeFreqMap() {
		for (UINT idx = 0; idx < _filesize; idx++)
			_freq[_document[idx]]++;
	}

	void MakeFreqQueue();

	void MakeFreqTree();

	std::priority_queue <ENCODE, std::vector<ENCODE>, cmp> MakeFreqTreeFromQueue(std::priority_queue <ENCODE, std::vector<ENCODE>, cmp> pq);

	void MakeHuffmancode(ENCODE *tree, std::string str);

	void GetEncodeMsg() { 
		for (UINT idx = 0; idx < _filesize; idx++) _writestr += _huffmancode[_document[idx]];
	}

	void WriteEncodedFile(FILE *output) {
		WriteEncodeMap(output);
		WriteEncodeStr(output);
	}
	void WriteEncodeMap(FILE *output);
	void WriteEncodeStr(FILE* output);

	void ReadEncodedHeader(FILE *file);

	void MakeHuffmanTree();
	void InitHuffmanTree(DECODE *tree) {
		tree->left = nullptr; tree->right = nullptr;
		tree->val = 256;
	}

	void SearchTree(DECODE* tree);

private:
	UINT _filesize = 0, _charcnt = 0;
	BYTE *_document = nullptr;
	std::map<BYTE, UINT> _freq;
	std::map<BYTE, std::string> _huffmancode;
	std::priority_queue <ENCODE, std::vector<ENCODE>, cmp> _pq;
	std::string _writestr = "";
	ENCODE *_encodetree = nullptr;
	DECODE *_decodetree = nullptr;
};

