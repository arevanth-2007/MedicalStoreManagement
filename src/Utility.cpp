/**
 * @file Utility.cpp
 * @brief Implementation of helper utility functions.
 */

#include "Utility.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits>
#include <chrono>
#include <ctime>
#include <algorithm>
#include <cctype>
#include <functional>

#ifdef _WIN32
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#endif

namespace MedicalStore {

    // Define ANSI colors
    const std::string Utility::RED = "\033[1;31m";
    const std::string Utility::GREEN = "\033[1;32m";
    const std::string Utility::YELLOW = "\033[1;33m";
    const std::string Utility::BLUE = "\033[1;34m";
    const std::string Utility::CYAN = "\033[1;36m";
    const std::string Utility::MAGENTA = "\033[1;35m";
    const std::string Utility::RESET = "\033[0m";

    // Set colors enabled by default on modern terminals, disabled dynamically on older Windows
    bool Utility::s_colorsEnabled = true;

    void Utility::initializeConsole() {
#ifdef _WIN32
        // Try to enable Virtual Terminal Processing on Windows to support ANSI escape sequences
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut == INVALID_HANDLE_VALUE) {
            s_colorsEnabled = false;
            return;
        }

        DWORD dwMode = 0;
        if (!GetConsoleMode(hOut, &dwMode)) {
            s_colorsEnabled = false;
            return;
        }

        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        if (!SetConsoleMode(hOut, dwMode)) {
            s_colorsEnabled = false;
        }
#else
        s_colorsEnabled = true; // Unix-like shells support ANSI escape sequences natively
#endif
    }

    void Utility::clearScreen() {
        if (s_colorsEnabled) {
            std::cout << "\033[2J\033[H" << std::flush;
        } else {
#ifdef _WIN32
            std::system("cls");
#else
            std::system("clear");
#endif
        }
    }

    void Utility::pause() {
        std::cout << "\nPress Enter to continue...";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }

    void Utility::printColor(const std::string& text, const std::string& colorCode) {
        if (s_colorsEnabled) {
            std::cout << colorCode << text << RESET;
        } else {
            std::cout << text;
        }
    }

    void Utility::printHeader(const std::string& title) {
        std::string border(50, '=');
        std::cout << "\n";
        printColor(border + "\n", BLUE);
        
        // Center the title
        int spaces = (50 - static_cast<int>(title.length())) / 2;
        if (spaces < 0) spaces = 0;
        std::string padding(spaces, ' ');
        
        printColor(padding + title + "\n", CYAN);
        printColor(border + "\n\n", BLUE);
    }

    std::string Utility::getInputString(const std::string& prompt, bool allowEmpty) {
        std::string val;
        while (true) {
            std::cout << prompt;
            if (std::cin.peek() == '\n') {
                std::cin.ignore();
            }
            std::getline(std::cin, val);
            
            // Trim leading/trailing whitespace
            val.erase(val.begin(), std::find_if(val.begin(), val.end(), [](unsigned char ch) {
                return !std::isspace(ch);
            }));
            val.erase(std::find_if(val.rbegin(), val.rend(), [](unsigned char ch) {
                return !std::isspace(ch);
            }).base(), val.end());

            if (!allowEmpty && val.empty()) {
                printColor("Error: Input cannot be empty! Please try again.\n", RED);
                continue;
            }
            break;
        }
        return val;
    }

    int Utility::getInputInt(const std::string& prompt, int minVal, int maxVal) {
        std::string valStr;
        int val;
        while (true) {
            std::cout << prompt;
            if (std::cin.peek() == '\n') {
                std::cin.ignore();
            }
            std::getline(std::cin, valStr);
            
            std::stringstream ss(valStr);
            if (ss >> val && ss.eof()) {
                if (val >= minVal && val <= maxVal) {
                    return val;
                } else {
                    printColor("Error: Input must be between " + std::to_string(minVal) + " and " + std::to_string(maxVal) + "!\n", RED);
                }
            } else {
                printColor("Error: Invalid numeric input! Please enter an integer.\n", RED);
            }
        }
    }

    double Utility::getInputDouble(const std::string& prompt, double minVal, double maxVal) {
        std::string valStr;
        double val;
        while (true) {
            std::cout << prompt;
            if (std::cin.peek() == '\n') {
                std::cin.ignore();
            }
            std::getline(std::cin, valStr);
            
            std::stringstream ss(valStr);
            if (ss >> val && ss.eof()) {
                if (val >= minVal && val <= maxVal) {
                    return val;
                } else {
                    // Using fixed precision formatting for range message
                    std::stringstream err;
                    err << std::fixed << std::setprecision(2);
                    err << "Error: Input must be between " << minVal << " and " << maxVal << "!\n";
                    printColor(err.str(), RED);
                }
            } else {
                printColor("Error: Invalid numeric input! Please enter a valid decimal number.\n", RED);
            }
        }
    }

    size_t Utility::djb2Hash(const std::string& str) {
        size_t hash = 5381;
        for (char c : str) {
            hash = ((hash << 5) + hash) + static_cast<unsigned char>(c);
        }
        return hash;
    }

    bool Utility::isValidDate(const std::string& dateStr) {
        // Date must be in YYYY-MM-DD format (exactly 10 characters)
        if (dateStr.length() != 10) return false;
        if (dateStr[4] != '-' || dateStr[7] != '-') return false;

        for (int i = 0; i < 10; ++i) {
            if (i == 4 || i == 7) continue;
            if (!std::isdigit(static_cast<unsigned char>(dateStr[i]))) return false;
        }

        int year = std::stoi(dateStr.substr(0, 4));
        int month = std::stoi(dateStr.substr(5, 2));
        int day = std::stoi(dateStr.substr(8, 2));

        if (year < 1900 || year > 2100) return false;
        if (month < 1 || month > 12) return false;
        if (day < 1 || day > 31) return false;

        // Days in months check
        static const int daysInMonths[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
        int maxDays = daysInMonths[month - 1];

        // Leap year validation
        if (month == 2) {
            bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
            if (isLeap) maxDays = 29;
        }

        return day <= maxDays;
    }

    std::string Utility::getCurrentDate() {
        auto now = std::chrono::system_clock::now();
        std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
        std::tm* localTime = std::localtime(&nowTime);

        std::stringstream ss;
        ss << std::setfill('0')
           << std::setw(4) << (localTime->tm_year + 1900) << "-"
           << std::setw(2) << (localTime->tm_mon + 1) << "-"
           << std::setw(2) << localTime->tm_mday;
        return ss.str();
    }

    std::string Utility::getCurrentDateTime() {
        auto now = std::chrono::system_clock::now();
        std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
        std::tm* localTime = std::localtime(&nowTime);

        std::stringstream ss;
        ss << std::setfill('0')
           << std::setw(4) << (localTime->tm_year + 1900) << "-"
           << std::setw(2) << (localTime->tm_mon + 1) << "-"
           << std::setw(2) << localTime->tm_mday << " "
           << std::setw(2) << localTime->tm_hour << ":"
           << std::setw(2) << localTime->tm_min << ":"
           << std::setw(2) << localTime->tm_sec;
        return ss.str();
    }

    int Utility::compareDates(const std::string& date1, const std::string& date2) {
        if (!isValidDate(date1) || !isValidDate(date2)) {
            return 0; // fallback if invalid
        }
        
        int y1 = std::stoi(date1.substr(0, 4));
        int m1 = std::stoi(date1.substr(5, 2));
        int d1 = std::stoi(date1.substr(8, 2));

        int y2 = std::stoi(date2.substr(0, 4));
        int m2 = std::stoi(date2.substr(5, 2));
        int d2 = std::stoi(date2.substr(8, 2));

        if (y1 != y2) return y1 - y2;
        if (m1 != m2) return m1 - m2;
        return d1 - d2;
    }

    bool Utility::isExpired(const std::string& expiryDate, const std::string& currentDate) {
        std::string refDate = currentDate.empty() ? getCurrentDate() : currentDate;
        return compareDates(expiryDate, refDate) < 0;
    }

    bool Utility::isNearExpiry(const std::string& expiryDate, const std::string& currentDate) {
        std::string refDate = currentDate.empty() ? getCurrentDate() : currentDate;
        
        // Already expired is not considered "near expiry" (it's expired)
        if (isExpired(expiryDate, refDate)) {
            return false;
        }

        int y1 = std::stoi(refDate.substr(0, 4));
        int m1 = std::stoi(refDate.substr(5, 2));

        int y2 = std::stoi(expiryDate.substr(0, 4));
        int m2 = std::stoi(expiryDate.substr(5, 2));

        int diffMonths = (y2 - y1) * 12 + (m2 - m1);
        
        // Near expiry defined as expiring in 3 months or less
        return diffMonths >= 0 && diffMonths <= 3;
    }

    void Utility::printTableBorder(const std::vector<int>& widths) {
        std::cout << "+";
        for (size_t i = 0; i < widths.size(); ++i) {
            std::cout << std::string(widths[i] + 2, '-') << "+";
        }
        std::cout << "\n";
    }

    void Utility::printTableRow(const std::vector<std::string>& cells, const std::vector<int>& widths, bool leftAlign) {
        std::cout << "|";
        for (size_t i = 0; i < cells.size() && i < widths.size(); ++i) {
            std::cout << " ";
            if (leftAlign) {
                std::cout << std::left << std::setw(widths[i]) << cells[i];
            } else {
                std::cout << std::right << std::setw(widths[i]) << cells[i];
            }
            std::cout << " |";
        }
        std::cout << "\n";
    }
}
