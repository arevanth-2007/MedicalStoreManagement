/**
 * @file Customer.cpp
 * @brief Implementation of the Customer class.
 */

#include "Customer.h"
#include <sstream>
#include <stdexcept>
#include <cctype>

namespace MedicalStore {

    Customer::Customer() {}

    Customer::Customer(const std::string& id, const std::string& name, const std::string& mobile, const std::string& address) {
        setCustomerID(id);
        setName(name);
        setMobileNumber(mobile);
        setAddress(address);
    }

    // Getters
    std::string Customer::getCustomerID() const { return m_customerID; }
    std::string Customer::getName() const { return m_name; }
    std::string Customer::getMobileNumber() const { return m_mobileNumber; }
    std::string Customer::getAddress() const { return m_address; }
    std::vector<std::string> Customer::getPurchaseHistory() const { return m_purchaseHistory; }

    // Setters (with validation)
    void Customer::setCustomerID(const std::string& id) {
        if (id.empty()) {
            throw std::invalid_argument("Customer ID cannot be empty.");
        }
        m_customerID = id;
    }

    void Customer::setName(const std::string& name) {
        if (name.empty()) {
            throw std::invalid_argument("Customer name cannot be empty.");
        }
        m_name = name;
    }

    void Customer::setMobileNumber(const std::string& mobile) {
        if (mobile.length() != 10) {
            throw std::invalid_argument("Mobile number must be exactly 10 digits.");
        }
        for (char c : mobile) {
            if (!std::isdigit(static_cast<unsigned char>(c))) {
                throw std::invalid_argument("Mobile number must contain only numeric digits.");
            }
        }
        m_mobileNumber = mobile;
    }

    void Customer::setAddress(const std::string& address) {
        if (address.empty()) {
            throw std::invalid_argument("Address cannot be empty.");
        }
        m_address = address;
    }

    void Customer::setPurchaseHistory(const std::vector<std::string>& history) {
        m_purchaseHistory = history;
    }

    void Customer::addPurchase(const std::string& invoiceSummary) {
        m_purchaseHistory.push_back(invoiceSummary);
    }

    std::string Customer::toString() const {
        std::stringstream ss;
        ss << m_customerID << "|"
           << m_name << "|"
           << m_mobileNumber << "|"
           << m_address << "|";
        
        for (size_t i = 0; i < m_purchaseHistory.size(); ++i) {
            ss << m_purchaseHistory[i];
            if (i + 1 < m_purchaseHistory.size()) {
                ss << ";";
            }
        }
        return ss.str();
    }

    Customer Customer::fromString(const std::string& line) {
        if (line.empty()) {
            throw std::runtime_error("Empty database line encountered.");
        }

        std::stringstream ss(line);
        std::string id, name, mobile, address, historyField;

        std::getline(ss, id, '|');
        std::getline(ss, name, '|');
        std::getline(ss, mobile, '|');
        std::getline(ss, address, '|');
        std::getline(ss, historyField, '|');

        Customer cust(id, name, mobile, address);

        if (!historyField.empty()) {
            std::stringstream histStream(historyField);
            std::string entry;
            while (std::getline(histStream, entry, ';')) {
                if (!entry.empty()) {
                    cust.addPurchase(entry);
                }
            }
        }

        return cust;
    }

    // Operator Overloads
    bool Customer::operator==(const Customer& other) const {
        return m_customerID == other.m_customerID;
    }

    bool Customer::operator!=(const Customer& other) const {
        return !(*this == other);
    }

    bool Customer::operator<(const Customer& other) const {
        return m_name < other.m_name;
    }
}
