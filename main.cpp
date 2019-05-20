#include <iostream>
#include <fstream>
#include <string>
#include "WordTrie.h"
#include "Words.h"



int main() {
	{
		WordTrie wt;
		std::ifstream ifs("wordlist.txt");
		int count = 0;
		for (std::string line; std::getline(ifs, line);) {
			wt.insert(line);
			++count;
		}
		std::cout << "Loaded " << count << " words\n";
		std::string prefix, letters;
		bool stop = false;
		while (!stop) {
			std::cout << "Enter a prefix: ";
			std::getline(std::cin, prefix);
			std::cout << "Enter the letters (? for wildcard): ";
			std::cin >> letters;
			auto words = wt.build_words(letters, prefix);
			std::sort(words.begin(), words.end(), [](const auto& a, const auto& b) {
				return Words::score(a) > Words::score(b);
			});
			std::cout << words.size() << " words found\n";
			for (const auto& word : words) {
				std::cout << word << " : " << Words::score(word) << '\n';
			}
			system("pause");
			system("cls");
		}
	}
	std::cin.ignore(2);
	return 0;
}