#include "pch.h"
#include <token.h>

namespace uci {

	void fill_str_tokens(std::vector<std::string>& out, const char* str) {
		const char* start = str;
		const char* end = nullptr;

		while (true) {

			if (bool endStr = *str == '\0'; (*str == ' ' || endStr) && end == nullptr) {
				end = str;
				std::string string = std::string(start, end);
				out.push_back(string);
				if (endStr) break;
				start = nullptr;
			}

			else if (*str != ' ' && end != nullptr) {
				start = str;
				end = nullptr;
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