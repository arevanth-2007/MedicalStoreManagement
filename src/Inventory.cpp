/**
 * @file Inventory.cpp
 * @brief Implementation of the Inventory management class.
 */

#include "Inventory.h"
#include "FileManager.h"
#include "Utility.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <stdexcept>

namespace MedicalStore {

    Inventory::Inventory(const std::string& dbDir) {
        m_dbDir = dbDir;
        m_medDbPath = dbDir + "/MedicineDatabase.txt";
        m_custDbPath = dbDir + "/CustomerDatabase.txt";
        m_suppDbPath = dbDir + "/SupplierDatabase.txt";
    }

    void Inventory::loadAllFromDisk() {
        FileManager::ensureDirectoryExists(m_dbDir);

        // Load Medicines
        m_medicines.clear();
        if (FileManager::fileExists(m_medDbPath)) {
            std::vector<std::string> lines = FileManager::readLines(m_medDbPath);
            for (const auto& line : lines) {
                if (!line.empty()) {
                    try {
                        m_medicines.push_back(Medicine::fromString(line));
                    } catch (const std::exception& e) {
                        std::cerr << "Database Load Warning: Skipping corrupted Medicine line. " << e.what() << "\n";
                    }
                }
            }
        }

        // Load Customers
        m_customers.clear();
        if (FileManager::fileExists(m_custDbPath)) {
            std::vector<std::string> lines = FileManager::readLines(m_custDbPath);
            for (const auto& line : lines) {
                if (!line.empty()) {
                    try {
                        m_customers.push_back(Customer::fromString(line));
                    } catch (const std::exception& e) {
                        std::cerr << "Database Load Warning: Skipping corrupted Customer line. " << e.what() << "\n";
                    }
                }
            }
        }

        // Load Suppliers
        m_suppliers.clear();
        if (FileManager::fileExists(m_suppDbPath)) {
            std::vector<std::string> lines = FileManager::readLines(m_suppDbPath);
            for (const auto& line : lines) {
                if (!line.empty()) {
                    try {
                        m_suppliers.push_back(Supplier::fromString(line));
                    } catch (const std::exception& e) {
                        std::cerr << "Database Load Warning: Skipping corrupted Supplier line. " << e.what() << "\n";
                    }
                }
            }
        }

        // Generate sample data if they are all empty
        generateSampleDataIfEmpty();
    }

    void Inventory::saveAllToDisk() {
        FileManager::ensureDirectoryExists(m_dbDir);

        // Save Medicines
        std::vector<std::string> medLines;
        for (const auto& med : m_medicines) {
            medLines.push_back(med.toString());
        }
        FileManager::writeLines(m_medDbPath, medLines);

        // Save Customers
        std::vector<std::string> custLines;
        for (const auto& cust : m_customers) {
            custLines.push_back(cust.toString());
        }
        FileManager::writeLines(m_custDbPath, custLines);

        // Save Suppliers
        std::vector<std::string> suppLines;
        for (const auto& supp : m_suppliers) {
            suppLines.push_back(supp.toString());
        }
        FileManager::writeLines(m_suppDbPath, suppLines);
    }

    void Inventory::generateSampleDataIfEmpty() {
        bool anyAdded = false;

        // Populate medicines if empty
        if (m_medicines.empty()) {
            m_medicines.push_back(Medicine("MED001", "Paracetamol", "GSK", "Tablet", "B101", "2026-01-01", "2027-12-31", 15.50, 150, "Pain reliever & fever reducer"));
            m_medicines.push_back(Medicine("MED002", "Amoxicillin", "Abbott", "Capsule", "B202", "2025-06-15", "2026-12-31", 45.00, 5, "Antibiotic (Low stock alert)"));
            m_medicines.push_back(Medicine("MED003", "Cough Syrup", "Pfizer", "Syrup", "B303", "2024-01-10", "2025-12-31", 85.00, 80, "Relief for cough (Expired medicine)"));
            m_medicines.push_back(Medicine("MED004", "Aspirin", "Bayer", "Tablet", "B404", "2025-02-20", "2026-08-30", 22.00, 120, "Blood thinner and pain killer (Near expiry)"));
            m_medicines.push_back(Medicine("MED005", "Lipitor", "Viatris", "Tablet", "B505", "2025-03-01", "2028-03-01", 120.00, 0, "Cholesterol lowering (Out of stock)"));
            anyAdded = true;
        }

        // Populate customers if empty
        if (m_customers.empty()) {
            m_customers.push_back(Customer("CUST001", "John Doe", "9876543210", "123 Baker Street, London"));
            m_customers.push_back(Customer("CUST002", "Jane Smith", "8765432109", "456 Elm Street, New York"));
            m_customers.push_back(Customer("CUST003", "Robert Johnson", "7654321098", "789 Oak Avenue, Chicago"));
            anyAdded = true;
        }

        // Populate suppliers if empty
        if (m_suppliers.empty()) {
            m_suppliers.push_back(Supplier("SUPP001", "PharmaDistributors", "9998887776", "info@pharmadist.com", "Industrial Area, Sector 5"));
            m_suppliers.push_back(Supplier("SUPP002", "MedLife Wholesale", "8887776665", "sales@medlifewholesale.com", "Trade Center, Tower B"));
            m_suppliers.push_back(Supplier("SUPP003", "Global Health Inc", "7776665554", "contact@globalhealth.com", "Biotech Park, Building 3"));
            anyAdded = true;
        }

        if (anyAdded) {
            saveAllToDisk();
        }
    }

