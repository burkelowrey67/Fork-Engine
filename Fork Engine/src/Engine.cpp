#include <engine.h>
#include <eval.h>
#include <move_search.h>

namespace fork {

	Engine::Engine() : position(core::Position::default_position()) {};
	Engine::~Engine() = default;

	void Engine::initialize() {

	}

	void Engine::set_position(core::Position* position) {
		this->position = position;
	}

	void Engine::go() {
		search::go(*position);
	}

	move::Move Engine::stop() {
		search::stop();
		return move::Move(search::get_info().bestMove);
	}


}