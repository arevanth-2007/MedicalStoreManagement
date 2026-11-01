/**
 * @file Login.cpp
 * @brief Implementation of the Login authentication class.
 */

#include "Login.h"
#include "Utility.h"
#include "FileManager.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <sstream>

#ifdef _WIN32
#include <conio.h>  // For _getch() masking on Windows
#endif

namespace MedicalStore {

    // Helper function for secure password input with asterisks masking
    static std::string readMaskedPassword(const std::string& prompt) {
        std::cout << prompt;
        std::string password;
#ifdef _WIN32
        char ch;
        while (true) {
            ch = static_cast<char>(_getch());
            if (ch == '\r' || ch == '\n') { // Enter key
                break;
            } else if (ch == '\b') { // Backspace
                if (!password.empty()) {
                    password.pop_back();
                    std::cout << "\b \b" << std::flush;
                }
            } else if (ch == 27) { // Escape key
                // Clear input
                while (!password.empty()) {
                    password.pop_back();
                    std::cout << "\b \b" << std::flush;
                }
            } else if (ch == 0 || ch == -32) {
                // Ignore function keys (arrow keys, F-keys, etc. start with 0 or -32 on Windows)
                _getch(); // consume second byte
            } else {
                password.push_back(ch);
                std::cout << '*' << std::flush;
            }
        }
        std::cout << "\n";
#else
        // Fallback for non-windows platforms (normal input)
        std::getline(std::cin, password);
#endif
        return password;
    }

    Login::Login(const std::string& loginDbPath) : m_loginDbPath(loginDbPath) {}

    void Login::showSplashScreen() const {
        Utility::clearScreen();
        std::cout << "\n\n\n";
        Utility::printColor("*******************************************************\n", Utility::BLUE);
        Utility::printColor("          MEDICAL STORE MANAGEMENT SYSTEM              \n", Utility::CYAN);
        Utility::printColor("                 Version 1.0                           \n", Utility::CYAN);
        Utility::printColor("       Summer Internship Project 2026                  \n", Utility::YELLOW);
        Utility::printColor("     Object Oriented Programming using C++             \n", Utility::MAGENTA);
        Utility::printColor("*******************************************************\n", Utility::BLUE);
        std::cout << "\n";
        Utility::printColor("Loading System...\n", Utility::GREEN);
        
        // Sleep for approximately 2 seconds to simulate loading
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }

    void Login::initializeDefaultAdmin() {
        if (!FileManager::fileExists(m_loginDbPath)) {
            // Default: admin / admin123
            size_t hashedPass = Utility::djb2Hash("admin123");
            std::vector<std::string> lines;
            lines.push_back("admin|" + std::to_string(hashedPass));
            FileManager::writeLines(m_loginDbPath, lines);
        }
    }

    bool Login::verifyCredentials(const std::string& username, size_t passwordHash) {
        initializeDefaultAdmin();

        std::vector<std::string> lines = FileManager::readLines(m_loginDbPath);
        if (lines.empty()) return false;

        std::stringstream ss(lines[0]);
        std::string dbUser, dbHashStr;
        std::getline(ss, dbUser, '|');
        std::getline(ss, dbHashStr, '|');

        return (username == dbUser && std::to_string(passwordHash) == dbHashStr);
    }

    bool Login::authenticate() {
        initializeDefaultAdmin();

        for (int attempt = 1; attempt <= 3; ++attempt) {
            Utility::clearScreen();
            Utility::printHeader("ADMINISTRATOR LOGIN");

            if (attempt > 1) {
                Utility::printColor("Warning: Invalid credentials. Attempt " + std::to_string(attempt) + " of 3.\n\n", Utility::YELLOW);
            }

            std::string user = Utility::getInputString("Enter Username: ");
            std::string pass = readMaskedPassword("Enter Password: ");

            size_t hashedInput = Utility::djb2Hash(pass);

            if (verifyCredentials(user, hashedInput)) {
                Utility::printColor("\nLogin Successful! Access Granted.\n", Utility::GREEN);
                std::this_thread::sleep_for(std::chrono::milliseconds(800));
                return true;
            }
        }

        Utility::clearScreen();
        Utility::printColor("\n============================================\n", Utility::RED);
        Utility::printColor("      ALERT: MAXIMUM LOGIN ATTEMPTS EXCEEDED \n", Utility::RED);
        Utility::printColor("============================================\n", Utility::RED);
        Utility::printColor("System Locked. Please contact your system administrator.\n", Utility::RED);
        Utility::pause();
        return false;
    }

    bool Login::changePassword() {
        Utility::printHeader("CHANGE ADMINISTRATOR PASSWORD");
        
        std::string curPass = readMaskedPassword("Enter Current Password: ");
        size_t hashedCur = Utility::djb2Hash(curPass);

        // Read current username from file
        std::vector<std::string> lines = FileManager::readLines(m_loginDbPath);
        std::string username = "admin";
        if (!lines.empty()) {
            std::stringstream ss(lines[0]);
            std::getline(ss, username, '|');
        }

        if (!verifyCredentials(username, hashedCur)) {
            Utility::printColor("\nError: Current password does not match!\n", Utility::RED);
            Utility::pause();
            return false;
        }

        std::string newPass = readMaskedPassword("Enter New Password: ");
        if (newPass.empty()) {
            Utility::printColor("\nError: New password cannot be empty!\n", Utility::RED);
            Utility::pause();
            return false;
        }

        std::string confirmPass = readMaskedPassword("Confirm New Password: ");
        if (newPass != confirmPass) {
            Utility::printColor("\nError: Passwords do not match! Password change aborted.\n", Utility::RED);
            Utility::pause();
            return false;
        }

        size_t hashedNew = Utility::djb2Hash(newPass);
        std::vector<std::string> newLines;
        newLines.push_back(username + "|" + std::to_string(hashedNew));

        if (FileManager::writeLines(m_loginDbPath, newLines)) {
            Utility::printColor("\nSuccess: Password changed successfully!\n", Utility::GREEN);
        } else {
            Utility::printColor("\nError: Failed to save new password. Disk I/O error.\n", Utility::RED);
        }

        Utility::pause();
        return true;
    }
}
