#include <string>
#include <iostream>
#include <fork_uci.h>

int main() {
    fork::ForkUCI fork;
    std::string line;

    while (std::getline(std::cin, line)) {
        fork.handle_uci_cmd(line);
    }
}