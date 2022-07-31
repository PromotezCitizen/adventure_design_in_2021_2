#include "imgHeader.h"
#include "PnmEdit.h"

int main() {
	CPnmEdit imgEdit;
	string path;
	int editExitFlag = 0;

	do {
		system("cls");
		cout << "파일의 경로를 입력해주세요(드래그&드랍 가능, 종료 : e, exit) >> ";
		cin >> path;

		if (path.compare("e") == 0 || path.compare("exit") == 0) break;

		if (!imgEdit.ReadImg(fopen(path.c_str(), "rb"))) {
			printf("ppm 혹은 pgm 파일이 아닙니다.\n");
			system("pause");
			continue;
		}

		while ((editExitFlag = imgEdit.CheckExitFlag()) != _EXIT_FLAG) {
			switch (editExitFlag) {
			case _DRAW_SQUARE:
				imgEdit.DrawSquare();
				cout << "square drew" << endl;
				break;
			case _TURN_RIGHT:
				imgEdit.TurnClockwise90();
				cout << "image turn right" << endl;
				break;
			case _TURN_LEFT:
				imgEdit.TurnCounterClockwise90();
				cout << "image turn left" << endl;
				break;
			case _IMG_COLOR_REVERSE:
				imgEdit.ReverseImgColor();
				cout << "image color reversed" << endl;
				break;
			case _CONVERT_EXTENSION:
				imgEdit.ConvertExtension();
				cout << "image extension converted" << endl;
				break;
			case _UNDO:
				imgEdit.Undo();
				cout << "excute undo" << endl;
				break;
			case _REWIND:
				imgEdit.Rewind();
				cout << "excute rewind" << endl;
				break;
			case _SAVE_IMG:
				imgEdit.SaveImg();
				cout << "image saved" << endl;
				break;
			case _SAVE_2_BMP:
				imgEdit.Convert2BMP();
				cout << "image saved to bmp" << endl;
				break;
			}
		}
	} while (true);
	

	return 0;
}