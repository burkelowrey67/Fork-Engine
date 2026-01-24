#include <fork_uci.h>	
#include <string>
#include <iostream>
#include <uci_handles.h>
#include <move_search.h>
#include <move_conversion.h>
#include <fstream>
#include <format>
#include <ios>
#include <uci.h>

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
            search.reset_data();
            start_search_events_jthread();
            uci::handle_go(search, position, cmd);
        }
        else if (cmd == "stop") {
            uci::handle_stop(search);
        }
        else if (cmd == "quit") {
            clear_log();
            exit(0);
        }
    }

    void ForkUCI::start_search_events_jthread() {

        if (searchEventsThread.joinable()) {
            searchEventsThread.request_stop();
            searchEventsThread.join();
        }
        


        searchEventsThread = std::jthread([this](std::stop_token st) {
            while (!st.stop_requested()) {

                uint64_t lastSeenVersion = 0;


                std::unique_lock<std::mutex> lock(search.m);
                search.cv.wait(lock, [&] {
                    return st.stop_requested()
                        || search.data.searchDone
                        || search.data.infoVersion != lastSeenVersion;
                    });

                if (st.stop_requested()) return;

                out(uci::format_search_info(search.data.info).data(), false);

                lastSeenVersion = search.data.infoVersion;


                if (search.data.searchDone) {
                    if (auto bestMove = search.data.info.bestMove) {
                        out(std::format("bestmove {}",
                            *uci::format_uci_move(move::Move(*bestMove))).c_str(), true);
                    }
                    else {
                        out("bestmove 0000", false);
                    }

                    return;
                }
            }
        });
    }

    void ForkUCI::out(const char* out, bool flush) {
        if (!flush) std::cout << out << "\n";
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

    void ForkUCI::clear_log() {
        std::ofstream log("fork_uci.log", std::ios::out | std::ios::trunc);
        log.close();
    }
}