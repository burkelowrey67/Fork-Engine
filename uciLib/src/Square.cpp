#include "pch.h"
#include <square.h>

namespace uci {

    std::optional<std::string> square_index_to_uci(unsigned int index) {
        if (index > 63) return std::nullopt;

        char file = 'a' + (index % 8);
        char rank = '1' + (index / 8);

        return std::string{ file, rank };
    }

    int uci_to_square_index(const std::string& sq) {
        if (sq.size() != 2) {
            return -1;
        }

        char file = sq[0];
        char rank = sq[1];

        if (file < 'a' || file > 'h' || rank < '1' || rank > '8') {
            return -1;
        }

        int fileIndex = file - 'a';   // 0–7
        int rankIndex = rank - '1';   // 0–7

        return rankIndex * 8 + fileIndex;
    }
}