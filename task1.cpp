#include <iostream>
#include<algorithm>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <ctime>

using namespace std;

// Structure to store transaction details
struct Transaction {
    time_t date;
    string category;
    double amount;
    string description;
    bool isIncome;
};

// Global variables
vector<Transaction> transactions;
double savingsGoal = 0.0;

// Function prototypes
void displayMenu();
void addIncome();
void addExpense();
void viewSummary();
void setSavingsGoal();
void calculateMonthlySummary();
void checkOverspending();
void checkSavingsGoal();
void provideSavingsSuggestions();
void clearInputBuffer();
double getValidAmount();
string getCurrentMonth();
string getFormattedDate(time_t date);

int main() {
    int choice;
    bool running = true;
    
    cout << "===== Budget & Expense Tracker =====\n";
    
    while (running) {
        displayMenu();
        
        cout << "Enter your choice (1-5): ";
        if (!(cin >> choice)) {
            cin.clear();
            clearInputBuffer();
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        
        switch (choice) {
            case 1:
                addIncome();
                break;
            case 2:
                addExpense();
                break;
            case 3:
                viewSummary();
                break;
            case 4:
                setSavingsGoal();
                break;
            case 5:
                cout << "\nThank you for using Budget & Expense Tracker!\n";
                running = false;
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
        
        if (running) {
            cout << "\nPress Enter to continue...";
            clearInputBuffer();
        }
    }
    
    return 0;
}

// Function to display the main menu
void displayMenu() {
    cout << "\n===== MENU =====\n";
    cout << "1. Add Income\n";
    cout << "2. Add Expense\n";
    cout << "3. View Summary\n";
    cout << "4. Set Savings Goal\n";
    cout << "5. Exit\n";
}

// Function to add income
void addIncome() {
    Transaction income;
    income.isIncome = true;
    income.date = time(nullptr); // Current date/time
    
    cout << "\n===== Add Income =====\n";
    
    cout << "Category (e.g., Salary, Freelance, Gift): ";
    clearInputBuffer();
    getline(cin, income.category);
    
    cout << "Amount: $";
    income.amount = getValidAmount();
    
    cout << "Description: ";
    getline(cin, income.description);
    
    transactions.push_back(income);
    cout << "\nIncome of $" << fixed << setprecision(2) << income.amount
         << " added successfully!\n";
    
    // Check if we're meeting our savings goal
    checkSavingsGoal();
}

// Function to add expense
void addExpense() {
    Transaction expense;
    expense.isIncome = false;
    expense.date = time(nullptr); // Current date/time
    
    cout << "\n===== Add Expense =====\n";
    
    cout << "Category (e.g., Food, Rent, Entertainment): ";
    clearInputBuffer();
    getline(cin, expense.category);
    
    cout << "Amount: $";
    expense.amount = getValidAmount();
    
    cout << "Description: ";
    getline(cin, expense.description);
    
    transactions.push_back(expense);
    cout << "\nExpense of $" << fixed << setprecision(2) << expense.amount
         << " added successfully!\n";
    
    // Check for overspending
    checkOverspending();
}

// Function to view summary
void viewSummary() {
    if (transactions.empty()) {
        cout << "\nNo transactions found. Start by adding income or expenses.\n";
        return;
    }
    
    cout << "\n===== Financial Summary =====\n";
    
    // Overall summary
    double totalIncome = 0.0;
    double totalExpenses = 0.0;
    
    for (const auto& transaction : transactions) {
        if (transaction.isIncome) {
            totalIncome += transaction.amount;
        } else {
            totalExpenses += transaction.amount;
        }
    }
    
    double balance = totalIncome - totalExpenses;
    double savingsRate = (totalIncome > 0) ? ((totalIncome - totalExpenses) / totalIncome * 100) : 0;
    
    cout << "Total Income: $" << fixed << setprecision(2) << totalIncome << endl;
    cout << "Total Expenses: $" << fixed << setprecision(2) << totalExpenses << endl;
    cout << "Current Balance: $" << fixed << setprecision(2) << balance << endl;
    cout << "Savings Rate: " << fixed << setprecision(1) << savingsRate << "%\n";
    
    if (savingsGoal > 0) {
        cout << "Savings Goal: $" << fixed << setprecision(2) << savingsGoal;
        if (balance >= savingsGoal) {
            cout << " (Achieved!)\n";
        } else {
            cout << " (Need $" << fixed << setprecision(2) << (savingsGoal - balance) << " more)\n";
        }
    }
    
    // Monthly summary
    calculateMonthlySummary();
    
    // Show recent transactions
    cout << "\n----- Recent Transactions -----\n";
    cout << left << setw(12) << "Date" << setw(15) << "Category" 
         << setw(10) << "Amount" << "Description\n";
    cout << string(60, '-') << endl;
    
    int count = 0;
    for (auto it = transactions.rbegin(); it != transactions.rend() && count < 10; ++it, ++count) {
        cout << left << setw(12) << getFormattedDate(it->date) 
             << setw(15) << it->category 
             << right << setw(9) << fixed << setprecision(2) << it->amount
             << " " << (it->isIncome ? "(+)" : "(-)") << " "
             << left << it->description << endl;
    }
    
    // Provide savings suggestions
    provideSavingsSuggestions();
}

// Function to set savings goal
void setSavingsGoal() {
    cout << "\n===== Set Savings Goal =====\n";
    
    if (savingsGoal > 0) {
        cout << "Current savings goal: $" << fixed << setprecision(2) << savingsGoal << endl;
        cout << "Would you like to update it? (y/n): ";
        char choice;
        cin >> choice;
        if (tolower(choice) != 'y') {
            return;
        }
    }
    
    cout << "Enter your savings goal amount: $";
    savingsGoal = getValidAmount();
    
    cout << "\nSavings goal set to $" << fixed << setprecision(2) << savingsGoal << "!\n";
    
    // Check if we're already meeting this goal
    checkSavingsGoal();
}

// Function to calculate monthly summary
void calculateMonthlySummary() {
    string currentMonth = getCurrentMonth();
    double monthlyIncome = 0.0;
    double monthlyExpenses = 0.0;
    
    struct tm* timeinfo;
    char buffer[80];
    
    for (const auto& transaction : transactions) {
        timeinfo = localtime(&transaction.date);
        strftime(buffer, sizeof(buffer), "%B %Y", timeinfo);
        
        if (string(buffer) == currentMonth) {
            if (transaction.isIncome) {
                monthlyIncome += transaction.amount;
            } else {
                monthlyExpenses += transaction.amount;
            }
        }
    }
    
    double monthlyBalance = monthlyIncome - monthlyExpenses;
    
    cout << "\n----- " << currentMonth << " Summary -----\n";
    cout << "Monthly Income: $" << fixed << setprecision(2) << monthlyIncome << endl;
    cout << "Monthly Expenses: $" << fixed << setprecision(2) << monthlyExpenses << endl;
    cout << "Monthly Balance: $" << fixed << setprecision(2) << monthlyBalance << endl;
    
    // Check category breakdown for current month
    if (!transactions.empty()) {
        // Create a map to store expenses by category
        vector<pair<string, double>> categoryExpenses;
        
        for (const auto& transaction : transactions) {
            if (!transaction.isIncome) {
                timeinfo = localtime(&transaction.date);
                strftime(buffer, sizeof(buffer), "%B %Y", timeinfo);
                
                if (string(buffer) == currentMonth) {
                    // Check if category already exists
                    bool found = false;
                    for (auto& catExp : categoryExpenses) {
                        if (catExp.first == transaction.category) {
                            catExp.second += transaction.amount;
                            found = true;
                            break;
                        }
                    }
                    
                    if (!found) {
                        categoryExpenses.push_back({transaction.category, transaction.amount});
                    }
                }
            }
        }
        
        if (!categoryExpenses.empty()) {
            cout << "\n----- Expense Breakdown -----\n";
            cout << left << setw(20) << "Category" << setw(10) << "Amount" 
                 << "% of Total\n";
            cout << string(50, '-') << endl;
            
            for (const auto& catExp : categoryExpenses) {
                double percentage = (monthlyExpenses > 0) ? 
                    (catExp.second / monthlyExpenses * 100) : 0;
                
                cout << left << setw(20) << catExp.first 
                     << "$" << right << setw(8) << fixed << setprecision(2) << catExp.second
                     << "  " << left << fixed << setprecision(1) << percentage << "%" << endl;
            }
        }
    }
}

// Function to check for overspending
void checkOverspending() {
    string currentMonth = getCurrentMonth();
    double monthlyIncome = 0.0;
    double monthlyExpenses = 0.0;
    
    struct tm* timeinfo;
    char buffer[80];
    
    for (const auto& transaction : transactions) {
        timeinfo = localtime(&transaction.date);
        strftime(buffer, sizeof(buffer), "%B %Y", timeinfo);
        
        if (string(buffer) == currentMonth) {
            if (transaction.isIncome) {
                monthlyIncome += transaction.amount;
            } else {
                monthlyExpenses += transaction.amount;
            }
        }
    }
    
    // Overspending alert logic
    if (monthlyExpenses > monthlyIncome && monthlyIncome > 0) {
        cout << "\n OVERSPENDING ALERT!!! \n";
        cout << "You've spent $" << fixed << setprecision(2) 
             << (monthlyExpenses - monthlyIncome) << " more than your income this month.\n";
        
        double overspendingRate = ((monthlyExpenses - monthlyIncome) / monthlyIncome) * 100;
        cout << "This is " << fixed << setprecision(1) << overspendingRate 
             << "% over your monthly income.\n";
    }
    
    // Spending threshold alerts
    if (monthlyIncome > 0) {
        double spentPercentage = (monthlyExpenses / monthlyIncome) * 100;
        
        if (spentPercentage >= 90 && spentPercentage < 100) {
            cout << "\n WARNING: You've spent " << fixed << setprecision(1) << spentPercentage 
                 << "% of your monthly income.\n";
        }
    }
}

// Function to check if we're meeting savings goal
void checkSavingsGoal() {
    if (savingsGoal <= 0) {
        return; // No goal set
    }
    
    double totalIncome = 0.0;
    double totalExpenses = 0.0;
    
    for (const auto& transaction : transactions) {
        if (transaction.isIncome) {
            totalIncome += transaction.amount;
        } else {
            totalExpenses += transaction.amount;
        }
    }
    
    double balance = totalIncome - totalExpenses;
    
    if (balance >= savingsGoal) {
        cout << "\n CONGRATULATIONS!! \n";
        cout << "You've reached your savings goal of $" << fixed << setprecision(2) << savingsGoal << "!\n";
        cout << "Current savings: $" << fixed << setprecision(2) << balance << endl;
    } else {
        double remaining = savingsGoal - balance;
        cout << "\nSavings Goal Progress: $" << fixed << setprecision(2) << balance 
             << " / $" << savingsGoal << endl;
        cout << "Still need: $" << fixed << setprecision(2) << remaining << endl;
    }
}

// Function to provide savings suggestions
void provideSavingsSuggestions() {
    // Only provide suggestions if we have enough data
    if (transactions.size() < 3) {
        return;
    }
    
    string currentMonth = getCurrentMonth();
    double monthlyIncome = 0.0;
    double monthlyExpenses = 0.0;
    
    // Create a map to store expenses by category
    vector<pair<string, double>> categoryExpenses;
    
    struct tm* timeinfo;
    char buffer[80];
    
    for (const auto& transaction : transactions) {
        timeinfo = localtime(&transaction.date);
        strftime(buffer, sizeof(buffer), "%B %Y", timeinfo);
        
        if (string(buffer) == currentMonth) {
            if (transaction.isIncome) {
                monthlyIncome += transaction.amount;
            } else {
                monthlyExpenses += transaction.amount;
                
                // Check if category already exists
                bool found = false;
                for (auto& catExp : categoryExpenses) {
                    if (catExp.first == transaction.category) {
                        catExp.second += transaction.amount;
                        found = true;
                        break;
                    }
                }
                
                if (!found) {
                    categoryExpenses.push_back({transaction.category, transaction.amount});
                }
            }
        }
    }
    
    // Only provide suggestions if expenses exceed 70% of income
    if (monthlyIncome > 0 && monthlyExpenses > (monthlyIncome * 0.7)) {
        // Sort categories by expense amount (descending)
        sort(categoryExpenses.begin(), categoryExpenses.end(), 
             [](const pair<string, double>& a, const pair<string, double>& b) {
                 return a.second > b.second;
             });
        
        cout << "\n SAVINGS SUGGESTIONS \n";
        
        // Target top spending categories
        if (!categoryExpenses.empty()) {
            double topCategoryAmount = categoryExpenses[0].second;
            string topCategory = categoryExpenses[0].first;
            
            double topCategoryPercentage = (topCategoryAmount / monthlyExpenses) * 100;
            
            if (topCategoryPercentage > 30) {
                cout << "1. Your highest expense category is '" << topCategory 
                     << "' at $" << fixed << setprecision(2) << topCategoryAmount
                     << " (" << fixed << setprecision(1) << topCategoryPercentage << "% of expenses).\n";
                
                cout << "   Try reducing this by 10% to save $" 
                     << fixed << setprecision(2) << (topCategoryAmount * 0.1) << " per month.\n";
            }
        }
        
        // Check for consistent saving rate
        double savingsRate = (monthlyIncome > 0) ? ((monthlyIncome - monthlyExpenses) / monthlyIncome * 100) : 0;
        
        if (savingsRate < 20) {
            cout << "2. Your current savings rate is " << fixed << setprecision(1) << savingsRate 
                 << "%. Try to aim for at least 20%.\n";
            double targetSavings = monthlyIncome * 0.2;
            double additionalSavingsNeeded = targetSavings - (monthlyIncome - monthlyExpenses);
            
            if (additionalSavingsNeeded > 0) {
                cout << "   You would need to reduce expenses by $" 
                     << fixed << setprecision(2) << additionalSavingsNeeded << " per month.\n";
            }
        }
        
        // Look for potential quick wins
        bool hasEntertainment = false;
        bool hasDining = false;
        double entertainmentAmount = 0.0;
        double diningAmount = 0.0;
        
        for (const auto& catExp : categoryExpenses) {
            if (catExp.first == "Entertainment" || 
                catExp.first == "entertainment" || 
                catExp.first == "Movies" || 
                catExp.first == "Gaming") {
                hasEntertainment = true;
                entertainmentAmount = catExp.second;
            }
            
            if (catExp.first == "Dining" || 
                catExp.first == "Restaurant" || 
                catExp.first == "Eating Out" || 
                catExp.first == "Food" || 
                catExp.first == "dining" || 
                catExp.first == "restaurants") {
                hasDining = true;
                diningAmount = catExp.second;
            }
        }
        
        if (hasEntertainment && entertainmentAmount > (monthlyIncome * 0.05)) {
            cout << "3. Your entertainment expenses are $" << fixed << setprecision(2) << entertainmentAmount 
                 << ", which is more than 5% of your income.\n";
            cout << "   Consider finding free or low-cost alternatives for entertainment.\n";
        }
        
        if (hasDining && diningAmount > (monthlyIncome * 0.1)) {
            cout << "4. You spent $" << fixed << setprecision(2) << diningAmount 
                 << " on dining out/restaurants.\n";
            cout << "   Try cooking at home more often to reduce this expense by half.\n";
        }
        
        // General suggestion
        cout << "5. Set up automatic transfers to a savings account right after receiving income.\n";
        cout << "   This 'pay yourself first' approach can help you save consistently.\n";
    } else if (monthlyIncome > 0) {
        // Good savings rate message
        double savingsRate = (monthlyIncome > 0) ? ((monthlyIncome - monthlyExpenses) / monthlyIncome * 100) : 0;
        
        if (savingsRate >= 20) {
            cout << "\n SAVINGS INSIGHT \n";
            cout << "Great job! Your current savings rate is " << fixed << setprecision(1) << savingsRate 
                 << "%, which is excellent.\n";
            cout << "Consider investing some of your savings for long-term growth.\n";
        }
    }
}

// Helper function to clear input buffer
void clearInputBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Helper function to get a valid monetary amount
double getValidAmount() {
    double amount;
    while (!(cin >> amount) || amount < 0) {
        cin.clear();
        clearInputBuffer();
        cout << "Invalid amount. Please enter a positive number: $";
    }
    clearInputBuffer();
    return amount;
}

// Helper function to get current month and year as string
string getCurrentMonth() {
    time_t now = time(nullptr);
    struct tm* timeinfo = localtime(&now);
    
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%B %Y", timeinfo);
    
    return string(buffer);
}

// Helper function to format date
string getFormattedDate(time_t date) {
    struct tm* timeinfo = localtime(&date);
    
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%m/%d/%Y", timeinfo);
    
    return string(buffer);
}
