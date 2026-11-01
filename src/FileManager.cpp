/**
 * @file FileManager.cpp
 * @brief Implementation of FileManager helpers.
 */

#include "FileManager.h"
#include <fstream>
#include <filesystem>
#include <iostream>

namespace MedicalStore {

    bool FileManager::fileExists(const std::string& filepath) {
        return std::filesystem::exists(filepath);
    }

    std::vector<std::string> FileManager::readLines(const std::string& filepath) {
        std::vector<std::string> lines;
        std::ifstream file(filepath);
        if (!file.is_open()) {
            return lines;
        }

        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
        file.close();
        return lines;
    }

    bool FileManager::writeLines(const std::string& filepath, const std::vector<std::string>& lines) {
        // Ensure parent directory exists before writing
        std::filesystem::path p(filepath);
        if (p.has_parent_path()) {
            ensureDirectoryExists(p.parent_path().string());
        }

        std::ofstream file(filepath, std::ios::trunc);
        if (!file.is_open()) {
            return false;
        }

        for (const auto& line : lines) {
            file << line << "\n";
        }
        file.close();
        return file.good();
    }

    bool FileManager::appendLine(const std::string& filepath, const std::string& line) {
        std::filesystem::path p(filepath);
        if (p.has_parent_path()) {
            ensureDirectoryExists(p.parent_path().string());
        }

        std::ofstream file(filepath, std::ios::app);
        if (!file.is_open()) {
            return false;
        }

        file << line << "\n";
        file.close();
        return file.good();
    }

    void FileManager::ensureDirectoryExists(const std::string& dirpath) {
        if (dirpath.empty()) return;
        if (!std::filesystem::exists(dirpath)) {
            try {
                std::filesystem::create_directories(dirpath);
            } catch (const std::filesystem::filesystem_error& e) {
                std::cerr << "Directory creation error: " << e.what() << "\n";
            }
        }
    }

    bool FileManager::copyFile(const std::string& srcPath, const std::string& destPath) {
        try {
            // 1. Verify source file exists
            if (!fileExists(srcPath)) {
                return false;
            }
            
            // 2. Verify source is a regular file
            if (!std::filesystem::is_regular_file(srcPath)) {
                return false;
            }

            // 3. Ensure destination directory exists
            std::filesystem::path dp(destPath);
            if (dp.has_parent_path()) {
                ensureDirectoryExists(dp.parent_path().string());
            }

            // 4. Perform C++17 copy
            std::filesystem::copy_file(
                srcPath, 
                destPath, 
                std::filesystem::copy_options::overwrite_existing
            );
            return true;
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "File copy error: " << e.what() << "\n";
            return false;
        }
    }

    bool FileManager::deleteFile(const std::string& filepath) {
        try {
            if (fileExists(filepath) && std::filesystem::is_regular_file(filepath)) {
                return std::filesystem::remove(filepath);
            }
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "File deletion error: " << e.what() << "\n";
        }
        return false;
    }

    bool FileManager::directoryExists(const std::string& dirpath) {
        try {
            return std::filesystem::is_directory(dirpath);
        } catch (const std::filesystem::filesystem_error& e) {
            // Gracefully ignore error and return false
        }
        return false;
    }
}
