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

		/*
		* @brief Encodes a move into an unsigned integer given move-related information.
		* @param startSquare:	Origin square.
		* @param endSquare:		Destination square.
		* @param pieceType:		Piece type of the movee.
		* @param capturedType:	Piece type the movee captures.
		* @param promotionType: Piece type the pawn (if a pawn) promotes to.
		* @param castleType:	Castle type (king side or queen side).
		* @param isEnPassant:	True if move is en passant.
		* @return unsigned 32-bit integer
		*/
		static uint32_t of(
			uint8_t startSquare, uint8_t endSqaure,
			core::PieceType pieceType,
			core::PieceType capturedType,
			core::PieceType promotionType,
			move::CastleType castleType, bool isEnPassant
		);

		/*
		* @brief Used for moves that capture.
		*/
		static uint32_t of(
			uint8_t startSquare, uint8_t endSquare,
			core::PieceType pieceType, core::PieceType capturedType
		);

		/*
		* @brief Used for pawn moves.
		*/
		static uint32_t of(
			uint8_t startSquare, uint8_t endSquare,
			core::PieceType capturedType, core::PieceType promotionType,
			bool isEnPassant
		);

		/*
		* Used for quiet moves.
		*/
		static uint32_t of(uint8_t startSquare, uint8_t endSquare, core::PieceType pieceType);

		/*
		* Used for castles.
		*/
		static uint32_t of(move::CastleType);
	};
}