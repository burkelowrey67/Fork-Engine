// searchLib.cpp : Defines the functions for the static library.


#include "pch.h"
#include <move_search.h>
#include <search_info.h>
#include <move/move_generation.h>
#include <move/move_application.h>
#include <eval.h>
#include <vector>
#include <cfloat>

namespace search {
    

    void Search::go(core::Position& position) {
        // Stop previous search if still running
        if (searchThread.joinable()) {
            searchThread.request_stop();
            searchThread.join();
        }

        searchThread = std::jthread([&position, this](std::stop_token st) {
            std::vector<uint32_t> moves;
            moves.reserve(256);
            move::generate_pseudolegal_moves(position, moves);

            {
                std::lock_guard<std::mutex> lock(infoMutex);
                info.depth = 1;
            }

            double bestScore = -DBL_MAX;
            // evaluate positions after moves are applied
            for (uint32_t move : moves) {
                if (st.stop_requested()) break;

                core::Position nextPosition = move::next_position(position, move);
                double score = position::evaluation::eval(nextPosition);
                // update best move if it gives a higher evaluation
                if (score > bestScore) {
                    bestScore = score;

                    std::lock_guard<std::mutex> lock(infoMutex);
                    info.bestMove = move;
                }
            }
            });
    }

    void Search::stop() {
        if (searchThread.joinable()) searchThread.request_stop();
    }

    search::SearchInfo Search::get_info() {
        std::lock_guard<std::mutex> lock(infoMutex);
        return info;
    }
}

