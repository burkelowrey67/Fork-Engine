#include <engine.h>
#include <eval.h>

namespace fork {

	double Engine::eval(chess::Position& position) {
		return position::evaluation::eval(position);
	}

	move::Move Engine::search(chess::Position& position) {
		return move::search(position);
	}
}