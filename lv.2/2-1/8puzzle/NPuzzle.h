#pragma once

#define _CRT_SECURE_NO_WARNINIGS

class CNPuzzle {
public:
	CNPuzzle() {
		player1 = nullptr; p1_move.clear();
		player2 = nullptr; p2_move.clear();

		p1_pos = { 0,0 };
		p2_pos = { 0,0 };

		tiles = 0;
	}
	~CNPuzzle() {
		if (player1 != nullptr) delete[] player1;
		if (player2 != nullptr) delete[] player2;
	}

	void Run();

protected:
	void PrintMap(BYTE *map, BYTE tiles);
	void PrintMap(BYTE *map1, BYTE *map2, BYTE tiles);

	void PrintVector(std::vector<BYTE> freq);

	void SetMapSize();

	BYTE *CreateMap(int tiles);

	void SwapVal(BYTE *val1, BYTE *val2);
	std::vector<BYTE> Shuffle(std::vector<BYTE> movefreq, BYTE *map, struct Position *pos, int tiles);
	std::vector<BYTE> ShuffleDat(std::vector<BYTE> movefreq, BYTE *map, struct Position *pos, struct Position temppos, int tiles, char direction);
	void AutoMove(BYTE *map, struct Position *pos, int tiles, char direction);

	std::vector<BYTE> MakeFreqVector(std::vector<BYTE> freq, char user_input, BYTE *map, struct Position pos, struct Position temppos, int tiles, bool is_player_move);
	std::vector<BYTE> MakeFreqVector(std::vector<BYTE> freq, BYTE direction, BYTE *map, struct Position pos, struct Position temppos, int tiles);

	void AIThread(bool *ai_break_flag, bool *player_break_flag);

	void PVPMode();
	void PVPModeLoop(bool *p1_end, bool *p2_end);

	void VSAiMode();
	void VSAiModeLoop(bool *ai_break_flag);

	void SoloPlayMode();

private:
	BYTE *player1 = nullptr; std::vector<BYTE> p1_move;
	BYTE *player2 = nullptr; std::vector<BYTE> p2_move;

	struct Position p1_pos = { 0,0 };
	struct Position p2_pos = { 0,0 };

	int tiles = 0;
};