#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>

// =================== Expense Class ===================
class Expense {
public:
    std::string date;
    std::string category;
    double amount;
    std::string description;

    Expense() : date(""), category(""), amount(0), description("") {}
    Expense(const std::string& d, const std::string& c, double a, const std::string& desc)
        : date(d), category(c), amount(a), description(desc) {}

    // Input an expense from user
    void inputExpense() {
        std::cout << "Enter date (YYYY-MM-DD): ";
        std::cin >> date;
        std::cin.ignore();
        std::cout << "Enter category: ";
        std::getline(std::cin, category);
        std::cout << "Enter amount: ";
        std::cin >> amount;
        std::cin.ignore();
        std::cout << "Enter description: ";
        std::getline(std::cin, description);
    }

    // Display expense
    void displayExpense() const {
        std::cout << std::setw(12) << date
                  << std::setw(15) << category
                  << std::setw(10) << std::fixed << std::setprecision(2) << amount
                  << std::setw(25) << description << "\n";
    }

    // Convert expense to CSV string
    std::string toCSV() const {
        std::ostringstream oss;
        oss << date << "," << category << "," << amount << "," << description;
        return oss.str();
    }

    // Load expense from CSV string
    static Expense fromCSV(const std::string& line) {
        std::istringstream iss(line);
        std::string d, c, amtStr, desc;
        std::getline(iss, d, ',');
        std::getline(iss, c, ',');
        std::getline(iss, amtStr, ',');
        std::getline(iss, desc);
        double amt = std::stod(amtStr);
        return Expense(d, c, amt, desc);
    }
};

// =================== Expense Manager Class ===================
class ExpenseManager {
private:
    std::vector<Expense> expenses;

public:
    // 1. Add a new expense
    void addExpense() {
        Expense e;
        e.inputExpense();
        expenses.push_back(e);
        std::cout << "Expense added successfully!\n";
    }

    // 2. View all expenses
    void viewExpenses() const {
        if (expenses.empty()) {
            std::cout << "No expenses recorded.\n";
            return;
        }
        std::cout << std::setw(12) << "Date"
                  << std::setw(15) << "Category"
                  << std::setw(10) << "Amount"
                  << std::setw(25) << "Description" << "\n";
        std::cout << "---------------------------------------------------------------\n";
        for (const auto& e : expenses) {
            e.displayExpense();
        }
    }

    // 3. Calculate total expenses
    void totalExpenses() const {
        double total = 0;
        for (const auto& e : expenses) total += e.amount;
        std::cout << "Total expenses: $" << std::fixed << std::setprecision(2) << total << "\n";
    }

    // 4. Save expenses to a CSV file
    void saveToFile(const std::string& filename) const {
        std::ofstream outFile(filename);
        for (const auto& e : expenses) {
            outFile << e.toCSV() << "\n";
        }
        outFile.close();
        std::cout << "Data saved to file successfully!\n";
    }

    // 5. Load expenses from a CSV file
    void loadFromFile(const std::string& filename) {
        std::ifstream inFile(filename);
        if (!inFile) return;

        std::string line;
        while (std::getline(inFile, line)) {
            expenses.push_back(Expense::fromCSV(line));
        }
        inFile.close();
    }
};

// =================== Main Function ===================
int main() {
    ExpenseManager manager;
    manager.loadFromFile("expenses.csv"); // Load previous data if available

    int choice;
    do {
        std::cout << "\n=== Personal Expense Tracker ===\n";
        std::cout << "1. Add Expense\n";
        std::cout << "2. View Expenses\n";
        std::cout << "3. Total Expenses\n";
        std::cout << "4. Save & Exit\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;

        // Handle invalid input
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input! Please enter a number.\n";
            continue;
        }

        switch (choice) {
            case 1:
                manager.addExpense();
                break;
            case 2:
                manager.viewExpenses();
                break;
            case 3:
                manager.totalExpenses();
                break;
            case 4:
                manager.saveToFile("expenses.csv");
                std::cout << "Exiting...\n";
                break;
            default:
                std::cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 4);

    return 0;
}
