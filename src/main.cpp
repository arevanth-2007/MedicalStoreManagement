/**
 * @file main.cpp
 * @brief Entry point and menu orchestrator for the Medical Store Management System.
 */

#include "Utility.h"
#include "FileManager.h"
#include "Inventory.h"
#include "Billing.h"
#include "ReportManager.h"
#include "Login.h"
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <iomanip>
#include <sstream>

using namespace MedicalStore;

// Function Prototypes for Submenus
void runMedicineMenu(Inventory& inventory);
void runCustomerMenu(Inventory& inventory);
void runSupplierMenu(Inventory& inventory);
void runInventoryAlertsMenu(const Inventory& inventory);
void runReportsMenu(const ReportManager& rm);
void runBackup(const Inventory& inventory);
void runRestore(Inventory& inventory);
void showAboutProject();

/**
 * @brief Main function.
 * @return exit status code.
 */
int main() {
    // 1. Initialize Console & ANSI Color Support
    Utility::initializeConsole();

    // 2. Perform Splash Screen Delay
    Login auth("database/LoginDatabase.txt");
    auth.showSplashScreen();

    // 3. User Authentication Loop (Max 3 attempts)
    if (!auth.authenticate()) {
        return 0; // Terminate if unauthorized
    }

    // 4. Initialize Data Managers
    Inventory inventory("database");
    inventory.loadAllFromDisk(); // Loads files or creates samples if empty

    Billing billing(inventory, "database/SalesDatabase.txt", "reports");
    ReportManager rm(inventory, "database/SalesDatabase.txt", "reports");

    // 5. Main Dashboard & Menu Loop
    while (true) {
        Utility::clearScreen();
        
        // Render Professional ASCII Dashboard Banner
        std::cout << "\n";
        Utility::printColor("==================================================\n", Utility::BLUE);
        Utility::printColor("            ADMINISTRATOR DASHBOARD               \n", Utility::CYAN);
        Utility::printColor("==================================================\n", Utility::BLUE);

        std::cout << std::left << std::setw(30) << "Total Medicines Catalog:" << inventory.getTotalMedicinesCount() << "\n";
        std::cout << std::left << std::setw(30) << "Total Customers Registered:" << inventory.getTotalCustomersCount() << "\n";
        std::cout << std::left << std::setw(30) << "Total Suppliers Directory:" << inventory.getTotalSuppliersCount() << "\n";
        std::cout << std::left << std::setw(30) << "Total Sales Transactions:" << rm.getTotalSalesCount() << "\n";
        
        std::stringstream revStr;
        revStr << std::fixed << std::setprecision(2) << "$" << rm.getTodayRevenue();
        std::cout << std::left << std::setw(30) << "Today's Net Revenue:" << revStr.str() << "\n";

        // Warnings and Alerts Summary
        size_t outCount = inventory.getOutOfStockMedicines().size();
        size_t lowCount = inventory.getLowStockMedicines().size();
        size_t expCount = inventory.getExpiredMedicines().size();
        size_t nearCount = inventory.getNearExpiryMedicines().size();

        if (outCount > 0) {
            std::cout << Utility::RED << "CRITICAL: Out of Stock Medicines: " << outCount << Utility::RESET << "\n";
        }
        if (lowCount > 0) {
            std::cout << Utility::YELLOW << "ALERT: Low Stock Medicines (<10): " << lowCount << Utility::RESET << "\n";
        }
        if (expCount > 0) {
            std::cout << Utility::RED << "WARNING: Expired Medicines: " << expCount << Utility::RESET << "\n";
        }
        if (nearCount > 0) {
            std::cout << Utility::YELLOW << "NOTICE: Near Expiry Medicines (<3 mo): " << nearCount << Utility::RESET << "\n";
        }

        Utility::printColor("==================================================\n", Utility::BLUE);
        std::cout << "\n";
        
        // Main Options UI
        std::cout << "MAIN MENU OPERATIONS:\n";
        std::cout << "1. Medicine Management\n";
        std::cout << "2. Customer Management\n";
        std::cout << "3. Supplier Management\n";
        std::cout << "4. Billing System\n";
        std::cout << "5. Inventory Reports\n";
        std::cout << "6. Business Reports\n";
        std::cout << "7. Backup Database\n";
        std::cout << "8. Restore Database\n";
        std::cout << "9. Change Password\n";
        std::cout << "10. About Project\n";
        std::cout << "11. Exit\n";
        std::cout << "\n";

        int choice = Utility::getInputInt("Select option (1-11): ", 1, 11);

        if (choice == 11) {
            inventory.saveAllToDisk(); // Save before exiting
            Utility::clearScreen();
            std::cout << "\n";
            Utility::printColor("Saving database changes to files...\n", Utility::GREEN);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            Utility::printColor("Thank you for using Medical Store Management System. Goodbye!\n\n", Utility::GREEN);
            break;
        }

        switch (choice) {
            case 1:
                runMedicineMenu(inventory);
                break;
            case 2:
                runCustomerMenu(inventory);
                break;
            case 3:
                runSupplierMenu(inventory);
                break;
            case 4:
                billing.performBilling();
                break;
            case 5:
                runInventoryAlertsMenu(inventory);
                break;
            case 6:
                runReportsMenu(rm);
                break;
            case 7:
                runBackup(inventory);
                break;
            case 8:
                runRestore(inventory);
                break;
            case 9:
                auth.changePassword();
                break;
            case 10:
                showAboutProject();
                break;
            default:
                break;
        }
    }
    return 0;
}

