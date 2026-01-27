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


    void Search::go(core::Position& position, const SearchLimits& searchLimits) {

        // Stop previous search if still running
        if (searchThread.joinable()) {
            searchThread.request_stop();
            searchThread.join();
        }

        searchThread = std::jthread([&position, searchLimits, this](std::stop_token st) {
            std::vector<uint32_t> moves;
            moves.reserve(256);
            move::generate_pseudolegal_moves(position, moves);

            {
                std::lock_guard<std::mutex> lock(m);
                data.info.depth = 1;
                data.info.eval = 0;
                data.info.nodesVisited = 0;
                ++data.infoVersion;
            }

            bool white = position.toMove == core::Color::White;
            double bestScore = white ? -DBL_MAX : DBL_MAX;

            // evaluate positions after moves are applied
            for (uint32_t move : moves) {

                std::unique_lock<std::mutex> lock(m);
                if (st.stop_requested() ||
                        data.info.nodesVisited >= searchLimits.nodes ||
                        (searchLimits.depth.has_value() && data.info.depth >= *searchLimits.depth)) break;
                lock.unlock();

                core::Position nextPosition = move::next_position(position, move);
                if (nextPosition.is_enemy_king_attacked()) continue;
                double score = position::evaluation::eval(nextPosition);

                // update best move if it gives a higher evaluation
                if (bool white = position.toMove == core::Color::White;
                    (white && score > bestScore) ||
                    (!white && score < bestScore)) {

                    bestScore = score;
                    std::lock_guard<std::mutex> lock(m);
                    data.info.bestMove = move;
                    data.info.eval = score;
                }

                lock.lock();
                data.info.nodesVisited = *data.info.nodesVisited + 1;
                ++data.infoVersion;
                lock.unlock();
            }

            {
                std::lock_guard<std::mutex> lock(m);
                data.searchDone = true;
            }
            cv.notify_one();
            });
    }

    void Search::stop(bool notifyListeners) {
        if (searchThread.joinable()) searchThread.request_stop();
        
        if (notifyListeners)  {
            std::lock_guard<std::mutex> lock(m);
            data.searchDone = true;
            cv.notify_one();
        }
    }

    search::SearchInfo Search::get_info() {
        std::lock_guard<std::mutex> lock(m);
        return data.info;
    }

    void Search::reset_search_state() {
    }

    void Search::reset_data() {
        reset_search_info();
        reset_search_done();
    }

    void Search::reset_search_info() {
        std::lock_guard<std::mutex> lock(m);
        data.info.bestMove = std::nullopt;
        data.info.depth = std::nullopt;
        data.info.eval = std::nullopt;
        data.info.nodesVisited = std::nullopt;
    }

    void Search::reset_search_done() {
        std::lock_guard<std::mutex> lock(m);
        data.searchDone = false;
    }
}

