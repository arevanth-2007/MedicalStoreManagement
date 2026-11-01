/**
 * @file Billing.cpp
 * @brief Implementation of the Billing system class.
 */

#include "Billing.h"
#include "Utility.h"
#include "FileManager.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <stdexcept>

namespace MedicalStore {

    Billing::Billing(Inventory& inventory, const std::string& salesDbPath, const std::string& invoiceDir)
        : m_inventory(inventory), m_salesDbPath(salesDbPath), m_invoiceDir(invoiceDir) {}

    std::string Billing::generateInvoiceNumber() {
        int salesCount = 0;
        if (FileManager::fileExists(m_salesDbPath)) {
            std::vector<std::string> lines = FileManager::readLines(m_salesDbPath);
            salesCount = static_cast<int>(lines.size());
        }

        // Get date string without dashes (YYYYMMDD)
        std::string curDate = Utility::getCurrentDate();
        curDate.erase(std::remove(curDate.begin(), curDate.end(), '-'), curDate.end());

        std::stringstream ss;
        ss << "INV-" << curDate << "-" << std::setw(4) << std::setfill('0') << (salesCount + 1);
        return ss.str();
    }

    void Billing::performBilling() {
        Utility::printHeader("BILLING SYSTEM - GENERATE INVOICE");

        // Step 1: Customer identification
        std::string custIdInput = Utility::getInputString("Enter Customer ID (or type 'GUEST' for guest checkout): ");
        Customer* customer = nullptr;
        std::string customerName = "Walk-in Guest";

        std::string upperId = custIdInput;
        std::transform(upperId.begin(), upperId.end(), upperId.begin(), ::toupper);

        if (upperId != "GUEST") {
            customer = m_inventory.findCustomerByIDMutable(custIdInput);
            if (customer == nullptr) {
                Utility::printColor("Customer ID not found in database!\n", Utility::YELLOW);
                std::string createNew = Utility::getInputString("Would you like to register a new customer? (Y/N): ");
                if (createNew == "Y" || createNew == "y") {
                    std::string newId = "CUST" + std::to_string(m_inventory.getTotalCustomersCount() + 101);
                    Utility::printColor("Creating profile with auto-generated ID: " + newId + "\n", Utility::CYAN);
                    std::string name = Utility::getInputString("Enter Customer Name: ");
                    std::string mobile;
                    while (true) {
                        try {
                            mobile = Utility::getInputString("Enter Mobile Number: ");
                            // Validate format by creating a temp Customer or check inline
                            if (mobile.length() != 10) {
                                throw std::invalid_argument("Mobile must be 10 digits.");
                            }
                            for (char c : mobile) {
                                if (!std::isdigit(static_cast<unsigned char>(c))) {
                                    throw std::invalid_argument("Digits only.");
                                }
                            }
                            break;
                        } catch (const std::exception& e) {
                            Utility::printColor("Error: Invalid mobile number! Must be 10 digits. Try again.\n", Utility::RED);
                        }
                    }
                    std::string addr = Utility::getInputString("Enter Address: ");

                    try {
                        Customer newCust(newId, name, mobile, addr);
                        m_inventory.addCustomer(newCust);
                        customer = m_inventory.findCustomerByIDMutable(newId);
                        if (customer) {
                            customerName = customer->getName();
                            Utility::printColor("Customer registered successfully!\n", Utility::GREEN);
                        }
                    } catch (const std::exception& e) {
                        Utility::printColor("Error registering customer: " + std::string(e.what()) + "\n", Utility::RED);
                    }
                } else {
                    Utility::printColor("Proceeding as Walk-in Guest.\n", Utility::YELLOW);
                }
            } else {
                customerName = customer->getName();
                Utility::printColor("Customer recognized: " + customerName + "\n", Utility::GREEN);
            }
        }

        // Step 2: Shopping Cart compilation
        std::vector<BillItem> cart;
        bool addMore = true;

        while (addMore) {
            Utility::printHeader("ADD ITEMS TO CART");
            
            // Display current cart if not empty
            if (!cart.empty()) {
                std::cout << "Current Cart Items:\n";
                std::vector<int> cartWidths = { 6, 20, 8, 6, 10 };
                Utility::printTableBorder(cartWidths);
                Utility::printTableRow({ "S.No", "Medicine Name", "Price", "Qty", "Total" }, cartWidths);
                Utility::printTableBorder(cartWidths);
                for (size_t i = 0; i < cart.size(); ++i) {
                    std::stringstream prStr, totStr;
                    prStr << std::fixed << std::setprecision(2) << cart[i].price;
                    totStr << std::fixed << std::setprecision(2) << cart[i].total;
                    Utility::printTableRow({
                        std::to_string(i + 1),
                        cart[i].name,
                        prStr.str(),
                        std::to_string(cart[i].quantity),
                        totStr.str()
                    }, cartWidths);
                }
                Utility::printTableBorder(cartWidths);
                std::cout << "\n";
            }

            std::string query = Utility::getInputString("Enter Medicine ID or Name to search: ");
            std::vector<Medicine> matches;
            
            // Check if ID matches directly
            const Medicine* directMed = m_inventory.findMedicineByID(query);
            if (directMed) {
                matches.push_back(*directMed);
            } else {
                // Try searching by name
                matches = m_inventory.searchMedicines(query, 2);
            }

            if (matches.empty()) {
                Utility::printColor("No medicine matches found for: " + query + "\n", Utility::RED);
                std::string retry = Utility::getInputString("Try again? (Y/N): ");
                if (retry == "N" || retry == "n") {
                    break;
                }
                continue;
            }

            Medicine* selectedMed = nullptr;
            if (matches.size() > 1) {
                Utility::printColor("Multiple matches found. Select from table:\n", Utility::CYAN);
                std::vector<int> selWidths = { 6, 8, 20, 10, 10, 8 };
                Utility::printTableBorder(selWidths);
                Utility::printTableRow({ "Index", "ID", "Name", "Category", "Price", "Stock" }, selWidths);
                Utility::printTableBorder(selWidths);
                for (size_t i = 0; i < matches.size(); ++i) {
                    std::stringstream prStr;
                    prStr << std::fixed << std::setprecision(2) << matches[i].getPrice();
                    Utility::printTableRow({
                        std::to_string(i + 1),
                        matches[i].getMedicineID(),
                        matches[i].getName(),
                        matches[i].getCategory(),
                        prStr.str(),
                        std::to_string(matches[i].getQuantity())
                    }, selWidths);
                }
                Utility::printTableBorder(selWidths);
                
                int idx = Utility::getInputInt("Enter selected Index (1-" + std::to_string(matches.size()) + "): ", 1, static_cast<int>(matches.size()));
                selectedMed = m_inventory.findMedicineByIDMutable(matches[idx - 1].getMedicineID());
            } else {
                selectedMed = m_inventory.findMedicineByIDMutable(matches[0].getMedicineID());
            }

            if (selectedMed == nullptr) continue;

            // Medicine warnings/error checks
            if (Utility::isExpired(selectedMed->getExpiryDate())) {
                Utility::printColor("Error: Medicine '" + selectedMed->getName() + "' is EXPIRED! Cannot sell expired medicine.\n", Utility::RED);
                Utility::pause();
                continue;
            }

            if (selectedMed->getQuantity() <= 0) {
                Utility::printColor("Error: Medicine '" + selectedMed->getName() + "' is OUT OF STOCK!\n", Utility::RED);
                Utility::pause();
                continue;
            }

            // Enter Quantity
            int stock = selectedMed->getQuantity();
            Utility::printColor("Available Stock: " + std::to_string(stock) + " units\n", Utility::CYAN);
            int qty = Utility::getInputInt("Enter quantity to purchase: ", 1, stock);

            // Add or Update in Cart
            auto cartIt = std::find_if(cart.begin(), cart.end(), [&](const BillItem& item) {
                return item.medicineID == selectedMed->getMedicineID();
            });

            if (cartIt != cart.end()) {
                if (cartIt->quantity + qty > stock) {
                    Utility::printColor("Error: Combined cart quantity (" + std::to_string(cartIt->quantity + qty) + ") exceeds available stock (" + std::to_string(stock) + ")!\n", Utility::RED);
                } else {
                    cartIt->quantity += qty;
                    cartIt->total = cartIt->quantity * cartIt->price;
                    Utility::printColor("Cart item updated successfully.\n", Utility::GREEN);
                }
            } else {
                BillItem item;
                item.medicineID = selectedMed->getMedicineID();
                item.name = selectedMed->getName();
                item.price = selectedMed->getPrice();
                item.quantity = qty;
                item.total = qty * item.price;
                cart.push_back(item);
                Utility::printColor("Item added to cart.\n", Utility::GREEN);
            }

            std::string promptMore = Utility::getInputString("Add another medicine? (Y/N): ");
            addMore = (promptMore == "Y" || promptMore == "y");
        }

        if (cart.empty()) {
            Utility::printColor("Billing transaction cancelled (Cart is empty).\n", Utility::YELLOW);
            Utility::pause();
            return;
        }

        // Configurable GST
        double gstPercent = Utility::getInputDouble("Enter GST percentage (0 to 28, default is 18%): ", 0.0, 28.0);

        // Totals Calculation
        double subtotal = 0.0;
        for (const auto& item : cart) {
            subtotal += item.total;
        }
        double gstAmount = subtotal * (gstPercent / 100.0);
        double grandTotal = subtotal + gstAmount;

        std::string invoiceNum = generateInvoiceNumber();
        std::string dateTimeStr = Utility::getCurrentDateTime();

        // Step 3: Format Invoice text
        std::vector<std::string> invoiceText;
        std::stringstream lineStr;

        invoiceText.push_back("====================================================");
        invoiceText.push_back("             MEDICAL STORE - INVOICE                ");
        invoiceText.push_back("====================================================");
        
        lineStr.str("");
        lineStr << std::left << std::setw(18) << "Invoice Number:" << invoiceNum;
        invoiceText.push_back(lineStr.str());

        lineStr.str("");
        lineStr << std::left << std::setw(18) << "Customer Name:" << customerName;
        invoiceText.push_back(lineStr.str());

        lineStr.str("");
        lineStr << std::left << std::setw(18) << "Date & Time:" << dateTimeStr;
        invoiceText.push_back(lineStr.str());
        
        invoiceText.push_back("----------------------------------------------------");
        
        lineStr.str("");
        lineStr << std::left << std::setw(20) << "Medicine" 
                << std::right << std::setw(8) << "Qty" 
                << std::right << std::setw(10) << "Price" 
                << std::right << std::setw(12) << "Total";
        invoiceText.push_back(lineStr.str());
        
        invoiceText.push_back("----------------------------------------------------");

        for (const auto& item : cart) {
            lineStr.str("");
            lineStr << std::fixed << std::setprecision(2)
                    << std::left << std::setw(20) << (item.name.length() > 19 ? item.name.substr(0, 16) + "..." : item.name)
                    << std::right << std::setw(8) << item.quantity
                    << std::right << std::setw(10) << item.price
                    << std::right << std::setw(12) << item.total;
            invoiceText.push_back(lineStr.str());
        }

        invoiceText.push_back("----------------------------------------------------");
        
        lineStr.str("");
        lineStr << std::fixed << std::setprecision(2)
                << std::left << std::setw(30) << "Subtotal:" 
                << std::right << std::setw(20) << subtotal;
        invoiceText.push_back(lineStr.str());

        lineStr.str("");
        lineStr << std::fixed << std::setprecision(2)
                << std::left << std::setw(30) << "GST (" + std::to_string(static_cast<int>(gstPercent)) + "%):" 
                << std::right << std::setw(20) << gstAmount;
        invoiceText.push_back(lineStr.str());

        lineStr.str("");
        lineStr << std::fixed << std::setprecision(2)
                << std::left << std::setw(30) << "Grand Total:" 
                << std::right << std::setw(20) << grandTotal;
        invoiceText.push_back(lineStr.str());
        
        invoiceText.push_back("----------------------------------------------------");
        invoiceText.push_back("                   THANK YOU                        ");
        invoiceText.push_back("                  VISIT AGAIN                       ");
        invoiceText.push_back("====================================================");

        // Step 4: Write Invoice to console
        std::cout << "\n";
        for (const auto& line : invoiceText) {
            Utility::printColor(line + "\n", Utility::GREEN);
        }

        // Step 5: Save invoice to unique file in reports/
        std::string invoiceFileName = m_invoiceDir + "/Invoice_" + invoiceNum + ".txt";
        FileManager::writeLines(invoiceFileName, invoiceText);

        // Step 6: Log sale to SalesDatabase.txt
        std::stringstream saleLog;
        saleLog << invoiceNum << "|"
                << dateTimeStr << "|"
                << customerName << "|"
                << std::fixed << std::setprecision(2) << subtotal << "|"
                << gstPercent << "|"
                << grandTotal << "|";
        for (size_t i = 0; i < cart.size(); ++i) {
            saleLog << cart[i].medicineID << ":" << cart[i].quantity;
            if (i + 1 < cart.size()) {
                saleLog << ";";
            }
        }
        FileManager::appendLine(m_salesDbPath, saleLog.str());

        // Step 7: Deduct Inventory Stock & Update Customer History
        for (const auto& item : cart) {
            Medicine* m = m_inventory.findMedicineByIDMutable(item.medicineID);
            if (m) {
                m->setQuantity(m->getQuantity() - item.quantity);
            }
        }

        if (customer) {
            std::stringstream summary;
            summary << invoiceNum << " (" << dateTimeStr.substr(0, 10) << "): Grand Total $" << std::fixed << std::setprecision(2) << grandTotal;
            customer->addPurchase(summary.str());
        }

        // Save all changes permanently
        m_inventory.saveAllToDisk();

        Utility::printColor("\nSuccess: Invoice generated and saved. Stock updated!\n", Utility::GREEN);
        Utility::pause();
    }
}
