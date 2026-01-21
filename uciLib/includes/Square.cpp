#include "pch.h"
#include <square.h>

namespace uci {
    char* square_index_to_uci(const unsigned int index) {
        if (index > 63) return nullptr;

        char file = 'a' + (index % 8);
        char rank = '1' + (index / 8);
        return (std::string() + file + rank).data();  // concatenate chars into a string
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