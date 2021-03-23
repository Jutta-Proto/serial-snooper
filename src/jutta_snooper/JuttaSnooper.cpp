#include "JuttaSnooper.hpp"
#include "jutta_proto/JuttaConnection.hpp"
#include "logger/Logger.hpp"
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

//---------------------------------------------------------------------------
namespace jutta_snooper {
//---------------------------------------------------------------------------
JuttaSnooper::JuttaSnooper(const std::string& device) : connection(device) {}

void JuttaSnooper::run() {
    // std::thread readThread(readStringStdin, connection);

    std::vector<uint8_t> readBuffer{};
    std::vector<uint8_t> writeBuffer{'T', 'Y', ':', '\r', '\n'};
    while (true) {
        // Write test data:
        static_cast<void>(connection.write_decoded(writeBuffer));
        SPDLOG_DEBUG("Wrote {} bytes.", writeBuffer.size());
        std::this_thread::sleep_for(std::chrono::seconds{2});

        static_cast<void>(connection.read_decoded(readBuffer));
        if (!readBuffer.empty()) {
            std::string resultRead = jutta_proto::JuttaConnection::vec_to_string(readBuffer);
            SPDLOG_DEBUG("Read {} bytes: ", readBuffer.size(), resultRead);

            // jutta_proto::JuttaConnection::print_bytes(buffer);
            readBuffer.clear();
        }
    }
    // readThread.join();
}

std::string JuttaSnooper::readStringStdin() {
    std::cout << "$ ";
    std::string s;
    std::cin >> s;
    return s;
}

void JuttaSnooper::readThreadRun(const jutta_proto::JuttaConnection& connection) {
    std::vector<uint8_t> readBuffer{};
    while (true) {
        static_cast<void>(connection.read_decoded(readBuffer));
        if (!readBuffer.empty()) {
            SPDLOG_DEBUG("Read {} bytes.", readBuffer.size());
            // jutta_proto::JuttaConnection::print_bytes(buffer);
            readBuffer.clear();
        }
    }

}
//---------------------------------------------------------------------------
}  // namespace jutta_snooper
//---------------------------------------------------------------------------
