#include "jutta_snooper/JuttaSnooper.hpp"
#include "logger/Logger.hpp"

int main(int /**argc**/, char** /**argv**/) {
    logger::setup_logger(spdlog::level::debug);
    SPDLOG_INFO("Launching Version: {} {}", JUTTA_SNOOPER_VERSION, JUTTA_SNOOPER_VERSION_NAME);

    jutta_snooper::JuttaSnooper snooper("/dev/serial0");
    snooper.run();
    return 0;
}
