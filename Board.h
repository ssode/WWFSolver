#pragma once

#include <array>

struct Tile;

class Board {

private:

	std::array<std::array<Tile, 15>, 15> board = { 
		{}
	};

public:

	enum class TileType {
		NONE,
		DL,
		DW,
		TL,
		TW
	};

	struct Tile {
		int row;
		int col;
		char letter;
		TileType type;
	};

	Board() {

	}

};