#include <position/position.h>
#include <color.h>
#include <bit>
#include <array>
#include <move/move_application.h>
#include <position/bit_board.h>
#include <move/move_lookup.h>

namespace core {

	Position::Position(
		uint64_t (&_bitBoards)[12],
		bool _w_kingSideCastle, bool _b_kingSideCastle,
		bool _w_queenSideCastle, bool _b_queenSideCastle,
		uint8_t _enPassantSquare, core::Color _toMove

	) : bitBoards(_bitBoards), 
		w_kingSideCastle(_w_kingSideCastle), w_queenSideCastle(_w_queenSideCastle),
		b_kingSideCastle(_b_kingSideCastle), b_queenSideCastle(_b_queenSideCastle), 
		enPassantSquare(_enPassantSquare), toMove(_toMove) {

		update_masks();
		update_lookup();
	}

	void Position::update_masks() {
		friendlyPieces = toMove == core::Color::White ?
			bitBoards[0] | bitBoards[1] | bitBoards[2] | bitBoards[3] | bitBoards[4] | bitBoards[5] :
			bitBoards[6] | bitBoards[7] | bitBoards[8] | bitBoards[9] | bitBoards[10] | bitBoards[11];

		enemyPieces = toMove == core::Color::Black ?
			bitBoards[0] | bitBoards[1] | bitBoards[2] | bitBoards[3] | bitBoards[4] | bitBoards[5] :
			bitBoards[6] | bitBoards[7] | bitBoards[8] | bitBoards[9] | bitBoards[10] | bitBoards[11];

		allPieces = friendlyPieces | enemyPieces;
	}

	void Position::update_lookup() {
		std::fill(pieceIndexAtSquare, pieceIndexAtSquare + 64, -1);

		for (int p = 0; p < 12; p++) {
			uint64_t bitBoard = bitBoards[p];

			while (bitBoard != 0) {
				int squareIndex = std::countr_zero(bitBoard);
				pieceIndexAtSquare[squareIndex] = p;
				bitBoard &= bitBoard - 1;
			}
		}
	}

	uint64_t& Position::get_bit_board_ref(core::piece::PieceType pieceType, core::Color color) {
		int encoded = (static_cast<int>(color) << 3) | static_cast<int>(pieceType);

		switch (encoded) {
		case 0b0:		return bitBoards[0];
		case 0b1:		return bitBoards[1];
		case 0b10:		return bitBoards[2];
		case 0b11:		return bitBoards[3];
		case 0b100:		return bitBoards[4];
		case 0b101:		return bitBoards[5];
		case 0b1000:	return bitBoards[6];
		case 0b1001:	return bitBoards[7];
		case 0b1010:	return bitBoards[8];
		case 0b1011:	return bitBoards[9];
		case 0b1100:	return bitBoards[10];
		case 0b1101:	return bitBoards[11];
		default:		return bitBoards[0];
		}
	}

	uint64_t Position::get_bit_board(core::piece::PieceType pieceType, core::Color color) {
		return get_bit_board_ref(pieceType, color);
	}

	uint64_t& Position::get_friendly_pawn_bit_board_ref() {
		return get_pawn_bit_board_ref(toMove);
	}

	uint64_t& Position::get_friendly_knight_bit_board_ref() {
		return get_knight_bit_board_ref(toMove);
	}

	uint64_t& Position::get_friendly_bishop_bit_board_ref() {
		return get_bishop_bit_board_ref(toMove);
	}

	uint64_t& Position::get_friendly_rook_bit_board_ref() {
		return get_rook_bit_board_ref(toMove);
	}

	uint64_t& Position::get_friendly_queen_bit_board_ref() {
		return get_queen_bit_board_ref(toMove);
	}

	uint64_t& Position::get_friendly_king_bit_board_ref() {
		return get_king_bit_board_ref(toMove);
	}

	uint64_t& Position::get_enemy_pawn_bit_board_ref() {
		return get_king_bit_board_ref(core::opposite_color(toMove));
	}

