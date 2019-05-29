#include "WordTrie.h"
 
WordTrie::Node* WordTrie::_search(const std::string& val) const {
    Node* ptr = root.get();
    for (const auto &c : val) {
        int i = index(c);
        if (ptr->children[i] == nullptr) {
            return nullptr;
        }
        ptr = ptr->children[i].get();
    }
	return ptr;
}

void WordTrie::_build_words(std::unordered_set<std::string>& results, std::string letters, std::string prefix, Node *ptr) const {
    if (search(prefix)) {
        results.insert(prefix);
    }

    for (int i = 0; i < letters.size(); ++i) {
        const char ch = letters[i];
        if (ch == '_') {
            for (char c = 'a'; c <= 'z'; ++c) {
                int idx = index(c);
				if (ptr->children[idx] != nullptr) {
					letters.erase(i, 1);
					ptr = ptr->children[idx].get();
					prefix += c;
					_build_words(results, letters, prefix, ptr);
					prefix.pop_back();
					ptr = ptr->parent;
					letters.insert(i, 1, '_');
				}
            }
        } else {
            int idx = index(ch);
            if (ptr->children[idx] != nullptr) {
                letters.erase(i, 1);
                ptr = ptr->children[idx].get();
                prefix += ch;
                _build_words(results, letters, prefix, ptr);
                prefix.pop_back();
                ptr = ptr->parent;
                letters.insert(i, 1, ch);
            }
        }
    }
}

void WordTrie::insert(const std::string& word) {
    if (search(word)) return;
    Node* ptr = root.get();
    for (const auto &c : word) {
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

bool WordTrie::search(const std::string& val) const {
	Node* res = _search(val);
	return res != nullptr && res->word;
}

void WordTrie::remove(const std::string& val) {
    Node* ptr = _search(val);
    if (ptr == nullptr) {
        return;
    }
    ptr->word = false;
    ptr = ptr->parent;
    for (auto itr = val.rbegin(); itr != val.rend(); ++itr) {
        if (ptr->num_children > 0) break;
        ptr = ptr->parent;
        ptr->children[index(*itr - 'a')] = nullptr;
    }
}

std::vector<std::string> WordTrie::build_words(const std::string& letters, const std::string& prefix) const {
    std::unordered_set<std::string> words;
	Node* start = prefix == "" ? root.get() : _search(prefix);
	if (start != nullptr)
		_build_words(words, letters, prefix, start);
    return std::vector<std::string>(words.begin(), words.end());
}

void WordTrie::_build_template(std::unordered_set<std::string>& results, std::deque<char>& tmp, std::string& letters, std::string& build, Node* ptr) const {
	if (search(build)) {
		results.insert(build);
	}

	if (tmp.empty())
		return;

	char t = tmp.front();

	if (t == '_') {
		for (int j = 0; j < letters.size(); ++j) {
			char c = letters[j];
			if (c == '_') {
				for (char ch = 'a'; ch <= 'z'; ++ch) {
					int idx = index(ch);
					if (ptr->children[idx] != nullptr) {
						letters.erase(j, 1);
						ptr = ptr->children[idx].get();
						build += ch;
						tmp.pop_front();
						_build_template(results, tmp, letters, build, ptr);
						tmp.push_front(t);
						build.pop_back();
						ptr = ptr->parent;
						letters.insert(j, 1, c);
					}
				}
			} else {
				int idx = index(c);
				if (ptr->children[idx] != nullptr) {
					letters.erase(j, 1);
					ptr = ptr->children[idx].get();
					build += c;
					tmp.pop_front();
					_build_template(results, tmp, letters, build, ptr);
					tmp.push_front(t);
					build.pop_back();
					ptr = ptr->parent;
					letters.insert(j, 1, c);
				}
			}
		}
	} else {
		int idx = index(t);
		if (ptr->children[idx] != nullptr) {
			build += t;
			ptr = ptr->children[idx].get();
			tmp.pop_front();
			_build_template(results, tmp, letters, build, ptr);
			tmp.push_front(t);
			ptr = ptr->parent;
			build.pop_back();
		}
	}
}

std::vector<std::string> WordTrie::build_from_template(const std::string& templ, std::string& letters) const {
	std::unordered_set<std::string> words;
	std::string prefix;
	std::deque<char> tmp(templ.begin(), templ.end());
	Node* start = nullptr;
	if (tmp.front() == '_') {
		start = root.get();
	} else {
		while (tmp.front() != '_') {
			prefix += tmp.front();
			tmp.pop_front();
		}
		start = _search(prefix);
	}
	if (start != nullptr)
		_build_template(words, tmp, letters, prefix, start);
	return std::vector<std::string>(words.begin(), words.end());
}