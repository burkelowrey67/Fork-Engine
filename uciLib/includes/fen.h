#pragma once
#include <position/position.h>
#include <string>
#include <unordered_map>

namespace uci::fen {

	/**
	 * @brief Parses a FEN string and constructs a chess position.
	 *
	 * Converts a Forsyth–Edwards Notation (FEN) string into a
	 * @ref core::Position object representing the same board state,
	 * including piece placement, side to move, castling rights,
	 * en passant target square, halfmove clock, and fullmove number.
	 * 
	 * Note: Behavior is undefined if the FEN string is malformed.
	 *
	 * @param fen: A valid FEN string describing a chess position.
	 * @return core::Position The position represented by the FEN string.
	 *	
	 */
	std::optional<core::Position> parse(const std::string& fen);

	/**
	 * @brief Formats a chess position as a FEN string.
	 *
	 * Serializes the given @ref core::Position into a valid
	 * Forsyth–Edwards Notation (FEN) string that fully describes
	 * the current game state.
	 *
	 * @param position The chess position to be formatted.
	 *
	 * @return std::string A FEN string representing the position.
	 */
	std::string format(core::Position& position);
}