#include <iostream>
#include<algorithm>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <limits>

using namespace std;

// Forward declarations
class Account;
class Bank;

// Account class
class Account {
private:
    string accountNumber;
    string accountHolder;
    double balance;
    string pin;
    vector<string> transactionHistory;

public:
    // Constructor
    Account(string accNum, string holder, double initialBalance, string pin)
        : accountNumber(accNum), accountHolder(holder), balance(initialBalance), pin(pin) {
        // Record initial deposit
        addTransaction("Initial deposit: $" + to_string(initialBalance));
    }

    // Getters
    string getAccountNumber() const { return accountNumber; }
    string getAccountHolder() const { return accountHolder; }
    double getBalance() const { return balance; }
    
    // Verify PIN
    bool verifyPin(const string& inputPin) const {
        return pin == inputPin;
    }
    
    // Change PIN
    bool changePin(const string& oldPin, const string& newPin) {
        if (verifyPin(oldPin)) {
            pin = newPin;
            addTransaction("PIN changed");
            return true;
        }
        return false;
    }

    // Deposit money
    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            addTransaction("Deposit: $" + to_string(amount));
        }
    }

    // Withdraw money
    bool withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            addTransaction("Withdrawal: $" + to_string(amount));
            return true;
        }
        return false;
    }

    // Add transaction to history
    void addTransaction(const string& transaction) {
        transactionHistory.push_back(transaction);
    }

    // Display transaction history
    void displayTransactionHistory() const {
        cout << "\n===== Transaction History for Account " << accountNumber << " =====\n";
        if (transactionHistory.empty()) {
            cout << "No transactions yet.\n";
        } else {
            for (const auto& transaction : transactionHistory) {
                cout << " - " << transaction << endl;
            }
        }
    }
};

// Bank class
class Bank {
private:
    vector<Account> accounts;
    string bankName;

public:
    // Constructor
    Bank(const string& name) : bankName(name) {}

    // Get bank name
    string getBankName() const { return bankName; }

    // Create new account
    void createAccount() {
        string accountNumber, accountHolder, pin;
        double initialBalance;

        cout << "\n===== Create New Account =====\n";
        
        // Generate account number (simple method for demo)
        accountNumber = "ACC" + to_string(accounts.size() + 1001);
        
        cout << "Account Number: " << accountNumber << " (Generated automatically)\n";
        
        cout << "Enter Account Holder Name: ";
        cin.ignore();
        getline(cin, accountHolder);
        
        cout << "Enter Initial Deposit Amount: $";
        while (!(cin >> initialBalance) || initialBalance < 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid amount. Please enter a positive number: $";
        }
        
        cout << "Create PIN (4 digits): ";
        cin >> pin;
        while (pin.length() != 4 || !all_of(pin.begin(), pin.end(), ::isdigit)) {
            cout << "Invalid PIN. Must be 4 digits: ";
            cin >> pin;
        }
        
        accounts.push_back(Account(accountNumber, accountHolder, initialBalance, pin));
        
        cout << "\nAccount created successfully!" << endl;
        cout << "Account Number: " << accountNumber << endl;
        cout << "Account Holder: " << accountHolder << endl;
        cout << "Initial Balance: $" << fixed << setprecision(2) << initialBalance << endl;
    }

    // Find account by account number
    Account* findAccount(const string& accountNumber) {
        for (auto& account : accounts) {
            if (account.getAccountNumber() == accountNumber) {
                return &account;
            }
        }
        return nullptr;
    }

    // Display all accounts (admin function)
    void displayAllAccounts() const {
        cout << "\n===== All Accounts at " << bankName << " =====\n";
        if (accounts.empty()) {
            cout << "No accounts found.\n";
        } else {
            cout << left << setw(15) << "Account #" << setw(25) << "Account Holder" 
                 << right << setw(15) << "Balance" << endl;
            cout << string(55, '-') << endl;
            
            for (const auto& account : accounts) {
                cout << left << setw(15) << account.getAccountNumber() 
                     << setw(25) << account.getAccountHolder()
                     << right << setw(15) << fixed << setprecision(2) << account.getBalance() << endl;
            }
        }
    }

    // Save accounts to file
    void saveAccountsToFile(const string& filename) {
        ofstream file(filename);
        if (file.is_open()) {
            for (const auto& account : accounts) {
                file << account.getAccountNumber() << ","
                     << account.getAccountHolder() << ","
                     << account.getBalance() << endl;
            }
            file.close();
            cout << "Accounts saved to " << filename << endl;
        } else {
            cout << "Unable to open file for saving accounts." << endl;
        }
    }
};

