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
	core::Position next_position(core::Position, const move::Move);

	/*
	*@brief Returns a position copy with the applied move.
	* @param position : Chess position.
	* @param move : Move object that is applied to the position.
	* @return position
	*/
	core::Position next_position(core::Position, const uint32_t move);

	/*
	* @brief Applies a move to a position.
	* @param position:	Chess position.
	* @param move:		Encoded move that is applied to the position.
	*/
	void apply_move(core::Position&, const uint32_t move);

	/*
	* @brief Applies a move to a position.
	* @param position:	Chess position.
	* @param move:		Move object that is applied to the position.
	*/
	void apply_move(core::Position&, const move::Move);
}