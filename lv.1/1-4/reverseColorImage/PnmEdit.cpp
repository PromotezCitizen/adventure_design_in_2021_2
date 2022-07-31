#include "PnmEdit.h"

CPnmEdit::CPnmEdit() {
	_Undo = new UNDO;
	_Undohead = _Undo;
}

UINT CPnmEdit::Pow10(UINT _index) {
	UINT return_val = 1;
	for (UINT i = 0; i < _index; i++) return_val *= 10;
	return return_val;
}

bool CPnmEdit::ReadImg(FILE *input_file) {
	if (input_file == nullptr) {
		printf("�ش� ���� ����\n");
		return false;
	}

	if (!ReadImgHeader(input_file)) return false;

	ReadImgColorData(input_file);

	return true;
}

bool CPnmEdit::ReadImgHeader(FILE *input_file) {
	// �̹��� �����(BOM, ���� ũ��, ���� ũ��, �ִ� ��� ������ ����
	fread(_Undo->_BOM, sizeof(char), 2, input_file);
	if (_Undo->_BOM[0] != 0x50) return false;
	else if ((_Undo->_BOM[1] != 0x36) && (_Undo->_BOM[1] != 0x35)) return false;

	fseek(input_file, 1, SEEK_CUR);

	ReadImgHeaderData(&_Undo->_ImgCol, input_file, SPACE);
	ReadImgHeaderData(&_Undo->_ImgRow, input_file, ENTER);
	ReadImgHeaderData(&_Undo->_MaxBright, input_file, ENTER);

	_Undo->_ColorBytes = (_Undo->_BOM[1] == 0x36) ? 3 : 1;

	return true;
}

template<typename DEGIT>
void CPnmEdit::ReadImgHeaderData(DEGIT *imgdata, FILE *input_file, UCHAR spacer) {
	// �о�� �����Ͱ� 0x353132�� ��� 10���� 512�� ��ȯ�ϴ� ����
	UCHAR c;
	while (true) { 
		fread(&c, sizeof(char), 1, input_file);
		if (c == spacer) break;
		*imgdata *= 10;
		*imgdata += CHAR_TO_INT(c);
	}
}

void CPnmEdit::ReadImgColorData(FILE *input_file) {
	_Undo->_Color = new BYTE[_Undo->_ImgRow * _Undo->_ImgCol * _Undo->_ColorBytes];
	fread(_Undo->_Color, sizeof(BYTE), _Undo->_ImgRow * _Undo->_ImgCol * _Undo->_ColorBytes, input_file);
}

