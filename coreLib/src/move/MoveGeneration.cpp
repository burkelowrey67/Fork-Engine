#include "move/move_generation.h"
#include "position/bit_board.h"
#include <move/movement_masks.h>
#include <vector>

namespace move {
	
	// generate moves from hash_table that maps "best" moves to previously searched positions.
	// implement later

	static void generate_king_side_castle(core::Position& position, std::vector<uint32_t>& moves) {

		if (
			(position.toMove == core::Color::White && !position.w_kingSideCastle) ||
			(position.toMove == core::Color::Black && !position.b_kingSideCastle) ||
			(position.allPieces & 0x60ULL) ||
			(position.allPieces & 0x6000000000000000ULL)
			) {
			return;
		}

		moves.push_back(Move::of(move::CastleType::KingSide));
	}

	static void generate_queen_side_castle(core::Position& position, std::vector<uint32_t>& moves) {

		if (
			(position.toMove == core::Color::White && !position.w_queenSideCastle) ||
			(position.toMove == core::Color::Black && !position.b_queenSideCastle) ||
			(position.allPieces & 0xEUL) ||
			(position.allPieces & 0xE00000000000000ULL)
			) {
			return;
		}

		moves.push_back(Move::of(move::CastleType::KingSide));
	}

	// Generates moves from a movement, given a piece type and a start square. 
	// Assumes non-pawn piece types.
	static void generate_moves_from_movement_mask(uint64_t movementMask, core::PieceType pieceType, unsigned int startSquare, core::Position& position, std::vector<uint32_t>& moves) {
		if (pieceType == core::PieceType::Pawn) return;

		while (movementMask) {
			int endSquare = core::bit_board::get_first_square_index(movementMask);
			core::PieceType pieceAtEndSquare = static_cast<core::PieceType>(position.pieceIndexAtSquare[endSquare]);
			moves.push_back(Move::of(startSquare, endSquare, pieceType, pieceAtEndSquare));
			core::bit_board::remove_first_one(movementMask);
		}
	}

	static void generate_pawn_moves_from_movement_mask(uint64_t& mask, unsigned int startSquare, core::Position& position, std::vector<uint32_t>& moves, bool attacking) {
		while (mask) {
			unsigned int endSquare = core::bit_board::get_first_square_index(mask);

			core::PieceType capturedType = core::PieceType::None;

			if (
				int pieceIndex = position.pieceIndexAtSquare[endSquare];
				pieceIndex != -1
				) {
				capturedType = static_cast<core::PieceType>(pieceIndex);
			}

			if ((core::bit_board::EIGHTH_RANK & core::bit_board::square_to_bit_board(endSquare)) != 0) {
				for (int p = 0; static_cast<int>(core::PieceType::N); p++) {
					moves.push_back(move::Move::of(startSquare, endSquare, capturedType, static_cast<core::PieceType>(p), false));
					continue;
				}
			}

			else if (position.enPassantSquare == startSquare + 1 && attacking) {
				capturedType = static_cast<core::PieceType>(position.pieceIndexAtSquare[startSquare + 1]);
				moves.push_back(move::Move::of(startSquare, endSquare, capturedType, core::PieceType::None, true));
				continue;
			}

			else if (position.enPassantSquare == startSquare - 1 && attacking) {
				capturedType = static_cast<core::PieceType>(position.pieceIndexAtSquare[startSquare - 1]);
				moves.push_back(move::Move::of(startSquare, endSquare, capturedType, core::PieceType::None, true));
				continue;
			}

			else moves.push_back(move::Move::of(startSquare, endSquare, core::PieceType::Pawn, capturedType));
		}
	}

	static void generate_pawn_moves(core::Position& position, std::vector<uint32_t>& moves) {
		uint64_t pawns = position.bitBoards[core::Position::pawn_index(position.toMove)];

		while (pawns) {
			int squareIndex = core::bit_board::get_first_square_index(pawns);
			uint64_t mask = move::mask::get_pawn_mask(position.toMove, squareIndex, false);
			generate_moves_from_movement_mask(mask, core::PieceType::Pawn, squareIndex, position, moves);

			mask = move::mask::get_pawn_mask(position.toMove, squareIndex, true); 
			mask &= position.enemyPieces;
			generate_moves_from_movement_mask(mask, core::PieceType::Pawn, squareIndex, position, moves);
			core::bit_board::remove_first_one(pawns);
		}
	}


	static void generate_piece_moves(core::PieceType pieceType, core::Position& position, std::vector<uint32_t>& moves) {
		if (pieceType == core::PieceType::Pawn) return;
		uint64_t pieces = position.bitBoards[core::Position::colored_index(position.toMove, pieceType)];

		while (pieces) {
			int squareIndex = core::bit_board::get_first_square_index(pieces);
			uint64_t mask = move::mask::lookup(pieceType, squareIndex, position.allPieces);
			generate_moves_from_movement_mask(mask, pieceType, squareIndex, position, moves);
			core::bit_board::remove_first_one(pieces);
		}
	}

	void generate_pseudolegal_moves(core::Position& position, std::vector<uint32_t>& moves) {
		generate_pawn_moves(position, moves);
		generate_king_side_castle(position, moves);
		generate_queen_side_castle(position, moves);


		for (int p = 1; static_cast<int>(core::PieceType::N); p++) {
			generate_piece_moves(static_cast<core::PieceType>(p), position, moves);
		}
	}
}