// ==========================================
// SUBMENU IMPLEMENTATIONS
// ==========================================

void runMedicineMenu(Inventory& inventory) {
    while (true) {
        Utility::clearScreen();
        Utility::printHeader("MEDICINE MANAGEMENT MENU");
        std::cout << "1. Add New Medicine\n";
        std::cout << "2. View All Medicines\n";
        std::cout << "3. Search Medicine\n";
        std::cout << "4. Update Medicine Info\n";
        std::cout << "5. Delete Medicine\n";
        std::cout << "6. Sort Medicines\n";
        std::cout << "7. Return to Main Menu\n\n";

        int choice = Utility::getInputInt("Enter Choice (1-7): ", 1, 7);
        if (choice == 7) break; // Return

        switch (choice) {
            case 1: {
                Utility::clearScreen();
                Utility::printHeader("ADD NEW MEDICINE RECORD");
                std::string id = Utility::getInputString("Enter Medicine ID (e.g. MED001): ");
                
                if (inventory.findMedicineByID(id) != nullptr) {
                    Utility::printColor("\nError: A medicine with this ID already exists!\n", Utility::RED);
                    Utility::pause();
                    break;
                }

                std::string name = Utility::getInputString("Enter Medicine Name: ");
                std::string company = Utility::getInputString("Enter Manufacturer Company: ");
                std::string category = Utility::getInputString("Enter Category: ");
                std::string batch = Utility::getInputString("Enter Batch Number: ");
                
                std::string mfg;
                while (true) {
                    mfg = Utility::getInputString("Enter Mfg Date (YYYY-MM-DD): ");
                    if (Utility::isValidDate(mfg)) break;
                    Utility::printColor("Error: Invalid date format. Enter as YYYY-MM-DD!\n", Utility::RED);
                }

                std::string exp;
                while (true) {
                    exp = Utility::getInputString("Enter Expiry Date (YYYY-MM-DD): ");
                    if (Utility::isValidDate(exp)) {
                        if (Utility::compareDates(exp, mfg) >= 0) break;
                        Utility::printColor("Error: Expiry date cannot be earlier than Manufacturing date!\n", Utility::RED);
                    } else {
                        Utility::printColor("Error: Invalid date format. Enter as YYYY-MM-DD!\n", Utility::RED);
                    }
                }

                double price = Utility::getInputDouble("Enter Price per unit ($): ", 0.01, 10000.0);
                int qty = Utility::getInputInt("Enter Initial Quantity: ", 0, 100000);
                std::string desc = Utility::getInputString("Enter Brief Description: ", true);

                try {
                    Medicine m(id, name, company, category, batch, mfg, exp, price, qty, desc);
                    if (inventory.addMedicine(m)) {
                        Utility::printColor("\nSuccess: Medicine added to inventory!\n", Utility::GREEN);
                    } else {
                        Utility::printColor("\nError: Failed to add medicine record (Duplicate ID check failed).\n", Utility::RED);
                    }
                } catch (const std::exception& e) {
                    Utility::printColor("\nError: Validation check failed. Details: " + std::string(e.what()) + "\n", Utility::RED);
                }
                Utility::pause();
                break;
            }
            case 2:
                Utility::clearScreen();
                Utility::printHeader("ALL REGISTERED MEDICINES");
                inventory.viewAllMedicines();
                Utility::pause();
                break;
            case 3: {
                Utility::clearScreen();
                Utility::printHeader("SEARCH MEDICINE DATABASE");
                std::cout << "Search Options:\n";
                std::cout << "1. Search by Medicine ID\n";
                std::cout << "2. Search by Name\n";
                std::cout << "3. Search by Company\n";
                std::cout << "4. Search by Category\n";
                std::cout << "5. Return\n\n";
                
                int opt = Utility::getInputInt("Enter search type (1-5): ", 1, 5);
                if (opt == 5) break;

                std::string q = Utility::getInputString("Enter query keywords: ");
                std::vector<Medicine> matches = inventory.searchMedicines(q, opt);

                if (matches.empty()) {
                    Utility::printColor("\nNo matching medicines found.\n", Utility::YELLOW);
                } else {
                    Utility::printColor("\nFound " + std::to_string(matches.size()) + " matches:\n", Utility::GREEN);
                    
                    std::vector<int> colWidths = { 8, 18, 12, 10, 8, 6, 12 };
                    Utility::printTableBorder(colWidths);
                    Utility::printTableRow(std::vector<std::string>{ "ID", "Name", "Company", "Category", "Price", "Qty", "Expiry" }, colWidths);
                    Utility::printTableBorder(colWidths);
                    for (const auto& med : matches) {
                        std::stringstream prStr;
                        prStr << std::fixed << std::setprecision(2) << med.getPrice();
                        Utility::printTableRow(std::vector<std::string>{
                            med.getMedicineID(),
                            med.getName(),
                            med.getCompany(),
                            med.getCategory(),
                            prStr.str(),
                            std::to_string(med.getQuantity()),
                            med.getExpiryDate()
                        }, colWidths);
                    }
                    Utility::printTableBorder(colWidths);
                }
                Utility::pause();
                break;
            }
            case 4: {
                Utility::clearScreen();
                Utility::printHeader("UPDATE MEDICINE DETAILS");
                std::string id = Utility::getInputString("Enter ID of medicine to update: ");
                const Medicine* match = inventory.findMedicineByID(id);
                if (match == nullptr) {
                    Utility::printColor("\nError: Medicine ID not found.\n", Utility::RED);
                    Utility::pause();
                    break;
                }

                Utility::printColor("Updating Record: " + match->getName() + "\n\n", Utility::CYAN);
                std::string name = Utility::getInputString("Enter New Name (leave empty to keep current: '" + match->getName() + "'): ", true);
                if (name.empty()) name = match->getName();

                std::string company = Utility::getInputString("Enter New Company (leave empty to keep current: '" + match->getCompany() + "'): ", true);
                if (company.empty()) company = match->getCompany();

                std::string category = Utility::getInputString("Enter New Category (leave empty to keep current: '" + match->getCategory() + "'): ", true);
                if (category.empty()) category = match->getCategory();

                std::string batch = Utility::getInputString("Enter New Batch (leave empty to keep current: '" + match->getBatchNumber() + "'): ", true);
                if (batch.empty()) batch = match->getBatchNumber();

                std::string mfg;
                while (true) {
                    mfg = Utility::getInputString("Enter Mfg Date YYYY-MM-DD (leave empty to keep: '" + match->getMfgDate() + "'): ", true);
                    if (mfg.empty()) {
                        mfg = match->getMfgDate();
                        break;
                    }
                    if (Utility::isValidDate(mfg)) break;
                    Utility::printColor("Error: Invalid date format!\n", Utility::RED);
                }

                std::string exp;
                while (true) {
                    exp = Utility::getInputString("Enter Expiry Date YYYY-MM-DD (leave empty to keep: '" + match->getExpiryDate() + "'): ", true);
                    if (exp.empty()) {
                        exp = match->getExpiryDate();
                        break;
                    }
                    if (Utility::isValidDate(exp)) {
                        if (Utility::compareDates(exp, mfg) >= 0) break;
                        Utility::printColor("Error: Expiry date cannot be earlier than MFG date!\n", Utility::RED);
                    } else {
                        Utility::printColor("Error: Invalid date format!\n", Utility::RED);
                    }
                }

                std::cout << "Current Price: $" << std::fixed << std::setprecision(2) << match->getPrice() << "\n";
                double price = Utility::getInputDouble("Enter New Price (0.01 - 10000.0, or type current value to keep): ", 0.01, 10000.0);

                std::cout << "Current Quantity: " << match->getQuantity() << " units\n";
                int qty = Utility::getInputInt("Enter New Quantity (0 - 100000, or type current value to keep): ", 0, 100000);

                std::string desc = Utility::getInputString("Enter New Description (leave empty to keep current: '" + match->getDescription() + "'): ", true);
                if (desc.empty()) desc = match->getDescription();

                try {
                    Medicine newMed(id, name, company, category, batch, mfg, exp, price, qty, desc);
                    if (inventory.updateMedicine(id, newMed)) {
                        Utility::printColor("\nSuccess: Medicine details updated!\n", Utility::GREEN);
                    } else {
                        Utility::printColor("\nError: Update failed.\n", Utility::RED);
                    }
                } catch (const std::exception& e) {
                    Utility::printColor("\nError: Validation check failed. Details: " + std::string(e.what()) + "\n", Utility::RED);
                }
                Utility::pause();
                break;
            }
            case 5: {
                Utility::clearScreen();
                Utility::printHeader("DELETE MEDICINE RECORD");
                std::string id = Utility::getInputString("Enter ID of medicine to delete: ");
                
                const Medicine* match = inventory.findMedicineByID(id);
                if (match == nullptr) {
                    Utility::printColor("\nError: Medicine ID not found.\n", Utility::RED);
                    Utility::pause();
                    break;
                }

                Utility::printColor("Warning: You are deleting '" + match->getName() + "' permanently.\n", Utility::YELLOW);
                std::string confirm = Utility::getInputString("Confirm deletion? (Y/N): ");
                if (confirm == "Y" || confirm == "y") {
                    if (inventory.deleteMedicine(id)) {
                        Utility::printColor("\nSuccess: Medicine record removed.\n", Utility::GREEN);
                    } else {
                        Utility::printColor("\nError: Failed to delete record.\n", Utility::RED);
                    }
                } else {
                    Utility::printColor("\nDeletion cancelled.\n", Utility::YELLOW);
                }
                Utility::pause();
                break;
            }
            case 6: {
                Utility::clearScreen();
                Utility::printHeader("SORT MEDICINE DATABASE");
                std::cout << "Sort Options:\n";
                std::cout << "1. Sort Alphabetically by Name\n";
                std::cout << "2. Sort by Price (Ascending)\n";
                std::cout << "3. Sort by Stock Quantity (Ascending)\n";
                std::cout << "4. Sort by Manufacturer Company\n";
                std::cout << "5. Sort by Expiry Date (Chronological)\n";
                std::cout << "6. Cancel\n\n";

                int opt = Utility::getInputInt("Select sorting criteria (1-6): ", 1, 6);
                if (opt == 6) break;

                inventory.sortMedicines(opt);
                Utility::printColor("\nSuccess: Database sorted! Viewing updated database:\n", Utility::GREEN);
                inventory.viewAllMedicines();
                Utility::pause();
                break;
            }
            default:
                break;
        }
    }
}

