#include <move/move_application.h>
#include <position/bit_board.h>
#include <color.h>
#include <move/move_decode.h>

namespace move::move_application {



	static void apply_castle(chess::Position& position, uint32_t& move) {

		unsigned int kingStartIndex;
		unsigned int kingEndIndex;
		unsigned int rookStartIndex;
		unsigned int rookEndIndex;

		switch (position.toMove)
		{
		case chess::Color::White:
			kingStartIndex =	chess::bit_board::DEFAULT_W_KING_INDEX;
			kingEndIndex =		chess::bit_board::POST_W_Q_CASTLE_KING_INDEX;
			rookStartIndex =	chess::bit_board::PRE_W_Q_CASTLE_ROOK_INDEX;
			rookEndIndex =		chess::bit_board::POST_W_Q_CASTLE_ROOK_INDEX;
			break;
		case chess::Color::Black:
			kingStartIndex =	chess::bit_board::DEFAULT_B_KING_INDEX;
			kingEndIndex =		chess::bit_board::POST_B_Q_CASTLE_KING_INDEX;
			rookStartIndex =	chess::bit_board::PRE_B_Q_CASTLE_ROOK_INDEX;
			rookEndIndex =		chess::bit_board::POST_B_Q_CASTLE_ROOK_INDEX;
			break;
		default: kingStartIndex = 0; kingEndIndex = 0; rookStartIndex = 0; rookEndIndex = 0;
		}

		chess::bit_board::move_bit(position.get_friendly_king_bit_board_ref(), kingStartIndex, kingEndIndex);
		chess::bit_board::move_bit(position.get_friendly_rook_bit_board_ref(), rookStartIndex, rookEndIndex);
	}

	static void apply_en_passant(chess::Position& position, uint32_t& move) {
		chess::bit_board::set_bit_zero(
			position.bitBoards[position.pieceIndexAtSquare[position.enPassantSquare]], 
			position.enPassantSquare
		);

		chess::bit_board::move_bit(
			position.bitBoards[piece::colored_index(position.toMove, move::decode::piece_type(move))],
			move::decode::start_square(move),
			move::decode::end_square(move)
		);
	}

	static void apply_promotion(chess::Position& position, uint32_t& move) {
		chess::bit_board::set_bit_zero(
			position.bitBoards[piece::colored_index(position.toMove, move::decode::piece_type(move))],
			move::decode::start_square(move)
		);

		chess::bit_board::set_bit_one(
			position.bitBoards[piece::colored_index(position.toMove, move::decode::promotion_type(move))],
			move::decode::end_square(move)
		);

		if (move::decode::captured_type(move) != piece::PieceType::None) {
			chess::bit_board::set_bit_zero(
				position.bitBoards[piece::colored_index(position.toMove, move::decode::captured_type(move))],
				move::decode::end_square(move)
			);
		}
	}

	static void apply_normal_move(chess::Position& position, uint32_t& move) {
		chess::bit_board::move_bit(
			position.get_bit_board_ref(move::decode::piece_type(move), position.toMove),
			move::decode::start_square(move),
			move::decode::end_square(move)
		);

		if (move::decode::captured_type(move) != piece::PieceType::None) {
			chess::bit_board::set_bit_zero(
				position.bitBoards[piece::colored_index(position.toMove, move::decode::captured_type(move))],
				move::decode::end_square(move)
			);
		}
	}

	chess::Position get_next(chess::Position position, move::Move& move) {
		chess::Position copy = chess::Position(position);
		apply_move(copy, move);
		return copy;
	}

	void apply_move(chess::Position& position, move::Move& move) {
		apply_move(position, move.encodedMove);
	}

	void apply_move(chess::Position& position, uint32_t& move) {

		if (move::decode::castle_type(move) != move::CastleType::None) {
			apply_castle(position, move);
			position.enPassantSquare = -1;
		}

		else if (move::decode::promotion_type(move) != piece::PieceType::None) {
			apply_promotion(position, move);
			position.enPassantSquare = -1;
		}

		else if (move::decode::is_en_passant(move)) {
			apply_en_passant(position, move);
			position.enPassantSquare = -1;
		}

		else {
			apply_normal_move(position, move);

			if (
				int endSquare = move::decode::end_square(move);
				move::decode::piece_type(move) == piece::PieceType::Pawn &&
				endSquare - move::decode::start_square(move) == 18
				) position.enPassantSquare = endSquare;

			else position.enPassantSquare = -1;
		}

		position.toMove = chess::opposite_color(position.toMove);
	}
}