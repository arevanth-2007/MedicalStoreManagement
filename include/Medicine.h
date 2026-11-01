/**
 * @file Medicine.h
 * @brief Definition of the Medicine class representing a medicine item.
 */

#ifndef MEDICINE_H
#define MEDICINE_H

#include <string>

namespace MedicalStore {

    /**
     * @class Medicine
     * @brief Represents a single medicine product in the pharmacy store.
     */
    class Medicine {
    private:
        std::string m_medicineID;   ///< Unique ID of the medicine (e.g. MED001)
        std::string m_name;         ///< Name of the medicine
        std::string m_company;      ///< Company/Manufacturer name
        std::string m_category;     ///< Category (e.g. Tablet, Syrup, Capsule)
        std::string m_batchNumber;  ///< Batch code
        std::string m_mfgDate;      ///< Manufacturing date (YYYY-MM-DD)
        std::string m_expiryDate;   ///< Expiry date (YYYY-MM-DD)
        double m_price;             ///< Retail price per unit
        int m_quantity;             ///< Current stock quantity
        std::string m_description;  ///< Brief description of the drug

    public:
        /**
         * @brief Default constructor.
         */
        Medicine();

        /**
         * @brief Parameterized constructor.
         * @throws std::invalid_argument if invalid values are passed (empty strings, negative price/qty).
         */
        Medicine(const std::string& id, const std::string& name, const std::string& company,
                 const std::string& category, const std::string& batch, const std::string& mfg,
                 const std::string& expiry, double price, int qty, const std::string& desc);

        // Getters
        std::string getMedicineID() const;
        std::string getName() const;
        std::string getCompany() const;
        std::string getCategory() const;
        std::string getBatchNumber() const;
        std::string getMfgDate() const;
        std::string getExpiryDate() const;
        double getPrice() const;
        int getQuantity() const;
        std::string getDescription() const;

        // Setters (with validation)
        void setMedicineID(const std::string& id);
        void setName(const std::string& name);
        void setCompany(const std::string& company);
        void setCategory(const std::string& category);
        void setBatchNumber(const std::string& batch);
        void setMfgDate(const std::string& mfg);
        void setExpiryDate(const std::string& expiry);
        void setPrice(double price);
        void setQuantity(int qty);
        void setDescription(const std::string& desc);

        /**
         * @brief Serializes the medicine object to a single-line delimited string.
         * @return The delimited string.
         */
        std::string toString() const;

        /**
         * @brief Deserializes a single line from the database into a Medicine object.
         * @param line The delimited database line.
         * @return A Medicine object populated with fields.
         * @throws std::runtime_error if parsing fails.
         */
        static Medicine fromString(const std::string& line);

        // Operator Overloads
        /**
         * @brief Overloaded equality operator comparing IDs.
         */
        bool operator==(const Medicine& other) const;

        /**
         * @brief Overloaded inequality operator comparing IDs.
         */
        bool operator!=(const Medicine& other) const;

        /**
         * @brief Overloaded less-than operator comparing names alphabetically.
         */
        bool operator<(const Medicine& other) const;
    };
}

#endif // MEDICINE_H
