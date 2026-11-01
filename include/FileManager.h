/**
 * @file FileManager.h
 * @brief High-level helper class for safe file operations, directory checking, and backup procedures.
 */

#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>
#include <vector>

namespace MedicalStore {

    /**
     * @class FileManager
     * @brief A wrapper class that handles system-level file systems, I/O streams, and directory actions.
     */
    class FileManager {
    public:
        /**
         * @brief Checks whether a file exists at the specified path.
         * @param filepath The path to the file.
         * @return True if the file exists.
         */
        static bool fileExists(const std::string& filepath);

        /**
         * @brief Reads all lines from a text file into a vector of strings.
         * @param filepath The path to the file.
         * @return A vector containing each line as a string.
         */
        static std::vector<std::string> readLines(const std::string& filepath);

        /**
         * @brief Writes a vector of strings to a text file (overwriting existing content).
         * @param filepath The path to the file.
         * @param lines The lines to write.
         * @return True if the write operation succeeded.
         */
        static bool writeLines(const std::string& filepath, const std::vector<std::string>& lines);

        /**
         * @brief Appends a single line to a text file.
         * @param filepath The path to the file.
         * @param line The line to append.
         * @return True if the append operation succeeded.
         */
        static bool appendLine(const std::string& filepath, const std::string& line);

        /**
         * @brief Creates a directory (including any missing parent directories) if it doesn't already exist.
         * @param dirpath The path of the directory.
         */
        static void ensureDirectoryExists(const std::string& dirpath);

        /**
         * @brief Copies a file from a source path to a destination path (overwriting if it exists).
         * @param srcPath The source file path.
         * @param destPath The destination file path.
         * @return True if the copy succeeded.
         */
        static bool copyFile(const std::string& srcPath, const std::string& destPath);

        /**
         * @brief Deletes a file at the specified path.
         * @param filepath The path of the file to delete.
         * @return True if deletion succeeded, false otherwise.
         */
        static bool deleteFile(const std::string& filepath);

        /**
         * @brief Checks whether a directory exists at the specified path.
         * @param dirpath The path of the directory.
         * @return True if it exists and is a directory.
         */
        static bool directoryExists(const std::string& dirpath);
    };
}

#endif // FILE_MANAGER_H
