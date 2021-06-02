#pragma once

#include <jutta_proto/JuttaConnection.hpp>
#include <memory>
#include <string>
#include <thread>
#include <mutex>

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

 public:
    explicit JuttaSnooper(std::string&& device);
    JuttaSnooper(JuttaSnooper&&) = default;
    JuttaSnooper(const JuttaSnooper&) = default;
    JuttaSnooper& operator=(JuttaSnooper&&) = default;
    JuttaSnooper& operator=(const JuttaSnooper&) = default;
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