void runCustomerMenu(Inventory& inventory) {
    while (true) {
        Utility::clearScreen();
        Utility::printHeader("CUSTOMER MANAGEMENT MENU");
        std::cout << "1. Add New Customer\n";
        std::cout << "2. View Registered Customers\n";
        std::cout << "3. Search Customer\n";
        std::cout << "4. Update Customer Info\n";
        std::cout << "5. Delete Customer Profile\n";
        std::cout << "6. Back to Main Menu\n\n";

        int choice = Utility::getInputInt("Enter Choice (1-6): ", 1, 6);
        if (choice == 6) break;

        switch (choice) {
            case 1: {
                Utility::clearScreen();
                Utility::printHeader("REGISTER NEW CUSTOMER");
                std::string id = Utility::getInputString("Enter Customer ID (e.g. CUST001): ");

                if (inventory.findCustomerByID(id) != nullptr) {
                    Utility::printColor("\nError: Customer with this ID already exists!\n", Utility::RED);
                    Utility::pause();
                    break;
                }

                std::string name = Utility::getInputString("Enter Customer Name: ");
                std::string mobile = Utility::getInputString("Enter 10-digit Mobile Number: ");
                std::string address = Utility::getInputString("Enter Address: ");

                try {
                    Customer c(id, name, mobile, address);
                    if (inventory.addCustomer(c)) {
                        Utility::printColor("\nSuccess: Customer registered!\n", Utility::GREEN);
                    } else {
                        Utility::printColor("\nError: Failed to register customer.\n", Utility::RED);
                    }
                } catch (const std::exception& e) {
                    Utility::printColor("\nError: Validation failed. Details: " + std::string(e.what()) + "\n", Utility::RED);
                }
                Utility::pause();
                break;
            }
            case 2:
                Utility::clearScreen();
                Utility::printHeader("CUSTOMER DATABASE");
                inventory.viewAllCustomers();
                Utility::pause();
                break;
            case 3: {
                Utility::clearScreen();
                Utility::printHeader("SEARCH CUSTOMER PROFILES");
                std::cout << "Search Options:\n";
                std::cout << "1. Search by Customer ID\n";
                std::cout << "2. Search by Name\n";
                std::cout << "3. Search by Mobile Number\n";
                std::cout << "4. Cancel\n\n";

                int opt = Utility::getInputInt("Enter option (1-4): ", 1, 4);
                if (opt == 4) break;

                std::string q = Utility::getInputString("Enter keyword: ");
                std::vector<Customer> matches = inventory.searchCustomers(q, opt);

                if (matches.empty()) {
                    Utility::printColor("\nNo matching customer found.\n", Utility::YELLOW);
                } else {
                    Utility::printColor("\nFound " + std::to_string(matches.size()) + " matches:\n", Utility::GREEN);
                    std::vector<int> colWidths = { 10, 20, 15, 30 };
                    Utility::printTableBorder(colWidths);
                    Utility::printTableRow(std::vector<std::string>{ "ID", "Name", "Mobile No", "Address" }, colWidths);
                    Utility::printTableBorder(colWidths);
                    for (const auto& cust : matches) {
                        Utility::printTableRow(std::vector<std::string>{
                            cust.getCustomerID(),
                            cust.getName(),
                            cust.getMobileNumber(),
                            cust.getAddress()
                        }, colWidths);
                    }
                    Utility::printTableBorder(colWidths);
                }
                Utility::pause();
                break;
            }
            case 4: {
                Utility::clearScreen();
                Utility::printHeader("UPDATE CUSTOMER INFORMATION");
                std::string id = Utility::getInputString("Enter ID of customer to update: ");
                const Customer* match = inventory.findCustomerByID(id);
                if (match == nullptr) {
                    Utility::printColor("\nError: Customer ID not found.\n", Utility::RED);
                    Utility::pause();
                    break;
                }

                Utility::printColor("Updating Record for: " + match->getName() + "\n\n", Utility::CYAN);
                std::string name = Utility::getInputString("Enter New Name (leave empty to keep current: '" + match->getName() + "'): ", true);
                if (name.empty()) name = match->getName();

                std::string mobile = Utility::getInputString("Enter New Mobile Number (leave empty to keep: '" + match->getMobileNumber() + "'): ", true);
                if (mobile.empty()) mobile = match->getMobileNumber();

                std::string address = Utility::getInputString("Enter New Address (leave empty to keep: '" + match->getAddress() + "'): ", true);
                if (address.empty()) address = match->getAddress();

                try {
                    Customer newCust(id, name, mobile, address);
                    if (inventory.updateCustomer(id, newCust)) {
                        Utility::printColor("\nSuccess: Customer details updated!\n", Utility::GREEN);
                    } else {
                        Utility::printColor("\nError: Update failed.\n", Utility::RED);
                    }
                } catch (const std::exception& e) {
                    Utility::printColor("\nError: Validation failed. Details: " + std::string(e.what()) + "\n", Utility::RED);
                }
                Utility::pause();
                break;
            }
            case 5: {
                Utility::clearScreen();
                Utility::printHeader("DELETE CUSTOMER PROFILE");
                std::string id = Utility::getInputString("Enter Customer ID to delete: ");
                const Customer* match = inventory.findCustomerByID(id);
                if (match == nullptr) {
                    Utility::printColor("\nError: Customer ID not found.\n", Utility::RED);
                    Utility::pause();
                    break;
                }

                Utility::printColor("Warning: Removing '" + match->getName() + "' and their purchase log permanently.\n", Utility::YELLOW);
                std::string confirm = Utility::getInputString("Confirm delete? (Y/N): ");
                if (confirm == "Y" || confirm == "y") {
                    if (inventory.deleteCustomer(id)) {
                        Utility::printColor("\nSuccess: Customer deleted successfully.\n", Utility::GREEN);
                    } else {
                        Utility::printColor("\nError: Deletion failed.\n", Utility::RED);
                    }
                } else {
                    Utility::printColor("\nDeletion cancelled.\n", Utility::YELLOW);
                }
                Utility::pause();
                break;
            }
            default:
                break;
        }
    }
}

