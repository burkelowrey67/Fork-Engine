#include "pch.h"
#include <uci_handles.h>
#include <iostream>
#include <position/position.h>
#include <uci.h>
#include <optional>
#include <move_conversion.h>

namespace uci {

	void handle_setoption(search::Search& search, std::string& options) {

	}

	void handle_ucinewgame(search::Search& search) {
		search.stop(false);
		search.reset_search_state();
		search.reset_data();
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
		search.stop(true);
	}

	void handle_quit(search::Search& search) {
		search.stop(false);
		exit(0);
	}
}