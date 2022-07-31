//#pragma once
//
//#include "imgHeader.h"
//
//// pbm파일 지원 안함
//
//class CPortableImgEdit {
//public:
//	CPortableImgEdit();
//	~CPortableImgEdit();
//
//	UINT pow10(UINT _index);
//	void swapRowCol(UINT* a1, UINT* a2);
//
//	bool readImgHeader(FILE* input_file);
//	template<typename DEGIT>
//	void setImgHeaderData(DEGIT* imgdata, FILE* input_file, UCHAR spacer);
//
//	void setImgData(FILE* input_file);
//	void setImgData(BYTE ***temp_color);
//
//	void printImgHeader();
//	void printImgData();
//
//	void reverseImg();
//	void drawSquareOnImg();
//
//	void saveExtensionConvert();
//	void saveExtensionConvertData(UCHAR file_type, FILE* write_file);
//
//	void saveEditedImg();
//	template<typename DEGIT>
//	void saveEditedImgSubHeader(DEGIT imgdata, FILE* wfile);
//	void saveEditedImgData(FILE* wfile);
//
//	void saveBOMColRowBright(char BOM[2], FILE* write_file);
//
//	void undoImgData();
//	void rewindImgData();
//
//	void turnRight();
//	void turnLeft();
//
//	char* getBOM() {
//		static char returnBOMArr[3] = { imgdata->BOM[0], imgdata->BOM[1], '\0' };
//		return returnBOMArr;
//	}
//	UINT getImgRow() { return imgdata->row; }
//	UINT getImgCol() { return imgdata->col; }
//	BYTE getImgMaxbright() { return imgdata->max_bright; }
//
//private:
//	void setUNDOChain();
//	void delDoubleLinkedList();
//	void memDeleteImgData(BYTE*** rgb_bw);
//	void memDeleteImgData(BYTE ***rgb_bw, UINT idx[2]);
//
//protected:
//	IMGHEADER* imgdata = NULL;
//	UNDO* imgundo = NULL;
//	BYTE color_idx = 0;
//	BYTE* color = NULL;
//	UINT r1 = 0, c1 = 0, r2 = 0, c2 = 0;
//};
//
//typedef CPortableImgEdit PIE;