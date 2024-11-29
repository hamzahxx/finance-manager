#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

#if defined(_WIN32) || defined(_WIN64)
#include <conio.h>
#define clearScreen() system("cls")
#else
#include <ncurses.h>
#define clearScreen() system("clear")
#endif

class Transaction {
 public:
  string type;      // Income or Expense
  string category;  // E.g. Food, Subscription
  double amount;    // Amount of transaction

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

void displayMenu() {
  cout << "\n-------MENU-------\n"
       << "1. Add a Transaction\n"
       << "2. Delete a Transaction\n"
       << "3. View all Transactions\n"
       << "4. Exit\n\n";
}

// Display of Transactions
int displayTransactions(const vector<Transaction> &trns) {
  // In case of Empty list
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
    cout << "------------------\n";
    if (trns[i].type == "Income") {
      totalIncome += trns[i].amount;
    } else if (trns[i].type == "Expense") {
      totalExpense += trns[i].amount;
    }
  }
  cout << "\n---- Summary ----" << endl
       << "Total Income: ₹" << totalIncome << endl
       << "Total Expense: ₹" << totalExpense << endl
       << "Net Balance: ₹" << (totalIncome - totalExpense) << endl
       << endl;

  cout << "Press ENTER/RETURN key to\nreturn to the main menu\n\n";
  cin.ignore();
  cin.get();
  return 0;
}

// Load Transactions
int loadTransactions(vector<Transaction> &trns, const string filename) {
  ifstream inFile(filename);

  if (!inFile.is_open()) {
    cout << "Error: Unable to open file for loading transactions.\n";
    return 1;
  }
  trns.clear();

  string line;
  while (getline(inFile, line)) {
    istringstream iss(line);
    string type, category;
    double amount;

    if (getline(iss, type, ',') && getline(iss, category, ',') &&
        iss >> amount) {
      if ((type == "Income" || type == "Expense") && !category.empty() &&
          amount > 0) {
        trns.emplace_back(type, category, amount);
      } else {
        cout << "Warning: Skipped invalid transaction in file: " << line
             << "\n";
      }
    } else {
      cout << "Warning: Skipped malformed line in file: " << line << "\n";
    }
  }
  cout << "\nTransactions loaded successfully from " << filename << ".\n";
  return 0;
}

// Save Transactions
int saveTransactions(const vector<Transaction> trns, const string filename) {
  ofstream outFile(filename);
  if (outFile.is_open()) {
    for (size_t i = 0; i < trns.size(); ++i) {
      const Transaction &txn = trns[i];
      outFile << txn.type << "," << txn.category << "," << txn.amount << "\n";
    }
    outFile.close();
    return 0;
  } else {
    return 1;
  }
}

// Adding of Transactions
int addTransaction(vector<Transaction> &trns, const string filename) {
  int input;
  double amount;
  string category;
  // Input for transaction type
  cout << "Transaction Type:\n"
       << "   1. Income\n"
       << "   2. Expense\n";
  while (true) {
    cin >> input;
    if (cin.fail() || input > 2) {
      cout << "Error: Please choose a valid transaction type (1 for Income, 2 "
              "for Expense): ";
      cin.clear();  // Clear error flag
      cin.ignore(numeric_limits<streamsize>::max(), '\n');

    } else {
      // Valid Input
      break;
    }
  }

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
      cin.clear();  // Clear error flag
      cin.ignore(numeric_limits<streamsize>::max(),
                 '\n');  // Discard invalid input
    } else if (amount <= 0) {
      // Handle non-positive values
      cout << "Invalid input. Amount must be greater than 0. Try again: ";
    } else {
      // Valid input
      break;
    }
  }

  // Transaction Type with Validation of Type
  if (input == 1) {
    trns.emplace_back("Income", category, amount);
  } else if (input == 2) {
    trns.emplace_back("Expense", category, amount);
  }

  // Save Check
  if (saveTransactions(trns, filename) != 0) {
    cout << "Transaction added, but saving failed.\n";
    return 1;
  }

  // transaction added successfully
  cout << "\nTransaction added and saved successfully!" << endl;
  return 0;
}

// Removal of Transactions
int removeTransaction(vector<Transaction> &trns, const string filename) {
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
    cin >> transactionId;  // Attempt to read input

    if (cin.fail()) {
      // Handle non-numeric input
      cout << "Invalid input. Please enter a valid transaction ID: ";
      cin.clear();                                          // Clear error flag
      cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Discard bad input
    } else if (transactionId <= 0 || transactionId > trns.size()) {
      // Handle out-of-bounds ID
      cout << "Transaction ID must be between 1 and " << trns.size()
           << ". Try again: ";
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

  // Save Check
  if (saveTransactions(trns, filename) != 0) {
    cout << "Transaction removed, but saving failed.\n";
    return 1;
  }

  cout << "\nTransaction removed and changes saved successfully!" << endl;
  return 0;
}

// Main Function

int main() {
  bool exit = false;
  vector<Transaction> trns;
  string filename = "transactions.csv";

  loadTransactions(trns, filename);

  while (!exit) {
    clearScreen();
    displayMenu();

    // Main menu input validation
    int option;
    while (true) {
      cin >> option;
      if (cin.fail()) {
        // Invalid Input
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\nError: Invalid input, Please try again: ";
      } else if (option > 4) {
        // Input above 4
        cout << "\nError: Please enter a valid option (1 to 4): ";
      } else {
        // Valid input
        break;
      }
    }

    switch (option) {
      case 1:
        clearScreen();
        cout << "Adding Transaction\n";
        addTransaction(trns, filename);
        break;
      case 2:
        clearScreen();
        cout << "Removing Transaction\n";
        removeTransaction(trns, filename);
        break;
      case 3:
        clearScreen();
        cout << "Displaying Transactions\n";
        displayTransactions(trns);
        break;
      case 4:
        clearScreen();
        exit = true;
        saveTransactions(trns, filename);
        cout << "Program Ended\n\n";
        break;
    }
  }
  return 0;
}