#include "JuttaSnooper.hpp"
#include "jutta_proto/JuttaConnection.hpp"
#include "logger/Logger.hpp"
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

//---------------------------------------------------------------------------
namespace jutta_snooper {
//---------------------------------------------------------------------------
JuttaSnooper::JuttaSnooper(std::string&& device) : connection(std::move(device)) {}

void JuttaSnooper::run() {
    shouldRun = true;
    cinReaderThread = std::make_unique<std::thread>(&JuttaSnooper::cin_run, this);
    uartReaderThread = std::make_unique<std::thread>(&JuttaSnooper::uart_run, this);

    while (shouldRun) { std::this_thread::sleep_for(std::chrono::milliseconds{250}); }

    cinReaderThread->join();
    uartReaderThread->join();

    /*std::vector<uint8_t> readBuffer{};
    std::vector<uint8_t> writeBuffer{'T', 'Y', ':', '\r', '\n'};
    while (true) {
        // Write test data:
        static_cast<void>(connection.write_decoded(writeBuffer));
        SPDLOG_DEBUG("Wrote {} bytes.", writeBuffer.size());
        std::this_thread::sleep_for(std::chrono::seconds{2});

        static_cast<void>(connection.read_decoded(readBuffer));
        if (!readBuffer.empty()) {
            std::string resultRead = jutta_proto::JuttaConnection::vec_to_string(readBuffer);
            SPDLOG_DEBUG("Read {} bytes: {}", readBuffer.size(), resultRead);

            // jutta_proto::JuttaConnection::print_bytes(buffer);
            readBuffer.clear();
        }
    }*/
    // readThread.join();
}

std::string JuttaSnooper::read_cin() {
    std::cout << "$ ";
    std::string s;
    std::cin >> s;
    return s;
}

void JuttaSnooper::cin_run() {
    while (shouldRun) {
        std::string s = read_cin();
        if (!s.empty()) {
            s.append("\r\n");
            connectionLock.lock();
            static_cast<void>(connection.write_decoded(s));
            connectionLock.unlock();
            std::cout << "Length: " << s.length() << '\n';
        }
    }
}

void JuttaSnooper::uart_run() {
    connection.init();
    std::vector<uint8_t> readBuffer{};
    while (true) {
        connectionLock.lock();
        static_cast<void>(connection.read_decoded(readBuffer));
        connectionLock.unlock();
        if (!readBuffer.empty()) {
            std::string resultRead = jutta_proto::JuttaConnection::vec_to_string(readBuffer);
            SPDLOG_DEBUG("Read {} bytes: {}", readBuffer.size(), resultRead);
            // jutta_proto::JuttaConnection::print_bytes(buffer);
            readBuffer.clear();
        }
    }
}
//---------------------------------------------------------------------------
}  // namespace jutta_snooper
//---------------------------------------------------------------------------
