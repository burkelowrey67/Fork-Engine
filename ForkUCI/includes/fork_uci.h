#pragma once
#include <position/position.h>
#include <move_search.h>
#include <string>

namespace fork {
	class ForkUCI {
	private:
		core::Position position;
		search::Search search;

	public:
		
		ForkUCI() : position(), search() {}

		void handle_uci_cmd(std::string& cmd);
	};
}