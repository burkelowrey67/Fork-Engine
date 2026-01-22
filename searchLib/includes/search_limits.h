#pragma once
#include <cmath>
#include <optional>

namespace search {
	struct SearchLimits {
        std::optional<int> depth = std::nullopt;
        std::optional<int> movetime = std::nullopt;
        std::optional<int> wtime = std::nullopt;
        std::optional<int> btime = std::nullopt;
        std::optional<int> winc = std::nullopt;
        std::optional<int> binc = std::nullopt;
        std::optional<int> movestogo = std::nullopt;
        int nodes = INT_MAX;
        bool infinite = false;
	};
}