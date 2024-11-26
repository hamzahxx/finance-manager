#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Transaction {
public:
    string type;     // Income or Expense
    string category; // E.g. Food, Subscription
    double amount;   // Amount of transaction

    // Constructor
    Transaction(const string &type, const string &category, double amount) {
        this->type = type;
        this->category = category;
        this->amount = amount;
    }

    void displayTransaction() const {
        cout << "Type: " << type << endl
             << "Category: " << category << endl
             << "Amount: " << amount << endl;
    }
};

// Display of Transactions
int displayTransactions(const vector<Transaction>& trns) {
    if (trns.empty()) {
        cout << "\n--------------------------\n"
             << "Transaction list is empty!"
             << "\n--------------------------\n";
        return 1;
    }
    double totalIncome = 0.0, totalExpense = 0.0;
    for (size_t i = 0; i < trns.size(); i++) {
        cout << "\nTransaction no: " << i + 1 << endl;
        trns[i].displayTransaction();
        cout << "---------------------\n";
        if (trns[i].type == "Income") {
            totalIncome += trns[i].amount;
        } else if (trns[i].type == "Expense") {
            totalExpense += trns[i].amount;
        }
    }
    cout << "\n---- Summary ----" << endl
         << "Total Income: ₹" << totalIncome << endl
         << "Total Expense: ₹" << totalExpense << endl
         << "Net Balance: ₹" << (totalIncome - totalExpense) << endl;
    return 0;
}

// Adding of Transactions
int addTransaction(vector<Transaction>& trns) {
    int input;
    double amount;
    string category;
    // Input for transaction type
    cout << "Transaction Type:\n"
         << "   1. Income\n"
         << "   2. Expense\n";
    cin >> input;

    // String input for category
    cout << "\nType the category of Transaction: ";
    cin >> category;

    // Double int input for amount
    cout << "\nEnter the amount: ";
    while (true) {
        cin >> amount;
        if (cin.fail()) {
            // Handle non-numeric input
            cout << "Invalid input. Please enter a valid numeric amount: ";
            cin.clear(); // Clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
        } else if (amount <= 0) {
            // Handle non-positive values
            cout << "Invalid input. Amount must be greater than 0. Try again: ";
        } else {
            // Valid input
            break;
        }
    }

    // Validating Amount
    if (amount <= 0) {
        cout << "Error: Amount must be greater than 0." << endl;
        return 1;
    }

    // Transaction Type with Validation of Type
    if (input == 1) {
        trns.emplace_back("Income", category, amount);
    }
    else if (input == 2) {
        trns.emplace_back("Expense", category, amount);
    }
    else {
        cout << "Error: Please choose a valid transaction type (1 for Income, 2 for Expense)" << endl;
        return 1;
    }

    // transaction added successfully
    cout << "\nTransaction added successfully!" << endl;
    return 0;
}

// Removal of Transactions
int removeTransaction(vector<Transaction>& trns) {
    // transaction list validation
    if (trns.empty()) {
        cout << "\n--------------------------\n"
             << "Transaction list is empty!"
             << "\n--------------------------\n";
        return 1;
    }
    int transactionId;
    displayTransactions(trns);
    cout << "\nEnter the id of the transaction to be removed: ";
    while (true) {
        cin >> transactionId; // Attempt to read input

        if (cin.fail()) {
            // Handle non-numeric input
            cout << "Invalid input. Please enter a valid transaction ID: ";
            cin.clear(); // Clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard bad input
        } else if (transactionId <= 0 || transactionId > trns.size()) {
            // Handle out-of-bounds ID
            cout << "Transaction ID must be between 1 and " << trns.size() << ". Try again: ";
        } else {
            // Valid transaction ID
            break;
        }
    }

    // transaction id validation
    if (transactionId > trns.size() || transactionId <= 0) {
        cout << "Error: Invalid transaction ID. Please try again." << endl;
        return 1;
    }
    transactionId -= 1;
    trns.erase(trns.begin() + transactionId);
    cout << "\nTransaction removed successfully." << endl;
    return 0;
}

// Main Function

int main() {
    bool exit = false;
    vector<Transaction> trns;
    while (!exit) {
        int option;
        cout << "\n-------MENU-------\n"
             << "1. Add a Transaction\n"
             << "2. Delete a Transaction\n"
             << "3. View all Transactions\n"
             << "4. Exit\n\n";
        cin >> option;
        switch (option) {
        case 1:
            addTransaction(trns);
            break;
        case 2:
            removeTransaction(trns);
            break;
        case 3:
            displayTransactions(trns);
            break;
        case 4:
            exit = true;
            break;
        default:
            cout << "Invalid Option\n";
            break;
        }
    }
    return 0;
}