    // --- Medicine CRUD Operations ---

    bool Inventory::addMedicine(const Medicine& med) {
        if (findMedicineByID(med.getMedicineID()) != nullptr) {
            return false; // Duplicate ID
        }
        m_medicines.push_back(med);
        saveAllToDisk();
        return true;
    }

    bool Inventory::updateMedicine(const std::string& id, const Medicine& updatedMed) {
        for (auto& med : m_medicines) {
            if (med.getMedicineID() == id) {
                try {
                    med.setName(updatedMed.getName());
                    med.setCompany(updatedMed.getCompany());
                    med.setCategory(updatedMed.getCategory());
                    med.setBatchNumber(updatedMed.getBatchNumber());
                    med.setMfgDate(updatedMed.getMfgDate());
                    med.setExpiryDate(updatedMed.getExpiryDate());
                    med.setPrice(updatedMed.getPrice());
                    med.setQuantity(updatedMed.getQuantity());
                    med.setDescription(updatedMed.getDescription());
                    saveAllToDisk();
                    return true;
                } catch (const std::exception& e) {
                    std::cerr << "Update Error: Failed to update Medicine. " << e.what() << "\n";
                    return false;
                }
            }
        }
        return false;
    }

    bool Inventory::deleteMedicine(const std::string& id) {
        auto it = std::remove_if(m_medicines.begin(), m_medicines.end(), [&](const Medicine& med) {
            return med.getMedicineID() == id;
        });

        if (it != m_medicines.end()) {
            m_medicines.erase(it, m_medicines.end());
            saveAllToDisk();
            return true;
        }
        return false;
    }

    const Medicine* Inventory::findMedicineByID(const std::string& id) const {
        for (const auto& med : m_medicines) {
            if (med.getMedicineID() == id) {
                return &med;
            }
        }
        return nullptr;
    }

    Medicine* Inventory::findMedicineByIDMutable(const std::string& id) {
        for (auto& med : m_medicines) {
            if (med.getMedicineID() == id) {
                return &med;
            }
        }
        return nullptr;
    }

    std::vector<Medicine> Inventory::searchMedicines(const std::string& query, int searchType) const {
        std::vector<Medicine> results;
        std::string lowerQuery = query;
        std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);

