#pragma once
#include <move_conversion.h>
#include <regex>
#include <token.h>
#include <square.h>

namespace uci {

	static bool is_valid_uci_move(const std::string& move) {
		static const std::regex uci_regex(R"(^[a-h][1-8][a-h][1-8][qrbn]?$)", std::regex_constants::ECMAScript);
		return std::regex_match(move, uci_regex);
	}

	static constexpr core::PieceType char_to_piece_type(const char* c) {
		switch (*c)
		{
		case 'n': return core::PieceType::Knight;
		case 'b': return core::PieceType::Bishop;
		case 'r': return core::PieceType::Rook;
		case 'q': return core::PieceType::Queen;
		default:  return core::PieceType::None;
		}
	}

	static move::CastleType get_castle(const int startSquare, const int endSquare, const core::PieceType& pieceType) {
		if (pieceType != core::PieceType::King) return move::CastleType::None;
		if ((startSquare == 4 && endSquare == 6) || (startSquare == 60 && endSquare == 62)) return move::CastleType::KingSide;
		if ((startSquare == 4 && endSquare == 2) || (startSquare == 60 && endSquare == 58)) return move::CastleType::QueenSide;
		return move::CastleType::None;
	}

	std::optional<move::Move> parse_uci_move(const char* move, core::Position& position) {
		if (!is_valid_uci_move(move)) return std::nullopt;

		const char* startSquareStr = move;
		const char* endSquareStr = move + 2;
		const char* promotionStr = move + 4;

		int startSquare = uci::uci_to_square_index(std::string(startSquareStr, endSquareStr));
		int endSquare = uci::uci_to_square_index(std::string(endSquareStr, promotionStr));
		core::PieceType promotionType = std::strlen(move) == 5 ?
			char_to_piece_type(promotionStr) : core::PieceType::None;

		core::PieceType pieceType = core::Position::index_to_piece_type(position.pieceIndexAtSquare[startSquare]);
		core::PieceType capturedType = core::Position::index_to_piece_type(position.pieceIndexAtSquare[endSquare]);

		move::CastleType castleType = get_castle(startSquare, endSquare, pieceType);

		// Will add en passant support later, and probably either need to change what information is stored in moves,
		// or how to figure out if move notation is en passant by the positional context.
		return move::Move::of(startSquare, endSquare, pieceType, capturedType, promotionType, castleType, false);
	}
}