void runSupplierMenu(Inventory& inventory) {
    while (true) {
        Utility::clearScreen();
        Utility::printHeader("SUPPLIER DIRECTORY MENU");
        std::cout << "1. Add Supplier\n";
        std::cout << "2. View Supplier List\n";
        std::cout << "3. Search Supplier\n";
        std::cout << "4. Update Supplier Info\n";
        std::cout << "5. Delete Supplier Profile\n";
        std::cout << "6. Back to Main Menu\n\n";

        int choice = Utility::getInputInt("Enter Choice (1-6): ", 1, 6);
        if (choice == 6) break;

        switch (choice) {
            case 1: {
                Utility::clearScreen();
                Utility::printHeader("ADD WHOLESALE SUPPLIER");
                std::string id = Utility::getInputString("Enter Supplier ID (e.g. SUPP001): ");

                if (inventory.findSupplierByID(id) != nullptr) {
                    Utility::printColor("\nError: Supplier with this ID already exists!\n", Utility::RED);
                    Utility::pause();
                    break;
                }

                std::string name = Utility::getInputString("Enter Supplier Name: ");
                std::string phone = Utility::getInputString("Enter Contact Phone: ");
                std::string email = Utility::getInputString("Enter Email: ");
                std::string addr = Utility::getInputString("Enter Office Address: ");

                try {
                    Supplier s(id, name, phone, email, addr);
                    if (inventory.addSupplier(s)) {
                        Utility::printColor("\nSuccess: Supplier added!\n", Utility::GREEN);
                    } else {
                        Utility::printColor("\nError: Failed to register supplier.\n", Utility::RED);
                    }
                } catch (const std::exception& e) {
                    Utility::printColor("\nError: Validation failed. Details: " + std::string(e.what()) + "\n", Utility::RED);
                }
                Utility::pause();
                break;
            }
            case 2:
                Utility::clearScreen();
                Utility::printHeader("SUPPLIER DIRECTORY");
                inventory.viewAllSuppliers();
                Utility::pause();
                break;
            case 3: {
                Utility::clearScreen();
                Utility::printHeader("SEARCH SUPPLIER DIRECTORY");
                std::cout << "Search Options:\n";
                std::cout << "1. Search by Supplier ID\n";
                std::cout << "2. Search by Name\n";
                std::cout << "3. Search by Phone Number\n";
                std::cout << "4. Cancel\n\n";

                int opt = Utility::getInputInt("Select Option (1-4): ", 1, 4);
                if (opt == 4) break;

                std::string q = Utility::getInputString("Enter keyword: ");
                std::vector<Supplier> matches = inventory.searchSuppliers(q, opt);

                if (matches.empty()) {
                    Utility::printColor("\nNo matching supplier found.\n", Utility::YELLOW);
                } else {
                    Utility::printColor("\nFound " + std::to_string(matches.size()) + " matches:\n", Utility::GREEN);
                    std::vector<int> colWidths = { 10, 20, 15, 25, 25 };
                    Utility::printTableBorder(colWidths);
                    Utility::printTableRow(std::vector<std::string>{ "ID", "Name", "Phone", "Email", "Address" }, colWidths);
                    Utility::printTableBorder(colWidths);
                    for (const auto& supp : matches) {
                        Utility::printTableRow(std::vector<std::string>{
                            supp.getSupplierID(),
                            supp.getName(),
                            supp.getPhone(),
                            supp.getEmail(),
                            supp.getAddress()
                        }, colWidths);
                    }
                    Utility::printTableBorder(colWidths);
                }
                Utility::pause();
                break;
            }
            case 4: {
                Utility::clearScreen();
                Utility::printHeader("UPDATE SUPPLIER RECORDS");
                std::string id = Utility::getInputString("Enter ID of supplier to update: ");
                const Supplier* match = inventory.findSupplierByID(id);
                if (match == nullptr) {
                    Utility::printColor("\nError: Supplier ID not found.\n", Utility::RED);
                    Utility::pause();
                    break;
                }

                Utility::printColor("Updating Record for: " + match->getName() + "\n\n", Utility::CYAN);
                std::string name = Utility::getInputString("Enter Supplier Name (leave empty to keep current: '" + match->getName() + "'): ", true);
                if (name.empty()) name = match->getName();

                std::string phone = Utility::getInputString("Enter Phone (leave empty to keep: '" + match->getPhone() + "'): ", true);
                if (phone.empty()) phone = match->getPhone();

                std::string email = Utility::getInputString("Enter Email (leave empty to keep: '" + match->getEmail() + "'): ", true);
                if (email.empty()) email = match->getEmail();

                std::string address = Utility::getInputString("Enter Address (leave empty to keep: '" + match->getAddress() + "'): ", true);
                if (address.empty()) address = match->getAddress();

                try {
                    Supplier newSupp(id, name, phone, email, address);
                    if (inventory.updateSupplier(id, newSupp)) {
                        Utility::printColor("\nSuccess: Supplier details updated!\n", Utility::GREEN);
                    } else {
                        Utility::printColor("\nError: Update failed.\n", Utility::RED);
                    }
                } catch (const std::exception& e) {
                    Utility::printColor("\nError: Validation failed. Details: " + std::string(e.what()) + "\n", Utility::RED);
                }
                Utility::pause();
                break;
            }
            case 5: {
                Utility::clearScreen();
                Utility::printHeader("DELETE SUPPLIER DIRECTORY PROFILE");
                std::string id = Utility::getInputString("Enter Supplier ID to delete: ");
                const Supplier* match = inventory.findSupplierByID(id);
                if (match == nullptr) {
                    Utility::printColor("\nError: Supplier ID not found.\n", Utility::RED);
                    Utility::pause();
                    break;
                }

                Utility::printColor("Warning: You are deleting Supplier '" + match->getName() + "' permanently.\n", Utility::YELLOW);
                std::string confirm = Utility::getInputString("Confirm delete? (Y/N): ");
                if (confirm == "Y" || confirm == "y") {
                    if (inventory.deleteSupplier(id)) {
                        Utility::printColor("\nSuccess: Supplier deleted successfully.\n", Utility::GREEN);
                    } else {
                        Utility::printColor("\nError: Deletion failed.\n", Utility::RED);
                    }
                } else {
                    Utility::printColor("\nDeletion cancelled.\n", Utility::YELLOW);
                }
                Utility::pause();
                break;
            }
            default:
                break;
        }
    }
}

