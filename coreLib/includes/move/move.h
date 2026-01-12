#pragma once

#include <piece/piece_type.h>
#include <move/castle_type.h>
#include <cstdint>


namespace move {
	
	struct Move {
	public:
		//			 E.P.   castle   promotion   captured   piece   end    start
		//0b00000000_0______00_______000_________000________000_____000000_000000
		//          23      21        18          15         12          6      0
		uint32_t encodedMove;

		Move(uint32_t encodedMove);
		Move();
		~Move();

		static uint32_t of(
			uint8_t startSquare, uint8_t endSqaure,
			piece::PieceType pieceType,
			piece::PieceType capturedType,
			piece::PieceType promotionType,
			move::CastleType castleType, bool isEnPassant
		);

		static uint32_t of(
			uint8_t startSquare, uint8_t endSquare,
			piece::PieceType pieceType, piece::PieceType capturedType
		);

		static uint32_t of(
			uint8_t startSquare, uint8_t endSquare,
			piece::PieceType capturedType, piece::PieceType promotionType,
			bool isEnPassant
		);

		static uint32_t of(uint8_t startSquare, uint8_t endSquare, piece::PieceType pieceType);

		static uint32_t of(move::CastleType);
	};
}