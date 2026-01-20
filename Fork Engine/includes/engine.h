#pragma once
#include <cstdint>
#include <move/move.h>
#include <atomic>
#include <string>
#include <vector>
#include <position/position.h>

namespace fork {

	class Engine {
	private:
		core::Position* position;

	public:

		Engine();
		~Engine();
		
		/*
		* @brief Initializes the engine's internal state.
		*/
		void initialize();

		/*
		* @breif Set's the engine's internal position.
		* @param position: Pointer to new position.
		*/
		void set_position(core::Position* position);

		/**
		 * @brief Starts the search thread that finds the best move.
		 * @param position: Reference to the current chess position to be searched.
		 *                 The position may be temporarily modified during the search
		 *                 but will be restored before the function returns.
		 */
		void go();

		/*
		* @brief Stops the search and returns the best move it has found.
		* @return best move
		*/
		move::Move stop();

		/*
		* @brief Quits the engine.
		*/
		void quit();
	};
}


