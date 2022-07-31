#include "Huffmancode.h"

using namespace std;

bool CHuffmancode::LoadEncodeFile() {
	FILE *file = GetFile("인코딩할 파일 입력", "rb");
	if (file == NULL) return false;

	InitDatas(_encodetree);

	GetOriginFileSize(file);
	ReadFile(file);
	fclose(file);

	MakeFreqMap();
	MakeFreqQueue();

	MakeFreqTree();
	MakeHuffmancode(_encodetree, "");

	GetEncodeMsg();

	return true;
}

bool CHuffmancode::SaveEncodeFile() {

	FILE *output = GetFile("부호화된 파일 출력", "wb");
	if (output == NULL) return false;
	
	WriteEncodedFile(output);
	fclose(output);

	DeleteTree(_encodetree);
	_encodetree = nullptr;

	return true;
}

bool CHuffmancode::LoadDecodeFIle() {
	FILE *file = GetFile("부호화된 파일 입력", "rb");
	if (file == NULL) return false;
	_huffmancode.clear();

	InitDatas(_decodetree);

	ReadEncodedHeader(file);
	GetEncodeDataSize(file);
	ReadFile(file);
	fclose(file);

	MakeHuffmanTree();

	return true;
}

bool CHuffmancode::SaveDecodeFile() {
	FILE *output = GetFile("복호화된 파일 출력", "wb");
	if (output == NULL) return false;
	UINT charcnt = 0;
	DECODE *temp = _decodetree;

	for (UINT idx = 0; idx < _filesize; idx++) {
		bitset<8>bs(_document[idx]);
		for (char bits = 7; bits > -1; bits--) {
			if (bs[bits] == 1) {
				if (temp->right == nullptr) {
					fwrite(&temp->val, sizeof(char), 1, output);
					temp = _decodetree->right;
					if (++charcnt == _charcnt) break;
					continue;
				}
				temp = temp->right;
			}
			else {
				if (temp->left == nullptr) {
					fwrite(&temp->val, sizeof(char), 1, output);
					temp = _decodetree->left;
					if (++charcnt == _charcnt) break;
					continue;
				}
				temp = temp->left;
			}
		}
		if (charcnt == _charcnt) break;
	}

	fclose(output);

	DeleteTree(_decodetree);
	_decodetree = nullptr;

	return true;
}

void CHuffmancode::MakeHuffmanTree() {
	DECODE *temp = nullptr;
	
	DeleteTree(_decodetree);
	_decodetree = new DECODE;
	InitHuffmanTree(_decodetree);

	EraseEmptyMapVal();
	for (auto e : _huffmancode) {
		temp = _decodetree;
		for (BYTE idx = 0; idx < e.second.length(); idx++) {
			if (e.second[idx] == '1') {
				if (temp->right == nullptr) {
					temp->right = new DECODE;
					InitHuffmanTree(temp->right);
				}
				temp = temp->right;
			}
			else {
				if (temp->left == nullptr) {
					temp->left = new DECODE;
					InitHuffmanTree(temp->left);
				}
				temp = temp->left;
			}
		}
		temp->val = e.first;
	}
}

void CHuffmancode::ReadEncodedHeader(FILE* file) {
	BYTE id = 0, huffmanlen = 0, len = 0;
	USHRT iter = 0, huffmansize = 0;
	UINT data = 0;
	string huffmanstr = "";

	fread(&iter, sizeof(USHRT), 1, file);

	fread(&huffmansize, sizeof(USHRT), 1, file);
	fread(&_charcnt, sizeof(UINT), 1, file);

	for (USHRT idx = 0; idx < huffmansize; idx++) {
		fread(&id, sizeof(BYTE), 1, file);
		fread(&huffmanlen, sizeof(BYTE), 1, file);
		fread(&data, sizeof(BYTE), (huffmanlen - 1) / 8 + 1, file);

		huffmanstr = "";
		bitset<64>bs(data);
		for (BYTE idx = 0; idx < huffmanlen; idx++) (bs[idx] == 1 ? huffmanstr = '1' + huffmanstr : huffmanstr = '0' + huffmanstr);
		_huffmancode[id] = huffmanstr;
	}
}

