#ifndef __SSODE_ARG_H
#define __SSODE_ARG_H

#include <string>
#include <algorithm>

class Args {

private:

	int argc;
	char **begin;
	char **end;

public:

	Args(int argc, char** argv) :
		argc(argc), begin(argv), end(argv+argc) {}

	bool has_option(const std::string& opt) const {
		return std::find(begin, end, opt) != end;
	}

	template<typename T>
	T get_option_or(const std::string& opt, const T& def) const {
		auto itr = std::find(begin, end, opt);
		if (itr != end && ++itr != end) {
			std::istringstream iss(*itr);
			T ret;
			iss >> ret;
			return ret;
		}
		return def;
	}

};

#endif