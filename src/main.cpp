#include "jutta_snooper/JuttaSnooper.hpp"
#include "logger/Logger.hpp"
#include <csignal>
#include <spdlog/spdlog.h>

// NOLINTNEXTLINE (cppcoreguidelines-avoid-non-const-global-variables)
jutta_snooper::JuttaSnooper* snooperPtr = nullptr;

void signal_handler(int signal) {
    if (signal == SIGINT) {
        if (snooperPtr) {
            SPDLOG_INFO("Ctrl+C pressed...");
            snooperPtr->stop();
        }
    }
}

int main(int /**argc**/, char** /**argv**/) {
    logger::setup_logger(spdlog::level::debug);
    SPDLOG_INFO("Launching Version: {} {}", JUTTA_SNOOPER_VERSION, JUTTA_SNOOPER_VERSION_NAME);
    std::signal(SIGINT, signal_handler);

    jutta_snooper::JuttaSnooper snooper("/dev/serial0");
    snooperPtr = &snooper;
    snooper.run();
    return 0;
}
