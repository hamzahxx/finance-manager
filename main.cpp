#include <ncurses.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Transaction {
 public:
  string type;      // Income or Expense
  string category;  // E.g. Food, Subscription
  double amount;    // Amount of transaction

  // Constructor
  Transaction(const string& type, const string& category, double amount) {
    this->type = type;
    this->category = category;
    this->amount = amount;
  }
};

// Display Transactions

int viewTransactionsList(vector<Transaction>& trns, WINDOW* menu) {
  if (trns.empty()) {
    wclear(menu);
    box(menu, 1, 0);
    mvwprintw(menu, 2, 2, "No transactions to display!");
    wrefresh(menu);
    getch();
    return 1;
  }

  int scroll = 0;
  int filterMode = 0;  // 0 = All, 1 = Income, 2 = Expense
  int maxDisplay = 4;
  bool done = false;

  while (!done) {
    wclear(menu);
    box(menu, 1, 0);

    // Header with instructions
    mvwprintw(menu, 1, 2, "TRANSACTIONS LIST (Q to menu)");
    mvwprintw(menu, 2, 2, "Filter: %s (F to change)",
              filterMode == 0   ? "All"
              : filterMode == 1 ? "Income"
                                : "Expense");

    // Filter transactions
    vector<Transaction> filtered;
    for (const auto& t : trns) {
      if (filterMode == 0 || (filterMode == 1 && t.type == "Income") ||
          (filterMode == 2 && t.type == "Expense")) {
        filtered.push_back(t);
      }
    }

    if (filtered.empty()) {
      mvwprintw(menu, 4, 2, "No matching transactions");
    } else {
      int startIdx = min(scroll, (int)filtered.size() - 1);
      int endIdx = min(startIdx + maxDisplay, (int)filtered.size());

      for (int i = startIdx; i < endIdx; i++) {
        int row = 4 + (i - startIdx);
        const auto& t = filtered[i];

        // Use color for income/expense if terminal supports it
        if (has_colors()) {
          if (t.type == "Income")
            wattron(menu, COLOR_PAIR(1));
          else
            wattron(menu, COLOR_PAIR(2));
        }

        mvwprintw(menu, row, 2, "%s: $%.2f (%s)", t.category.c_str(), t.amount,
                  t.type.c_str());

        if (has_colors()) {
          if (t.type == "Income")
            wattroff(menu, COLOR_PAIR(1));
          else
            wattroff(menu, COLOR_PAIR(2));
        }
      }

      // Scroll indicators
      if (startIdx > 0) {
        mvwprintw(menu, 3, 25, "More above");
      }
      if (endIdx < filtered.size()) {
        mvwprintw(menu, 4 + maxDisplay, 25, "More below");
      }
    }
    wrefresh(menu);

    int key = getch();
    switch (key) {
      case 'f':
      case 'F':
        filterMode = (filterMode + 1) % 3;
        scroll = 0;
        break;
      case KEY_UP:
        if (scroll > 0) scroll--;
        break;
      case KEY_DOWN:
        if (scroll + maxDisplay < (int)filtered.size()) scroll++;
        break;
      case 'q':
      case 'Q':
        done = true;
        break;
    }
  }
  return 0;
}


void displayMenu() {
  cout << "\n-------MENU-------\n"
       << "1. Add a Transaction\n"
       << "2. Delete a Transaction\n"
       << "3. View all Transactions\n"
       << "4. Exit\n\n";
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
        
        cout << "Adding Transaction\n";
        addTransaction(trns, filename);
        break;
      case 2:
        
        cout << "Removing Transaction\n";
        removeTransaction(trns, filename);
        break;
      case 3:
        
        cout << "Displaying Transactions\n";
        break;
      case 4:
        
        exit = true;
        saveTransactions(trns, filename);
        cout << "Program Ended\n\n";
        break;
    }
  }
  return 0;
}