void CPnmEdit::ReverseImgColor() {
	SetUndoChain();

	BYTE chanel = (_Undo->_ColorBytes == 1 ? 0 : ReverseImgChanel());

	for (UINT row = 0; row < _Undo->_ImgRow; row++) {
		for (UINT col = 0; col < _Undo->_ImgCol; col++) {
			switch (chanel) {
			case COLOR_R:
				_Undo->_Color[REVERSE_IMG_COLOR(row,col) + 0] = _Undo->_MaxBright - _Undo->_Color[REVERSE_IMG_COLOR(row, col) + 0];
				break;
			case COLOR_G:
				_Undo->_Color[REVERSE_IMG_COLOR(row, col) + 1] = _Undo->_MaxBright - _Undo->_Color[REVERSE_IMG_COLOR(row, col) + 1];
				break;		  																	 
			case COLOR_B:	  																	 
				_Undo->_Color[REVERSE_IMG_COLOR(row, col) + 2] = _Undo->_MaxBright - _Undo->_Color[REVERSE_IMG_COLOR(row, col) + 2];
				break;		  																	 
			case COLOR_RG:																		 
				_Undo->_Color[REVERSE_IMG_COLOR(row, col) + 0] = _Undo->_MaxBright - _Undo->_Color[REVERSE_IMG_COLOR(row, col) + 0];
				_Undo->_Color[REVERSE_IMG_COLOR(row, col) + 1] = _Undo->_MaxBright - _Undo->_Color[REVERSE_IMG_COLOR(row, col) + 1];
				break;		  																	 
			case COLOR_RB:	  																	 
				_Undo->_Color[REVERSE_IMG_COLOR(row, col) + 0] = _Undo->_MaxBright - _Undo->_Color[REVERSE_IMG_COLOR(row, col) + 0];
				_Undo->_Color[REVERSE_IMG_COLOR(row, col) + 2] = _Undo->_MaxBright - _Undo->_Color[REVERSE_IMG_COLOR(row, col) + 2];
				break;		  																	 
			case COLOR_GB:	  																	 
				_Undo->_Color[REVERSE_IMG_COLOR(row, col) + 1] = _Undo->_MaxBright - _Undo->_Color[REVERSE_IMG_COLOR(row, col) + 1];
				_Undo->_Color[REVERSE_IMG_COLOR(row, col) + 2] = _Undo->_MaxBright - _Undo->_Color[REVERSE_IMG_COLOR(row, col) + 2];
				break;		  																	 
			case COLOR_RGB:	  																	 
				_Undo->_Color[REVERSE_IMG_COLOR(row, col) + 0] = _Undo->_MaxBright - _Undo->_Color[REVERSE_IMG_COLOR(row, col) + 0];
				_Undo->_Color[REVERSE_IMG_COLOR(row, col) + 1] = _Undo->_MaxBright - _Undo->_Color[REVERSE_IMG_COLOR(row, col) + 1];
				_Undo->_Color[REVERSE_IMG_COLOR(row, col) + 2] = _Undo->_MaxBright - _Undo->_Color[REVERSE_IMG_COLOR(row, col) + 2];
				break;		  
			case 0:		  
				_Undo->_Color[REVERSE_IMG_COLOR(row, col)] = _Undo->_MaxBright - _Undo->_Color[REVERSE_IMG_COLOR(row, col)];
				break;
			}
		}
	}
}

BYTE CPnmEdit::ReverseImgChanel() {
	BYTE chanel = 0;

	cout << "�� ������ ä���� �������ּ���.(R : Red, G : Green, B : Blue)" << endl;
	cout << "1. R\t2. G\t3. B" << endl;
	cout << "4. RG\t5. RB\t6.GB" << endl;
	cout << "7. RGB" << endl;
	do {
		cout << ">> ";
		scanf_s("%hhu", &chanel);
	} while (chanel < 1 || 7 < chanel);

	return chanel;
}

void CPnmEdit::DrawSquare() {
	SetUndoChain();

	// ==================  �׸� �簢���� row, col�� ���� ����   ==================== //
	UINT r1, r2, c1, c2;
	BYTE *color = new BYTE[_Undo->_ColorBytes];

	SetSquarePos(&r1, &c1, &r2, &c2);
	// ============================================================================ //

	// =====================   �׸� �簢���� ���� �� ����   ======================= //
	SetSquareColor(((_Undo->_ColorBytes == 3) ? "(r, g, b) >> " : "(grayscale) >> "), color);
	// ============================================================================ //


	for (UINT row = r1; row < r2 + 1; row++) { // ���� �̹��� ���� �簢�� �׸�
		for (UINT col = c1; col < c2 + 1; col++) memcpy(&_Undo->_Color[(row * _Undo->_ImgCol + col) * _Undo->_ColorBytes], color, sizeof(BYTE) * _Undo->_ColorBytes);
		
	}
}

void CPnmEdit::SetSquarePos(UINT *r1, UINT *c1, UINT *r2, UINT *c2) {
	printf("�簢���� �׷��� ��ġ�� �Է����ּ���.(r1,c1,r2,c2)\n");
	printf("�簢���� ��ġ ������ row : 0~%d, col : 0~%d �Դϴ�.\n>> ", _Undo->_ImgRow - 1, _Undo->_ImgCol - 1);
	ignore = scanf("%u %u %u %u", r1, c1, r2, c2);

	(*r1 < *r2) ? void() : SwapRowCol(r1, r2);
	(*c1 < *c2) ? void() : SwapRowCol(c1, c2);

	*r1 = (*r1 > 0) ? *r1 : 0; *r2 = (*r2 < _Undo->_ImgRow) ? *r2 : _Undo->_ImgRow;
	*c1 = (*c1 > 0) ? *c1 : 0; *c2 = (*c2 < _Undo->_ImgCol) ? *c2 : _Undo->_ImgCol;
}