void runInventoryAlertsMenu(const Inventory& inventory) {
    while (true) {
        Utility::clearScreen();
        Utility::printHeader("INVENTORY ALERTS & NOTIFICATIONS");
        std::cout << "1. View Available Stock Catalog\n";
        std::cout << "2. View Low Stock Items (< 10 units)\n";
        std::cout << "3. View Expired Medicines\n";
        std::cout << "4. View Near Expiry Medicines (< 3 months)\n";
        std::cout << "5. View Out Of Stock Products\n";
        std::cout << "6. Back to Dashboard\n\n";

        int choice = Utility::getInputInt("Select Option (1-6): ", 1, 6);
        if (choice == 6) break;

        Utility::clearScreen();
        std::vector<int> colWidths = { 8, 18, 12, 10, 8, 6, 12 };

        switch (choice) {
            case 1:
                Utility::printHeader("AVAILABLE STOCK CATALOG");
                inventory.viewAllMedicines();
                break;
            case 2: {
                Utility::printHeader("LOW STOCK ALERTS (< 10 units)");
                std::vector<Medicine> low = inventory.getLowStockMedicines();
                if (low.empty()) {
                    Utility::printColor("All products are well-stocked! No alerts.\n", Utility::GREEN);
                } else {
                    Utility::printTableBorder(colWidths);
                    Utility::printTableRow(std::vector<std::string>{ "ID", "Name", "Company", "Category", "Price", "Qty", "Expiry" }, colWidths);
                    Utility::printTableBorder(colWidths);
                    for (const auto& med : low) {
                        std::stringstream prStr;
                        prStr << std::fixed << std::setprecision(2) << med.getPrice();
                        Utility::printTableRow(std::vector<std::string>{ med.getMedicineID(), med.getName(), med.getCompany(), med.getCategory(), prStr.str(), std::to_string(med.getQuantity()), med.getExpiryDate() }, colWidths);
                    }
                    Utility::printTableBorder(colWidths);
                }
                break;
            }
            case 3: {
                Utility::printHeader("EXPIRED MEDICINE LIST");
                std::vector<Medicine> expired = inventory.getExpiredMedicines();
                if (expired.empty()) {
                    Utility::printColor("Excellent! No expired medicines found in inventory.\n", Utility::GREEN);
                } else {
                    Utility::printTableBorder(colWidths);
                    Utility::printTableRow(std::vector<std::string>{ "ID", "Name", "Company", "Category", "Price", "Qty", "Expiry" }, colWidths);
                    Utility::printTableBorder(colWidths);
                    for (const auto& med : expired) {
                        std::stringstream prStr;
                        prStr << std::fixed << std::setprecision(2) << med.getPrice();
                        Utility::printTableRow(std::vector<std::string>{ med.getMedicineID(), med.getName(), med.getCompany(), med.getCategory(), prStr.str(), std::to_string(med.getQuantity()), med.getExpiryDate() }, colWidths);
                    }
                    Utility::printTableBorder(colWidths);
                }
                break;
            }
            case 4: {
                Utility::printHeader("NEAR EXPIRY MEDICINE ALERTS (< 3 months)");
                std::vector<Medicine> near = inventory.getNearExpiryMedicines();
                if (near.empty()) {
                    Utility::printColor("No medicines are expiring within the next 3 months.\n", Utility::GREEN);
                } else {
                    Utility::printTableBorder(colWidths);
                    Utility::printTableRow(std::vector<std::string>{ "ID", "Name", "Company", "Category", "Price", "Qty", "Expiry" }, colWidths);
                    Utility::printTableBorder(colWidths);
                    for (const auto& med : near) {
                        std::stringstream prStr;
                        prStr << std::fixed << std::setprecision(2) << med.getPrice();
                        Utility::printTableRow(std::vector<std::string>{ med.getMedicineID(), med.getName(), med.getCompany(), med.getCategory(), prStr.str(), std::to_string(med.getQuantity()), med.getExpiryDate() }, colWidths);
                    }
                    Utility::printTableBorder(colWidths);
                }
                break;
            }
            case 5: {
                Utility::printHeader("OUT OF STOCK MEDICINES");
                std::vector<Medicine> out = inventory.getOutOfStockMedicines();
                if (out.empty()) {
                    Utility::printColor("Success: No items are currently out of stock!\n", Utility::GREEN);
                } else {
                    Utility::printTableBorder(colWidths);
                    Utility::printTableRow(std::vector<std::string>{ "ID", "Name", "Company", "Category", "Price", "Qty", "Expiry" }, colWidths);
                    Utility::printTableBorder(colWidths);
                    for (const auto& med : out) {
                        std::stringstream prStr;
                        prStr << std::fixed << std::setprecision(2) << med.getPrice();
                        Utility::printTableRow(std::vector<std::string>{ med.getMedicineID(), med.getName(), med.getCompany(), med.getCategory(), prStr.str(), std::to_string(med.getQuantity()), med.getExpiryDate() }, colWidths);
                    }
                    Utility::printTableBorder(colWidths);
                }
                break;
            }
            default:
                break;
        }
        Utility::pause();
    }
}

