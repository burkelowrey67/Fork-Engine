#pragma once
#include <cstdint>
#include <piece/piece_type.h>

namespace position::bit_board {

    inline constexpr uint64_t EDGE_MASK = 0xFF818181818181FF;

    inline constexpr uint64_t A_FILE = 0x0101010101010101;
    inline constexpr uint64_t B_FILE = 0x0202020202020202;
    inline constexpr uint64_t C_FILE = 0x0404040404040404;
    inline constexpr uint64_t D_FILE = 0x0808080808080808;
    inline constexpr uint64_t E_FILE = 0x1010101010101010;
    inline constexpr uint64_t F_FILE = 0x2020202020202020;
    inline constexpr uint64_t G_FILE = 0x4040404040404040;
    inline constexpr uint64_t H_FILE = 0x8080808080808080;

    inline constexpr uint64_t FIRST_RANK =    0x00000000000000FF;
    inline constexpr uint64_t SECOND_RANK =   0x000000000000FF00;
    inline constexpr uint64_t THIRD_RANK =    0x0000000000FF0000;
    inline constexpr uint64_t FOURTH_RANK =   0x00000000FF000000;
    inline constexpr uint64_t FIFTH_RANK =    0x000000FF00000000;
    inline constexpr uint64_t SIXTH_RANK =    0x0000FF0000000000;
    inline constexpr uint64_t SEVENTH_RANK =  0x00FF000000000000;
    inline constexpr uint64_t EIGHTH_RANK =   0xFF00000000000000;

    inline constexpr uint64_t NE_CORNER = 0x8000000000000000;
    inline constexpr uint64_t NW_CORNER = 0x0100000000000000;
    inline constexpr uint64_t SW_CORNER = 0x0000000000000001;
    inline constexpr uint64_t SE_CORNER = 0x0000000000000080;

    inline constexpr uint64_t FILE_MASKS[8] = {
        0x0101010101010101, 0x0202020202020202, 0x0404040404040404, 0x0808080808080808,
        0x1010101010101010, 0x2020202020202020, 0x4040404040404040, 0x8080808080808080
    };

    inline constexpr uint64_t RANK_MASKS[8] = {
        0x00000000000000FF, 0x000000000000FF00, 0x0000000000FF0000, 0x00000000FF000000,
        0x000000FF00000000, 0x0000FF0000000000, 0x00FF000000000000, 0xFF00000000000000
    };

    inline constexpr uint64_t DEFAULT_W_KING =  0x0000000000000010;
    inline constexpr uint64_t DEFAULT_W_QUEEN = 0x0000000000000008;
    inline constexpr uint64_t DEFAULT_B_KING =  0x1000000000000000;
    inline constexpr uint64_t DEFAULT_B_QUEEN = 0x0800000000000000;

    inline constexpr unsigned int DEFAULT_W_KING_INDEX = 4;
    inline constexpr unsigned int DEFAULT_W_QUEEN_INDEX = 3;
    inline constexpr unsigned int DEFAULT_B_KING_INDEX = 60;
    inline constexpr unsigned int DEFAULT_B_QUEEN_INDEX = 59;
    
    inline constexpr unsigned int PRE_W_K_CASTLE_ROOK_INDEX = 7;
    inline constexpr unsigned int PRE_W_Q_CASTLE_ROOK_INDEX = 0;
    inline constexpr unsigned int PRE_B_K_CASTLE_ROOK_INDEX = 63;
    inline constexpr unsigned int PRE_B_Q_CASTLE_ROOK_INDEX = 56;

    inline constexpr unsigned int POST_W_K_CASTLE_KING_INDEX = 6;
    inline constexpr unsigned int POST_W_Q_CASTLE_KING_INDEX = 2;
    inline constexpr unsigned int POST_B_K_CASTLE_KING_INDEX = 62;
    inline constexpr unsigned int POST_B_Q_CASTLE_KING_INDEX = 58;

    inline constexpr unsigned int POST_W_K_CASTLE_ROOK_INDEX = 5;
    inline constexpr unsigned int POST_W_Q_CASTLE_ROOK_INDEX = 3;
    inline constexpr unsigned int POST_B_K_CASTLE_ROOK_INDEX = 61;

    inline constexpr unsigned int POST_B_Q_CASTLE_ROOK_INDEX = 59;

    inline constexpr uint64_t DEFAULT_POSITION[12] = {
        SECOND_RANK, 0x0000000000000042, 0x0000000000000024, 0x0000000000000081, DEFAULT_W_QUEEN, DEFAULT_W_KING,
        SEVENTH_RANK, 0x4200000000000000, 0x2400000000000000, 0x8100000000000000, DEFAULT_B_QUEEN, DEFAULT_B_KING
    };

	void remove_redundant_edge_bits(uint64_t& bitBoard, unsigned int squareIndex, piece::PieceType);
    uint64_t square_to_bit_board(unsigned int squareIndex);
    unsigned int get_first_square_index(uint64_t& bitBoard);
    void move_bit(uint64_t& bitBoard, unsigned int startSquare, unsigned int endSquare);
    void set_bit_one(uint64_t& bitBoard, unsigned int squareIndex);
    void set_bit_zero(uint64_t& bitBoard, unsigned int squareIndex);
    void remove_first_bit(uint64_t& bitBoard);
}