void CPnmEdit::SetSquareColor(const char* msg, BYTE* color) {
	printf("�簢�� ���λ��� ���� �Է����ּ���.");
	printf("%s", msg);
	for (int idx = 0; idx < _Undo->_ColorBytes; idx++) ignore = scanf_s("%hhu", &color[idx]);
}

void CPnmEdit::TurnClockwise90() {
	UINT temp_row = 0, temp_col = 0;
	BYTE *temp_color = NewImgColorArr(&temp_row, &temp_col);

	SetUndoChain();

	SwapRowCol(&_Undo->_ImgRow, &_Undo->_ImgCol);

	for (UINT col = 0; col < temp_col; col++) {
		for (UINT row = 0; row < temp_row; row++)
			memcpy(&_Undo->_Color[((/*row*/col * _Undo->_ImgCol + /*col*/(_Undo->_ImgCol - 1 - row)) * _Undo->_ColorBytes)], &temp_color[(row * temp_col + col) * _Undo->_ColorBytes], sizeof(BYTE) * _Undo->_ColorBytes);
	}

	DeleteImgColorArr(temp_color);
}

void CPnmEdit::TurnCounterClockwise90() {
	UINT temp_row = 0, temp_col = 0;
	BYTE *temp_color = NewImgColorArr(&temp_row, &temp_col);

	SetUndoChain();

	SwapRowCol(&_Undo->_ImgRow, &_Undo->_ImgCol);

	for (UINT col = 0; col < temp_col; col++) {
		for (UINT row = 0; row < temp_row; row++)
			memcpy(&_Undo->_Color[(/*row*/(_Undo->_ImgRow - 1 - col) * _Undo->_ImgCol + /*col*/row) * _Undo->_ColorBytes], &temp_color[(row * temp_col + col) * _Undo->_ColorBytes], sizeof(BYTE) * _Undo->_ColorBytes);
	}

	DeleteImgColorArr(temp_color);
}

BYTE *CPnmEdit::NewImgColorArr(UINT* temp_row, UINT* temp_col) {
	BYTE* color_arr = new BYTE[_Undo->_ImgRow * _Undo->_ImgCol * _Undo->_ColorBytes];
	(*temp_row) = _Undo->_ImgRow; (*temp_col) = _Undo->_ImgCol;
	memcpy(color_arr, _Undo->_Color, sizeof(BYTE) * _Undo->_ImgRow * _Undo->_ImgCol * _Undo->_ColorBytes);

	return color_arr;
}

void CPnmEdit::SaveImg() {
	string filepath = FILE_PATH;
	filepath.append("output_img");
	filepath.append(_Undo->_BOM[1] == 0x35 ? ".pgm" : ".ppm");

	FILE *write_file = fopen(filepath.c_str(), "wb");

	SaveBOMColRowBright(write_file);
	// rgb save
	SaveImgColorData(write_file);

	fclose(write_file);
}

void CPnmEdit::SaveBOMColRowBright(FILE *write_file) {
	char enter = ENTER, space = SPACE;
	// BOM
	fwrite(_Undo->_BOM, sizeof(char), 2, write_file);
	fwrite(&enter, sizeof(char), 1, write_file);
	// col
	SaveImgSubHeader(_Undo->_ImgCol, write_file);
	fwrite(&space, sizeof(char), 1, write_file);
	// row
	SaveImgSubHeader(_Undo->_ImgRow, write_file);
	fwrite(&enter, sizeof(char), 1, write_file);
	// max bright
	SaveImgSubHeader(_Undo->_MaxBright, write_file);
	fwrite(&enter, sizeof(char), 1, write_file);
}

template<typename DEGIT>
void CPnmEdit::SaveImgSubHeader(DEGIT imgdata, FILE *wfile) {
	UINT cnt, dat;
	char saveDat;
	cnt = log10(imgdata); dat = imgdata;
	while (cnt != -1) {
		saveDat = INT_TO_CHAR(dat / Pow10(cnt));
		fwrite(&saveDat, sizeof(char), 1, wfile);
		dat %= Pow10(cnt--);
	}
}

