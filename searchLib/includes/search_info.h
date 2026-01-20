#pragma once

namespace search {
    struct SearchInfo {
    public:
        static uint32_t bestMove;
        static int depth;
        static int nodesVisited;
    };
}