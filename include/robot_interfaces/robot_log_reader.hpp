/**
 * @file
 * @brief API to read the data from a robot log file.
 * @copyright 2020, Max Planck Gesellschaft. All rights reserved.
 * @license BSD 3-clause
 */
#pragma once

#include <fstream>
#include <vector>

#include <cereal/archives/binary.hpp>
#include <cereal/types/tuple.hpp>
#include <cereal/types/vector.hpp>

#include <robot_interfaces/robot_log_entry.hpp>

namespace robot_interfaces
{
/**
 * @brief Read the data from a robot log file.
 *
 * The data is read from the specified file and stored to the `data` member
 * where it can be accessed.
 */
template <typename Action, typename Observation>
class RobotBinaryLogReader
{
public:
    typedef RobotLogEntry<Action, Observation> LogEntry;

    std::vector<LogEntry> data;

    //! @copydoc RobotBinaryLogReader::read_file()
    RobotBinaryLogReader(const std::string &filename)
    {
        read_file(filename);
    }

    /**
     * @brief Read data from the specified file.
     *
     * The data is stored to RobotBinaryLogReader::data.
     *
     * @param filename Path to the robot log file.
     */
    void read_file(const std::string &filename)
    {
        std::ifstream infile(filename, std::ios::binary);
        if (!infile)
        {
            throw std::runtime_error("Failed to open file " + filename);
        }

        cereal::BinaryInputArchive archive(infile);

        std::uint32_t format_version;
        archive(format_version);

        if (format_version != 2)
        {
            throw std::runtime_error("Incompatible log file format.");
        }

        archive(data);
    }
};

}  // namespace robot_interfaces
