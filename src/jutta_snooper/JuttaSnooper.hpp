#pragma once

#include <jutta_proto/JuttaConnection.hpp>
#include <string>
#include <thread>

//---------------------------------------------------------------------------
namespace jutta_snooper {
//---------------------------------------------------------------------------
class JuttaSnooper {
 private:
    jutta_proto::JuttaConnection connection;

 public:
    explicit JuttaSnooper(const std::string& device);
    JuttaSnooper(JuttaSnooper&&) = default;
    JuttaSnooper(const JuttaSnooper&) = default;
    JuttaSnooper& operator=(JuttaSnooper&&) = default;
    JuttaSnooper& operator=(const JuttaSnooper&) = default;
    ~JuttaSnooper() = default;

    void run();

 private:
};
//---------------------------------------------------------------------------
}  // namespace jutta_snooper
//---------------------------------------------------------------------------
