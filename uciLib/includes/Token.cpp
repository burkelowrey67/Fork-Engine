#include "pch.h"
#include <token.h>

namespace uci {

	void fill_str_tokens(std::vector<std::string>& out, const char* str) {
		while (*str != '\0') {
			if (*str == ' ' && *(++str) != ' ') {
				std::string string = std::string(str);
				out.push_back(string);
				continue;
			}
			str++;
		}
	}

	void fill_char_ptr_tokens(std::vector<const char*>& out, const char* str) {
		while (*str != '\0') {
			if (*str == ' ' && *(++str) != ' ') out.push_back(str); continue;
			str++;
		}
	}
}