void runReportsMenu(const ReportManager& rm) {
    while (true) {
        Utility::clearScreen();
        Utility::printHeader("BUSINESS REPORTS MANAGER");
        std::cout << "1. Daily / Total Revenue & Sales Ledger\n";
        std::cout << "2. Medicine Directory Report\n";
        std::cout << "3. Customer Activity Report\n";
        std::cout << "4. Supplier Directory Report\n";
        std::cout << "5. Inventory Audit Report\n";
        std::cout << "6. Back to Dashboard\n\n";

        int choice = Utility::getInputInt("Select Option (1-6): ", 1, 6);
        if (choice == 6) break;

        Utility::clearScreen();
        switch (choice) {
            case 1:
                rm.generateSalesReport();
                break;
            case 2:
                rm.generateMedicineReport();
                break;
            case 3:
                rm.generateCustomerReport();
                break;
            case 4:
                rm.generateSupplierReport();
                break;
            case 5:
                rm.generateInventoryReport();
                break;
            default:
                break;
        }
    }
}

void runBackup(const Inventory& inventory) {
    Utility::clearScreen();
    Utility::printHeader("DATABASE BACKUP SYSTEM");

    std::string dbDir = inventory.getDbDirectory();
    std::string backupDir = "backup";

    Utility::printColor("Initializing data backups...\n", Utility::CYAN);

    FileManager::ensureDirectoryExists(backupDir);

    std::vector<std::string> dbFiles = {
        "MedicineDatabase.txt",
        "CustomerDatabase.txt",
        "SupplierDatabase.txt",
        "SalesDatabase.txt",
        "LoginDatabase.txt"
    };

    bool allSuccess = true;
    for (const auto& file : dbFiles) {
        std::string src = dbDir + "/" + file;
        std::string dest = backupDir + "/" + file;
        if (FileManager::fileExists(src)) {
            if (FileManager::copyFile(src, dest)) {
                std::cout << "Backed up: " << file << " -> backup/" << file << "\n";
            } else {
                Utility::printColor("Failed to backup: " + file + "\n", Utility::RED);
                allSuccess = false;
            }
        }
    }

    if (allSuccess) {
        Utility::printColor("\nSuccess: All database records backed up successfully into 'backup/' directory!\n", Utility::GREEN);
    } else {
        Utility::printColor("\nWarning: Some files failed to backup. Check disk permissions.\n", Utility::YELLOW);
    }

    Utility::pause();
}

