#include <fork_uci.h>	
#include <string>
#include <iostream>
#include <uci_handles.h>
#include <move_search.h>

namespace fork {

    void ForkUCI::handle_uci_cmd(std::string& cmd) {

        if (cmd.empty()) return;

        if (cmd == "uci") {
            uci::handle_uci();
        }
        else if (cmd == "isready") {
            uci::handle_isready();
        }
        else if (cmd.rfind("setoption", 0) == 0) {
            uci::handle_setoption(search, cmd);
        }
        else if (cmd == "ucinewgame") {
            uci::handle_ucinewgame(search);
        }
        else if (cmd.rfind("position", 0) == 0) {
            uci::handle_position(position, cmd);
        }
        else if (cmd.rfind("go", 0) == 0) {
            uci::handle_go(search, position, cmd);
        }
        else if (cmd == "stop") {
            uci::handle_stop(search);
        }
        else if (cmd == "quit") {
            uci::handle_quit(search);
        }
    }
}