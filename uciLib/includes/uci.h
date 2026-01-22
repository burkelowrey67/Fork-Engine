#pragma once
#include <fen.h>
#include <search_limits.h>

namespace uci {

	/*
	* @brief Parses UCI go command and fills a search::SearchLimits object.
	* @param out, 
	* @param params: Parameters' string.
	* @return std::optional<core::Position>
	*/
	void parse_go_cmd(search::SearchLimits& out, const char* params);
	
	/*
	* @brief Parses UCI position command into a core::Position object.
	* @param params: Parameters' string.
	* @return std::optional<core::Position>
	*/
	std::optional<core::Position> parse_position_cmd(const char* params);
}