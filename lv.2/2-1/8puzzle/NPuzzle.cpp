#define _CRT_SECURE_NO_WARNINGS

#include "puzzle8.h"
#include "NPuzzle.h"

void CNPuzzle::PrintMap(BYTE *map, BYTE tiles) {
	for (BYTE row = 0; row < tiles; row++) {
		for (BYTE col = 0; col < tiles; col++) printf("%2d ", map[row * tiles + col]);
		printf("\n");
	}
	printf("\n");
}

void CNPuzzle::PrintMap(BYTE *map1, BYTE *map2, BYTE tiles) {
	for (BYTE row = 0; row < tiles; row++) {
		for (BYTE col = 0; col < tiles; col++) printf("%2d ", map1[row * tiles + col]);
		printf("\t");
		for (BYTE col = 0; col < tiles; col++) printf("%2d ", map2[row * tiles + col]);
		printf("\n");
	}
	printf("\n");
}

void CNPuzzle::PrintVector(std::vector<BYTE> freq) {
	for (auto it = freq.begin(); it != freq.end(); it++) printf("%d ", *it);
	printf("\n");
}

std::vector<BYTE> CNPuzzle::Shuffle(std::vector<BYTE> movefreq, BYTE *map, struct Position *pos, int tiles) {
	std::random_device rd;
	std::mt19937 gen(rd()); // rd는 srand에서 초기화하는 역할
	std::uniform_int_distribution<> dis(0, 3); // 0부터 3까지의 값 중 하나를 생성함.
	char direction = 0;

	struct Position temppos = { 0,0 };

	for (UINT cnt = 0; cnt < 100; cnt++) {
		movefreq = ShuffleDat(movefreq, map, pos, *pos, tiles, dis(gen));
	}

	return movefreq;
}

std::vector<BYTE> CNPuzzle::ShuffleDat(std::vector<BYTE> movefreq, BYTE *map, struct Position *pos, struct Position temppos, int tiles, char direction) {
	switch (direction) {
	case UP:
		if (--(pos->_row) < 0)	++(pos->_row);
		else					movefreq = MakeFreqVector(movefreq, direction, map, *pos, temppos, tiles);
		break;
	case LEFT:
		if (--(pos->_col) < 0)	++(pos->_col);
		else					movefreq = MakeFreqVector(movefreq, direction, map, *pos, temppos, tiles);
		break;
	case DOWN:
		if (++(pos->_row) > tiles - 1)	--(pos->_row);
		else							movefreq = MakeFreqVector(movefreq, direction, map, *pos, temppos, tiles);
		break;
	case RIGHT:
		if (++(pos->_col) > tiles - 1)	--(pos->_col);
		else							movefreq = MakeFreqVector(movefreq, direction, map, *pos, temppos, tiles);
		break;
	}
	return movefreq;
}

void CNPuzzle::AutoMove(BYTE *map, struct Position *pos, int tiles, char direction) {
	struct Position temppos = *pos;
	switch (direction) {
	case UP:
		--(pos->_row);
		break;
	case LEFT:
		--(pos->_col);
		break;
	case DOWN:
		++(pos->_row);
		break;
	case RIGHT:
		++(pos->_col);
		break;
	}
	SwapVal(map + (pos->_row * tiles + pos->_col), map + (temppos._row * tiles + temppos._col));
}

BYTE *CNPuzzle::CreateMap(int tiles) {
	BYTE *map = new BYTE[tiles * tiles];
	std::fill_n(map, tiles * tiles, 0);
	for (BYTE idx = 1; idx < tiles * tiles; idx++) map[idx] = idx;

	return map;
}

