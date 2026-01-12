#include <move/move.h>
#include <piece/piece_type.h>
#include <move/castle_type.h>

namespace move {

	// encoded move format:
	//             EP  Castle  PromPiece  CapPiece     Piece  Sq2     Sq1
	// 0b00000000_|0___00______0|00_______00|0_________000|___0000|00_00|0000
	//            23   21        18         15          12          6       0

	Move::Move(uint32_t _encodedMove) : encodedMove(_encodedMove) { }
	Move::Move() : encodedMove(0) { }
	Move::~Move() = default;
	
	uint32_t Move::of(
		uint8_t startSquare, uint8_t endSquare,
		piece::PieceType pieceType,
		piece::PieceType capturedType,
		piece::PieceType promotionType,
		CastleType castleType, bool isEnPassant
	) {
			
		return
			startSquare | endSquare << 6 |
			(int)pieceType << 12 | (int)capturedType << 15 | (int)promotionType << 18 |
			(int)castleType << 21 | isEnPassant << 23;
	}

	uint32_t Move::of(
		uint8_t startSquare, uint8_t endSquare, piece::PieceType pieceType, piece::PieceType capturedType
	) {
		return of(startSquare, endSquare, pieceType, capturedType, piece::PieceType::None, CastleType::None, false);
	}

	uint32_t Move::of(uint8_t startSquare, uint8_t endSquare, piece::PieceType pieceType) {
		return of(startSquare, endSquare, pieceType, piece::PieceType::None, piece::PieceType::None, CastleType::None, false);
	}

	uint32_t Move::of(uint8_t startSquare, uint8_t endSquare, piece::PieceType capturedType, piece::PieceType promotionType, bool isEnPassant) {
		return of(startSquare, endSquare, piece::PieceType::Pawn, capturedType, promotionType, CastleType::None, isEnPassant);
	}

	uint32_t Move::of(move::CastleType castleType) {
		return of(0, 0, piece::PieceType::None, piece::PieceType::None, piece::PieceType::None, castleType, false);
	}
}