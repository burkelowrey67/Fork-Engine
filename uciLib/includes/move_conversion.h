#pragma once
#include "pch.h"
#include <move/move.h>
#include <position/position.h>
#include <optional>
#include <string>

namespace uci {

	/*
	* @brief Parses long algebraic coordinate notation and returns a move::Move object.
	* @param move: The move notation.
	* @param position: The position context.
	* @return move
	*/
	std::optional<move::Move> parse_uci_move(const std::string& move, const core::Position& position);

	/*
	* @brief Formats a move::Move object into long algebraic coordinate notation.
	* @param move: The move object.
	* @return notation
	*/
	std::optional<std::string> format_uci_move(move::Move move);
}