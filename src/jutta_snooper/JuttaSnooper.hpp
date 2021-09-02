#pragma once

#include "DataLogger.hpp"
#include <jutta_proto/JuttaConnection.hpp>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

//---------------------------------------------------------------------------
namespace jutta_snooper {
//---------------------------------------------------------------------------
class JuttaSnooper {
 private:
    bool shouldRun{false};
    std::unique_ptr<std::thread> cinReaderThread{nullptr};
    std::unique_ptr<std::thread> uartReaderThread{nullptr};

    jutta_proto::JuttaConnection connection;
    std::mutex connectionLock;

    DataLogger dataLogger;

 public:
    explicit JuttaSnooper(std::string&& device);
    JuttaSnooper(JuttaSnooper&&) = delete;
    JuttaSnooper(const JuttaSnooper&) = delete;
    JuttaSnooper& operator=(JuttaSnooper&&) = delete;
    JuttaSnooper& operator=(const JuttaSnooper&) = delete;
    ~JuttaSnooper() = default;

    void run();

 private:
    void cin_run();
    void uart_run();

    static std::string read_cin();
};
//---------------------------------------------------------------------------
}  // namespace jutta_snooper
//---------------------------------------------------------------------------
