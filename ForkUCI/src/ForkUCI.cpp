#include <fork_uci.h>	
#include <string>
#include <iostream>
#include <uci_handles.h>
#include <move_search.h>
#include <move_conversion.h>
#include <fstream>
#include <format>
#include <ios>

namespace fork {

    ForkUCI::ForkUCI() : position(), search() {
        log("Fork Engine init", "START UP");
    }

    ForkUCI::~ForkUCI() = default;

    void ForkUCI::handle_uci_cmd(std::string& cmd) {

        if (cmd.empty()) return;

        log_io(cmd.data(), true);

        if (cmd == "uci") {
            out("id name ForkEngine\nid author Burke Lowrey\nuciok", true);
        }
        else if (cmd == "isready") {
            out("readyok", true);
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
            start_search_events_jthread();
            uci::handle_go(search, position, cmd);
        }
        else if (cmd == "stop") {
            uci::handle_stop(search);
        }
        else if (cmd == "quit") {
            exit(0);
        }
    }

    void ForkUCI::start_search_events_jthread() {

        if (searchEventsThread.joinable()) {
            searchEventsThread.request_stop();
            searchEventsThread.join();
        }
        
        searchEventsThread = std::jthread([this] () {
            {
                std::unique_lock<std::mutex> lock(search.searchDoneMutex);
                search.cv.wait(lock, [&] { return search.searchDone; });
            }

            if (std::optional<uint32_t> bestMove = search.get_info().bestMove; bestMove.has_value()) {
                std::string moveStr = std::format("bestmove {}", *uci::format_uci_move(move::Move(*bestMove)));
                out(moveStr.data(), true);
            } 
            else {
                out("0000", false);
            }
        });
    }

    void ForkUCI::out(const char* out, bool flush) {
        if (flush) std::cout << out << "\n";
        else std::cout << out << "\n" << std::flush;
        log_io(out, false);
    }

    void ForkUCI::log(const char* message, const char* tag) {
        std::ofstream log("fork_uci.log", std::ios::app);
        std::string line = std::format("[{}]\n{}\n", tag, std::string(message));
        log << line << std::endl;
        log.close();
    }

    void ForkUCI::log_io(const char* message, bool in) {
        const char* tag = in ? "GUI -> IN" : "FORK ENGINE -> OUT";
        log(message, tag);
    }
}