std::vector<BYTE> CNPuzzle::MakeFreqVector(std::vector<BYTE> freq, char user_input, BYTE *map, struct Position pos, struct Position temppos, int tiles, bool is_player_move) {
	switch (user_input) {
	case 'w': case 'W':
		if (*(freq.end() - 1) == DOWN)	freq.pop_back();
		else							freq.push_back(UP);
		break;
	case 'a': case 'A':
		if (*(freq.end() - 1) == RIGHT) freq.pop_back();
		else							freq.push_back(LEFT);
		break;
	case 's': case 'S':
		if (*(freq.end() - 1) == UP)	freq.pop_back();
		else							freq.push_back(DOWN);
		break;
	case 'd': case 'D':
		if (*(freq.end() - 1) == LEFT)	freq.pop_back();
		else							freq.push_back(RIGHT);
		break;
	default:
		break;
	}

	SwapVal(map + (p1_pos._row * tiles + p1_pos._col), map + (temppos._row * tiles + temppos._col));;

	return freq;
}

std::vector<BYTE> CNPuzzle::MakeFreqVector(std::vector<BYTE> freq, BYTE direction, BYTE *map, struct Position pos, struct Position temppos, int tiles) {
	BYTE direct = ((direction % 2) == 0 ? ((direction / 2) == 0 ? DOWN : UP) : ((direction / 2) == 0 ? RIGHT : LEFT));

	SwapVal(map + (pos._row * tiles + pos._col), map + (temppos._row * tiles + temppos._col));
	if (freq.size() < 1)					freq.push_back(direction);
	else if (*(freq.end() - 1) == direct)	freq.pop_back();
	else									freq.push_back(direction);

	return freq;
}

void CNPuzzle::SwapVal(BYTE *val1, BYTE *val2) {
	BYTE temp = *val1;
	*val1 = *val2;
	*val2 = temp;
}

void CNPuzzle::SetMapSize() {
	printf("타일의 크기를 입력하세요(3 이상) >> ");
	do {
		std::ignore = scanf("%d", &tiles);
		if (tiles < 3) printf("3 이상의 값을 입력해주세요 >> ");
	} while (tiles < 3);
}

template <typename A>
bool CheckClearFlag(A *a, int arr_size) {
	for (int row_idx = 0; row_idx < arr_size; row_idx++) {
		for (int col_idx = 0; col_idx < arr_size; col_idx++) {
			if (a[row_idx * arr_size + col_idx] != row_idx * arr_size + col_idx) return false;
		}
	}
	return true;
}

void CNPuzzle::Run() {
	 int switch_case = 0;
	 bool exit_flag_vs = false, exit_flag_main = false, resize_map = true;

	while (!exit_flag_main){
		system("cls");
		if (resize_map) SetMapSize();

		while (!exit_flag_vs) {
			system("cls");
			printf("vs player - 1, vs ai - 2, solo play - 3, exit : e");

			switch (_getch()) {
			case __VS_PLAYER:
				player1 = CreateMap(tiles);
				player2 = CreateMap(tiles);

				p1_pos = { 0,0 };
				p2_pos = { 0,0 };

				p1_move = Shuffle(p1_move, player1, &p1_pos, tiles);
				p2_move = Shuffle(p2_move, player2, &p2_pos, tiles);

				PVPMode();

				delete[] player1; player1 = nullptr; p1_move.clear();
				delete[] player2; player2 = nullptr; p2_move.clear();
				break;
			case __VS_AI:
				player1 = CreateMap(tiles);
				player2 = CreateMap(tiles);

				p1_pos = { 0,0 };
				p2_pos = { 0,0 };

				p1_move = Shuffle(p1_move, player1, &p1_pos, tiles);
				p2_move = Shuffle(p2_move, player2, &p2_pos, tiles);

				VSAiMode();

				delete[] player1; player1 = nullptr; p1_move.clear();
				delete[] player2; player2 = nullptr; p2_move.clear();
				break;
			case __SOLOPLAY:
				player1 = CreateMap(tiles);

				p1_pos = { 0,0 };

				p1_move = Shuffle(p1_move, player1, &p1_pos, tiles);

				SoloPlayMode();

				delete[] player1; player1 = nullptr; p1_move.clear();
				break;
			case 'e': case 'E':
				exit_flag_vs = true;
				break;
			}
		}
		system("cls");
		printf("맵의 크기를 변경하여 다시 하시겠습니까?\nr : 맵 크기 변경 O , n : 맵 크기 변경 X, else : 종료");
		char t = _getch();
		if (t == 'r' || t == 'R') {
			resize_map = true;
			exit_flag_vs = false;
		}
		else if (t == 'n' || t == 'N') {
			resize_map = false;
			exit_flag_vs = false;
		}
		else exit_flag_main = true;
	}
}

