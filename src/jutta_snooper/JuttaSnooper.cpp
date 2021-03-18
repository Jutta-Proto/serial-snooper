#include "JuttaSnooper.hpp"
#include "jutta_proto/JuttaConnection.hpp"
#include <iostream>
#include <vector>

//---------------------------------------------------------------------------
namespace jutta_snooper {
//---------------------------------------------------------------------------
JuttaSnooper::JuttaSnooper(const std::string& device) : connection(device) {}

void JuttaSnooper::run() {
    std::vector<uint8_t> buffer{};
    while (true) {
        connection.read_decoded(buffer);
        if (!buffer.empty()) {
            std::cout << "Read " << buffer.size() << " bytes\n";
            jutta_proto::JuttaConnection::print_bytes(buffer);
            buffer.clear();
        }
    }
}
//---------------------------------------------------------------------------
}  // namespace jutta_snooper
//---------------------------------------------------------------------------
