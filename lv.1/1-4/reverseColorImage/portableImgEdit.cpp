//#include "portableImgEdit.h"
//
//CPortableImgEdit::CPortableImgEdit() {
//	imgdata = new IMGHEADER;
//	imgundo = new UNDO;
//	color_idx = 0;
//	color = NULL;
//	r1 = c1 = r2 = c2 = 0;
//}
//
//CPortableImgEdit::~CPortableImgEdit() {
//	delete[] color;
//	memDeleteImgData(imgdata->rgb_bw);
//	delDoubleLinkedList();
//	delete imgdata;
//}
//
//bool CPortableImgEdit::readImgHeader(FILE* input_file) {
//	// 이미지 헤더부(BOM, 가로 크기, 세로 크기, 최대 밝기 얻어오는 과정
//	fread(imgdata->BOM, sizeof(char), 2, input_file);
//	if (imgdata->BOM[0] != 0x50) return false;
//	else if ((imgdata->BOM[1] != 0x36) && (imgdata->BOM[1] != 0x35)) return false;
//
//	fseek(input_file, 1, SEEK_CUR);
//
//	setImgHeaderData(&imgdata->col, input_file, SPACE);
//	setImgHeaderData(&imgdata->row, input_file, ENTER);
//	setImgHeaderData(&imgdata->max_bright, input_file, ENTER);
//
//	color_idx = (imgdata->BOM[1] == 0x36) ? 3 : 1;
//	color = new BYTE[color_idx];
//
//	return true;
//}
//
//template<typename DEGIT>
//void CPortableImgEdit::setImgHeaderData(DEGIT* imgdata, FILE* input_file, UCHAR spacer) {	
//	// 읽어온 데이터가 0x353132일 경우 10진수 512로 변환하는 과정
//	UCHAR c;
//	while (true) { // row
//		fread(&c, sizeof(char), 1, input_file);
//		if (c == spacer) break;
//		*imgdata *= 10;
//		*imgdata += CHAR_TO_INT(c);
//	}
//}
//
//void CPortableImgEdit::setImgData(FILE* input_file) {
//	// 연결리스트 초기화 및 각 구조체의 rgb_bw 배열 동적할당
//	imgundo->BOM = imgdata->BOM[1];
//	imgundo->row = imgdata->row;
//	imgundo->col = imgdata->col;
//
//	imgundo->chain_idx = 0;
//
//	imgundo->before = NULL;
//	imgundo->next = NULL;
//
//	imgdata->rgb_bw = new UCHAR**[imgdata->row];
//	imgundo->rgb_bw = (UCHAR***)calloc(imgdata->row, sizeof(UCHAR**));
//	for (UINT row = 0; row < imgdata->row; row++) {
//		imgdata->rgb_bw[row] = new UCHAR * [imgdata->col];
//		imgundo->rgb_bw[row] = (UCHAR**)calloc(imgdata->col, sizeof(UCHAR*));
//		for (UINT col = 0; col < imgdata->col; col++) {
//			imgdata->rgb_bw[row][col] = new UCHAR[color_idx];
//			imgundo->rgb_bw[row][col] = (UCHAR*)calloc(color_idx, sizeof(UCHAR));
//			for (BYTE idx = 0; idx < color_idx; idx++) {
//				fread(&imgdata->rgb_bw[row][col][idx], sizeof(UCHAR), 1, input_file);
//				imgundo->rgb_bw[row][col][idx] = imgdata->rgb_bw[row][col][idx];
//			}
//		}
//	}
//}
//
//void CPortableImgEdit::setImgData(BYTE*** temp_color) {
//	//연결리스트 초기화 및 각 구조체의 rgb_bw 배열 동적할당
//	UINT temp = imgdata->row;
//	imgdata->row = imgdata->col;
//	imgdata->col = temp;
//
//	imgundo->BOM = imgdata->BOM[1];
//	imgundo->row = imgdata->row;
//	imgundo->col = imgdata->col;
//
//	imgundo->chain_idx = 0;
//
//	imgundo->before = NULL;
//	imgundo->next = NULL;
//
//	imgdata->rgb_bw = new UCHAR**[imgdata->row];
//	imgundo->rgb_bw = (UCHAR***)calloc(imgdata->row, sizeof(UCHAR**));
//	for (UINT row = 0; row < imgdata->row; row++) {
//		imgdata->rgb_bw[row] = new UCHAR * [imgdata->col];
//		imgundo->rgb_bw[row] = (UCHAR**)calloc(imgdata->col, sizeof(UCHAR*));
//		for (UINT col = 0; col < imgdata->col; col++) {
//			imgdata->rgb_bw[row][col] = new UCHAR[color_idx];
//			imgundo->rgb_bw[row][col] = (UCHAR*)calloc(color_idx, sizeof(UCHAR));
//		}
//	}
//}
//
//void CPortableImgEdit::printImgHeader() {
//	printf("BOM - %s\n", getBOM());
//	printf("col - %u, row - %u\nmax bright - %hhu\n", \
//		getImgCol(), getImgRow(), getImgMaxbright());
//}
//
//void CPortableImgEdit::printImgData() {
//	printf("%s\n", "datas(\'r g b\')\n");
//
//	for (UINT row = 0; row < imgdata->row; row++) {
//		for (UINT col = 0; col < imgdata->col; col++) {
//			printf("\'");
//			for (BYTE idx = 0; idx < color_idx; idx++) {
//				printf("%3d ", imgdata->rgb_bw[row][col][idx]);
//			}
//			printf("\'");
//		}
//		printf("\n");
//	}
//}
//
//void CPortableImgEdit::reverseImg() {
//	// 이미지 반전 및 undo chain에 rgb값 저장
//	setUNDOChain();
//
//	for (UINT row = 0; row < imgdata->row; row++) {
//		for (UINT col = 0; col < imgdata->col; col++) {
//			for (BYTE rgb = 0; rgb < color_idx; rgb++) {
//				imgdata->rgb_bw[row][col][rgb] = imgdata->max_bright - imgdata->rgb_bw[row][col][rgb];
//				imgundo->rgb_bw[row][col][rgb] = imgdata->rgb_bw[row][col][rgb];
//			}
//		}
//	}
//}
//
//void CPortableImgEdit::drawSquareOnImg() {
//	// ==================  그릴 사각형의 row, col의 범위 결정   ==================== //
//	printf("사각형이 그려질 위치를 입력해주세요.(r1,c1,r2,c2)\n");
//	printf("사각형의 위치 범위는 row : 0~%d, col : 0~%d 입니다.\n>> ", imgdata->row - 1, imgdata->col - 1);
//	ignore = scanf("%u %u %u %u", &r1, &c1, &r2, &c2);
//
//	(r1 < r2) ? void() : swapRowCol(&r1, &r2);
//	(c1 < c2) ? void() : swapRowCol(&c1, &c2);
//
//	r1 = (r1 > 0) ? r1 : 0; r2 = (r2 < imgdata->row) ? r2 : imgdata->row;
//	c1 = (c1 > 0) ? c1 : 0; c2 = (c2 < imgdata->row) ? c2 : imgdata->row;
//	// ============================================================================ //
//
//	// =====================   그릴 사각형의 내부 색 결정   ======================= //
//	printf("사각형 내부색의 값을 입력해주세요.");
//
//	if (imgdata->BOM[1] == 0x35) {
//		printf("(grayscale) >> ");
//		ignore = scanf("%hhu", &color[0]);
//	}
//	else {
//		printf("(r, g, b) >> ");
//		ignore = scanf("%hhu %hhu %hhu", &color[0], &color[1], &color[2]);
//	}
//	// =========================================================================== //
//
//	for (UINT row = r1; row < r2 + 1; row++) { // 원본 이미지 값에 사각형 그림
//		for (UINT col = c1; col < c2 + 1; col++) {
//			for (BYTE idx = 0; idx < color_idx; idx++) {
//				imgdata->rgb_bw[row][col][idx] = color[idx];
//			}
//		}
//	}
//
//	setUNDOChain();
//	for (UINT row = 0; row < imgdata->row; row++) { // undo chain에 rgb값 저장
//		for (UINT col = 0; col < imgdata->col; col++) {
//			for (BYTE idx = 0; idx < color_idx; idx++) {
//				imgundo->rgb_bw[row][col][idx] = imgdata->rgb_bw[row][col][idx];
//			}
//		}
//	}
//}
//
//void CPortableImgEdit::saveExtensionConvert() {
//	UCHAR file_type = (imgdata->BOM[1] == 0x35) ? 0x36 : 0x35;
//	string filepath = FILE_PATH;
//	filepath.append("output_extension_convert");
//	filepath.append(file_type == 0x35 ? ".pgm" : ".ppm");
//
//	char BOM[2] = { 'P', file_type };
//	FILE* write_file = fopen(filepath.c_str(), "wb");
//
//	saveBOMColRowBright(BOM, write_file);
//	// bw save
//	saveExtensionConvertData(file_type, write_file);
//
//	fclose(write_file);
//}
//
//void CPortableImgEdit::saveExtensionConvertData(UCHAR file_type, FILE* write_file) {
//	for (UINT row = 0; row < imgdata->row; row++) {
//		for (UINT col = 0; col < imgdata->col; col++) {
//			if (file_type == 0x35) {	// PPM to PGM
//				UCHAR avgRGB;
//				UCHAR* rgb_bw = imgdata->rgb_bw[row][col];
//				avgRGB = (rgb_bw[0] + rgb_bw[1] + rgb_bw[2]) / 3;
//				fwrite(&avgRGB, sizeof(UCHAR), 1, write_file);
//			}
//			else {						// PGM to PPM
//				UCHAR r, g, b;
//				r = g = b = imgdata->rgb_bw[row][col][0];
//				fwrite(&r, sizeof(UCHAR), 1, write_file);
//				fwrite(&g, sizeof(UCHAR), 1, write_file);
//				fwrite(&b, sizeof(UCHAR), 1, write_file);
//			}
//		}
//	}
//}
//
//void CPortableImgEdit::saveEditedImg() {
//	string filepath = FILE_PATH;
//	filepath.append("output_color_reverse");
//	filepath.append(imgdata->BOM[1] == 0x35 ? ".pgm" : ".ppm");
//
//	FILE* write_file = fopen(filepath.c_str(), "wb");
//
//	saveBOMColRowBright(imgdata->BOM, write_file);
//	// rgb save
//	saveEditedImgData(write_file);
//
//	fclose(write_file);
//}
//
//template<typename DEGIT>
//void CPortableImgEdit::saveEditedImgSubHeader(DEGIT imgdata, FILE* wfile) {
//	UINT cnt, dat, saveDat;
//	cnt = log10(imgdata); dat = imgdata;
//	while (cnt != -1) {
//		saveDat = INT_TO_CHAR(dat / pow10(cnt));
//		fwrite(&saveDat, sizeof(UCHAR), 1, wfile);
//		dat %= pow10(cnt--);
//	}
//}
//
//void CPortableImgEdit::saveEditedImgData(FILE* wfile) {
//	for (UINT row = 0; row < imgdata->row; row++) {
//		for (UINT col = 0; col < imgdata->col; col++) {
//			for (BYTE idx = 0; idx < color_idx; idx++) {
//				fwrite(&imgdata->rgb_bw[row][col][idx], sizeof(UCHAR), 1, wfile);
//			}
//		}
//	}
//}
//
//void CPortableImgEdit::saveBOMColRowBright(char BOM[2], FILE* write_file) {
//	BYTE enter = ENTER, space = SPACE;
//
//	// BOM
//	fwrite(BOM, sizeof(char), 2, write_file);
//	fwrite(&enter, sizeof(UCHAR), 1, write_file);
//	// col
//	saveEditedImgSubHeader(imgdata->col, write_file);
//	fwrite(&space, sizeof(UCHAR), 1, write_file);
//	// row
//	saveEditedImgSubHeader(imgdata->row, write_file);
//	fwrite(&enter, sizeof(UCHAR), 1, write_file);
//	// max bright
//	saveEditedImgSubHeader(imgdata->max_bright, write_file);
//	fwrite(&enter, sizeof(UCHAR), 1, write_file);
//}
//
//void CPortableImgEdit::delDoubleLinkedList() {
//	UNDO* delNode = imgundo, * temp;
//	while (delNode->before != NULL) delNode = delNode->before;
//
//	while (delNode != NULL) {
//		memDeleteImgData(delNode->rgb_bw);
//		temp = delNode->next;
//		delete delNode;
//		delNode = temp;
//	}
//	delete delNode;
//}
//
//void CPortableImgEdit::memDeleteImgData(BYTE*** rgb_bw) {
//	for (UINT row = 0; row < imgdata->row; row++) {
//		for (UINT col = 0; col < imgdata->col; col++) {
//			delete rgb_bw[row][col];
//		}
//		delete rgb_bw[row];
//	}
//	delete rgb_bw;
//}
//
//void CPortableImgEdit::memDeleteImgData(BYTE ***rgb_bw, UINT idx[2]) {
//	for (UINT row = 0; row < idx[0]; row++) {
//		for (UINT col = 0; col < idx[1]; col++) {
//			delete rgb_bw[row][col];
//		}
//		delete rgb_bw[row];
//	}
//	delete rgb_bw;
//}
//
//void CPortableImgEdit::setUNDOChain() {
//	if (imgundo->next == NULL) {
//		imgundo->next = new UNDO;
//		UNDO* next = imgundo->next;
//		next->BOM = imgundo->BOM;
//		next->row = imgundo->row;
//		next->col = imgundo->col;
//
//		next->before = imgundo;
//		next->next = NULL;
//
//		next->chain_idx = imgundo->chain_idx + 1;
//
//		next->rgb_bw = (UCHAR***)calloc(imgundo->row, sizeof(UCHAR**));
//		for (UINT row = 0; row < next->row; row++) {
//			next->rgb_bw[row] = (UCHAR**)calloc(next->col, sizeof(UCHAR*));
//			for (UINT col = 0; col < next->col; col++) {
//				next->rgb_bw[row][col] = (UCHAR*)calloc(color_idx, sizeof(UCHAR));
//			}
//		}
//		imgundo = imgundo->next;
//	}
//}
//
//void CPortableImgEdit::undoImgData() {
//	if (imgundo->before != NULL) {
//		imgundo = imgundo->before;
//
//		if (imgundo->next->row != imgundo->row) {
//			memDeleteImgData(imgdata->rgb_bw);
//
//		}
//		else{
//			for (UINT row = 0; row < imgdata->row; row++) {
//				for (UINT col = 0; col < imgdata->col; col++) {
//					for (BYTE idx = 0; idx < color_idx; idx++) {
//						imgdata->rgb_bw[row][col][idx] = imgundo->rgb_bw[row][col][idx];
//					}
//				}
//			}
//		}
//	}
//}
//
//void CPortableImgEdit::rewindImgData() {
//	if (imgundo->next != NULL) {
//		imgundo = imgundo->next;
//
//		for (UINT row = 0; row < imgdata->row; row++) {
//			for (UINT col = 0; col < imgdata->col; col++) {
//				for (BYTE idx = 0; idx < color_idx; idx++) {
//					imgdata->rgb_bw[row][col][idx] = imgundo->rgb_bw[row][col][idx];
//				}
//			}
//		}
//	}
//}
//
//
//#define ZERO 0
//
//// right memcpy(imgdata->rgb_bw[col][imgdata->col - row - 1], color_temp[row][col], sizeof(BYTE) *pnm);;
//// left  memcpy(imgdata->rgb_bw[imgdata->row - col - 1][row], color_temp[row][col], sizeof(BYTE) * pnm);
//void CPortableImgEdit::turnRight() {
//#ifndef TURN_RIGHT
//#define TURN_RIGHT 0
//#endif
//
//	BYTE ***color_temp = nullptr;
//	BYTE pnm = imgdata->BOM[1] == 0x35 ? 1 : 3;
//	UINT temp_row, temp_col;
//	color_temp = new BYTE * *[imgdata->row];
//	for (temp_row = 0; temp_row < imgdata->row; temp_row++) {
//		color_temp[temp_row] = new BYTE * [imgdata->col];
//		for (temp_col = 0; temp_col < imgdata->col; temp_col++) {
//			color_temp[temp_row][temp_col] = new BYTE[pnm];
//			memcpy(color_temp[temp_row][temp_col], imgdata->rgb_bw[temp_row][temp_col], sizeof(BYTE) * pnm);
//		}
//	}
//
//
//
//	memDeleteImgData(imgdata->rgb_bw);
//
//	setImgData(color_temp);
//
//	for (UINT col = 0; col < temp_col; col++) {
//		for (UINT row = 0; row < temp_row; row++) memcpy(imgdata->rgb_bw[col][imgdata->col - row - 1], color_temp[row][col], sizeof(BYTE) * pnm);
//	}
//
//	UINT t[2] = { temp_row, temp_col };
//	memDeleteImgData(color_temp, t);
//
//#undef TURN_RIGHT
//}
//
//void CPortableImgEdit::turnLeft() {
//#ifndef TURN_LEFT
//#define TURN_LEFT 1
//#endif
//
//	BYTE ***color_temp = nullptr;
//	BYTE pnm = imgdata->BOM[1] == 0x35 ? 1 : 3;
//	UINT temp_row, temp_col;
//	color_temp = new BYTE * *[imgdata->row];
//	for (temp_row = 0; temp_row < imgdata->row; temp_row++) {
//		color_temp[temp_row] = new BYTE * [imgdata->col];
//		for (temp_col = 0; temp_col < imgdata->col; temp_col++) {
//			color_temp[temp_row][temp_col] = new BYTE[pnm];
//			memcpy(color_temp[temp_row][temp_col], imgdata->rgb_bw[temp_row][temp_col], sizeof(BYTE) * pnm);
//		}
//	}
//
//	memDeleteImgData(imgdata->rgb_bw);
//
//	setImgData(color_temp);
//
//	for (UINT col = 0; col < temp_col; col++) {
//		for (UINT row = 0; row < temp_row; row++) memcpy(imgdata->rgb_bw[imgdata->row - col - 1][row], color_temp[row][col], sizeof(BYTE) * pnm);
//	}
//
//	UINT t[2] = { temp_row, temp_col };
//	memDeleteImgData(color_temp, t);
//
//#undef TURN_LEFT
//}
//
//// col <-> imgdata->row - col - 1
//// imgdata->col - row - 1 <-> row
//
//// row col imgdata dst, src
//
//// right - memcpy(imgdata->rgb_bw[col][imgdata->col - row - 1]
//// left  - memcpy(imgdata->rgb_bw[imgdata->row - col - 1][row]
//
//UINT CPortableImgEdit::pow10(UINT _index) {
//	UINT return_val = 1;
//	for (UINT i = 0; i < _index; i++) return_val *= 10;
//	return return_val;
//}
//
//void CPortableImgEdit::swapRowCol(UINT* a1, UINT* a2) {
//	UINT temp = *a1;
//	*a1 = *a2;
//	*a2 = temp;
//}