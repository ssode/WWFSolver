#pragma once

#include <array>
#include <cctype>
#include <memory>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>


	class WordTrie {

	private:

		static constexpr int index(const char c) {
			return c - 'a';
		}

		struct Node {

			Node* parent = nullptr;
			std::array<std::unique_ptr<Node>, 26> children;
			int num_children;
			bool word = false;


		};

		std::unique_ptr<Node> root = std::make_unique<Node>();

		Node* _search(const std::string& val) const {
			auto ptr = root.get();
			for (const auto& c : val) {
				int i = index(c);
				if (ptr->children[i] == nullptr) {
					return nullptr;
				}
				ptr = ptr->children[i].get();
			}
			return ptr->word ? ptr : nullptr;
		}

		void _build_words(std::set<std::string>& results, std::string letters, std::string prefix, Node* cursor) const {

			if (_search(prefix) != nullptr) {
				results.insert(prefix);
			}

			for (auto i = 0; i < letters.size(); ++i) {
				const char ch = letters[i];
				if (ch == '?') {
					for (char c = 'a'; c <= 'z'; ++c) {
						int idx = index(c);
						if (cursor->children[idx] == nullptr)
							continue;
						letters.erase(i, 1);
						cursor = cursor->children[idx].get();
						prefix += c;
						_build_words(results, letters, prefix, cursor);
						prefix.pop_back();
						cursor = cursor->parent;
						letters.insert(i, 1, '?');
					}
				} else {
					int idx = index(ch);
					if (cursor->children[idx] != nullptr) {
						letters.erase(i, 1);
						cursor = cursor->children[idx].get();
						prefix += ch;
						_build_words(results, letters, prefix, cursor);
						prefix.pop_back();
						cursor = cursor->parent;
						letters.insert(i, 1, ch);
					}
				}
				
				
			}
		}

	public:

		void insert(const std::string& word) {
			if (search(word))
				return;
			Node* ptr = root.get();
			for (const auto& c : word) {
				int i = index(c);
				if (ptr->children[i] == nullptr) {
					ptr->children[i] = std::make_unique<Node>();
					ptr->children[i]->parent = ptr;
					++(ptr->num_children);
				}
				ptr = ptr->children[i].get();
			}
			ptr->word = true;
		}

		bool search(const std::string& val) const {
			return _search(val) != nullptr;
		}

		void remove(const std::string& val) {
			Node* ptr = _search(val);
			if (ptr == nullptr) {
				return;
			}
			ptr->word = false;
			ptr = ptr->parent;
			for (auto itr = val.rbegin(); itr != val.rend(); ++itr) {
				if (ptr->num_children > 0)
					break;
				ptr = ptr->parent;
				ptr->children[index(*itr - 'a')] = nullptr;
			}
		}

		std::vector<std::string> build_words(const std::string& source, const std::string& prefix = "") const {
			std::set<std::string> words;
			_build_words(words, source, prefix, root.get());
			return std::vector<std::string>(words.begin(), words.end());
		}
	};


