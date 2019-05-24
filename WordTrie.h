#pragma once

#include <array>
#include <memory>
#include <vector>
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

		Node* _search(const std::string& val) const;

		void _build_words(std::set<std::string>& results, std::string letters, std::string prefix, Node* cursor) const;

	public:

		void insert(const std::string& word);

		bool search(const std::string& val) const;

		void remove(const std::string& val);


		std::vector<std::string> build_words(const std::string& source, const std::string& prefix = "") const;
	};


