// uciLib.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"
#include <uci.h>
#include <token.h>
#include <fen.h>
#include <move/move.h>
#include <move_conversion.h>
#include <move/move_application.h>
#include <cstdlib>
#include <stdexcept>
#include <format>


namespace uci {

	void parse_go_cmd(search::SearchLimits& out, const char* params) {
		std::vector<std::string> tokens;
		uci::fill_str_tokens(tokens, params);

		for (int i = 0; i < tokens.size(); i++) {
			if (tokens[i] == "infinite") out.infinite = true; continue;
			if (tokens[i] == "depth") out.depth = std::atoi(tokens[++i].data()); continue;
			if (tokens[i] == "movetime") out.movetime = std::atoi(tokens[++i].data()); continue;
			if (tokens[i] == "btime") out.btime = std::atoi(tokens[++i].data()); continue;
			if (tokens[i] == "wtime") out.wtime = std::atoi(tokens[++i].data()); continue;
			if (tokens[i] == "binc") out.winc = std::atoi(tokens[++i].data()); continue;
			if (tokens[i] == "winc") out.winc = std::atoi(tokens[++i].data()); continue;
			if (tokens[i] == "movestogo") out.movestogo = std::atoi(tokens[++i].data()); continue;
			if (tokens[i] == "nodes") out.nodes = std::atoi(tokens[++i].data()); continue;
		}
	}

	std::optional<core::Position> parse_position_cmd(const char* params) {
		std::vector<std::string> tokens;
		uci::fill_str_tokens(tokens, params);
		std::optional<core::Position> position;

		for (int i = 0; i < tokens.size(); i++) {
			if (tokens[i] == "startpos") {
				position = core::Position::default_position();
			}

			else if (tokens[i] == "fen") {
				try {
					std::string fen = tokens[++i];

					for (int j = i + 1; j < i + 6; j++) {
						fen += " " + tokens[j];
					}

					position = parse_fen(fen);
					i += 5;
				}
				catch (std::out_of_range) {
					continue;
				}
			}

			else if (tokens[i] == "moves") {
				if (!position.has_value()) return std::nullopt;
				
				for (int j = ++i; j < tokens.size(); j++) {
					std::optional<move::Move> move = parse_uci_move(tokens[j], *position);
					if (move.has_value()) move::apply_move(*position, *move);
				}
			}
		}

		return position;
	}

	std::string format_search_info(search::SearchInfo& info) {
		std::string infoStr = "info";
		if (info.depth.has_value())			infoStr += std::format(" depth {}", *info.depth);
		if (info.nodesVisited.has_value())	infoStr += std::format(" nodes {}", *info.nodesVisited);
		if (info.eval.has_value())			infoStr += std::format(" score cp {}", *info.eval);
		return infoStr;
	}
}