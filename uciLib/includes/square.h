#pragma once
#include <string>
#include <stdexcept>

namespace uci {

    inline constexpr std::string square_index_to_algebraic(const unsigned int index) {
        if (index > 63) throw std::invalid_argument("Invalid index");

        char file = 'a' + (index % 8);
        char rank = '1' + (index / 8);
        return std::string() + file + rank;  // concatenate chars into a string
    }

    static constexpr int algebraic_to_square_index(const std::string& sq) {
        if (sq.size() != 2) {
            throw std::invalid_argument("Invalid algebraic notation length");
        }

        char file = sq[0];
        char rank = sq[1];

        if (file < 'a' || file > 'h' || rank < '1' || rank > '8') {
            throw std::invalid_argument("Invalid file or rank in algebraic notation");
        }

        int fileIndex = file - 'a';   // 0–7
        int rankIndex = rank - '1';   // 0–7

        return rankIndex * 8 + fileIndex;
    }
}