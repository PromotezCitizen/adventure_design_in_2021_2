#pragma once
#include "imgHeader.h"

class CPnmEdit {
public:
	CPnmEdit();
	~CPnmEdit() { DeleteUndoChain(_Undohead); }

	bool ReadImg(FILE *input_file);

	void ReverseImgColor();

	void DrawSquare();

	void TurnClockwise90();
	void TurnCounterClockwise90();

	void SaveImg();

	void ConvertExtension();

	void Undo() { if (_Undo->_Before != nullptr) { _Undo = _Undo->_Before; } }
	void Rewind() { if (_Undo->_Next != nullptr) { _Undo = _Undo->_Next; } }

	void Convert2BMP();

	BYTE CheckExitFlag();

protected:
	UINT Pow10(UINT _index);
	void SwapRowCol(UINT *a1, UINT *a2) { UINT temp = *a1; *a1 = *a2; *a2 = temp; }

	// sub function of ReadImg
	bool ReadImgHeader(FILE *input_file);
	template<typename DEGIT>
	void ReadImgHeaderData(DEGIT *imgdata, FILE *input_file, UCHAR spacer);
	void ReadImgColorData(FILE *input_file);

	// sub function of ReverseImgColor
	BYTE ReverseImgChanel();

	// sub functions of ConvertExtension
	void SetCopyData(BYTE *temp_arr);

	// sub function of DrawSquare
	void SetSquarePos(UINT *r1, UINT *c1, UINT *r2, UINT *c2);
	void SetSquareColor(const char *msg, BYTE *color);

	// sub function of Turn**wise90
	BYTE *NewImgColorArr(UINT *temp_row, UINT *temp_col);
	void DeleteImgColorArr(BYTE *color_arr) { delete[] color_arr; }

	// sub function of SaveImg
	void SaveBOMColRowBright(FILE *write_file);
	template<typename DEGIT>
	void SaveImgSubHeader(DEGIT imgdata, FILE *wfile);
	void SaveImgColorData(FILE *wfile) { fwrite(_Undo->_Color, sizeof(BYTE), _Undo->_ImgRow * _Undo->_ImgCol * _Undo->_ColorBytes, wfile); }

	// functions of undo chain
	void SetUndoChain();
	void DeleteUndoChain(UNDO *t);
	void SetUndoChainData();

	// sub functions of Convert2BMP
	BITMAPFILEHEADER SetBmpHeader(DWORD dummy_dat);
	BITMAPINFOHEADER SetBmpInfo();
	BYTE* SetBmpColor(DWORD dummy_dat);
private:
	UNDO *_Undo = nullptr;
	UNDO *_Undohead = nullptr;
};

