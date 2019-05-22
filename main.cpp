#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include "WordTrie.h"
#include "Args.h"

static constexpr const std::array<int, 26> CHARSCORES = { 1, 4, 4, 2, 1, 4, 3, 3, 1, 10, 5, 2, 4, 2, 1, 4, 10, 1, 1, 1, 2, 5, 4, 8, 3, 10 };

unsigned score(const std::string& word) {
	unsigned score = 0;
	for (const char& ch : word) {
		score += CHARSCORES[ch - 'a'];
	}
	return score;
}

int main(int argc, char** argv) {
	Args args(argc, argv);
	if (argc < 2) {
		std::cout << "Usage: wwfsolver [options] [letters]\n";
		return 1;
	} else if (args.has_option("-?") || args.has_option("--help")) {
		std::cout 
			<< "Usage: wwfsolver [options] [letters]\n"
			<< "Options:\n" 
			<< "\t-p [prefix] : sets the prefix of the search string\n"
			<< "\t-l [limit] : sets a limit on the number of results returned\n"
			<< "\t-lc [num_letters] : specifies the number of letters in the words to be returned\n"
			<< "\t-f [filename] : specifies a file to use as input (1 word per line)\n";
		return 0;
	}
	int count;
	std::string filename = args.get_option_or<std::string>("-f", "wordlist.txt");
	std::string prefix = args.get_option_or<std::string>("-p", "");
	std::string letters = argv[argc - 1];
	int letter_count = args.get_option_or<int>("-lc", 0);
	std::transform(prefix.begin(), prefix.end(), prefix.begin(), ::tolower);
	std::transform(letters.begin(), letters.end(), letters.begin(), ::tolower);
	std::ifstream ifs(filename);
	if (!ifs.is_open()) {
		std::cout << "Could not open input file: " << filename << '\n';
		return 1;
	}
	int num_words = 0;
	WordTrie wt;
	for (std::string line; std::getline(ifs, line); ++num_words) {
		wt.insert(line);
	}
	std::cout << "Loaded " << num_words << " words from " << filename << '\n';
	auto words = wt.build_words(letters, prefix);
	std::sort(words.begin(), words.end(), [](const auto& a, const auto& b) {
		return score(a) > score(b);
	});
	count = args.get_option_or<int>("-l", words.size());
	count = std::min<int>(count, words.size());
	for (int i = 0; i < count; ++i) {
		if (letter_count < 1)
			std::cout << words[i] << " : " << score(words[i]) << '\n';
		else
			if (words[i].size() == letter_count)
				std::cout << words[i] << " : " << score(words[i]) << '\n';
	}
	return 0;
}