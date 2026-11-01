/**
 * @file Supplier.h
 * @brief Definition of the Supplier class representing a wholesale supplier.
 */

#ifndef SUPPLIER_H
#define SUPPLIER_H

#include <string>

namespace MedicalStore {

    /**
     * @class Supplier
     * @brief Represents a supplier who distributes medicines to the medical store.
     */
    class Supplier {
    private:
        std::string m_supplierID;   ///< Unique Supplier ID (e.g. SUPP001)
        std::string m_name;         ///< Supplier Name
        std::string m_phone;        ///< Contact Phone Number
        std::string m_email;        ///< Email Address
        std::string m_address;      ///< Business Address

    public:
        /**
         * @brief Default constructor.
         */
        Supplier();

        /**
         * @brief Parameterized constructor.
         * @throws std::invalid_argument if parameters are invalid (empty fields, invalid email or invalid phone).
         */
        Supplier(const std::string& id, const std::string& name, const std::string& phone, const std::string& email, const std::string& address);

        // Getters
        std::string getSupplierID() const;
        std::string getName() const;
        std::string getPhone() const;
        std::string getEmail() const;
        std::string getAddress() const;

        // Setters (with validation)
        void setSupplierID(const std::string& id);
        void setName(const std::string& name);
        void setPhone(const std::string& phone);
        void setEmail(const std::string& email);
        void setAddress(const std::string& address);

        /**
         * @brief Serializes the supplier object to a single-line delimited string.
         * @return The delimited string.
         */
        std::string toString() const;

        /**
         * @brief Deserializes a single line from the database into a Supplier object.
         * @param line The delimited database line.
         * @return A Supplier object populated with fields.
         * @throws std::runtime_error if parsing fails.
         */
        static Supplier fromString(const std::string& line);

        // Operator Overloads
        /**
         * @brief Overloaded equality operator comparing IDs.
         */
        bool operator==(const Supplier& other) const;

        /**
         * @brief Overloaded inequality operator comparing IDs.
         */
        bool operator!=(const Supplier& other) const;

        /**
         * @brief Overloaded less-than operator comparing names alphabetically.
         */
        bool operator<(const Supplier& other) const;
    };
}

#endif // SUPPLIER_H
