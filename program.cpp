#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Transaction {
public:
    string type;     // Income or Expense
    string catagory; // E.g. Food, Subscription
    double amount;   // Amount of transaction

    // Constructor
    Transaction(const string &type, const string &catagory, double amount) {
        this->type = type;
        this->catagory = catagory;
        this->amount = amount;
    }

    void displayTransaction() const {
        cout << "Type: " << type << endl
             << "Catagory: " << catagory << endl
             << "Amount: " << amount << endl;
    }
};

int addTransaction(vector<Transaction>& trns) {
    int input;
    double amount;
    string catagory;
    // Input for transaction type
    cout << "Transaction Type:\n"
         << "   1. Income\n"
         << "   2. Expense\n";
    cin >> input;

    // String input for catagory
    cout << "\nType the catagory of Transaction: ";
    cin >> catagory;

    // Double int input for amount
    cout << "\nEnter the amount: ";
    cin >> amount;

    // Validating Amount
    if (amount <= 0) {
        cout << "Error: Amount must be greater than 0." << endl;
        return 1;
    }

    // Transaction Type with Validation of Type
    if (input == 1) {
        trns.emplace_back("Income", catagory, amount);
    }
    else if (input == 2) {
        trns.emplace_back("Expense", catagory, amount);
    }
    else {
        cout << "Error in type of transaction" << endl;
        return 1;
    }

    // transaction added successfully
    cout << "\nTransaction added successfully\n";
    return 0;
}

int displayTransactions(const vector<Transaction>& trns) {
    if (trns.empty()) {
        cout << "Transcation list is empty!\n";
        return 1;
    }
    for (int i = 0; i < trns.size(); i++) {
        cout << "\nTransaction no: " << i+1 << endl;
        trns[i].displayTransaction();
        cout << "---------------------\n";
    }
    return 0;
}

int main() {
    bool exit = false;
    vector<Transaction> trns;
    // trns.emplace_back("Income", "Salary", 5000);
    // trns[0].displayTransaction();
    while (!exit) {
        int option;
        cout << "\n-------MENU-------\n"
             << "1. Add a Transaction\n"
             << "2. View all Transactions\n"
             << "3. Exit\n\n";
        cin >> option;
        switch (option) {
        case 1:
            addTransaction(trns);
            break;
        case 2:
            displayTransactions(trns);
            break;
        case 3:
            exit = true;
            break;
        default:
            cout << "Invalid Option\n";
            break;
        }
    }
    return 0;
}