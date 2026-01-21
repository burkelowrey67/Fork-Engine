#pragma once
#include <string>
#include <stdexcept>

namespace uci {

	char* square_index_to_uci(const unsigned int index);
	int uci_to_square_index(const std::string& sq);
}