# Medical Store Management System

A highly modular, professional, offline command-line application written in **C++17** for managing a medical store. The project is designed using core **Object-Oriented Programming (OOP)** principles and uses persistent file handling. It features an interactive colored dashboard, secure hashed login credentials, billing workflows, automated stock levels audits, and dynamic reports generation.

This project is tailored specifically to serve as a **Second Year B.Tech CSE Summer Internship Project** on Object-Oriented Programming in C++.

---

## Project Objectives
- Develop a complete, offline Medical Store Management System.
- Demonstrate core Object-Oriented Programming (OOP) concepts in C++.
- Implement standard file handling (`std::fstream`) for persistent storage.
- Manage medicines, customers, suppliers, billing, and inventory efficiently.
- Generate formatted sales and inventory reports.
- Build a professional, robust command-line application with strict input validation.

---

## Key Features

1. **User Authentication Panel**
   - Hashed password verification using the standard **DJB2** hashing algorithm.
   - Restricts access to authorized administrators only (max 3 login attempts).
   - Dynamic password change functionality updating persistent files in real-time.
   - Professional loading splash screen.

2. **Live Stats Dashboard**
   - Displays real-time aggregate statistics upon login (total medicines, customers, suppliers, sales transactions).
   - Tracks and displays today's revenue.
   - Highlights critical system notifications: out-of-stock, low-stock warnings, expired products, and near-expiry notices.

3. **Submenu Loops**
   - Submenu panels (Medicine, Customer, Supplier, Billing, Inventory, Reports) operate in continuous loops.
   - Users are never returned to the main menu automatically after a single operation.

4. **Medicine Management (CRUD + Search & Sort)**
   - Create, Read, Update, and Delete medicine records.
   - Search medicines by ID, Name, Manufacturer Company, or Category using case-insensitive substring matches.
   - Sort medicines alphabetically, by price (ascending), by quantity (ascending), or chronologically by expiry date.

5. **Customer & Supplier Directory**
   - Full CRUD support for customers (contact info, address, purchase ledger).
   - Full CRUD support for wholesale suppliers (contact info, email, address).
   - Installs pre-loaded sample records on first startup (5 Medicines, 3 Customers, 3 Suppliers) if database files are empty.

6. **Interactive Billing System**
   - Real-time cart compiler with multi-item selection.
   - Automatic quantity stock validation and inventory deduction.
   - Restricts sales of expired medicines.
   - Auto-generates unique Invoice Numbers (`INV-YYYYMMDD-XXXX`).
   - Configurable GST and grand total calculations.
   - Generates beautifully formatted ASCII receipts printed on the console and saved to `reports/Invoice_INV-XXXX.txt`.
   - Records sales to `database/SalesDatabase.txt`.

7. **Database Backup & Restore**
   - Installs backup utilities that copy database text files to the `backup/` directory.
   - Restores database states back to the application and hot-reloads data in memory.

8. **Colored ANSI Console Formatting**
   - Uses ANSI escape codes to render visual success states (Green), warning prompts (Yellow), errors (Red), and banners (Blue).
   - Tables are neatly formatted and aligned using `std::setw`, `std::left`, `std::right`, and `std::setprecision` inside `<iomanip>`.

---

## Folder Structure

```text
MedicalStoreManagement/
├── database/                   # Database text files (autosaved)
│   ├── MedicineDatabase.txt
│   ├── CustomerDatabase.txt
│   ├── SupplierDatabase.txt
│   ├── SalesDatabase.txt
│   └── LoginDatabase.txt
├── backup/                     # Database backup files
├── reports/                    # Generated sales, inventory, and invoice reports
├── docs/                       # Project documentation & Mermaid diagrams
│   ├── flowchart.md
│   └── class_diagram.md
├── include/                    # Header files (*.h)
│   ├── Medicine.h
│   ├── Customer.h
│   ├── Supplier.h
│   ├── Inventory.h
│   ├── Billing.h
│   ├── ReportManager.h
│   ├── Login.h
│   ├── Utility.h
│   └── FileManager.h
├── src/                        # Source files (*.cpp)
│   ├── Medicine.cpp
│   ├── Customer.cpp
│   ├── Supplier.cpp
│   ├── Inventory.cpp
│   ├── Billing.cpp
│   ├── ReportManager.cpp
│   ├── Login.cpp
│   ├── Utility.cpp
│   ├── FileManager.cpp
│   └── main.cpp
└── README.md                   # Setup and project guide
```

---

## OOP Concepts Demonstrated

- **Classes & Objects**: Encapsulating real-world entities like `Medicine`, `Customer`, and `Supplier` into logical C++ models.
- **Encapsulation**: Specifying all model members as private data fields, only exposed via validated public getters and setters.
- **Constructors**: Utilizing parameterized and default constructors with member initialization lists.
- **Composition**: Formulating complex controllers (like `Billing` and `ReportManager`) that reference and manipulate the `Inventory` object.
- **Namespaces**: Isolating the codebase within a distinct `MedicalStore` namespace to avoid naming collisions.
- **Abstraction**: Separating interface designs (`.h` header files) from structural execution logic (`.cpp` source files).
- **STL Containers**: Storing databases dynamically using `std::vector`, manipulating strings using `std::string`, and using `<algorithm>` APIs like `std::sort` and `std::remove_if` to handle database querying and sorting.
- **File Handling**: Writing and reading structured records using `std::ifstream` and `std::ofstream`.
- **Exception Handling**: Implementing try-catch scopes to prevent file operation failures and numeric conversions (`std::stod`, `std::stoi`) from crashing the software on dirty inputs.

---

## How to Compile

This project is fully offline and requires no internet APIs or external libraries. It requires a C++17 compliant compiler (such as modern MinGW g++).

Open your terminal, navigate to the `MedicalStoreManagement` directory, and run the following command:

```bash
g++ -std=c++17 -Wall -Wextra -Iinclude src/main.cpp src/Medicine.cpp src/Customer.cpp src/Supplier.cpp src/Billing.cpp src/Inventory.cpp src/ReportManager.cpp src/Login.cpp src/Utility.cpp src/FileManager.cpp -o MedicalStore.exe
```

---

## How to Run

After compilation, launch the executable directly in your command line:

```bash
./MedicalStore.exe
```

### Default Credentials
- **Default Username:** `admin`
- **Default Password:** `admin123`

*(You can change the password permanently via Main Menu option 7).*

---

## Diagrams & Viva Support

The project includes report-ready documentation inside the `docs/` folder:
- **System flowchart**: [flowchart.md](file:///c:/Users/aavul/OneDrive/Desktop/SummerProject/MedicalStoreManagement/docs/flowchart.md)
- **OOP Class diagram**: [class_diagram.md](file:///c:/Users/aavul/OneDrive/Desktop/SummerProject/MedicalStoreManagement/docs/class_diagram.md)

Open these files in any markdown visualizer supporting Mermaid (such as GitHub, VS Code preview, or online Mermaid editors) to view or copy them for your summer internship report.

---

## Future Scope
- **SQL Database Integration**: Porting the text database to SQLite or MySQL using C++ ODBC wrappers.
- **Network Syncing**: Adding network socket features to synchronize inventories across multiple store terminals.
- **Graphical User Interface (GUI)**: Implementing a GUI using Qt or wxWidgets while preserving the core OOP modular backend.
