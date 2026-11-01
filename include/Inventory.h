/**
 * @file Inventory.h
 * @brief Definition of the Inventory class managing medicine, customer, and supplier databases.
 */

#ifndef INVENTORY_H
#define INVENTORY_H

#include "Medicine.h"
#include "Customer.h"
#include "Supplier.h"
#include <vector>
#include <string>

namespace MedicalStore {

    /**
     * @class Inventory
     * @brief Manages the in-memory database vectors and coordinates filesystem synchronization.
     */
    class Inventory {
    private:
        std::vector<Medicine> m_medicines;   ///< All medicine records
        std::vector<Customer> m_customers;   ///< All customer records
        std::vector<Supplier> m_suppliers;   ///< All supplier records

        std::string m_dbDir;                ///< Directory path where database files are stored
        std::string m_medDbPath;            ///< Path to Medicine database file
        std::string m_custDbPath;           ///< Path to Customer database file
        std::string m_suppDbPath;           ///< Path to Supplier database file

    public:
        /**
         * @brief Constructor setting up the database paths.
         * @param dbDir The database directory path.
         */
        Inventory(const std::string& dbDir = "database");

        /**
         * @brief Load all databases from text files.
         */
        void loadAllFromDisk();

        /**
         * @brief Save all databases to text files.
         */
        void saveAllToDisk();

        /**
         * @brief Populates the database with sample data if files are empty.
         */
        void generateSampleDataIfEmpty();

        // --- Medicine CRUD Operations ---
        /**
         * @brief Adds a new medicine to the inventory.
         * @param med The medicine to add.
         * @return True if successful, false if duplicate ID exists.
         */
        bool addMedicine(const Medicine& med);

        /**
         * @brief Updates an existing medicine's details.
         * @param id The ID of the medicine to update.
         * @param updatedMed The updated medicine object.
         * @return True if successful.
         */
        bool updateMedicine(const std::string& id, const Medicine& updatedMed);

        /**
         * @brief Deletes a medicine from the inventory.
         * @param id The ID of the medicine to delete.
         * @return True if deleted.
         */
        bool deleteMedicine(const std::string& id);

        /**
         * @brief Prints a table of all medicines to the console.
         */
        void viewAllMedicines() const;

        /**
         * @brief Finds a medicine by its unique ID.
         * @param id The medicine ID.
         * @return Pointer to const Medicine, or nullptr if not found.
         */
        const Medicine* findMedicineByID(const std::string& id) const;

        /**
         * @brief Finds a medicine by ID and returns a mutable pointer.
         * @param id The medicine ID.
         * @return Pointer to Medicine, or nullptr if not found.
         */
        Medicine* findMedicineByIDMutable(const std::string& id);

        /**
         * @brief Searches medicines based on criteria (1: ID, 2: Name, 3: Company, 4: Category).
         * @param query The search term.
         * @param searchType The search field type.
         * @return Vector of matching medicines.
         */
        std::vector<Medicine> searchMedicines(const std::string& query, int searchType) const;

        /**
         * @brief Sorts medicines based on criteria (1: Name, 2: Price, 3: Quantity, 4: Company, 5: Expiry Date).
         * @param criteria The sorting field code.
         */
        void sortMedicines(int criteria);

        // --- Customer CRUD Operations ---
        /**
         * @brief Registers a new customer in the database.
         * @param cust The customer object to add.
         * @return True if successful, false if duplicate ID exists.
         */
        bool addCustomer(const Customer& cust);

        /**
         * @brief Updates an existing customer's contact details.
         * @param id The ID of the customer to update.
         * @param updatedCust The updated customer object.
         * @return True if successful.
         */
        bool updateCustomer(const std::string& id, const Customer& updatedCust);

        /**
         * @brief Deletes a customer profile from the database.
         * @param id The ID of the customer to delete.
         * @return True if deleted.
         */
        bool deleteCustomer(const std::string& id);

        /**
         * @brief Prints a table of all customers.
         */
        void viewAllCustomers() const;

        /**
         * @brief Finds a customer by ID.
         * @param id The customer ID.
         * @return Pointer to const Customer, or nullptr if not found.
         */
        const Customer* findCustomerByID(const std::string& id) const;

        /**
         * @brief Finds a customer by ID and returns a mutable pointer.
         * @param id The customer ID.
         * @return Pointer to Customer, or nullptr if not found.
         */
        Customer* findCustomerByIDMutable(const std::string& id);

        /**
         * @brief Searches customers based on criteria (1: ID, 2: Name, 3: Mobile).
         * @param query The search keyword.
         * @param searchType The search criteria index.
         * @return Vector of matching customers.
         */
        std::vector<Customer> searchCustomers(const std::string& query, int searchType) const;

        // --- Supplier CRUD Operations ---
        /**
         * @brief Registers a new supplier in the system.
         * @param supp The supplier object to add.
         * @return True if successful, false if duplicate ID exists.
         */
        bool addSupplier(const Supplier& supp);

        /**
         * @brief Updates supplier records.
         * @param id The ID of the supplier.
         * @param updatedSupp The updated supplier object.
         * @return True if successful.
         */
        bool updateSupplier(const std::string& id, const Supplier& updatedSupp);

        /**
         * @brief Removes a supplier profile.
         * @param id The ID of the supplier.
         * @return True if deleted.
         */
        bool deleteSupplier(const std::string& id);

        /**
         * @brief Prints a table of all wholesale suppliers.
         */
        void viewAllSuppliers() const;

        /**
         * @brief Finds a supplier by ID.
         * @param id The supplier ID.
         * @return Pointer to const Supplier, or nullptr if not found.
         */
        const Supplier* findSupplierByID(const std::string& id) const;

        /**
         * @brief Searches suppliers based on criteria (1: ID, 2: Name, 3: Phone).
         * @param query The search keyword.
         * @param searchType The search field index.
         * @return Vector of matching suppliers.
         */
        std::vector<Supplier> searchSuppliers(const std::string& query, int searchType) const;

        // --- Metrics & Alert Accessors ---
        size_t getTotalMedicinesCount() const;
        size_t getTotalCustomersCount() const;
        size_t getTotalSuppliersCount() const;

        std::vector<Medicine> getLowStockMedicines() const;
        std::vector<Medicine> getExpiredMedicines() const;
        std::vector<Medicine> getNearExpiryMedicines() const;
        std::vector<Medicine> getOutOfStockMedicines() const;

        const std::vector<Medicine>& getMedicinesList() const { return m_medicines; }
        const std::vector<Customer>& getCustomersList() const { return m_customers; }
        const std::vector<Supplier>& getSuppliersList() const { return m_suppliers; }
        
        std::string getDbDirectory() const { return m_dbDir; }
    };
}

#endif // INVENTORY_H
