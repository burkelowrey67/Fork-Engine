#pragma once
#include <string>
#include <stdexcept>
#include <optional>

namespace uci {

	std::optional<std::string> square_index_to_uci(const unsigned int index);
	int uci_to_square_index(const std::string& sq);
}