/**
 * @file Login.h
 * @brief Definition of the Login class managing user authentication and security.
 */

#ifndef LOGIN_H
#define LOGIN_H

#include <string>

namespace MedicalStore {

    /**
     * @class Login
     * @brief Handles system authentication, login retry lockouts, and persistent password hashing.
     */
    class Login {
    private:
        std::string m_loginDbPath;  ///< File path where credentials are saved

        /**
         * @brief Reads credentials from disk and verifies them.
         * @param username The username input.
         * @param passwordHash The hash of the password input.
         * @return True if credentials match stored values.
         */
        bool verifyCredentials(const std::string& username, size_t passwordHash);

        /**
         * @brief Automatically creates the default administrator login file if missing.
         */
        void initializeDefaultAdmin();

    public:
        /**
         * @brief Constructor setting the path for the login database.
         * @param loginDbPath The path to LoginDatabase.txt.
         */
        Login(const std::string& loginDbPath = "database/LoginDatabase.txt");

        /**
         * @brief Displays the startup splash screen and pauses for 2 seconds.
         */
        void showSplashScreen() const;

        /**
         * @brief Coordinates the login prompt allowing up to 3 attempts.
         * @return True if authentication succeeds.
         */
        bool authenticate();

        /**
         * @brief Updates the administrator password in persistent storage.
         * @return True if change was successful.
         */
        bool changePassword();
    };
}

#endif // LOGIN_H