void CHuffmancode::SearchTree(DECODE* tree) {
	if (tree != nullptr) {
		SearchTree(tree->left);
		SearchTree(tree->right);
		if (tree->val < 256) printf("0x%02X\n", tree->val);
	}
}

template <typename H>
void CHuffmancode::InitDatas(H* tree) {
	if (_document != nullptr) {
		delete[] _document;
		_document = nullptr;
	}
	_freq.clear();
	_huffmancode.clear();
	while (!_pq.empty()) _pq.pop();
	DeleteTree(tree);
	_writestr = "";
}

FILE *CHuffmancode::GetFile(const char* rw, const char* opentype) {
	string filepath;
	printf("(%s)%s", rw, "파일 경로를 입력해주세요 >> ");
	cin >> filepath;
	FILE *file = fopen(filepath.c_str(), opentype);
	return file;
}

void CHuffmancode::MakeFreqQueue() {
	ENCODE temp;
	for (auto iter = _freq.begin(); iter != _freq.end(); iter++) {
		temp.left = nullptr;
		temp.right = nullptr;
		temp.val = iter->first;
		temp.freq = iter->second;
		_pq.push(temp);
	}
}

void CHuffmancode::MakeFreqTree() {
	while (_pq.size() >= 2) _pq = MakeFreqTreeFromQueue(_pq);

	_encodetree = new ENCODE;
	*_encodetree = _pq.top(); _pq.pop();
}

priority_queue <ENCODE, vector<ENCODE>, cmp> CHuffmancode::MakeFreqTreeFromQueue(priority_queue <ENCODE, vector<ENCODE>, cmp> pq) {
	ENCODE *head = new ENCODE, *left = new ENCODE, *right = new ENCODE;

	*right	= pq.top();	pq.pop();
	*left	= pq.top();	pq.pop();

	head->right = right;	head->left = left;
	head->val = 256;		head->freq = _SUM_FREQ(right, left);

	pq.push(*head);

	return pq;
}

void CHuffmancode::MakeHuffmancode(ENCODE *tree, string str) {
	if (tree != nullptr) {
		MakeHuffmancode(tree->left, str + "0");
		MakeHuffmancode(tree->right, str + "1");
		if (tree->val <= _MAX_ASCII) _huffmancode[tree->val] = str.c_str();
	}
}

void CHuffmancode::WriteEncodeMap(FILE *output) {
	USHRT iter = 0xFFFF, huffmansize = _huffmancode.size();
	BYTE huffmanlen = 0, len = 0;
	UINT buffer = 0;

	fwrite(&iter, sizeof(USHRT), 1, output);
	fwrite(&huffmansize, sizeof(USHRT), 1, output);
	fwrite(&_filesize, sizeof(UINT), 1, output);

	for (auto e : _huffmancode) {
		huffmanlen = e.second.length(); buffer = 0;
		for (BYTE idx = 0; idx < huffmanlen; idx++) {
			if (e.second[idx] == '1') buffer |= 1;
			if (idx == huffmanlen - 1) break;
			buffer <<= 1;
		}

		fwrite(&e.first, sizeof(BYTE), 1, output);
		fwrite(&huffmanlen, sizeof(BYTE), 1, output);

		// ----- 허프만 부호화 결과를 가변길이로 저장 -----
		fwrite(&buffer, sizeof(BYTE), (huffmanlen - 1) / 8 + 1, output);
	}
}

void CHuffmancode::WriteEncodeStr(FILE *output) {
	BYTE buffer = 0;
	for (UINT idx = 0; idx < _writestr.length(); idx++) {
		if (_writestr[idx] == '1') buffer |= 1;

		if (idx % 8 == 7) {
			fwrite(&buffer, sizeof(BYTE), 1, output);
			buffer = 0;
			continue;
		}
		buffer = buffer << 1;
	}

	BYTE temp = 7 - (_writestr.length() % 8);
	if (temp != 7) {
		buffer = buffer << temp;
		fwrite(&buffer, sizeof(BYTE), 1, output);
	}
}