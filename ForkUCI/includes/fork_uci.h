#pragma once
#include <position/position.h>
#include <move_search.h>
#include <string>

namespace fork {
	class ForkUCI {
	private:
		core::Position position;
		search::Search search;
		std::jthread searchEventsThread;

		void start_search_events_jthread();

		void out(const char* out, bool flush);

		void log(const char* message, const char* tag);
		void log_io(const char* message, bool in);
		void clear_log();

	public:
		
		ForkUCI();
		~ForkUCI();

		void handle_uci_cmd(std::string& cmd);
	};
}