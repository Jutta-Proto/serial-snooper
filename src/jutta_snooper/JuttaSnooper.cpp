#include "JuttaSnooper.hpp"
#include "jutta_proto/JuttaConnection.hpp"
#include "logger/Logger.hpp"
#include <vector>

//---------------------------------------------------------------------------
namespace jutta_snooper {
//---------------------------------------------------------------------------
JuttaSnooper::JuttaSnooper(const std::string& device) : connection(device) {}

void JuttaSnooper::run() {
    std::vector<uint8_t> readBuffer{};
    std::vector<uint8_t> writeBuffer{'H', 'a', 'l', 'l', 'o', '\r', '\n'};
    while (true) {
        // Write test data:
        connection.write_decoded(writeBuffer);
        SPDLOG_DEBUG("Wrote {} bytes.", writeBuffer.size());

        // Read test data:
        connection.read_decoded(readBuffer);
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
