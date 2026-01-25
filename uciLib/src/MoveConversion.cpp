#pragma once
#include "pch.h"
#include <move_conversion.h>
#include <regex>
#include <token.h>
#include <square.h>
#include <move/move_decode.h>
#include <optional>

namespace uci {

	static const std::regex uci_regex(R"(^[a-h][1-8][a-h][1-8][qrbn]?$)", std::regex_constants::ECMAScript);

	static bool is_valid_uci_move(const std::string& move) {
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

	static constexpr char piece_type_to_char(const core::PieceType pieceType) {
		switch (pieceType) 
		{
		case core::PieceType::Knight: return 'n';
		case core::PieceType::Bishop: return 'b';
		case core::PieceType::Rook: return 'r';
		case core::PieceType::Queen: return 'q';
		default: return '?';
		}
	}

	static move::CastleType get_castle(const int startSquare, const int endSquare, const core::PieceType& pieceType) {
		if (pieceType != core::PieceType::King) return move::CastleType::None;
		if ((startSquare == 4 && endSquare == 6) || (startSquare == 60 && endSquare == 62)) return move::CastleType::KingSide;
		if ((startSquare == 4 && endSquare == 2) || (startSquare == 60 && endSquare == 58)) return move::CastleType::QueenSide;
		return move::CastleType::None;
	}

	std::optional<move::Move> parse_uci_move(const std::string& move, const core::Position& position) {
		if (!is_valid_uci_move(move)) return std::nullopt;

		const char* start = move.data();

		const char* startSquareStr = start;
		const char* endSquareStr = start + 2;
		const char* promotionStr = start + 4;

		int startSquare = uci::uci_to_square_index(std::string(startSquareStr, endSquareStr));
		int endSquare = uci::uci_to_square_index(std::string(endSquareStr, promotionStr));
		core::PieceType promotionType = move.size() == 5 ?
			char_to_piece_type(promotionStr) : core::PieceType::None;

		core::PieceType pieceType = core::Position::index_to_piece_type(position.pieceIndexAtSquare[startSquare]);
		core::PieceType capturedType = core::Position::index_to_piece_type(position.pieceIndexAtSquare[endSquare]);

		move::CastleType castleType = get_castle(startSquare, endSquare, pieceType);

		// Will add en passant support later, and probably either need to change what information is stored in moves,
		// or how to figure out if move notation is en passant by the positional context.
		return move::Move::of(startSquare, endSquare, pieceType, capturedType, promotionType, castleType, false);
	}

	std::optional<std::string> format_uci_move(move::Move move) {
		std::string uciMove = std::string();

		core::PieceType promotionType = move::decode::promotion_type(move.encodedMove);

		std::optional<std::string> startSquare = square_index_to_uci(move::decode::start_square(move.encodedMove));
		std::optional<std::string> endSquare = square_index_to_uci(move::decode::end_square(move.encodedMove));

		if (!startSquare.has_value() || !endSquare.has_value()) return std::nullopt;

		uciMove += *startSquare + *endSquare;

		if (char promotionChar = piece_type_to_char(promotionType); promotionChar != '?') uciMove += promotionChar;

		return uciMove;
	}
}