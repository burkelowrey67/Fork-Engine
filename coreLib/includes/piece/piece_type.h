#pragma once
#include <color.h>

namespace core::piece {
	enum class PieceType : uint8_t {
		Pawn, Knight, Bishop, Rook, Queen, King, 
		None, N
	};

	/*
	* @brief Returns the pawn's index into a position's bit board array.
	* @param color: Color of pawn.
	* @return index
	*/
	inline int pawn_index(core::Color color) {
		return color == core::Color::White ? 0 : 6;
	}

	/*
	* @brief Returns the knight's index into a position's bit board array.
	* @param color: Color of knight.
	* @return index
	*/
	inline int knight_index(core::Color color) {
		return color == core::Color::White ? 1 : 7;
	}

	/*
	* @brief Returns the bishop's index into a position's bit board array.
	* @param color: Color of bishop.
	* @return index
	*/
	inline int bishop_index(core::Color color) {
		return color == core::Color::White ? 2 : 8;
	}

	/*
	* @brief Returns the rook's index into a position's bit board array.
	* @param color: Color of rook.
	* @return index
	*/
	inline int rook_index(core::Color color) {
		return color == core::Color::White ? 3 : 9;
	}

	/*
	* @brief Returns the queen's index into a position's bit board array.
	* @param color: Color of queen.
	* @return index
	*/
	inline int queen_index(core::Color color) {
		return color == core::Color::White ? 4 : 10;
	}

	/*
	* @brief Returns the pawn's index into a position's bit board array.
	* @param color: Color of king.
	* @return index
	*/
	inline int king_index(core::Color color) {
		return color == core::Color::White ? 5 : 11;
	}


	/*
	* @brief Returns a piece's index into a position's bit board array.
	* @param color: Color of piece.
	* @return index
	*/
	inline int colored_index(core::Color color, PieceType pieceType) {
		switch (pieceType)
		{
		case core::piece::PieceType::Pawn:
			return pawn_index(color);
		case core::piece::PieceType::Knight:
			return knight_index(color);
		case core::piece::PieceType::Bishop:
			return bishop_index(color);
		case core::piece::PieceType::Rook:
			return rook_index(color);
		case core::piece::PieceType::Queen:
			return queen_index(color);
		case core::piece::PieceType::King:
			return king_index(color);
		default:
			return -1;
		}
	}

	inline int non_colored_index(PieceType pieceType) {
		return colored_index(core::Color::White, pieceType);
	}
}