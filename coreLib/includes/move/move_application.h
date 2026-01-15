#pragma once

#include <position/position.h>
#include <move/move.h>

namespace move {

	/*
	* @brief Returns a position copy with the applied move.
	* @param position:	Chess position.
	* @param move:		Move object that is applied to the position.
	* @return position
	*/
	chess::Position next_position(chess::Position, const move::Move&);

	/*
	* @brief Applies a move to a position.
	* @param position:	Chess position.
	* @param move:		Encoded move that is applied to the position.
	*/
	void apply_move(chess::Position&, const uint32_t& move);

	/*
	* @brief Applies a move to a position.
	* @param position:	Chess position.
	* @param move:		Move object that is applied to the position.
	*/
	void apply_move(chess::Position&, const move::Move&);
}