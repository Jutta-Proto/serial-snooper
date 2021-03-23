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
    explicit JuttaSnooper(std::string&& device);
    JuttaSnooper(JuttaSnooper&&) = default;
    JuttaSnooper(const JuttaSnooper&) = default;
    JuttaSnooper& operator=(JuttaSnooper&&) = default;
    JuttaSnooper& operator=(const JuttaSnooper&) = default;
    ~JuttaSnooper() = default;

    void run();

 private:
    static std::string readStringStdin();
    static void readThreadRun(const jutta_proto::JuttaConnection& connection);
};
//---------------------------------------------------------------------------
}  // namespace jutta_snooper
//---------------------------------------------------------------------------
