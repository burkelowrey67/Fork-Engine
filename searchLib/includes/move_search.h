#pragma once

#include <move/move.h>
#include <position/position.h>
#include <search_info.h>
#include <atomic>
#include <thread>
#include <mutex>


namespace search {

	class Search {
	private:
		SearchInfo info;
		std::mutex infoMutex;
		std::jthread searchThread;

	public:
		/*
		 * @brief Searches the current position and updates the best move.
		 * @param position: Reference to the current chess position to be searched.
		 *                 The position may be temporarily modified during the search
		 *                 but will be restored before the function returns.
		 */
		void go(core::Position& position);

		/*
		* @brief Stops the search and returns the best move it has found.
		* @return best move
		*/
		void stop();

		/*
		* @breif Returns the search info in a struct.
		* @return search info
		*/
		search::SearchInfo get_info();
	};
}