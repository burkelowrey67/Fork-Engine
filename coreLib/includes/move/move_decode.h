#pragma once
#include <cstdint>
#include <piece/piece_type.h>
#include <move/castle_type.h>

namespace move::decode {

	/*
	* @brief Decodes a move integer's start square.
	* @param move: Encoded unsigned 32-bit move integer.
	* @return start square index
	*/
	inline int start_square(uint32_t& move) {
		return move & 0x3F;
	}

	/*
	* @brief Decodes a move integer's end square.
	* @param move: Encoded unsigned 32-bit move integer.
	* @return end square index
	*/
	inline int end_square(uint32_t& move) {
		return (move >> 6) & 0x3F;
	}

	/*
	* @brief Decodes a move integer's movee.
	* @param move: Encoded unsigned 32-bit move integer.
	* @return movee piece type
	*/
	inline piece::PieceType piece_type(uint32_t& move) {
		return static_cast<piece::PieceType>((move >> 12) & 0x3);
	}

	/*
	* @brief Decodes a move integer's captured piece.
	* @param move: Encoded unsigned 32-bit move integer.
	* @return captured piece type
	*/
	inline piece::PieceType captured_type(uint32_t& move) {
		return static_cast<piece::PieceType>((move >> 15) & 0x3);
	}

	/*
	* @brief Decodes a move integer's promotion piece.
	* @param move: Encoded unsigned 32-bit move integer.
	* @return promotion piece type
	*/
	inline piece::PieceType promotion_type(uint32_t& move) {
		return static_cast<piece::PieceType>((move >> 18) & 0x3);
	}

	/*
	* @brief Decodes a move integer's castling type.
	* @param move: Encoded unsigned 32-bit move integer.
	* @return castle type (king side or queen side)
	*/
	inline move::CastleType castle_type(uint32_t& move) {
		return static_cast<move::CastleType>((move >> 21) & 0x3);
	}

	/*
	* @brief Decodes, then returns true if a move integer is en passant.
	* @param move: Encoded unsigned 32-bit move integer.
	* @return bool
	*/
	inline bool is_en_passant(uint32_t& move) {
		return (move >> 23);
	}
}