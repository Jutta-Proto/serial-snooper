#include "logger/Logger.hpp"

int main(int /**argc**/, char** /**argv**/) {
    logger::setup_logger(spdlog::level::debug);
    SPDLOG_INFO("Launching Version: {} {}", JUTTA_SERIAL_SNOOPER_VERSION, JUTTA_SERIAL_SNOOPER_VERSION_NAME);
    return 0;
}
