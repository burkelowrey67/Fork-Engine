#include <move/move_application.h>
#include <position/bit_board.h>
#include <color.h>
#include <move/move_decode.h>

namespace move {

	static void apply_castle(core::Position& position, const uint32_t move) {

		unsigned int kingStartIndex;
		unsigned int kingEndIndex;
		unsigned int rookStartIndex;
		unsigned int rookEndIndex;

		switch (position.toMove)
		{
		case core::Color::White:
			kingStartIndex =	core::bit_board::DEFAULT_W_KING_INDEX;
			kingEndIndex =		core::bit_board::POST_W_Q_CASTLE_KING_INDEX;
			rookStartIndex =	core::bit_board::PRE_W_Q_CASTLE_ROOK_INDEX;
			rookEndIndex =		core::bit_board::POST_W_Q_CASTLE_ROOK_INDEX;
			break;
		case core::Color::Black:
			kingStartIndex =	core::bit_board::DEFAULT_B_KING_INDEX;
			kingEndIndex =		core::bit_board::POST_B_Q_CASTLE_KING_INDEX;
			rookStartIndex =	core::bit_board::PRE_B_Q_CASTLE_ROOK_INDEX;
			rookEndIndex =		core::bit_board::POST_B_Q_CASTLE_ROOK_INDEX;
			break;
		default: kingStartIndex = 0; kingEndIndex = 0; rookStartIndex = 0; rookEndIndex = 0;
		}

		core::bit_board::move_bit(position.get_friendly_king_bit_board_ref(), kingStartIndex, kingEndIndex);
		core::bit_board::move_bit(position.get_friendly_rook_bit_board_ref(), rookStartIndex, rookEndIndex);
	}

	static void apply_en_passant(core::Position& position, const uint32_t move) {
		core::bit_board::set_bit_zero(
			position.bitBoards[position.pieceIndexAtSquare[position.enPassantSquare]], 
			position.enPassantSquare
		);

		core::bit_board::move_bit(
			position.bitBoards[core::Position::colored_index(position.toMove, move::decode::piece_type(move))],
			move::decode::start_square(move),
			move::decode::end_square(move)
		);
	}

	static void apply_promotion(core::Position& position, const uint32_t move) {
		core::bit_board::set_bit_zero(
			position.bitBoards[core::Position::colored_index(position.toMove, move::decode::piece_type(move))],
			move::decode::start_square(move)
		);

		core::bit_board::set_bit_one(
			position.bitBoards[core::Position::colored_index(position.toMove, move::decode::promotion_type(move))],
			move::decode::end_square(move)
		);

		if (move::decode::captured_type(move) != core::PieceType::None) {
			core::bit_board::set_bit_zero(
				position.bitBoards[core::Position::colored_index(position.toMove, move::decode::captured_type(move))],
				move::decode::end_square(move)
			);
		}
	}

	static void apply_normal_move(core::Position& position, const uint32_t move) {
		core::bit_board::move_bit(
			position.get_bit_board_ref(move::decode::piece_type(move), position.toMove),
			move::decode::start_square(move),
			move::decode::end_square(move)
		);

		if (core::PieceType captured = move::decode::captured_type(move); captured != core::PieceType::None) {
			core::bit_board::set_bit_zero(
				position.get_bit_board_ref(captured, core::opposite_color(position.toMove)),
				move::decode::end_square(move)
			);
		}
	}

	core::Position next_position(core::Position position, const uint32_t move) {
		core::Position copy = core::Position(position);
		apply_move(copy, move);
		return copy;
	}

	core::Position next_position(core::Position& position, const move::Move move) {
		return next_position(position, move.encodedMove);
	}

	void apply_move(core::Position& position, const move::Move move) {
		apply_move(position, move.encodedMove);
	}

	void apply_move(core::Position& position, const uint32_t move) {

		if (move::decode::castle_type(move) != move::CastleType::None) {
			apply_castle(position, move);
			position.enPassantSquare = -1;
		}

		else if (move::decode::promotion_type(move) != core::PieceType::None) {
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
				move::decode::piece_type(move) == core::PieceType::Pawn &&
				endSquare - move::decode::start_square(move) == 18
				) position.enPassantSquare = endSquare;

			else position.enPassantSquare = -1;
		}

		position.toMove = core::opposite_color(position.toMove);
		position.update_masks();
		position.update_lookup();
	}
}