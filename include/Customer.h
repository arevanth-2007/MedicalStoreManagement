/**
 * @file Customer.h
 * @brief Definition of the Customer class representing a store customer.
 */

#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include <vector>

namespace MedicalStore {

    /**
     * @class Customer
     * @brief Represents a customer of the medical store, tracking contact details and purchases.
     */
    class Customer {
    private:
        std::string m_customerID;               ///< Unique Customer ID (e.g. CUST001)
        std::string m_name;                     ///< Customer Name
        std::string m_mobileNumber;             ///< 10-digit mobile number
        std::string m_address;                  ///< Residential address
        std::vector<std::string> m_purchaseHistory; ///< List of previous bills/invoices (summaries)

    public:
        /**
         * @brief Default constructor.
         */
        Customer();

        /**
         * @brief Parameterized constructor.
         * @throws std::invalid_argument if parameters are invalid (empty fields or invalid mobile number).
         */
        Customer(const std::string& id, const std::string& name, const std::string& mobile, const std::string& address);

        // Getters
        std::string getCustomerID() const;
        std::string getName() const;
        std::string getMobileNumber() const;
        std::string getAddress() const;
        std::vector<std::string> getPurchaseHistory() const;

        // Setters (with validation)
        void setCustomerID(const std::string& id);
        void setName(const std::string& name);
        void setMobileNumber(const std::string& mobile);
        void setAddress(const std::string& address);
        void setPurchaseHistory(const std::vector<std::string>& history);

        /**
         * @brief Adds an entry to the customer's purchase history.
         * @param invoiceSummary A summary of the invoice.
         */
        void addPurchase(const std::string& invoiceSummary);

        /**
         * @brief Serializes the customer object to a single-line delimited string.
         * @return The delimited string.
         */
        std::string toString() const;

        /**
         * @brief Deserializes a single line from the database into a Customer object.
         * @param line The delimited database line.
         * @return A Customer object populated with fields.
         * @throws std::runtime_error if parsing fails.
         */
        static Customer fromString(const std::string& line);

        // Operator Overloads
        /**
         * @brief Overloaded equality operator comparing IDs.
         */
        bool operator==(const Customer& other) const;

        /**
         * @brief Overloaded inequality operator comparing IDs.
         */
        bool operator!=(const Customer& other) const;

        /**
         * @brief Overloaded less-than operator comparing names alphabetically.
         */
        bool operator<(const Customer& other) const;
    };
}

#endif // CUSTOMER_H