void CNPuzzle::AIThread(bool *ai_break_flag, bool *player_break_flag) {
	while (!p2_move.empty() && (*player_break_flag == false)) {
		OutputDebugString(L"메시지\n");
		BYTE saved_move = *(p2_move.end() - 1);
		BYTE direction = saved_move % 2 == 0 ? (saved_move / 2 == 0 ? DOWN : UP) : (saved_move / 2 == 0 ? RIGHT : LEFT);
		p2_move.pop_back();
		AutoMove(player2, &p2_pos, tiles, direction);
		system("cls");
		PrintMap(player1, player2, tiles);
		if (CheckClearFlag(player2, tiles)) {
			*ai_break_flag = true;
			break;
		}
		*ai_break_flag = false;
		Sleep(1000);
	}
}

void CNPuzzle::PVPMode() {
	bool p1_end = false, p2_end = false;

	PVPModeLoop(&p1_end, &p2_end);

	system("cls");
	if (p1_end) printf("player1 승리\n");
	else		printf("player2 승리\n");
}

void CNPuzzle::PVPModeLoop(bool *p1_end, bool *p2_end) {
	struct Position p1_temppos;
	struct Position p2_temppos;

	while (true) {
		system("cls");
		PrintMap(player1, player2, tiles);
		*p1_end = CheckClearFlag(player1, tiles);
		*p2_end = CheckClearFlag(player2, tiles);

		if (*p1_end || *p2_end) break;

		p1_temppos = p1_pos;
		p2_temppos = p2_pos;

		switch (_getch()) {
		case 'w': case 'W':
			if (--p1_pos._row < 0)	++p1_pos._row;
			else					SwapVal(player1 + (p1_pos._row * tiles + p1_pos._col), player1 + (p1_temppos._row * tiles + p1_temppos._col));
			break;
		case 'a': case 'A':
			if (--p1_pos._col < 0)	++p1_pos._col;
			else					SwapVal(player1 + (p1_pos._row * tiles + p1_pos._col), player1 + (p1_temppos._row * tiles + p1_temppos._col));
			break;
		case 's': case 'S':
			if (++p1_pos._row > tiles - 1)	--p1_pos._row;
			else							SwapVal(player1 + (p1_pos._row * tiles + p1_pos._col), player1 + (p1_temppos._row * tiles + p1_temppos._col));
			break;
		case 'd': case 'D':
			if (++p1_pos._col > tiles - 1)	--p1_pos._col;
			else							SwapVal(player1 + (p1_pos._row * tiles + p1_pos._col), player1 + (p1_temppos._row * tiles + p1_temppos._col));
			break;
		case __VKEY:
		{
			switch (_getch()) {
			case __VKEY_UP:
				if (--p2_pos._row < 0)	++p2_pos._row;
				else					SwapVal(player2 + (p2_pos._row * tiles + p2_pos._col), player2 + (p2_temppos._row * tiles + p2_temppos._col));
				break;
			case __VKEY_LEFT:
				if (--p2_pos._col < 0)	++p2_pos._col;
				else					SwapVal(player2 + (p2_pos._row * tiles + p2_pos._col), player2 + (p2_temppos._row * tiles + p2_temppos._col));
				break;
			case __VKEY_DOWN:
				if (++p2_pos._row > tiles - 1)	--p2_pos._row;
				else							SwapVal(player2 + (p2_pos._row * tiles + p2_pos._col), player2 + (p2_temppos._row * tiles + p2_temppos._col));
				break;
			case __VKEY_RIGHT:
				if (++p2_pos._col > tiles - 1)	--p2_pos._col;
				else							SwapVal(player2 + (p2_pos._row * tiles + p2_pos._col), player2 + (p2_temppos._row * tiles + p2_temppos._col));
				break;
			}
		}
		default:
			break;
		}
	}
}

