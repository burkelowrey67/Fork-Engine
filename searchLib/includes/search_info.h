#pragma once
#include <optional>

namespace search {
    struct SearchInfo {
    public:
        std::optional<uint32_t> bestMove = std::nullopt;
        std::optional<int> depth = std::nullopt;
        std::optional<int> nodesVisited = std::nullopt;
        std::optional<double> eval = std::nullopt;
    };
}