void CPnmEdit::ConvertExtension() {
	BYTE *temp_arr = new BYTE[_Undo->_ImgRow * _Undo->_ImgCol * _Undo->_ColorBytes];

	memcpy(temp_arr, _Undo->_Color, sizeof(BYTE) * _Undo->_ImgRow * _Undo->_ImgCol * _Undo->_ColorBytes);

	SetUndoChain();

	free(_Undo->_Color);

	_Undo->_ColorBytes = (_Undo->_ColorBytes == 1 ? 3 : 1);
	_Undo->_BOM[1] = (_Undo->_BOM[1] == 0x35 ? 0x36 : 0x35);
	_Undo->_Color = new BYTE[_Undo->_ImgRow * _Undo->_ImgCol * _Undo->_ColorBytes];

	SetCopyData(temp_arr);

	delete[] temp_arr;
}

void CPnmEdit::SetCopyData(BYTE* temp_arr) {
	BYTE* pixel = new BYTE[_Undo->_ColorBytes];

	for (UINT row = 0; row < _Undo->_ImgRow; row++) {
		for (UINT col = 0; col < _Undo->_ImgCol; col++) {
			fill_n(pixel, _Undo->_ColorBytes, /*origin img is pgm?*/ _Undo->_ColorBytes == 3 ? /*pgm to ppm*/ temp_arr[(row * _Undo->_ImgCol + col) * _PGM_COLOR_BYTES] : /*ppm to pgm*/ _GET_COLOR_AVG(temp_arr + (row * _Undo->_ImgCol + col) * _PPM_COLOR_BYTES));
			memcpy(_Undo->_Color + (row * _Undo->_ImgCol + col) * _Undo->_ColorBytes, pixel, sizeof(BYTE) * _Undo->_ColorBytes);
		}
	}

	delete[] pixel;
}

void CPnmEdit::SetUndoChain() {
	if (_Undo->_Next != nullptr) {
		DeleteUndoChain(_Undo->_Next);
		_Undo->_Next = nullptr;
	}
	SetUndoChainData();
}

void CPnmEdit::DeleteUndoChain(UNDO *t) {
	UNDO *del_node = t;
	while (del_node != nullptr) {
		t = t->_Next;

		delete[] del_node->_Color;
		delete del_node;

		del_node = t;
	}
}

void CPnmEdit::SetUndoChainData() {
	UNDO *origin;
	_Undo->_Next = new UNDO;
	_Undo->_Next->_Before = _Undo;
	_Undo->_Next->_Next = nullptr;
	origin = _Undo;
	_Undo = _Undo->_Next;
	memcpy(_Undo->_BOM, origin->_BOM, sizeof(char) * 2);
	_Undo->_MaxBright = origin->_MaxBright;
	_Undo->_ColorBytes = origin->_ColorBytes;
	_Undo->_ImgRow = origin->_ImgRow;
	_Undo->_ImgCol = origin->_ImgCol;
	_Undo->_Color = new BYTE[_Undo->_ImgRow * _Undo->_ImgCol * _Undo->_ColorBytes];
	memcpy(_Undo->_Color, origin->_Color, sizeof(BYTE) * _Undo->_ImgRow * _Undo->_ImgCol * _Undo->_ColorBytes);
}

#define MAKETWOCC(ch0, ch1) ((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8))

