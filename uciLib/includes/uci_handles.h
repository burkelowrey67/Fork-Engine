#pragma once
#include <move_search.h>

namespace uci {

	void handle_setoption(search::Search& search, std::string& options);

	void handle_ucinewgame(search::Search& search);

	void handle_position(core::Position& out, const std::string& params);

	void handle_go(search::Search& search, core::Position& position, const std::string& params);

	void handle_stop(search::Search& search);

	void handle_quit(search::Search& search);
}