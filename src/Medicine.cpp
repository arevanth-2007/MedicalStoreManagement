/**
 * @file Medicine.cpp
 * @brief Implementation of the Medicine class.
 */

#include "Medicine.h"
#include "Utility.h"
#include <sstream>
#include <iomanip>
#include <stdexcept>

namespace MedicalStore {

    Medicine::Medicine() 
        : m_price(0.0), m_quantity(0) {}

    Medicine::Medicine(const std::string& id, const std::string& name, const std::string& company,
                       const std::string& category, const std::string& batch, const std::string& mfg,
                       const std::string& expiry, double price, int qty, const std::string& desc) {
        setMedicineID(id);
        setName(name);
        setCompany(company);
        setCategory(category);
        setBatchNumber(batch);
        setMfgDate(mfg);
        setExpiryDate(expiry);
        setPrice(price);
        setQuantity(qty);
        setDescription(desc);
    }

    // Getters
    std::string Medicine::getMedicineID() const { return m_medicineID; }
    std::string Medicine::getName() const { return m_name; }
    std::string Medicine::getCompany() const { return m_company; }
    std::string Medicine::getCategory() const { return m_category; }
    std::string Medicine::getBatchNumber() const { return m_batchNumber; }
    std::string Medicine::getMfgDate() const { return m_mfgDate; }
    std::string Medicine::getExpiryDate() const { return m_expiryDate; }
    double Medicine::getPrice() const { return m_price; }
    int Medicine::getQuantity() const { return m_quantity; }
    std::string Medicine::getDescription() const { return m_description; }

    // Setters (with validation)
    void Medicine::setMedicineID(const std::string& id) {
        if (id.empty()) {
            throw std::invalid_argument("Medicine ID cannot be empty.");
        }
        m_medicineID = id;
    }

    void Medicine::setName(const std::string& name) {
        if (name.empty()) {
            throw std::invalid_argument("Medicine name cannot be empty.");
        }
        m_name = name;
    }

    void Medicine::setCompany(const std::string& company) {
        if (company.empty()) {
            throw std::invalid_argument("Company name cannot be empty.");
        }
        m_company = company;
    }

    void Medicine::setCategory(const std::string& category) {
        if (category.empty()) {
            throw std::invalid_argument("Category cannot be empty.");
        }
        m_category = category;
    }

    void Medicine::setBatchNumber(const std::string& batch) {
        if (batch.empty()) {
            throw std::invalid_argument("Batch number cannot be empty.");
        }
        m_batchNumber = batch;
    }

    void Medicine::setMfgDate(const std::string& mfg) {
        if (!Utility::isValidDate(mfg)) {
            throw std::invalid_argument("Invalid manufacturing date format (must be YYYY-MM-DD and a valid date).");
        }
        if (!m_expiryDate.empty() && Utility::compareDates(mfg, m_expiryDate) > 0) {
            throw std::invalid_argument("Manufacturing date cannot be after expiry date.");
        }
        m_mfgDate = mfg;
    }

    void Medicine::setExpiryDate(const std::string& expiry) {
        if (!Utility::isValidDate(expiry)) {
            throw std::invalid_argument("Invalid expiry date format (must be YYYY-MM-DD and a valid date).");
        }
        if (!m_mfgDate.empty() && Utility::compareDates(m_mfgDate, expiry) > 0) {
            throw std::invalid_argument("Expiry date cannot be before manufacturing date.");
        }
        m_expiryDate = expiry;
    }

    void Medicine::setPrice(double price) {
        if (price <= 0.0) {
            throw std::invalid_argument("Price must be greater than zero.");
        }
        m_price = price;
    }

    void Medicine::setQuantity(int qty) {
        if (qty < 0) {
            throw std::invalid_argument("Quantity cannot be negative.");
        }
        m_quantity = qty;
    }

    void Medicine::setDescription(const std::string& desc) {
        // Description can be empty, so no empty validation
        m_description = desc;
    }

    std::string Medicine::toString() const {
        std::stringstream ss;
        ss << m_medicineID << "|"
           << m_name << "|"
           << m_company << "|"
           << m_category << "|"
           << m_batchNumber << "|"
           << m_mfgDate << "|"
           << m_expiryDate << "|"
           << std::fixed << std::setprecision(2) << m_price << "|"
           << m_quantity << "|"
           << m_description;
        return ss.str();
    }

    Medicine Medicine::fromString(const std::string& line) {
        if (line.empty()) {
            throw std::runtime_error("Empty database line encountered.");
        }

        std::stringstream ss(line);
        std::string id, name, company, category, batch, mfg, expiry, priceStr, qtyStr, desc;

        std::getline(ss, id, '|');
        std::getline(ss, name, '|');
        std::getline(ss, company, '|');
        std::getline(ss, category, '|');
        std::getline(ss, batch, '|');
        std::getline(ss, mfg, '|');
        std::getline(ss, expiry, '|');
        std::getline(ss, priceStr, '|');
        std::getline(ss, qtyStr, '|');
        std::getline(ss, desc, '|');

        double price = 0.0;
        int qty = 0;

        try {
            price = priceStr.empty() ? 0.0 : std::stod(priceStr);
            qty = qtyStr.empty() ? 0 : std::stoi(qtyStr);
        } catch (const std::exception& e) {
            throw std::runtime_error("Failed to parse numeric fields in Medicine: " + std::string(e.what()));
        }

        return Medicine(id, name, company, category, batch, mfg, expiry, price, qty, desc);
    }

    // Operator Overloads
    bool Medicine::operator==(const Medicine& other) const {
        return m_medicineID == other.m_medicineID;
    }

    bool Medicine::operator!=(const Medicine& other) const {
        return !(*this == other);
    }

    bool Medicine::operator<(const Medicine& other) const {
        return m_name < other.m_name;
    }
}
