#pragma once
#include <cmath>

namespace uci {
	struct SearchLimits {
        int depth = -1;
        int movetime = -1;
        int wtime = -1;
        int btime = -1;
        int winc = 0;
        int binc = 0;
        int movestogo = -1;
        int nodes = INT_MAX;
        bool infinite = false;
	};
}