void runRestore(Inventory& inventory) {
    Utility::clearScreen();
    Utility::printHeader("DATABASE RESTORE SYSTEM");
    Utility::printColor("Warning: Restoring will overwrite all current system data with the backup!\n", Utility::RED);
    std::string confirm = Utility::getInputString("Are you absolutely sure you want to proceed? (Y/N): ");
    if (confirm != "Y" && confirm != "y") {
        Utility::printColor("\nRestore process aborted.\n", Utility::YELLOW);
        Utility::pause();
        return;
    }

    std::string dbDir = inventory.getDbDirectory();
    std::string backupDir = "backup";

    if (!FileManager::fileExists(backupDir)) {
        Utility::printColor("\nError: Backup folder not found! No backup exists.\n", Utility::RED);
        Utility::pause();
        return;
    }

    std::vector<std::string> dbFiles = {
        "MedicineDatabase.txt",
        "CustomerDatabase.txt",
        "SupplierDatabase.txt",
        "SalesDatabase.txt",
        "LoginDatabase.txt"
    };

    Utility::printColor("\nRestoring files...\n", Utility::CYAN);
    bool allSuccess = true;
    for (const auto& file : dbFiles) {
        std::string src = backupDir + "/" + file;
        std::string dest = dbDir + "/" + file;
        if (FileManager::fileExists(src)) {
            if (FileManager::copyFile(src, dest)) {
                std::cout << "Restored: " << file << " -> database/" << file << "\n";
            } else {
                Utility::printColor("Failed to restore: " + file + "\n", Utility::RED);
                allSuccess = false;
            }
        }
    }

    if (allSuccess) {
        Utility::printColor("\nSuccess: Databases restored successfully! Reloading data...\n", Utility::GREEN);
        inventory.loadAllFromDisk(); // Reload
    } else {
        Utility::printColor("\nError: Failed to restore databases completely. Files may be corrupted.\n", Utility::RED);
    }

    Utility::pause();
}

