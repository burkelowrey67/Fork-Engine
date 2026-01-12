#pragma once
#include <color.h>

namespace piece {
	enum class PieceType : uint8_t {
		Pawn, Knight, Bishop, Rook, Queen, King, 
		None, N
	};



	inline int pawn_index(chess::Color color) {
		return color == chess::Color::White ? 0 : 6;
	}

	inline int knight_index(chess::Color color) {
		return color == chess::Color::White ? 1 : 7;
	}

	inline int bishop_index(chess::Color color) {
		return color == chess::Color::White ? 2 : 8;
	}

	inline int rook_index(chess::Color color) {
		return color == chess::Color::White ? 3 : 9;
	}

	inline int queen_index(chess::Color color) {
		return color == chess::Color::White ? 4 : 10;
	}

	inline int king_index(chess::Color color) {
		return color == chess::Color::White ? 5 : 11;
	}

	inline int colored_index(chess::Color color, PieceType pieceType) {
		switch (pieceType)
		{
		case piece::PieceType::Pawn:
			return pawn_index(color);
		case piece::PieceType::Knight:
			return knight_index(color);
		case piece::PieceType::Bishop:
			return bishop_index(color);
		case piece::PieceType::Rook:
			return rook_index(color);
		case piece::PieceType::Queen:
			return queen_index(color);
		case piece::PieceType::King:
			return king_index(color);
		default:
			return -1;
		}
	}

	inline int non_colored_index(PieceType pieceType) {
		return colored_index(chess::Color::White, pieceType);
	}
}