#include "pch.h"
#include <uci_handles.h>
#include <iostream>
#include <position/position.h>
#include <uci.h>
#include <optional>
#include <move_conversion.h>

namespace uci {

	void handle_uci() {
		std::cout <<
			"id name ForkEngine\n" <<
			"id author Burke Lowrey\n" <<
			"uciok\n" <<
			std::flush;
	}

	void handle_isready() {
		std::cout << "isreadyok\n" << std::flush;
	}

	void handle_setoption(search::Search& search, std::string& options) {

	}

	void handle_ucinewgame(search::Search& search) {
		search.stop();
		search.reset();
	}

	void handle_position(core::Position& out, const std::string& params) {
		if (auto position = parse_position_cmd(params.data())) out = *position;
	}

	void handle_go(search::Search& search, core::Position& position, const std::string& params) {
		search::SearchLimits searchLimits = search::SearchLimits();
		parse_go_cmd(searchLimits, params.data());
		search.go(position, searchLimits);
	}

	void handle_stop(search::Search& search) {
		search.stop();

		if (std::optional<move::Move> bestMove = search.get_info().bestMove; bestMove.has_value()) {
			if (std::optional<std::string> moveStr = uci::format_uci_move(*bestMove); moveStr.has_value()) {
				std::cout << "bestmove " << *moveStr << "\n" << std::flush;
			}
			else {
				std::cout << "bestmove " << 0000 << "\n" << std::flush;
			}
		}
	}

	void handle_quit(search::Search& search) {
		search.stop();
		exit(0);
	}
}