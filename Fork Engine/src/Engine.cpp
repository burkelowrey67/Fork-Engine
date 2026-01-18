#include <engine.h>
#include <eval.h>

namespace fork {

	double Engine::eval(core::Position& position) {
		return position::evaluation::eval(position);
	}

	move::Move Engine::search(core::Position& position) {
		return move::search(position);
	}
}