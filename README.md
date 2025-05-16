# The-Student-Spot-Summer-Internship-May-2025

# Menu driven program using functions
# 📈 Budget & Expense Tracker (C++ Console App)

> An intuitive, menu-driven C++ console application to manage personal finances by tracking trades, expenses, and generating transaction history
> All without external dependencies or a database.
> Built with object-oriented design and a focus on extensibility.

## 🚀 Features

- ✅ Multi-account Support  
- ✅ Secure PIN Authentication  
- ✅ Deposits, Withdrawals & Trade Records  
- ✅ Comprehensive Transaction History  
- ✅ Admin View for All Accounts  
- ✅ Data Persistence via File I/O  
- ✅ Modern C++ Best Practices


## 💡 Use Case

Designed for individuals who want a fast, offline solution to track both **expenses** and **investment trades**, all in one place. Ideal for developers, students, and finance enthusiasts.


## 🛠️ Tech Stack

- **Language:** C++
- **Paradigm:** Object-Oriented Programming (OOP)
- **File Handling:** `<fstream>` for data persistence
- **Standard Library:** STL (`<vector>`, `<string>`, `<iomanip>`, `<algorithm>`)
- **Compiler:** GCC / MinGW (Cross-platform support)

---

## 📁 Project Structure

<pre> ```plaintext BudgetExpenseTracker/ ├── task3.cpp # Main application code ├── README.md # Project documentation └── accounts.txt # Exported accounts data ``` </pre>


## 🧩 Core Modules

| Module    | Description                                                                       |
| --------- | --------------------------------------------------------------------------------- |
| `Account` | Manages individual account operations (auth, deposit, withdraw, transaction log). |
| `Bank`    | Manages all accounts, creates users, admin controls, and file saving.             |

## 🔐 Security Features

--> 4-digit PIN required for each account
--> PIN change functionality
--> Validation on every financial action
--> Input sanitization for numerical entries

## 📈 Sample Transactions

===== Transaction History for Account ACC1001 =====
 - Initial deposit: $1000
 - Deposit: $500
 - Withdrawal: $250
 - PIN changed

## 🧠 Potential Improvements

Add categories for expenses and trades
Use file encryption for better security
Add graphical visualization using a GUI (SFML/Qt)
Integrate SQLite or JSON for better data structure
Multi-user cloud sync with REST APIs

## 📃 License

MIT License — feel free to use, fork, or enhance. Attribution is appreciated.

## 🤝 Connect

Made with 💻 and ☕ by Saidatta Dasari
Let’s build tools that empower people to track their money better. 🚀

