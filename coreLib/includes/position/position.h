#pragma once
#include <cstdint>
#include <color.h>
#include <piece/piece_type.h>

namespace core {

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

		core::Color toMove;

		int fullMoveClock;
		int halfMoveClock;

		Position(
			uint64_t (&bitBoards)[12],
			bool w_kingSideCastle, bool b_kingSideCastle, 
			bool w_queenSideCastle, bool b_queenSideCastle,
			uint8_t enPassantSquare, core::Color toMove, 
			int fullMoveClock, int halfMoveClock
		);

		static Position default_position();

		void update_masks();
		void update_lookup();


		uint64_t& get_bit_board_ref(core::PieceType, core::Color);
		uint64_t get_bit_board(core::PieceType pieceType, core::Color color);

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
				
		uint64_t& get_pawn_bit_board_ref(core::Color);
		uint64_t& get_knight_bit_board_ref(core::Color);
		uint64_t& get_bishop_bit_board_ref(core::Color);
		uint64_t& get_rook_bit_board_ref(core::Color);
		uint64_t& get_queen_bit_board_ref(core::Color);
		uint64_t& get_king_bit_board_ref(core::Color);

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

		uint64_t get_pawn_bit_board(core::Color);
		uint64_t get_knight_bit_board(core::Color);
		uint64_t get_bishop_bit_board(core::Color);
		uint64_t get_rook_bit_board(core::Color);
		uint64_t get_queen_bit_board(core::Color);
		uint64_t get_king_bit_board(core::Color);

		int piece_count();
		int piece_count(core::Color);
		int num_pawns(core::Color);
		int num_knights(core::Color);
		int num_bishops(core::Color);
		int num_rooks(core::Color);
		int num_queens(core::Color);
		int num_pieces(core::PieceType, core::Color);

		int piece_diff();
		int pawn_diff();
		int knight_diff();
		int bishop_diff();
		int rook_diff();
		int queen_diff();

		bool is_king_attacked();
		bool is_king_attacked(core::Color);

		bool is_square_attacked(unsigned int squareIndex, core::Color attacking);
		bool is_empty_square(unsigned int squareIndex);
		bool is_enemy_square(unsigned int squareIndex);

		/*
		* @brief Returns the pawn's index into a position's bit board array.
		* @param color: Color of pawn.
		* @return index
		*/
		static int pawn_index(core::Color color);

		/*
		* @brief Returns the knight's index into a position's bit board array.
		* @param color: Color of knight.
		* @return index
		*/
		static int knight_index(core::Color color);

		/*
		* @brief Returns the bishop's index into a position's bit board array.
		* @param color: Color of bishop.
		* @return index
		*/
		static int bishop_index(core::Color color);

		/*
		* @brief Returns the rook's index into a position's bit board array.
		* @param color: Color of rook.
		* @return index
		*/
		static int rook_index(core::Color color);

		/*
		* @brief Returns the queen's index into a position's bit board array.
		* @param color: Color of queen.
		* @return index
		*/
		static int queen_index(core::Color color);

		/*
		* @brief Returns the pawn's index into a position's bit board array.
		* @param color: Color of king.
		* @return index
		*/
		static int king_index(core::Color color);

		/*
		* @brief Returns a piece's index into a position's bit board array.
		* @param color: Color of piece.
		* @return index
		*/
		static int colored_index(core::Color color, core::PieceType pieceType);

		static int non_colored_index(core::PieceType pieceType);

		/*
		* @brief Converts bit board array index into a piece type.
		* @param index: Bit board array index.
		* @return piece type
		*/
		static core::PieceType index_to_piece_type(int index);
	};
};