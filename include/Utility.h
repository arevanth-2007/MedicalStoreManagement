/**
 * @file Utility.h
 * @brief Helper utility functions for console rendering, input validation, and date calculations.
 */

#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <vector>
#include <cstddef>

namespace MedicalStore {

    /**
     * @class Utility
     * @brief A utility class containing static helper methods for UI formatting, input validation, and date functions.
     */
    class Utility {
    public:
        // Color codes for ANSI terminal output
        static const std::string RED;
        static const std::string GREEN;
        static const std::string YELLOW;
        static const std::string BLUE;
        static const std::string CYAN;
        static const std::string MAGENTA;
        static const std::string RESET;

        /**
         * @brief Enables ANSI color support in Windows terminal (if applicable).
         */
        static void initializeConsole();

        /**
         * @brief Clears the console screen.
         */
        static void clearScreen();

        /**
         * @brief Pauses the program and waits for the user to press Enter.
         */
        static void pause();

        /**
         * @brief Prints text in a specific color (automatically falls back to plain text if colors are disabled).
         * @param text The text to display.
         * @param colorCode The ANSI color code to apply.
         */
        static void printColor(const std::string& text, const std::string& colorCode);

        /**
         * @brief Prints a standardized screen header with ASCII borders.
         * @param title The title text.
         */
        static void printHeader(const std::string& title);

        /**
         * @brief Reads a string input from the user with optional non-empty validation.
         * @param prompt The prompt message.
         * @param allowEmpty True to allow empty strings.
         * @return The validated input string.
         */
        static std::string getInputString(const std::string& prompt, bool allowEmpty = false);

        /**
         * @brief Reads and validates an integer input within a specified range.
         * @param prompt The prompt message.
         * @param minVal The minimum acceptable value.
         * @param maxVal The maximum acceptable value.
         * @return The validated integer.
         */
        static int getInputInt(const std::string& prompt, int minVal, int maxVal);

        /**
         * @brief Reads and validates a double input within a specified range.
         * @param prompt The prompt message.
         * @param minVal The minimum acceptable value.
         * @param maxVal The maximum acceptable value.
         * @return The validated double.
         */
        static double getInputDouble(const std::string& prompt, double minVal, double maxVal);

        /**
         * @brief Hashes a string using the DJB2 algorithm.
         * @param str The string to hash.
         * @return The hash value as a size_t.
         */
        static size_t djb2Hash(const std::string& str);

        /**
         * @brief Validates if a date string is in the format YYYY-MM-DD.
         * @param dateStr The date string to validate.
         * @return True if the date is valid.
         */
        static bool isValidDate(const std::string& dateStr);

        /**
         * @brief Retrieves the current local date in YYYY-MM-DD format.
         * @return The date string.
         */
        static std::string getCurrentDate();

        /**
         * @brief Retrieves the current local date and time in YYYY-MM-DD HH:MM:SS format.
         * @return The date-time string.
         */
        static std::string getCurrentDateTime();

        /**
         * @brief Compares two dates in YYYY-MM-DD format.
         * @param date1 The first date.
         * @param date2 The second date.
         * @return An integer < 0 if date1 < date2, 0 if equal, and > 0 if date1 > date2.
         */
        static int compareDates(const std::string& date1, const std::string& date2);

        /**
         * @brief Checks if a medicine is expired relative to the current date.
         * @param expiryDate The expiry date.
         * @param currentDate The reference date (defaults to current system date if empty).
         * @return True if expired.
         */
        static bool isExpired(const std::string& expiryDate, const std::string& currentDate = "");

        /**
         * @brief Checks if a medicine is near expiry (within 3 months).
         * @param expiryDate The expiry date.
         * @param currentDate The reference date (defaults to current system date if empty).
         * @return True if near expiry.
         */
        static bool isNearExpiry(const std::string& expiryDate, const std::string& currentDate = "");

        /**
         * @brief Prints a horizontal table border.
         * @param widths The column widths.
         */
        static void printTableBorder(const std::vector<int>& widths);

        /**
         * @brief Prints a row of a table with proper alignment.
         * @param cells The cell contents.
         * @param widths The column widths.
         * @param leftAlign True to left-align, false to right-align.
         */
        static void printTableRow(const std::vector<std::string>& cells, const std::vector<int>& widths, bool leftAlign = true);

    private:
        static bool s_colorsEnabled;
    };
}

#endif // UTILITY_H