        for (const auto& med : m_medicines) {
            std::string matchField;
            switch (searchType) {
                case 1: // ID
                    matchField = med.getMedicineID();
                    break;
                case 2: // Name
                    matchField = med.getName();
                    break;
                case 3: // Company
                    matchField = med.getCompany();
                    break;
                case 4: // Category
                    matchField = med.getCategory();
                    break;
                default:
                    continue;
            }
            std::transform(matchField.begin(), matchField.end(), matchField.begin(), ::tolower);

            if (matchField.find(lowerQuery) != std::string::npos) {
                results.push_back(med);
            }
        }
        return results;
    }

    void Inventory::sortMedicines(int criteria) {
        switch (criteria) {
            case 1: // Sort by Name
                std::sort(m_medicines.begin(), m_medicines.end()); // uses overloaded operator<
                break;
            case 2: // Sort by Price
                std::sort(m_medicines.begin(), m_medicines.end(), [](const Medicine& a, const Medicine& b) {
                    return a.getPrice() < b.getPrice();
                });
                break;
            case 3: // Sort by Quantity
                std::sort(m_medicines.begin(), m_medicines.end(), [](const Medicine& a, const Medicine& b) {
                    return a.getQuantity() < b.getQuantity();
                });
                break;
            case 4: // Sort by Company
                std::sort(m_medicines.begin(), m_medicines.end(), [](const Medicine& a, const Medicine& b) {
                    return a.getCompany() < b.getCompany();
                });
                break;
            case 5: // Sort by Expiry Date
                std::sort(m_medicines.begin(), m_medicines.end(), [](const Medicine& a, const Medicine& b) {
                    return Utility::compareDates(a.getExpiryDate(), b.getExpiryDate()) < 0;
                });
                break;
            default:
                break;
        }
        saveAllToDisk();
    }

    void Inventory::viewAllMedicines() const {
        if (m_medicines.empty()) {
            Utility::printColor("No medicines found in the system.\n", Utility::YELLOW);
            return;
        }

        std::vector<int> colWidths = { 8, 18, 12, 10, 8, 6, 12 };
        Utility::printTableBorder(colWidths);
        Utility::printTableRow({ "ID", "Name", "Company", "Category", "Price", "Qty", "Expiry" }, colWidths);
        Utility::printTableBorder(colWidths);

        for (const auto& med : m_medicines) {
            std::stringstream priceStr;
            priceStr << std::fixed << std::setprecision(2) << med.getPrice();
            
            std::vector<std::string> row = {
                med.getMedicineID(),
                med.getName(),
                med.getCompany(),
                med.getCategory(),
                priceStr.str(),
                std::to_string(med.getQuantity()),
                med.getExpiryDate()
            };

            if (Utility::isExpired(med.getExpiryDate())) {
                std::cout << Utility::RED;
            } else if (med.getQuantity() == 0) {
                std::cout << Utility::RED;
            } else if (med.getQuantity() < 10 || Utility::isNearExpiry(med.getExpiryDate())) {
                std::cout << Utility::YELLOW;
            }
            
            Utility::printTableRow(row, colWidths);
            std::cout << Utility::RESET;
        }
        Utility::printTableBorder(colWidths);
    }

    // --- Customer CRUD Operations ---

    bool Inventory::addCustomer(const Customer& cust) {
        if (findCustomerByID(cust.getCustomerID()) != nullptr) {
            return false; // Duplicate ID
        }
        m_customers.push_back(cust);
        saveAllToDisk();
        return true;
    }

    bool Inventory::updateCustomer(const std::string& id, const Customer& updatedCust) {
        for (auto& cust : m_customers) {
            if (cust.getCustomerID() == id) {
                try {
                    cust.setName(updatedCust.getName());
                    cust.setMobileNumber(updatedCust.getMobileNumber());
                    cust.setAddress(updatedCust.getAddress());
                    saveAllToDisk();
                    return true;
                } catch (const std::exception& e) {
                    std::cerr << "Update Error: Failed to update Customer. " << e.what() << "\n";
                    return false;
                }
            }
        }
        return false;
    }

    bool Inventory::deleteCustomer(const std::string& id) {
        auto it = std::remove_if(m_customers.begin(), m_customers.end(), [&](const Customer& cust) {
            return cust.getCustomerID() == id;
        });

        if (it != m_customers.end()) {
            m_customers.erase(it, m_customers.end());
            saveAllToDisk();
            return true;
        }
        return false;
    }

    const Customer* Inventory::findCustomerByID(const std::string& id) const {
        for (const auto& cust : m_customers) {
            if (cust.getCustomerID() == id) {
                return &cust;
            }
        }
        return nullptr;
    }

    Customer* Inventory::findCustomerByIDMutable(const std::string& id) {
        for (auto& cust : m_customers) {
            if (cust.getCustomerID() == id) {
                return &cust;
            }
        }
        return nullptr;
    }

    std::vector<Customer> Inventory::searchCustomers(const std::string& query, int searchType) const {
        std::vector<Customer> results;
        std::string lowerQuery = query;
        std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);

        for (const auto& cust : m_customers) {
            std::string matchField;
            switch (searchType) {
                case 1: // ID
                    matchField = cust.getCustomerID();
                    break;
                case 2: // Name
                    matchField = cust.getName();
                    break;
                case 3: // Mobile Number
                    matchField = cust.getMobileNumber();
                    break;
                default:
                    continue;
            }
            std::transform(matchField.begin(), matchField.end(), matchField.begin(), ::tolower);

            if (matchField.find(lowerQuery) != std::string::npos) {
                results.push_back(cust);
            }
        }
        return results;
    }

    void Inventory::viewAllCustomers() const {
        if (m_customers.empty()) {
            Utility::printColor("No customers found in the system.\n", Utility::YELLOW);
            return;
        }

        std::vector<int> colWidths = { 10, 20, 15, 30 };
        Utility::printTableBorder(colWidths);
        Utility::printTableRow({ "ID", "Name", "Mobile No", "Address" }, colWidths);
        Utility::printTableBorder(colWidths);

        for (const auto& cust : m_customers) {
            Utility::printTableRow({
                cust.getCustomerID(),
                cust.getName(),
                cust.getMobileNumber(),
                cust.getAddress()
            }, colWidths);
        }
        Utility::printTableBorder(colWidths);
    }

    // --- Supplier CRUD Operations ---

    bool Inventory::addSupplier(const Supplier& supp) {
        if (findSupplierByID(supp.getSupplierID()) != nullptr) {
            return false; // Duplicate ID
        }
        m_suppliers.push_back(supp);
        saveAllToDisk();
        return true;
    }

    bool Inventory::updateSupplier(const std::string& id, const Supplier& updatedSupp) {
        for (auto& supp : m_suppliers) {
            if (supp.getSupplierID() == id) {
                try {
                    supp.setName(updatedSupp.getName());
                    supp.setPhone(updatedSupp.getPhone());
                    supp.setEmail(updatedSupp.getEmail());
                    supp.setAddress(updatedSupp.getAddress());
                    saveAllToDisk();
                    return true;
                } catch (const std::exception& e) {
                    std::cerr << "Update Error: Failed to update Supplier. " << e.what() << "\n";
                    return false;
                }
            }
        }
        return false;
    }

    bool Inventory::deleteSupplier(const std::string& id) {
        auto it = std::remove_if(m_suppliers.begin(), m_suppliers.end(), [&](const Supplier& supp) {
            return supp.getSupplierID() == id;
        });

        if (it != m_suppliers.end()) {
            m_suppliers.erase(it, m_suppliers.end());
            saveAllToDisk();
            return true;
        }
        return false;
    }

    const Supplier* Inventory::findSupplierByID(const std::string& id) const {
        for (const auto& supp : m_suppliers) {
            if (supp.getSupplierID() == id) {
                return &supp;
            }
        }
        return nullptr;
    }

    std::vector<Supplier> Inventory::searchSuppliers(const std::string& query, int searchType) const {
        std::vector<Supplier> results;
        std::string lowerQuery = query;
        std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);

        for (const auto& supp : m_suppliers) {
            std::string matchField;
            switch (searchType) {
                case 1: // ID
                    matchField = supp.getSupplierID();
                    break;
                case 2: // Name
                    matchField = supp.getName();
                    break;
                case 3: // Phone
                    matchField = supp.getPhone();
                    break;
                default:
                    continue;
            }
            std::transform(matchField.begin(), matchField.end(), matchField.begin(), ::tolower);

            if (matchField.find(lowerQuery) != std::string::npos) {
                results.push_back(supp);
            }
        }
        return results;
    }

    void Inventory::viewAllSuppliers() const {
        if (m_suppliers.empty()) {
            Utility::printColor("No suppliers found in the system.\n", Utility::YELLOW);
            return;
        }

        std::vector<int> colWidths = { 10, 20, 15, 25, 25 };
        Utility::printTableBorder(colWidths);
        Utility::printTableRow({ "ID", "Name", "Phone", "Email", "Address" }, colWidths);
        Utility::printTableBorder(colWidths);

        for (const auto& supp : m_suppliers) {
            Utility::printTableRow({
                supp.getSupplierID(),
                supp.getName(),
                supp.getPhone(),
                supp.getEmail(),
                supp.getAddress()
            }, colWidths);
        }
        Utility::printTableBorder(colWidths);
    }

    // --- Dashboard & Alert Helpers ---

    size_t Inventory::getTotalMedicinesCount() const { return m_medicines.size(); }
    size_t Inventory::getTotalCustomersCount() const { return m_customers.size(); }
    size_t Inventory::getTotalSuppliersCount() const { return m_suppliers.size(); }

    std::vector<Medicine> Inventory::getLowStockMedicines() const {
        std::vector<Medicine> results;
        for (const auto& med : m_medicines) {
            if (med.getQuantity() > 0 && med.getQuantity() < 10) {
                results.push_back(med);
            }
        }
        return results;
    }

    std::vector<Medicine> Inventory::getExpiredMedicines() const {
        std::vector<Medicine> results;
        for (const auto& med : m_medicines) {
            if (Utility::isExpired(med.getExpiryDate())) {
                results.push_back(med);
            }
        }
        return results;
    }

    std::vector<Medicine> Inventory::getNearExpiryMedicines() const {
        std::vector<Medicine> results;
        for (const auto& med : m_medicines) {
            if (Utility::isNearExpiry(med.getExpiryDate())) {
                results.push_back(med);
            }
        }
        return results;
    }

    std::vector<Medicine> Inventory::getOutOfStockMedicines() const {
        std::vector<Medicine> results;
        for (const auto& med : m_medicines) {
            if (med.getQuantity() == 0) {
                results.push_back(med);
            }
        }
        return results;
    }
}
