#pragma once

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <vector>

//---------------------------------------------------------------------------
namespace jutta_snooper {
//---------------------------------------------------------------------------
class DataLogger {
 private:
    const std::filesystem::path BASE_FOLDER_PATH = "output";
    const std::filesystem::path RAW_FILE_PATH = BASE_FOLDER_PATH / "raw.bin";
    const std::filesystem::path RAW_WITH_LINES_FILE_PATH = BASE_FOLDER_PATH / "raw_with_lines.bin";
    const std::filesystem::path LOG_FILE_PATH = BASE_FOLDER_PATH / "log.txt";

    std::ofstream rawFile;
    std::ofstream rawLinesFile;
    std::ofstream logFile;

 public:
    DataLogger();
    DataLogger(DataLogger&&) = delete;
    DataLogger(const DataLogger&) = delete;
    DataLogger& operator=(DataLogger&&) = delete;
    DataLogger& operator=(const DataLogger&) = delete;
    ~DataLogger();

    void log(const std::vector<uint8_t>& buf);

 private:
    void init();
    static const std::filesystem::path get_cur_time_base_path();
};
//---------------------------------------------------------------------------
}  // namespace jutta_snooper
//---------------------------------------------------------------------------
