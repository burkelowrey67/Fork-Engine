#pragma once
#include <vector>
#include <string>

namespace uci {
	
	void fill_str_tokens(std::vector<std::string>& out, const char* str);
	void fill_char_ptr_tokens(std::vector<const char*>& out, const char* str);
}