void showAboutProject() {
    Utility::clearScreen();
    Utility::printHeader("ABOUT PROJECT - INFORMATICS");
    
    std::vector<int> colWidths = { 20, 35 };
    Utility::printTableBorder(colWidths);
    Utility::printTableRow(std::vector<std::string>{ "Detail Title", "System Value" }, colWidths);
    Utility::printTableBorder(colWidths);
    
    Utility::printTableRow(std::vector<std::string>{ "Project Name", "Medical Store Management System" }, colWidths);
    Utility::printTableRow(std::vector<std::string>{ "Version", "1.0 (Stable)" }, colWidths);
    Utility::printTableRow(std::vector<std::string>{ "Implementation", "Offline Console Command Line App" }, colWidths);
    Utility::printTableRow(std::vector<std::string>{ "Language", "C++17 (Visual Studio Code + MinGW)" }, colWidths);
    Utility::printTableRow(std::vector<std::string>{ "Main Paradigm", "Object-Oriented Programming (OOP)" }, colWidths);
    Utility::printTableRow(std::vector<std::string>{ "OOP Principles", "Classes, Encapsulation, Composition" }, colWidths);
    Utility::printTableRow(std::vector<std::string>{ "File Storage", "std::fstream (Delimited Plaintext DB)" }, colWidths);
    Utility::printTableRow(std::vector<std::string>{ "Developer Info", "B.Tech CSE Summer Intern 2026" }, colWidths);
    Utility::printTableRow(std::vector<std::string>{ "Internship Course", "Summer Internship Project on OOP in C++" }, colWidths);
    
    Utility::printTableBorder(colWidths);
    Utility::pause();
}
