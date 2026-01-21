#pragma once
#include "pch.h"
#include <move/move.h>
#include <position/position.h>
#include <optional>

namespace uci {

	/*
	* @brief Parses algebraic chess notation and returns a move::Move object.
	* @param move: The move notation.
	* @param position: The position context.
	* @return move
	*/
	std::optional<move::Move> parse_uci_move(const std::string&, core::Position& position);
}