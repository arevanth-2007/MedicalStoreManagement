/**
 * @file Billing.h
 * @brief Definition of the Billing class managing purchase checkouts and invoice generation.
 */

#ifndef BILLING_H
#define BILLING_H

#include "Inventory.h"
#include <string>
#include <vector>

namespace MedicalStore {

    /**
     * @struct BillItem
     * @brief Holds information about a medicine item added to the active shopping cart.
     */
    struct BillItem {
        std::string medicineID;    ///< ID of the medicine
        std::string name;          ///< Name of the medicine
        double price;              ///< Price per unit
        int quantity;              ///< Selected quantity
        double total;              ///< Subtotal for this item (price * quantity)
    };

    /**
     * @class Billing
     * @brief Coordinates shopping cart building, stock deduction, database logging, and receipt printing.
     */
    class Billing {
    private:
        Inventory& m_inventory;      ///< Reference to the shared Inventory instance
        std::string m_salesDbPath;   ///< Path to the Sales Database text file
        std::string m_invoiceDir;    ///< Directory path for saving printable invoices

        /**
         * @brief Helper to generate a unique invoice number.
         * @return The invoice number string.
         */
        std::string generateInvoiceNumber();

    public:
        /**
         * @brief Constructor.
         * @param inventory Reference to the system Inventory.
         * @param salesDbPath Path to the SalesDatabase.txt.
         * @param invoiceDir Directory where individual customer invoice text files will be stored.
         */
        Billing(Inventory& inventory, 
                const std::string& salesDbPath = "database/SalesDatabase.txt",
                const std::string& invoiceDir = "reports");

        /**
         * @brief Launches the interactive checkout terminal UI loop.
         */
        void performBilling();
    };
}

#endif // BILLING_H
