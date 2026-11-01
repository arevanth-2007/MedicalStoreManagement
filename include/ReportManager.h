/**
 * @file ReportManager.h
 * @brief Definition of the ReportManager class responsible for creating business reports.
 */

#ifndef REPORT_MANAGER_H
#define REPORT_MANAGER_H

#include "Inventory.h"
#include <string>
#include <vector>

namespace MedicalStore {

    /**
     * @struct SaleRecord
     * @brief Represents a single logged transaction extracted from the Sales Database.
     */
    struct SaleRecord {
        std::string invoiceNum;   ///< Unique ID of invoice
        std::string dateTime;     ///< Time of sale
        std::string customerName; ///< Customer who purchased
        double subtotal;          ///< Pre-tax price
        double gstPercent;        ///< GST percentage applied
        double grandTotal;        ///< Net paid price
        std::string itemDetails;  ///< Raw item details (ID:qty;ID:qty...)
    };

    /**
     * @class ReportManager
     * @brief Formats system databases and analytics into user-friendly console summaries and saves them as files.
     */
    class ReportManager {
    private:
        Inventory& m_inventory;       ///< Shared Inventory reference
        std::string m_salesDbPath;    ///< Path to SalesDatabase.txt
        std::string m_reportsDir;     ///< Path to reports output folder

        /**
         * @brief Parses the Sales database text file into a vector of SaleRecords.
         * @return A vector of parsed sales.
         */
        std::vector<SaleRecord> parseSalesDb() const;

    public:
        /**
         * @brief Constructor.
         * @param inventory Reference to system Inventory.
         * @param salesDbPath Path to the SalesDatabase.txt file.
         * @param reportsDir Folder path where reports are written to disk.
         */
        ReportManager(Inventory& inventory,
                      const std::string& salesDbPath = "database/SalesDatabase.txt",
                      const std::string& reportsDir = "reports");

        /**
         * @brief Generates a comprehensive Medicine list report.
         */
        void generateMedicineReport() const;

        /**
         * @brief Generates a detailed Customer profile and purchase summary report.
         */
        void generateCustomerReport() const;

        /**
         * @brief Generates a Supplier contact and directory report.
         */
        void generateSupplierReport() const;

        /**
         * @brief Generates an Inventory audit report highlighting low stock and expired medicines.
         */
        void generateInventoryReport() const;

        /**
         * @brief Generates a sales and revenue audit report.
         */
        void generateSalesReport() const;

        // --- Live Stats Helpers for Dashboard ---
        /**
         * @brief Calculates and returns today's accumulated revenue.
         * @return The revenue amount.
         */
        double getTodayRevenue() const;

        /**
         * @brief Calculates and returns cumulative net revenue.
         * @return The cumulative revenue.
         */
        double getTotalRevenue() const;

        /**
         * @brief Returns the total sales transaction count.
         * @return The count.
         */
        size_t getTotalSalesCount() const;
    };
}

#endif // REPORT_MANAGER_H
