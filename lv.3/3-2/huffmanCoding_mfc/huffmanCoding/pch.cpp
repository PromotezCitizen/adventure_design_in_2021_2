// pch.cpp: 미리 컴파일된 헤더에 해당하는 소스 파일

#include "pch.h"

// 미리 컴파일된 헤더를 사용하는 경우 컴파일이 성공하려면 이 소스 파일이 필요합니다.

void appendPq(priority_queue<TREE_A, vector<TREE_A>, comp_a>& pq, map<UCHAR, UINT> huffman) {
	TREE_A datas;
	for (auto e : huffman) {
		datas.right_node = NULL;
		datas.left_node = NULL;
		datas.data = e.first;
		datas.freq = e.second;
		pq.push(datas);
	}
}

void appendTree(priority_queue<TREE_A, vector<TREE_A>, comp_a>& pq, TREE_A* parent) {

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

void deleteTree(TREE_A* tree) {
	if (tree != NULL) {
		deleteTree(tree->left_node);
		deleteTree(tree->right_node);
		delete tree;
	}
}

//void printTree(TREE_A* tree, int cnt) {
//	if (tree != NULL) {
//		cnt++;
//		printTree(tree->left_node, cnt);
//		printTree(tree->right_node, cnt);
//		if (tree->data < MAX_ASCII) {
//			bitset<8> bt(tree->data);
//			cout << bt << '\t' << tree->freq << endl;
//			//printf("%hd(%d) - %d\n", tree->data, tree->freq, cnt - 1);
//		}
//	}
//}
//
//void printHUFFMAN(map<UCHAR, string> huffman_code) {
//	int cnt = 0;
//	for (auto e : huffman_code) {
//		if (e.first < MAX_ASCII) printf("%d-%s ", e.first, e.second.c_str());
//		if ((++cnt) % 8 == 0) printf("\n");
//	}
//	printf("\n");
//}

void huffmanEncoding() {
	map<UCHAR, UINT> huffman;
	map<UCHAR, string> huffman_code;
	FILE* file = fopen(FILE_NAME, "rb");
	int cnt = 0;
	char index = 0;

	priority_queue<TREE_A, vector<TREE_A>, comp_a> pq;

	while (fscanf(file, "%c", &index) != EOF) {
		huffman[index]++;
	}

	fseek(file, 0, SEEK_SET);

	appendPq(pq, huffman);

	TREE_A* parent = new TREE_A;
	while (pq.size() >= 2) { appendTree(pq, parent); }

	makeHUFFMAN(parent, huffman_code);
	deleteTree(parent);

	string ss = encodingHUFFMAN_A(file, cnt, huffman_code);

	writeFILE(ss, cnt, huffman_code);

	fclose(file);
}

void makeHUFFMAN(TREE_A* node, map<UCHAR, string>& huffman_code) {
	makeHUFFMANrec(node, "", huffman_code);
}

void makeHUFFMANrec(TREE_A* node, string ss, map<UCHAR, string>& huffman_code) {
	if (node != NULL) {
		makeHUFFMANrec(node->left_node, ss + "0", huffman_code);
		makeHUFFMANrec(node->right_node, ss + "1", huffman_code);

		if (node->data < MAX_ASCII) huffman_code[node->data] = ss.c_str();
	}
	return;
}

string encodingHUFFMAN_A(FILE* file, int& cnt, map<UCHAR, string> huffman_code) {
	BYTE index;
	string ss = "";

	while (fscanf(file, "%c", &index) != EOF) {
		ss += huffman_code[index];
		cnt++;
	}

	return ss;
}

void writeFILE(string ss, int num_char, map<UCHAR, string> huffman_code) {
	writeFILEbin(huffman_code, num_char);
	writeFILEtxt(ss);
	huffman_code.clear();
	system("pause");
}


void writeFILEbin(map<UCHAR, string> huffman_code, int num_char) {
	const char* filename = "asdf_encode.txt";
	FILE* wfile_bin = fopen(filename, "w+b");

	UINT buffer = 0;
	UCHAR len = 0;

	int cnt = huffman_code.size();

	BYTE iter = 255;
	fwrite(&iter, sizeof(BYTE), 1, wfile_bin);
	fwrite(&iter, sizeof(BYTE), 1, wfile_bin);

	fwrite(&cnt, sizeof(int), 1, wfile_bin);
	fwrite(&num_char, sizeof(int), 1, wfile_bin);

	for (auto e : huffman_code) {
		len = e.second.length();

		for (int idx = 0; idx < len; idx++) { // 길이 31까지만 가능
			if (e.second[idx] == '1') buffer |= 1;
			buffer = buffer << 1;
		}
		buffer = buffer >> 1;

		fwrite(&e.first, sizeof(UCHAR), 1, wfile_bin);
		fwrite(&len, sizeof(UCHAR), 1, wfile_bin);
		fwrite(&buffer, sizeof(UINT), 1, wfile_bin);
		buffer = 0;
	}

	fclose(wfile_bin);
}

void writeFILEtxt(string ss) {
	const char* filename = "asdf_encode.txt";

	FILE* wfile = fopen(filename, "a+b");
	BYTE buffer = 0;

	for (int idx = 0; idx < ss.length(); idx++) {
		if (ss[idx] == '1') buffer |= 1;

		if (idx % 8 == 7) {
			fwrite(&buffer, sizeof(BYTE), 1, wfile);
			buffer = 0;
		}
		buffer = buffer << 1;
	}

	BYTE temp = 7 - (ss.length() % 8);
	buffer = buffer << temp;
	if (temp != 7) fwrite(&buffer, sizeof(BYTE), 1, wfile);

	fclose(wfile);
}

/* ===================================================== */

void huffmanDecoding() {
	string ss("");
	map<UCHAR, string> huffman_code;

	const char* filename = "asdf_encode.txt";
	FILE* rfile = fopen(filename, "rb");

	int cnt = decodingHUFFMANfromFILE(rfile, huffman_code);

	if (cnt > 0) {
		int temp = 0;
		DECODE_A* decode = new DECODE_A;
		decodingTREE(decode, huffman_code);
		decodingFILE(rfile, decode, cnt);
	}
	fclose(rfile);
}

int decodingHUFFMANfromFILE(FILE* file, map<UCHAR, string>& huffman_code) {
	string ss = "";
	UCHAR id, len;
	UINT data;
	int cnt_encoded, cnt_numchar;

	BYTE iter;
	fread(&iter, sizeof(BYTE), 1, file);
	fread(&iter, sizeof(BYTE), 1, file);

	fread(&cnt_encoded, sizeof(int), 1, file);
	fread(&cnt_numchar, sizeof(int), 1, file);

	for (int idx = 0; idx < cnt_encoded; idx++) {
		fread(&id, sizeof(UCHAR), 1, file);
		fread(&len, sizeof(UCHAR), 1, file);
		fread(&data, sizeof(int), 1, file);

		bitset<32>bs(data); // bit연산으로 원래 허프만코드 복호화

		for (int i = 0; i < len; i++) {
			if (bs[0]) ss = "1" + ss;
			else ss = "0" + ss;
			bs = bs >> 1;
		}

		huffman_code[id] = ss;
		ss = "";
	}

	return cnt_numchar;
}

void decodingTREE(DECODE_A* decode, map<UCHAR, string> huffman_code) {
	decode->left_node = NULL;
	decode->right_node = NULL;

	for (auto e : huffman_code) {
		appendDECODETREE(decode, e.first, e.second);
	}
}

void appendDECODETREE(DECODE_A* decode, USHORT f, string s) {
	DECODE_A* temp = decode;

	for (int cnt = 0; cnt < s.size(); cnt++) {
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


void decodingFILE(FILE* rfile, DECODE_A* tree, int cnt) {
	FILE* wfile = fopen("asdf_bkup.txt", "w+b");

	BYTE data;
	DECODE_A* temp = tree;

	for (int i = 0; i < cnt; ) {
		fread(&data, sizeof(BYTE), 1, rfile);
		bitset<8> bs(data);
		for (int bit = 7; bit > -1; bit--) {
			if (bs[bit] == 1) {
				if (temp->right_node == NULL) {
					fwrite(&temp->data, sizeof(BYTE), 1, wfile);
					temp = tree->right_node;
					if (++i == cnt) break;
					continue;
				}
				temp = temp->right_node;
			}
			else {
				if (temp->left_node == NULL) {
					fwrite(&temp->data, sizeof(BYTE), 1, wfile);
					temp = tree->left_node;
					if (++i == cnt) break;
					continue;
				}
				temp = temp->left_node;
			}
		}
		if (i == cnt) break;
	}

	fclose(rfile);
	fclose(wfile);
}

void initTREE(DECODE_A* tree) {
	tree->left_node = NULL;
	tree->right_node = NULL;
	tree->data = -1;
	tree->huffman = "";
}