	uint64_t& Position::get_enemy_knight_bit_board_ref() {
		return get_knight_bit_board_ref(core::opposite_color(toMove));
	}

	uint64_t& Position::get_enemy_bishop_bit_board_ref() {
		return get_bishop_bit_board_ref(core::opposite_color(toMove));
	}

	uint64_t& Position::get_enemy_rook_bit_board_ref() {
		return get_rook_bit_board_ref(core::opposite_color(toMove));
	}

	uint64_t& Position::get_enemy_queen_bit_board_ref() {
		return get_queen_bit_board_ref(core::opposite_color(toMove));
	}

	uint64_t& Position::get_enemy_king_bit_board_ref() {
		return get_king_bit_board_ref(core::opposite_color(toMove));
	}

	uint64_t& Position::get_pawn_bit_board_ref(core::Color color) {
		return color == core::Color::White ? bitBoards[0] : bitBoards[6];
	}

	uint64_t& Position::get_knight_bit_board_ref(core::Color color) {
		return color == core::Color::White ? bitBoards[1] : bitBoards[7];
	}

	uint64_t& Position::get_bishop_bit_board_ref(core::Color color) {
		return color == core::Color::White ? bitBoards[2] : bitBoards[8];
	}

	uint64_t& Position::get_rook_bit_board_ref(core::Color color) {
		return color == core::Color::White ? bitBoards[3] : bitBoards[9];
	}

	uint64_t& Position::get_queen_bit_board_ref(core::Color color) {
		return color == core::Color::White ? bitBoards[4] : bitBoards[10];
	}

	uint64_t& Position::get_king_bit_board_ref(core::Color color) {
		return color == core::Color::White ? bitBoards[5] : bitBoards[11];
	}

	uint64_t Position::get_friendly_pawn_bit_board() {
		return get_pawn_bit_board(toMove);
	}

	uint64_t Position::get_friendly_knight_bit_board() {
		return get_knight_bit_board(toMove);
	}

	uint64_t Position::get_friendly_bishop_bit_board() {
		return get_bishop_bit_board(toMove);
	}

	uint64_t Position::get_friendly_rook_bit_board() {
		return get_rook_bit_board(toMove);
	}

	uint64_t Position::get_friendly_queen_bit_board() {
		return get_queen_bit_board(toMove);
	}

	uint64_t Position::get_friendly_king_bit_board() {
		return get_king_bit_board(toMove);
	}

	uint64_t Position::get_enemy_pawn_bit_board() {
		return get_king_bit_board(core::opposite_color(toMove));
	}

	uint64_t Position::get_enemy_knight_bit_board() {
		return get_knight_bit_board(core::opposite_color(toMove));
	}

	uint64_t Position::get_enemy_bishop_bit_board() {
		return get_bishop_bit_board(core::opposite_color(toMove));
	}

	uint64_t Position::get_enemy_rook_bit_board() {
		return get_rook_bit_board(core::opposite_color(toMove));
	}

	uint64_t Position::get_enemy_queen_bit_board() {
		return get_queen_bit_board(core::opposite_color(toMove));
	}

	uint64_t Position::get_enemy_king_bit_board() {
		return get_king_bit_board(core::opposite_color(toMove));
	}

	uint64_t Position::get_pawn_bit_board(core::Color color) {
		return color == core::Color::White ? bitBoards[0] : bitBoards[6];
	}

	uint64_t Position::get_knight_bit_board(core::Color color) {
		return color == core::Color::White ? bitBoards[1] : bitBoards[7];
	}

	uint64_t Position::get_bishop_bit_board(core::Color color) {
		return color == core::Color::White ? bitBoards[2] : bitBoards[8];
	}

	uint64_t Position::get_rook_bit_board(core::Color color) {
		return color == core::Color::White ? bitBoards[3] : bitBoards[9];
	}

	uint64_t Position::get_queen_bit_board(core::Color color) {
		return color == core::Color::White ? bitBoards[4] : bitBoards[10];
	}

	uint64_t Position::get_king_bit_board(core::Color color) {
		return color == core::Color::White ? bitBoards[5] : bitBoards[11];
	}

