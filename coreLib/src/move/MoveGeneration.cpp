#include "move/move_generation.h"
#include "position/bit_board.h"
#include <move/movement_masks.h>
#include <vector>

namespace move::generation {
	
	// generate moves from hash_table that maps "best" moves to previously searched positions.
	// implement later

	static void generate_king_side_castle(chess::Position& position, std::vector<uint32_t>& moves) {

		if (
			(position.toMove == chess::Color::White && !position.w_kingSideCastle) ||
			(position.toMove == chess::Color::Black && !position.b_kingSideCastle) ||
			(position.allPieces & 0x60ULL) ||
			(position.allPieces & 0x6000000000000000ULL)
			) {
			return;
		}

		moves.push_back(Move::of(move::CastleType::KingSide));
	}

	static void generate_queen_side_castle(chess::Position& position, std::vector<uint32_t>& moves) {

		if (
			(position.toMove == chess::Color::White && !position.w_queenSideCastle) ||
			(position.toMove == chess::Color::Black && !position.b_queenSideCastle) ||
			(position.allPieces & 0xEUL) ||
			(position.allPieces & 0xE00000000000000ULL)
			) {
			return;
		}

		moves.push_back(Move::of(move::CastleType::KingSide));
	}

	// Generates moves from a movement, given a piece type and a start square. 
	// Assumes non-pawn piece types.
	static void generate_moves_from_movement_mask(uint64_t movementMask, piece::PieceType pieceType, unsigned int startSquare, chess::Position& position, std::vector<uint32_t>& moves) {
		if (pieceType == piece::PieceType::Pawn) return;

		while (movementMask) {
			int endSquare = chess::bit_board::get_first_square_index(movementMask);
			piece::PieceType pieceAtEndSquare = static_cast<piece::PieceType>(position.pieceIndexAtSquare[endSquare]);
			moves.push_back(Move::of(startSquare, endSquare, pieceType, pieceAtEndSquare));
			chess::bit_board::remove_first_one(movementMask);
		}
	}

	static void generate_pawn_moves_from_movement_mask(uint64_t& mask, unsigned int startSquare, chess::Position& position, std::vector<uint32_t>& moves, bool attacking) {
		while (mask) {
			unsigned int endSquare = chess::bit_board::get_first_square_index(mask);

			piece::PieceType capturedType = piece::PieceType::None;

			if (
				int pieceIndex = position.pieceIndexAtSquare[endSquare];
				pieceIndex != -1
				) {
				capturedType = static_cast<piece::PieceType>(pieceIndex);
			}

			if ((chess::bit_board::EIGHTH_RANK & chess::bit_board::square_to_bit_board(endSquare)) != 0) {
				for (int p = 0; static_cast<int>(piece::PieceType::N); p++) {
					moves.push_back(move::Move::of(startSquare, endSquare, capturedType, static_cast<piece::PieceType>(p), false));
					continue;
				}
			}

			else if (position.enPassantSquare == startSquare + 1 && attacking) {
				capturedType = static_cast<piece::PieceType>(position.pieceIndexAtSquare[startSquare + 1]);
				moves.push_back(move::Move::of(startSquare, endSquare, capturedType, piece::PieceType::None, true));
				continue;
			}

			else if (position.enPassantSquare == startSquare - 1 && attacking) {
				capturedType = static_cast<piece::PieceType>(position.pieceIndexAtSquare[startSquare - 1]);
				moves.push_back(move::Move::of(startSquare, endSquare, capturedType, piece::PieceType::None, true));
				continue;
			}

			else moves.push_back(move::Move::of(startSquare, endSquare, piece::PieceType::Pawn, capturedType));
		}
	}

	static void generate_pawn_moves(chess::Position& position, std::vector<uint32_t>& moves) {
		uint64_t pawns = position.bitBoards[piece::pawn_index(position.toMove)];

		while (pawns) {
			int squareIndex = chess::bit_board::get_first_square_index(pawns);
			uint64_t mask = move::masks::get_pawn_mask(position.toMove, squareIndex, false);
			generate_moves_from_movement_mask(mask, piece::PieceType::Pawn, squareIndex, position, moves);

			mask = move::masks::get_pawn_mask(position.toMove, squareIndex, true); 
			mask &= position.enemyPieces;
			generate_moves_from_movement_mask(mask, piece::PieceType::Pawn, squareIndex, position, moves);
			chess::bit_board::remove_first_one(pawns);
		}
	}


	static void generate_piece_moves(piece::PieceType pieceType, chess::Position& position, std::vector<uint32_t>& moves) {
		if (pieceType == piece::PieceType::Pawn) return;
		uint64_t pieces = position.bitBoards[piece::colored_index(position.toMove, pieceType)];

		while (pieces) {
			int squareIndex = chess::bit_board::get_first_square_index(pieces);
			uint64_t mask = move::lookups::lookup(pieceType, squareIndex, position.allPieces);
			generate_moves_from_movement_mask(mask, pieceType, squareIndex, position, moves);
			chess::bit_board::remove_first_one(pieces);
		}
	}

	void generate_pseudolegal_moves(chess::Position& position, std::vector<uint32_t>& moves) {
		generate_pawn_moves(position, moves);
		generate_king_side_castle(position, moves);
		generate_queen_side_castle(position, moves);


		for (int p = 1; static_cast<int>(piece::PieceType::N); p++) {
			generate_piece_moves(static_cast<piece::PieceType>(p), position, moves);
		}
	}
}