void CPnmEdit::Convert2BMP() {
	FILE *output = fopen("C:\\Users\\Han\\Desktop\\output.bmp", "wb");

	if (output == nullptr) {
		cout << "file open error" << endl;
		return;
	}

	DWORD dummy_col = (_Undo->_ImgCol * _Undo->_ColorBytes % 4 != 0 ? 4 - (_Undo->_ImgCol % 4) : 0);
	DWORD dummy_dat = dummy_col * _Undo->_ImgRow;
	
	BYTE dummy_arr[3] = { 0,0,0 };

	UINT padding = 0;

	CONV2BMP convert = { SetBmpHeader(dummy_dat) , SetBmpInfo() , SetBmpColor(dummy_dat) };

	for (UINT row = 0; row < _Undo->_ImgRow; row++) {
		for (UINT col = 0; col < _Undo->_ImgCol; col++) {
			if (_Undo->_ColorBytes == 1) {
				fill_n(convert.bmp_color + padding, 3, _Undo->_Color[((_Undo->_ImgRow - row) * _Undo->_ImgCol + col) * _Undo->_ColorBytes]);
				padding += 3;
			}
			else {
				for (BYTE color = 0; color < _Undo->_ColorBytes; color++)
					convert.bmp_color[padding++] = _Undo->_Color[((_Undo->_ImgRow - row) * _Undo->_ImgCol + col + 1) * _Undo->_ColorBytes - 1 - color];
			}
		}
		if (dummy_col != 0) {
			fill_n(convert.bmp_color + padding, dummy_col, 0);
			padding += dummy_col;
		}
	}

	fwrite(&convert.bmpheader,	sizeof(BITMAPFILEHEADER), 1, output);
	fwrite(&convert.bmpinfo,	sizeof(BITMAPINFOHEADER), 1, output);
	fwrite(convert.bmp_color,	sizeof(BYTE), _Undo->_ImgRow * _Undo->_ImgCol * _RGB + dummy_dat, output);

	fclose(output);

	delete[] convert.bmp_color;
}

BITMAPFILEHEADER CPnmEdit::SetBmpHeader(DWORD dummy_dat) {
	DWORD headerSize = sizeof(BITMAPFILEHEADER), infoSize = sizeof(BITMAPINFOHEADER);
	BITMAPFILEHEADER bmpHeader = {
		MAKETWOCC(0x42,0x4D),																/*bfType		- BMP ���� �ѹ�*/
		static_cast <DWORD> (_msize(_Undo->_Color)) + headerSize + infoSize + dummy_dat,	/*bfSize		- ���� ��ü ũ��*/
		0,																					/*bfReserved1	- not use*/
		0,																					/*bfReserved2	- not use*/
		headerSize + infoSize																/*bfOffBits		- �̹��� ������ ���� ��ġ*/
	};
	return bmpHeader;
}

BITMAPINFOHEADER CPnmEdit::SetBmpInfo() {
	BITMAPINFOHEADER bmpInfo = {
		40,												/*biSize			- BITMAPINFOHEADER ũ��*/
		static_cast<long>(_Undo->_ImgCol),				/*biWidth			- ��Ʈ�� ���� ũ��*/
		static_cast<long>(_Undo->_ImgRow),				/*biHeight			- ��Ʈ�� ���� ũ��*/
		1,												/*biPlanes			- ������ ��*/
		24,												/*biBitCount		- �ȼ� ǥ�� ��Ʈ��*/
		BI_RGB,											/*biCompression		- ���� ��� BI_RGB(0), BI_RLE4(2), BI_RLE8(1)*/
		static_cast<DWORD>(_msize(_Undo->_Color)),		/*biSizeImage		- �ȼ� ������ ũ��*/
		MAKEFOURCC(0x00, 0x00, 0x0B, 0x12),				/*biXPelsPerMeter	- ���� �ػ�*/
		MAKEFOURCC(0x00, 0x00, 0x0B, 0x12),				/*biYPelsPerMeter	- ���� �ػ�*/
		0,												/*biClrUsed			- ������̺� �ε��� ����*/
		0												/*biClrImportant	- ȭ�� ��¿� ���� �ε��� ����*/
	};
	return bmpInfo;
}

BYTE *CPnmEdit::SetBmpColor(DWORD dummy_dat){
	BYTE* bmpColor = new BYTE[_Undo->_ImgRow * _Undo->_ImgCol * _RGB + dummy_dat];
	return bmpColor;
}

BYTE CPnmEdit::CheckExitFlag() {
	BYTE menu = 255;
	printf("%s %s\n%s %s\n%s %s\n%s %s\n%s %s\n>> ", "0. ����", "1. �簢�� �׸���", "2. ���������� ȸ��", "3.�������� ȸ��", "4. �̹��� ���� ����", \
		"5.Ȯ���� ����", "6. ���� ���", "7. �����", "8. �̹��� ����", "9. BMP�� ����");
	do {
	    ignore = scanf_s("%hhu", &menu);
		if (menu < 0 || 9 < menu) cout << "���� ���� ���Դϴ�. 0 ~ 9 ������ ���� �Է��ϼ���\n>> ";
	} while (menu < 0 || 9 < menu);
	
	return menu;
}