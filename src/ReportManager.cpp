/**
 * @file ReportManager.cpp
 * @brief Implementation of the ReportManager class.
 */

#include "ReportManager.h"
#include "FileManager.h"
#include "Utility.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace MedicalStore {

    ReportManager::ReportManager(Inventory& inventory, const std::string& salesDbPath, const std::string& reportsDir)
        : m_inventory(inventory), m_salesDbPath(salesDbPath), m_reportsDir(reportsDir) {}

    std::vector<SaleRecord> ReportManager::parseSalesDb() const {
        std::vector<SaleRecord> records;
        if (!FileManager::fileExists(m_salesDbPath)) {
            return records;
        }

        std::vector<std::string> lines = FileManager::readLines(m_salesDbPath);
        for (const auto& line : lines) {
            if (line.empty()) continue;

            std::stringstream ss(line);
            SaleRecord r;
            std::string subStr, gstStr, grandStr;

            std::getline(ss, r.invoiceNum, '|');
            std::getline(ss, r.dateTime, '|');
            std::getline(ss, r.customerName, '|');
            std::getline(ss, subStr, '|');
            std::getline(ss, gstStr, '|');
            std::getline(ss, grandStr, '|');
            std::getline(ss, r.itemDetails, '|');

            try {
                r.subtotal = subStr.empty() ? 0.0 : std::stod(subStr);
                r.gstPercent = gstStr.empty() ? 0.0 : std::stod(gstStr);
                r.grandTotal = grandStr.empty() ? 0.0 : std::stod(grandStr);
                records.push_back(r);
            } catch (const std::exception& e) {
                std::cerr << "Report Error: Skipping corrupted sales log entry: " << e.what() << "\n";
            }
        }
        return records;
    }

    double ReportManager::getTodayRevenue() const {
        std::string today = Utility::getCurrentDate();
        std::vector<SaleRecord> records = parseSalesDb();
        double revenue = 0.0;
        
        for (const auto& r : records) {
            if (r.dateTime.length() >= 10 && r.dateTime.substr(0, 10) == today) {
                revenue += r.grandTotal;
            }
        }
        return revenue;
    }

    double ReportManager::getTotalRevenue() const {
        std::vector<SaleRecord> records = parseSalesDb();
        double revenue = 0.0;
        for (const auto& r : records) {
            revenue += r.grandTotal;
        }
        return revenue;
    }

    size_t ReportManager::getTotalSalesCount() const {
        return parseSalesDb().size();
    }

    void ReportManager::generateMedicineReport() const {
        Utility::printHeader("GENERATE MEDICINE REPORT");
        std::string dateStr = Utility::getCurrentDateTime();

        std::vector<std::string> lines;
        lines.push_back("==============================================================================");
        lines.push_back("                         MEDICINE DIRECTORY REPORT                            ");
        lines.push_back("                      Generated: " + dateStr);
        lines.push_back("==============================================================================");
        lines.push_back("");

        const auto& meds = m_inventory.getMedicinesList();
        
        std::stringstream ss;
        ss << std::left << std::setw(10) << "ID"
           << std::setw(20) << "Name"
           << std::setw(15) << "Company"
           << std::setw(12) << "Category"
           << std::right << std::setw(10) << "Price"
           << std::setw(10) << "Quantity";
        lines.push_back(ss.str());
        lines.push_back("------------------------------------------------------------------------------");

        for (const auto& med : meds) {
            ss.str("");
            ss << std::fixed << std::setprecision(2)
               << std::left << std::setw(10) << med.getMedicineID()
               << std::setw(20) << (med.getName().length() > 19 ? med.getName().substr(0, 16) + "..." : med.getName())
               << std::setw(15) << (med.getCompany().length() > 14 ? med.getCompany().substr(0, 11) + "..." : med.getCompany())
               << std::setw(12) << med.getCategory()
               << std::right << std::setw(10) << med.getPrice()
               << std::setw(10) << med.getQuantity();
            lines.push_back(ss.str());
        }
        lines.push_back("==============================================================================");

        // Display to console
        for (const auto& line : lines) {
            std::cout << line << "\n";
        }

        std::string filename = m_reportsDir + "/MedicineReport.txt";
        FileManager::writeLines(filename, lines);
        Utility::printColor("\nSuccess: Report saved to " + filename + "\n", Utility::GREEN);
        Utility::pause();
    }

    void ReportManager::generateCustomerReport() const {
        Utility::printHeader("GENERATE CUSTOMER REPORT");
        std::string dateStr = Utility::getCurrentDateTime();

        std::vector<std::string> lines;
        lines.push_back("==============================================================================");
        lines.push_back("                         CUSTOMER DIRECTORY REPORT                            ");
        lines.push_back("                      Generated: " + dateStr);
        lines.push_back("==============================================================================");
        lines.push_back("");

        const auto& custs = m_inventory.getCustomersList();
        
        std::stringstream ss;
        ss << std::left << std::setw(10) << "ID"
           << std::setw(20) << "Name"
           << std::setw(15) << "Mobile"
           << std::setw(15) << "Purchases Count"
           << std::setw(20) << "Address";
        lines.push_back(ss.str());
        lines.push_back("------------------------------------------------------------------------------");

        for (const auto& cust : custs) {
            ss.str("");
            ss << std::left << std::setw(10) << cust.getCustomerID()
               << std::setw(20) << cust.getName()
               << std::setw(15) << cust.getMobileNumber()
               << std::setw(15) << cust.getPurchaseHistory().size()
               << std::setw(20) << (cust.getAddress().length() > 19 ? cust.getAddress().substr(0, 16) + "..." : cust.getAddress());
            lines.push_back(ss.str());
        }
        lines.push_back("==============================================================================");

        for (const auto& line : lines) {
            std::cout << line << "\n";
        }

        std::string filename = m_reportsDir + "/CustomerReport.txt";
        FileManager::writeLines(filename, lines);
        Utility::printColor("\nSuccess: Report saved to " + filename + "\n", Utility::GREEN);
        Utility::pause();
    }

    void ReportManager::generateSupplierReport() const {
        Utility::printHeader("GENERATE SUPPLIER REPORT");
        std::string dateStr = Utility::getCurrentDateTime();

        std::vector<std::string> lines;
        lines.push_back("==============================================================================");
        lines.push_back("                         SUPPLIER DIRECTORY REPORT                            ");
        lines.push_back("                      Generated: " + dateStr);
        lines.push_back("==============================================================================");
        lines.push_back("");

        const auto& supps = m_inventory.getSuppliersList();
        
        std::stringstream ss;
        ss << std::left << std::setw(10) << "ID"
           << std::setw(22) << "Name"
           << std::setw(15) << "Phone"
           << std::setw(30) << "Email";
        lines.push_back(ss.str());
        lines.push_back("------------------------------------------------------------------------------");

        for (const auto& supp : supps) {
            ss.str("");
            ss << std::left << std::setw(10) << supp.getSupplierID()
               << std::setw(22) << supp.getName()
               << std::setw(15) << supp.getPhone()
               << std::setw(30) << supp.getEmail();
            lines.push_back(ss.str());
        }
        lines.push_back("==============================================================================");

        for (const auto& line : lines) {
            std::cout << line << "\n";
        }

        std::string filename = m_reportsDir + "/SupplierReport.txt";
        FileManager::writeLines(filename, lines);
        Utility::printColor("\nSuccess: Report saved to " + filename + "\n", Utility::GREEN);
        Utility::pause();
    }

    void ReportManager::generateInventoryReport() const {
        Utility::printHeader("GENERATE INVENTORY AUDIT REPORT");
        std::string dateStr = Utility::getCurrentDateTime();

        std::vector<Medicine> lowStock = m_inventory.getLowStockMedicines();
        std::vector<Medicine> expired = m_inventory.getExpiredMedicines();
        std::vector<Medicine> nearExp = m_inventory.getNearExpiryMedicines();
        std::vector<Medicine> outOfStock = m_inventory.getOutOfStockMedicines();

        std::vector<std::string> lines;
        lines.push_back("==============================================================================");
        lines.push_back("                         INVENTORY AUDIT REPORT                               ");
        lines.push_back("                      Generated: " + dateStr);
        lines.push_back("==============================================================================");
        lines.push_back("");

        lines.push_back("SUMMARY STATISTICS:");
        lines.push_back("-------------------");
        lines.push_back("Total Medicine Catalog Size : " + std::to_string(m_inventory.getTotalMedicinesCount()));
        lines.push_back("Out Of Stock Products       : " + std::to_string(outOfStock.size()));
        lines.push_back("Low Stock Products (< 10)   : " + std::to_string(lowStock.size()));
        lines.push_back("Expired Products            : " + std::to_string(expired.size()));
        lines.push_back("Near Expiry Products (3 mo) : " + std::to_string(nearExp.size()));
        lines.push_back("");

        lines.push_back("1. OUT OF STOCK MEDICINES:");
        lines.push_back("--------------------------");
        if (outOfStock.empty()) {
            lines.push_back("No medicines are out of stock.");
        } else {
            for (const auto& med : outOfStock) {
                lines.push_back(" - ID: " + med.getMedicineID() + " | Name: " + med.getName() + " | Company: " + med.getCompany());
            }
        }
        lines.push_back("");

        lines.push_back("2. LOW STOCK MEDICINES (<10 units):");
        lines.push_back("-----------------------------------");
        if (lowStock.empty()) {
            lines.push_back("No medicines are low on stock.");
        } else {
            for (const auto& med : lowStock) {
                lines.push_back(" - ID: " + med.getMedicineID() + " | Name: " + med.getName() + " | Qty Left: " + std::to_string(med.getQuantity()));
            }
        }
        lines.push_back("");

        lines.push_back("3. EXPIRED MEDICINES:");
        lines.push_back("---------------------");
        if (expired.empty()) {
            lines.push_back("No expired medicines found.");
        } else {
            for (const auto& med : expired) {
                lines.push_back(" - ID: " + med.getMedicineID() + " | Name: " + med.getName() + " | Expired On: " + med.getExpiryDate());
            }
        }
        lines.push_back("");

        lines.push_back("4. NEAR EXPIRY MEDICINES (Within 3 Months):");
        lines.push_back("-------------------------------------------");
        if (nearExp.empty()) {
            lines.push_back("No near-expiry medicines found.");
        } else {
            for (const auto& med : nearExp) {
                lines.push_back(" - ID: " + med.getMedicineID() + " | Name: " + med.getName() + " | Expires On: " + med.getExpiryDate());
            }
        }
        lines.push_back("==============================================================================");

        for (const auto& line : lines) {
            std::cout << line << "\n";
        }

        std::string filename = m_reportsDir + "/InventoryReport.txt";
        FileManager::writeLines(filename, lines);
        Utility::printColor("\nSuccess: Report saved to " + filename + "\n", Utility::GREEN);
        Utility::pause();
    }

    void ReportManager::generateSalesReport() const {
        Utility::printHeader("GENERATE SALES & REVENUE REPORT");
        std::string dateStr = Utility::getCurrentDateTime();

        std::vector<SaleRecord> sales = parseSalesDb();
        double totalRev = 0.0;
        double todayRev = 0.0;
        std::string todayDate = Utility::getCurrentDate();

        for (const auto& r : sales) {
            totalRev += r.grandTotal;
            if (r.dateTime.length() >= 10 && r.dateTime.substr(0, 10) == todayDate) {
                todayRev += r.grandTotal;
            }
        }

        std::vector<std::string> lines;
        lines.push_back("==============================================================================");
        lines.push_back("                           SALES AUDIT REPORT                                 ");
        lines.push_back("                      Generated: " + dateStr);
        lines.push_back("==============================================================================");
        lines.push_back("");

        std::stringstream ss;
        ss << std::fixed << std::setprecision(2);
        lines.push_back("FINANCIAL OVERVIEW:");
        lines.push_back("-------------------");
        lines.push_back("Total Invoices Processed : " + std::to_string(sales.size()));
        ss.str("");
        ss << "Today's Revenue          : $" << todayRev;
        lines.push_back(ss.str());
        ss.str("");
        ss << "Cumulative Net Revenue   : $" << totalRev;
        lines.push_back(ss.str());
        lines.push_back("");

        lines.push_back("TRANSACTION LEDGER:");
        lines.push_back("-------------------");
        ss.str("");
        ss << std::left << std::setw(15) << "Invoice No"
           << std::setw(20) << "Date & Time"
           << std::setw(20) << "Customer"
           << std::right << std::setw(15) << "Paid Total";
        lines.push_back(ss.str());
        lines.push_back("------------------------------------------------------------------------------");

        for (const auto& r : sales) {
            ss.str("");
            ss << std::fixed << std::setprecision(2)
               << std::left << std::setw(15) << r.invoiceNum
               << std::setw(20) << r.dateTime
               << std::setw(20) << (r.customerName.length() > 19 ? r.customerName.substr(0, 16) + "..." : r.customerName)
               << std::right << std::setw(15) << r.grandTotal;
            lines.push_back(ss.str());
        }
        lines.push_back("==============================================================================");

        for (const auto& line : lines) {
            std::cout << line << "\n";
        }

        std::string filename = m_reportsDir + "/SalesReport.txt";
        FileManager::writeLines(filename, lines);
        Utility::printColor("\nSuccess: Report saved to " + filename + "\n", Utility::GREEN);
        Utility::pause();
    }
}