void CNPuzzle::VSAiMode() {
	bool ai_break_flag = false, player_break_flag = false;

	std::thread th(&CNPuzzle::AIThread, this, &ai_break_flag, &player_break_flag); // ai

	VSAiModeLoop(&ai_break_flag);

	th.join();

	if (ai_break_flag == false) player_break_flag = true;

	system("cls");
	PrintMap(player1, player2, tiles);
	if (player_break_flag == true)									printf("플레이어 승\n");
	else if (ai_break_flag == true && player_break_flag == true)	printf("AI 승\n");
}

void CNPuzzle::VSAiModeLoop(bool *ai_break_flag) {
	struct Position temppos;
	char get_ch = 0;

	while (!CheckClearFlag(player1, tiles)) {
		system("cls");
		PrintMap(player1, player2, tiles);
		temppos = p1_pos;
		get_ch = _getch();
		switch (get_ch) {
		case 'w': case 'W':
			if (--p1_pos._row < 0)	++p1_pos._row;
			else					p1_move = MakeFreqVector(p1_move, get_ch, player1, p1_pos, temppos, tiles, true);
			break;
		case 'a': case 'A':
			if (--p1_pos._col < 0)	++p1_pos._col;
			else					p1_move = MakeFreqVector(p1_move, get_ch, player1, p1_pos, temppos, tiles, true);
			break;
		case 's': case 'S':
			if (++p1_pos._row > tiles - 1)	--p1_pos._row;
			else							p1_move = MakeFreqVector(p1_move, get_ch, player1, p1_pos, temppos, tiles, true);
			break;
		case 'd': case 'D':
			if (++p1_pos._col > tiles - 1)	--p1_pos._col;
			else							p1_move = MakeFreqVector(p1_move, get_ch, player1, p1_pos, temppos, tiles, true);
			break;
		case 'h': case 'H':
			printf("%s\n", *(p1_move.end() - 1) % 2 == 0 ? *(p1_move.end() - 1) / 2 == 0 ? "DOWN" : "UP" : *(p1_move.end() - 1) / 2 == 0 ? "RIGHT" : "LEFT");
			Sleep(100);
			break;
		default:
			break;
		}
		if (*ai_break_flag) {
			break;
		}
	}
}

void CNPuzzle::SoloPlayMode() {
	struct Position temppos;
	char get_ch = 0;
	while (!CheckClearFlag(player1, tiles)) {
		system("cls");
		PrintMap(player1, tiles);
		temppos = p1_pos;
		get_ch = _getch();
		switch (get_ch) {
		case 'w': case 'W':
			if (--p1_pos._row < 0)	++p1_pos._row;
			else					p1_move = MakeFreqVector(p1_move, get_ch, player1, p1_pos, temppos, tiles, true);
			break;
		case 'a': case 'A':
			if (--p1_pos._col < 0)	++p1_pos._col;
			else					p1_move = MakeFreqVector(p1_move, get_ch, player1, p1_pos, temppos, tiles, true);
			break;
		case 's': case 'S':
			if (++p1_pos._row > tiles - 1)	--p1_pos._row;
			else							p1_move = MakeFreqVector(p1_move, get_ch, player1, p1_pos, temppos, tiles, true);
			break;
		case 'd': case 'D':
			if (++p1_pos._col > tiles - 1)	--p1_pos._col;
			else							p1_move = MakeFreqVector(p1_move, get_ch, player1, p1_pos, temppos, tiles, true);
			break;
		case 'h': case 'H':
			printf("%s\n", *(p1_move.end() - 1) % 2 == 0 ? *(p1_move.end() - 1) / 2 == 0 ? "DOWN" : "UP" : *(p1_move.end() - 1) / 2 == 0 ? "RIGHT" : "LEFT");
			Sleep(100);
			break;
		}
	}
	system("cls");
	PrintMap(player1, tiles);
}