	int Position::piece_count() {
		return piece_count(core::Color::White) + piece_count(core::Color::Black);
	}

	int Position::piece_count(core::Color color) {
		return color == core::Color::White ?
			std::popcount(bitBoards[1]) + std::popcount(bitBoards[2]) +
			std::popcount(bitBoards[3]) + std::popcount(bitBoards[4]) :
			std::popcount(bitBoards[7]) + std::popcount(bitBoards[8]) +
			std::popcount(bitBoards[9]) + std::popcount(bitBoards[10]);
	}

	int Position::num_pawns(core::Color color) {
		return std::popcount(get_pawn_bit_board(color));
	}

	int Position::num_knights(core::Color color) {
		return std::popcount(get_knight_bit_board(color));
	}

	int Position::num_bishops(core::Color color) {
		return std::popcount(get_bishop_bit_board(color));
	}

	int Position::num_rooks(core::Color color) {
		return std::popcount(get_rook_bit_board(color));
	}

	int Position::num_queens(core::Color color) {
		return std::popcount(get_queen_bit_board(color));
	}

	int Position::num_pieces(core::piece::PieceType pieceType, core::Color color) {
		switch (pieceType)
		{
		case core::piece::PieceType::Pawn:
			return num_pawns(color);
		case core::piece::PieceType::Knight:
			return num_knights(color);
		case core::piece::PieceType::Bishop:
			return num_bishops(color);
		case core::piece::PieceType::Rook:
			return num_rooks(color);
		case core::piece::PieceType::Queen:
			return num_queens(color);
		default:
			return -1;
		}
	}

	int Position::piece_diff() {
		return piece_count(core::Color::White) - piece_count(core::Color::Black);
	}

	int Position::pawn_diff() {
		return num_pawns(core::Color::White) - num_pawns(core::Color::Black);
	}

	int Position::knight_diff() {
		return num_knights(core::Color::White) - num_knights(core::Color::Black);
	}

	int Position::bishop_diff() {
		return num_bishops(core::Color::White) - num_bishops(core::Color::Black);
	}

	int Position::rook_diff() {
		return num_rooks(core::Color::White) - num_rooks(core::Color::Black);
	}

	int Position::queen_diff() {
		return num_queens(core::Color::White) - num_queens(core::Color::Black);
	}

	bool Position::is_king_attacked() {
		return Position::is_king_attacked(toMove);
	}

	bool Position::is_king_attacked(core::Color color) {
		int kingSquare = std::countr_zero(Position::get_pawn_bit_board(color));
		return is_square_attacked(kingSquare, color);
	}

	bool Position::is_square_attacked(unsigned int squareIndex, core::Color attacking) {
		int lowBound = core::piece::colored_index(attacking, core::piece::PieceType::Pawn);
		uint64_t squareIndexMap = core::bit_board::square_to_bit_board(squareIndex);

		for (int piece = lowBound; piece < lowBound + 6; piece++) {
			uint64_t attackingPieces = bitBoards[piece];

			while (attackingPieces) {
				int currentIndex = core::bit_board::get_first_square_index(attackingPieces);
				core::piece::PieceType pieceType = static_cast<core::piece::PieceType>(piece);

				uint64_t attackingMask;

				if (pieceType == core::piece::PieceType::Pawn) {
					attackingMask = move::mask::get_pawn_mask(toMove, currentIndex, true);
				}

				else attackingMask = move::mask::lookup(pieceType, currentIndex, allPieces);

				if ((squareIndexMap & attackingMask)) {
					return true;
				}
				
				core::bit_board::remove_first_one(attackingPieces);
			}
		}

		return false;
	}

	bool Position::is_empty_square(unsigned int squareIndex) {
		return pieceIndexAtSquare[squareIndex] != -1;
	}

	bool Position::is_enemy_square(unsigned int squareIndex) {
		switch (toMove)
		{
		case core::Color::White: return pieceIndexAtSquare[squareIndex] > 5;
		case core::Color::Black: return pieceIndexAtSquare[squareIndex] <= 5;
		default: return false;
		}
	}
}
