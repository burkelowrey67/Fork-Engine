#pragma once

#include <move/move.h>
#include <position/position.h>
#include <search_info.h>
#include <atomic>
#include <mutex>
#include <search_limits.h>
#include <search_data.h>


namespace search {

	class Search {

	public:
		std::mutex m;
		std::condition_variable cv;
		SearchData data;

	private:

		std::jthread searchThread;

		void reset_search_info();
		void reset_search_done();

	public:

		Search() : data(), searchThread() {}

		/*
		 * @brief Searches the current position and updates the best move.
		 * @param position: Reference to the current chess position to be searched.
		 *                 The position may be temporarily modified during the search
		 *                 but will be restored before the function returns.
		 * @param searchLimits: Reference to a search limit struct that defines bounds to the search.
		 */
		void go(core::Position& position, const	SearchLimits& searchLimits);

		/*
		* @brief Stops the search and returns the best move it has found.
		* @param notifyListeners: True if notifying search listeners is desired.
		*/
		void stop(bool notifyListeners);

		/*
		* @breif Returns the search info in a struct.
		* @return search info
		*/
		search::SearchInfo get_info();

		/*
		* @brief Resets search state. Required if you want to restart the search.
		*/
		void reset_search_state();

		/*
		* @brief Resets search data.
		*/
		void reset_data();
	};
}	