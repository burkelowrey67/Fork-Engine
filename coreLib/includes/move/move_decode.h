#pragma once
#include <cstdint>
#include <piece/piece_type.h>
#include <move/castle_type.h>

namespace move::decode {

	inline int start_square(uint32_t& move) {
		return move & 0x3F;
	}

	inline int end_square(uint32_t& move) {
		return (move >> 6) & 0x3F;
	}

	inline piece::PieceType piece_type(uint32_t& move) {
		return static_cast<piece::PieceType>((move >> 12) & 0x3);
	}

	inline piece::PieceType captured_type(uint32_t& move) {
		return static_cast<piece::PieceType>((move >> 15) & 0x3);
	}

	inline piece::PieceType promotion_type(uint32_t& move) {
		return static_cast<piece::PieceType>((move >> 18) & 0x3);
	}

	inline move::CastleType castle_type(uint32_t& move) {
		return static_cast<move::CastleType>((move >> 21) & 0x3);
	}

	inline bool is_en_passant(uint32_t& move) {
		return (move >> 23);
	}
}