// Function to display main menu
void displayMainMenu() {
    cout << "\n===== BANKING SYSTEM MENU =====\n";
    cout << "1. Create New Account\n";
    cout << "2. Account Login\n";
    cout << "3. Admin View (All Accounts)\n";
    cout << "4. Save Accounts to File\n";
    cout << "5. Exit\n";
    cout << "Choice: ";
}

// Function to display account menu
void displayAccountMenu(const string& accountNumber) {
    cout << "\n===== ACCOUNT MENU (" << accountNumber << ") =====\n";
    cout << "1. Check Balance\n";
    cout << "2. Deposit\n";
    cout << "3. Withdraw\n";
    cout << "4. Transaction History\n";
    cout << "5. Change PIN\n";
    cout << "6. Logout\n";
    cout << "Choice: ";
}

int main() {
    Bank bank("C++ Banking System");
    int choice;
    bool running = true;
    
    cout << "Welcome to " << bank.getBankName() << "!" << endl;
    
    while (running) {
        displayMainMenu();
        
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        
        switch (choice) {
            case 1: // Create New Account
                bank.createAccount();
                break;
                
            case 2: { // Account Login
                string accountNumber, pin;
                cout << "\n===== Account Login =====\n";
                cout << "Enter Account Number: ";
                cin >> accountNumber;
                cout << "Enter PIN: ";
                cin >> pin;
                
                Account* account = bank.findAccount(accountNumber);
                if (account && account->verifyPin(pin)) {
                    cout << "\nLogin successful. Welcome, " << account->getAccountHolder() << "!" << endl;
                    
                    bool accountMenuRunning = true;
                    while (accountMenuRunning) {
                        displayAccountMenu(accountNumber);
                        
                        int accountChoice;
                        if (!(cin >> accountChoice)) {
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Invalid input. Please enter a number.\n";
                            continue;
                        }
                        
                        switch (accountChoice) {
                            case 1: // Check Balance
                                cout << "\nCurrent Balance: $" << fixed << setprecision(2) 
                                     << account->getBalance() << endl;
                                break;
                                
                            case 2: { // Deposit
                                double amount;
                                cout << "\nEnter deposit amount: $";
                                while (!(cin >> amount) || amount <= 0) {
                                    cin.clear();
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    cout << "Invalid amount. Please enter a positive number: $";
                                }
                                account->deposit(amount);
                                cout << "Deposit successful. New balance: $" << fixed << setprecision(2) 
                                     << account->getBalance() << endl;
                                break;
                            }
                                
                            case 3: { // Withdraw
                                double amount;
                                cout << "\nEnter withdrawal amount: $";
                                while (!(cin >> amount) || amount <= 0) {
                                    cin.clear();
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    cout << "Invalid amount. Please enter a positive number: $";
                                }
                                
                                if (account->withdraw(amount)) {
                                    cout << "Withdrawal successful. New balance: $" << fixed << setprecision(2) 
                                         << account->getBalance() << endl;
                                } else {
                                    cout << "Withdrawal failed. Insufficient funds." << endl;
                                }
                                break;
                            }
                                
                            case 4: // Transaction History
                                account->displayTransactionHistory();
                                break;
                                
                            case 5: { // Change PIN
                                string oldPin, newPin;
                                cout << "\nEnter current PIN: ";
                                cin >> oldPin;
                                cout << "Enter new PIN (4 digits): ";
                                cin >> newPin;
                                
                                while (newPin.length() != 4 || !all_of(newPin.begin(), newPin.end(), ::isdigit)) {
                                    cout << "Invalid PIN. Must be 4 digits: ";
                                    cin >> newPin;
                                }
                                
                                if (account->changePin(oldPin, newPin)) {
                                    cout << "PIN changed successfully!" << endl;
                                } else {
                                    cout << "PIN change failed. Incorrect current PIN." << endl;
                                }
                                break;
                            }
                                
                            case 6: // Logout
                                cout << "\nLogging out..." << endl;
                                accountMenuRunning = false;
                                break;
                                
                            default:
                                cout << "Invalid choice. Please try again." << endl;
                        }
                        
                        if (accountMenuRunning) {
                            cout << "\nPress Enter to continue...";
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cin.get();
                        }
                    }
                } else {
                    cout << "\nLogin failed. Invalid account number or PIN." << endl;
                }
                break;
            }
                
            case 3: // Admin View (All Accounts)
                bank.displayAllAccounts();
                break;
                
            case 4: { // Save Accounts to File
                string filename;
                cout << "\nEnter filename to save accounts: ";
                cin >> filename;
                bank.saveAccountsToFile(filename);
                break;
            }
                
            case 5: // Exit
                cout << "\nThank you for using " << bank.getBankName() << "!" << endl;
                running = false;
                break;
                
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
        
        if (running) {
            cout << "\nPress Enter to continue...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
    }
    
    return 0;
}
