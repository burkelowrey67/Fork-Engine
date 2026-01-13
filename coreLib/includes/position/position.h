#pragma once
#include <cstdint>
#include <color.h>
#include <piece/piece_type.h>

namespace chess {

	class Position {

	public:

		uint64_t (&bitBoards)[12];
		uint64_t friendlyPieces;
		uint64_t enemyPieces;
		uint64_t allPieces;
		int pieceIndexAtSquare[64];
		uint8_t enPassantSquare;

		bool w_kingSideCastle;
		bool w_queenSideCastle;
		bool b_kingSideCastle;
		bool b_queenSideCastle;

		chess::Color toMove;

		Position(
			uint64_t (&bitBoards)[12],
			bool w_kingSideCastle, bool b_kingSideCastle, 
			bool w_queenSideCastle, bool b_queenSideCastle,
			uint8_t enPassantSquare, chess::Color _toMove
		);

		Position();

		void update_masks();
		void update_lookup();


		uint64_t& get_bit_board_ref(piece::PieceType, chess::Color);
		uint64_t get_bit_board(piece::PieceType pieceType, chess::Color color);

		uint64_t& get_friendly_pawn_bit_board_ref();
		uint64_t& get_friendly_knight_bit_board_ref();
		uint64_t& get_friendly_bishop_bit_board_ref();
		uint64_t& get_friendly_rook_bit_board_ref();
		uint64_t& get_friendly_queen_bit_board_ref();
		uint64_t& get_friendly_king_bit_board_ref();

		uint64_t& get_enemy_pawn_bit_board_ref();
		uint64_t& get_enemy_knight_bit_board_ref();
		uint64_t& get_enemy_bishop_bit_board_ref();
		uint64_t& get_enemy_rook_bit_board_ref();
		uint64_t& get_enemy_queen_bit_board_ref();
		uint64_t& get_enemy_king_bit_board_ref();
				
		uint64_t& get_pawn_bit_board_ref(chess::Color);
		uint64_t& get_knight_bit_board_ref(chess::Color);
		uint64_t& get_bishop_bit_board_ref(chess::Color);
		uint64_t& get_rook_bit_board_ref(chess::Color);
		uint64_t& get_queen_bit_board_ref(chess::Color);
		uint64_t& get_king_bit_board_ref(chess::Color);

		uint64_t get_friendly_pawn_bit_board();
		uint64_t get_friendly_knight_bit_board();
		uint64_t get_friendly_bishop_bit_board();
		uint64_t get_friendly_rook_bit_board();
		uint64_t get_friendly_queen_bit_board();
		uint64_t get_friendly_king_bit_board();

		uint64_t get_enemy_pawn_bit_board();
		uint64_t get_enemy_knight_bit_board();
		uint64_t get_enemy_bishop_bit_board();
		uint64_t get_enemy_rook_bit_board();
		uint64_t get_enemy_queen_bit_board();
		uint64_t get_enemy_king_bit_board();

		uint64_t get_pawn_bit_board(chess::Color);
		uint64_t get_knight_bit_board(chess::Color);
		uint64_t get_bishop_bit_board(chess::Color);
		uint64_t get_rook_bit_board(chess::Color);
		uint64_t get_queen_bit_board(chess::Color);
		uint64_t get_king_bit_board(chess::Color);

		int piece_count();
		int piece_count(chess::Color);
		int num_pawns(chess::Color);
		int num_knights(chess::Color);
		int num_bishops(chess::Color);
		int num_rooks(chess::Color);
		int num_queens(chess::Color);
		int num_pieces(piece::PieceType, chess::Color);

		int piece_diff();
		int pawn_diff();
		int knight_diff();
		int bishop_diff();
		int rook_diff();
		int queen_diff();

		bool is_king_attacked();
		bool is_king_attacked(chess::Color);

		bool is_square_attacked(unsigned int squareIndex, chess::Color attacking);
		bool is_empty_square(unsigned int squareIndex);
		bool is_enemy_square(unsigned int squareIndex);

	};
};