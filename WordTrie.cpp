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
    return ptr->word ? ptr : nullptr;
}

void WordTrie::_build_words(std::set<std::string>& results, std::string letters, std::string prefix, Node *ptr) const {
    if (_search(prefix) != nullptr) {
        results.insert(prefix);
    }

    for (auto i = 0; i < letters.size(); ++i) {
        const char ch = letters[i];
        if (ch == '_') {
            for (char c = 'a'; c <= 'z'; ++c) {
                int idx = index(c);
                if (ptr->children[idx] == nullptr) continue;
                letters.erase(i, 1);
                ptr = ptr->children[idx].get();
                prefix += c;
                _build_words(results, letters, prefix, ptr);
                prefix.pop_back();
                ptr = ptr->parent;
                letters.insert(i, 1, '_');
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
    std::set<std::string> words;
    if (prefix != "") {
        auto loc = _search(prefix);
        if (loc != nullptr) {
            _build_words(words, letters, prefix, loc);
        }
    }
    return std::vector<std::string>(words.begin(), words.end());
}

/*void WordTrie::_build_from_template(std::set<std::string>& results, const std::string& templ, std::string letters, Node* ptr) const {
    for (auto i = 0; i < letters.size(); ++i) {
        char ch = letters[i];
        if (ch == '_') {
            for (char c = 'a'; c <= 'z'; ++c) {
                int idx = index(c);
                if (ptr->children[idx] == nullptr) continue;
                letters.erase(i, 1);
                ptr = ptr->children[idx].get();
                //prefix += c;
                _build_words(results, letters, prefix, ptr);
                prefix.pop_back();
                ptr = ptr->parent;
                letters.insert(i, 1, '_');
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

// template will use _ for empty and

std::vector<std::string> WordTrie::build_from_template(const std::string& templ, const std::string& letters) const {
    
}*/
