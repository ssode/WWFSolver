#pragma once

#include "WordTrie.h"
#include<map>

class Words {

private:

	WordTrie tree;

public:

	static constexpr const std::array<int, 26> CHARSCORES = { 1, 4, 4, 2, 1, 4, 3, 3, 1, 10, 5, 2, 4, 2, 1, 4, 10, 1, 1, 1, 2, 5, 4, 8, 3, 10 };

	enum class TileType {
		NONE,
		DL,
		DW,
		TL,
		TW
	};

	static int score(const std::string& word) {
		int score = 0;
		for (const auto& ch : word) {
			score += CHARSCORES[ch - 'a'];
		}
		return score;
	}

};
