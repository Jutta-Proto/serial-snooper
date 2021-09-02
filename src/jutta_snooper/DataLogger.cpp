#include "DataLogger.hpp"
#include <cassert>
#include <chrono>
#include <filesystem>
#include <jutta_proto/JuttaConnection.hpp>
#include <logger/Logger.hpp>
#include <string>
#include <spdlog/spdlog.h>

//---------------------------------------------------------------------------
namespace jutta_snooper {
//---------------------------------------------------------------------------
DataLogger::DataLogger() { init(); }

DataLogger::~DataLogger() {
    rawFile.close();
    rawLinesFile.close();
    logFile.close();
}

void DataLogger::log(const std::vector<uint8_t>& buf) {
    // NOLINTNEXTLINE (cppcoreguidelines-pro-type-reinterpret-cast)
    rawFile.write(reinterpret_cast<const char*>(buf.data()), static_cast<std::streamsize>(buf.size()));
    // NOLINTNEXTLINE (cppcoreguidelines-pro-type-reinterpret-cast)
    rawLinesFile.write(reinterpret_cast<const char*>(buf.data()), static_cast<std::streamsize>(buf.size()));
    static const char newLine = '\n';
    rawLinesFile.write(&newLine, 1);
    std::string resultRead = jutta_proto::JuttaConnection::vec_to_string(buf);
    logFile << "Read " << buf.size() << " bytes: " << resultRead;
    SPDLOG_DEBUG("Read {} bytes: {}", buf.size(), resultRead);
}

void DataLogger::init() {
    if (std::filesystem::exists(BASE_FOLDER_PATH)) {
        SPDLOG_INFO("Output directory already exists. Moving it...");
        const std::filesystem::path newPath = get_cur_time_base_path();
        std::filesystem::rename(BASE_FOLDER_PATH, newPath);
        SPDLOG_INFO("Old output directory moved to: {}", newPath.string());
    }
    std::filesystem::create_directory(BASE_FOLDER_PATH);

    rawFile.open(RAW_FILE_PATH, std::ofstream::out | std::ofstream::binary);
    assert(rawFile.is_open());
    rawLinesFile.open(RAW_WITH_LINES_FILE_PATH, std::ofstream::out | std::ofstream::binary);
    assert(rawLinesFile.is_open());
    logFile.open(LOG_FILE_PATH, std::ofstream::out);
    assert(logFile.is_open());
}

const std::filesystem::path DataLogger::get_cur_time_base_path() {
    return "output_" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
}
//---------------------------------------------------------------------------
}  // namespace jutta_snooper
//---------------------------------------------------------------------------