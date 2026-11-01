/**
 * @file Supplier.cpp
 * @brief Implementation of the Supplier class.
 */

#include "Supplier.h"
#include <sstream>
#include <stdexcept>
#include <cctype>

namespace MedicalStore {

    Supplier::Supplier() {}

    Supplier::Supplier(const std::string& id, const std::string& name, const std::string& phone, const std::string& email, const std::string& address) {
        setSupplierID(id);
        setName(name);
        setPhone(phone);
        setEmail(email);
        setAddress(address);
    }

    // Getters
    std::string Supplier::getSupplierID() const { return m_supplierID; }
    std::string Supplier::getName() const { return m_name; }
    std::string Supplier::getPhone() const { return m_phone; }
    std::string Supplier::getEmail() const { return m_email; }
    std::string Supplier::getAddress() const { return m_address; }

    // Setters (with validation)
    void Supplier::setSupplierID(const std::string& id) {
        if (id.empty()) {
            throw std::invalid_argument("Supplier ID cannot be empty.");
        }
        m_supplierID = id;
    }

    void Supplier::setName(const std::string& name) {
        if (name.empty()) {
            throw std::invalid_argument("Supplier name cannot be empty.");
        }
        m_name = name;
    }

    void Supplier::setPhone(const std::string& phone) {
        if (phone.length() != 10) {
            throw std::invalid_argument("Supplier phone number must be exactly 10 digits.");
        }
        for (char c : phone) {
            if (!std::isdigit(static_cast<unsigned char>(c))) {
                throw std::invalid_argument("Supplier phone number must contain only numeric digits.");
            }
        }
        m_phone = phone;
    }

    void Supplier::setEmail(const std::string& email) {
        if (email.empty()) {
            throw std::invalid_argument("Supplier email cannot be empty.");
        }
        size_t atPos = email.find('@');
        size_t dotPos = email.find('.', atPos);
        if (atPos == std::string::npos || dotPos == std::string::npos || atPos == 0 || dotPos <= atPos + 1 || dotPos == email.length() - 1) {
            throw std::invalid_argument("Invalid email format (must contain '@' and '.' in valid sequence).");
        }
        m_email = email;
    }

    void Supplier::setAddress(const std::string& address) {
        if (address.empty()) {
            throw std::invalid_argument("Supplier address cannot be empty.");
        }
        m_address = address;
    }

    std::string Supplier::toString() const {
        std::stringstream ss;
        ss << m_supplierID << "|"
           << m_name << "|"
           << m_phone << "|"
           << m_email << "|"
           << m_address;
        return ss.str();
    }

    Supplier Supplier::fromString(const std::string& line) {
        if (line.empty()) {
            throw std::runtime_error("Empty database line encountered.");
        }

        std::stringstream ss(line);
        std::string id, name, phone, email, address;

        std::getline(ss, id, '|');
        std::getline(ss, name, '|');
        std::getline(ss, phone, '|');
        std::getline(ss, email, '|');
        std::getline(ss, address, '|');

        return Supplier(id, name, phone, email, address);
    }

    // Operator Overloads
    bool Supplier::operator==(const Supplier& other) const {
        return m_supplierID == other.m_supplierID;
    }

    bool Supplier::operator!=(const Supplier& other) const {
        return !(*this == other);
    }

    bool Supplier::operator<(const Supplier& other) const {
        return m_name < other.m_name;
    }
}
