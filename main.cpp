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

// Draw Dashboard
void drawDashboard(WINDOW* win, vector<Transaction>& trns) {
  wclear(win);
  box(win, 0, 0);
  wrefresh(win);

  double totalIncome = 0, totalExpense = 0;
  for (auto& t : trns) {
    if (t.type == "Income")
      totalIncome += t.amount;
    else
      totalExpense += t.amount;
  }

  mvwprintw(win, 1, 2, "=== PERSONAL FINANCE MANAGER ===");
  mvwprintw(win, 3, 5, "Total Income: %.2f", totalIncome);
  mvwprintw(win, 4, 5, "Total Expense: %.2f", totalExpense);
  mvwprintw(win, 5, 5, "Net Balance: %.2f", totalIncome - totalExpense);
  wrefresh(win);
}

// Load Transactions
int loadTransactions(vector<Transaction>& trns, const string& filename) {
  ifstream inFile(filename);

  if (!inFile.is_open()) {
    printw("Error: Unable to open file for loading transactions.\n");
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
        printw("Warning: Skipped invalid transaction in file: %s\n",
               line.c_str());
      }
    } else {
      printw("Warning: Skipped malformed line in file: %s\n", line.c_str());
    }
  }
  printw("\nTransactions loaded successfully from %s.\n", filename.c_str());
  printw("\nPress any key to continue.");
  getch();

  clear();
  return 0;
}

// Save Transactions
int saveTransactions(const vector<Transaction>& trns, const string& filename) {
  ofstream outFile(filename);
  if (outFile.is_open()) {
    for (size_t i = 0; i < trns.size(); ++i) {
      const Transaction& txn = trns[i];
      outFile << txn.type << "," << txn.category << "," << txn.amount << "\n";
    }
    outFile.close();
    return 0;
  } else {
    printw("Transactions Saving was Unsuccessful.\n");
    printw("\nPress any key to continue.");
    getch();
    return 1;
  }
}

// Adding of Transactions
int addTransaction(vector<Transaction>& trns, const string filename, WINDOW* menu) {
  int choice = 0;
  string type;
  string category;
  double amount;
  bool done = false;

  vector<string> menu_options = {"Add Income", "Add Expense",
                                 "Back to Main Menu"};

  while (!done) {
    wclear(menu);

    box(menu, 1, 0);

    // Draw menu for transaction type
    mvwprintw(menu, 1, 2, "Select transaction type");
    for (int i = 0; i < menu_options.size(); ++i) {
      if (i == choice) wattron(menu, A_REVERSE);
      mvwprintw(menu, 3 + i, 3, menu_options[i].c_str());
      if (i == choice) wattroff(menu, A_REVERSE);
    }

    wrefresh(menu);

    // Navigating the menu
    int key = getch();
    switch (key) {
      case KEY_UP:
        if (choice > 0) choice--;
        break;
      case KEY_DOWN:
        if (choice < menu_options.size() - 1) choice++;
        break;
      case 10:  // Enter key
        if (choice == 2) {
          done = true;
          wclear(menu);
          wmove(menu, 1, 2);
          wprintw(menu, "Transaction was cancelled!");
          wrefresh(menu);
          getch();

          noecho();
          curs_set(0);
          return 1;
        } else {
          if (choice == 0)
            type = "Income";
          else
            type = "Expense";

          echo();
          curs_set(1);

          wclear(menu);
          box(menu, 1, 0);
          wmove(menu, 1, 2);
          wprintw(menu, "Transaction details:");
          wmove(menu, 3, 3);
          wprintw(menu, "Type: %s", type.c_str());

          bool valid_category = false;
          while (!valid_category) {
            char cat_buf[100];
            wmove(menu, 4, 3);
            wprintw(menu, "Category: ");
            wrefresh(menu);
            wgetstr(menu, cat_buf);
            category = string(cat_buf);

            if (category.empty()) {
              wmove(menu, 6, 3);
              wprintw(menu, "Error: Category cannot be empty!");
              wrefresh(menu);
              wmove(menu, 4, 2);
              wclrtoeol(menu);
            } else {
              valid_category = true;
              wmove(menu, 6, 3);
              wclrtoeol(menu);
              wrefresh(menu);
            }
          }

          bool valid_amount = false;
          while (!valid_amount) {
            char amount_buf[100];
            wmove(menu, 5, 3);
            wprintw(menu, "Amount: ");
            wrefresh(menu);
            wclrtoeol(menu);
            wgetstr(menu, amount_buf);

            try {
              amount = stod(amount_buf);
              if (amount <= 0) {
                wmove(menu, 7, 2);
                wprintw(menu, "Error: Amount must be above 0!");
                wrefresh(menu);
              } else {
                valid_amount = true;
                wmove(menu, 7, 1);
                wclrtoeol(menu);
                wrefresh(menu);
              }
            } catch (const invalid_argument& e) {
              wmove(menu, 7, 4);
              wprintw(menu, "Error: Enter a valid number!");
              wrefresh(menu);
            }
          }

          trns.emplace_back(type, category, amount);
          done = true;
        }
        break;
    }
  }

  // Save Check
  if (saveTransactions(trns, filename) != 0) {
    wclear(menu);
    wmove(menu, 1, 1);
    wprintw(menu, "Transaction added, but saving failed.");
    refresh();
    getch();
    return 1;
  }

  // transaction added successfully
  wclear(menu);
  wmove(menu, 1, 1);
  wprintw(menu, "Transaction added!");
  wrefresh(menu);
  getch();

  noecho();
  curs_set(0);
  return 0;
}

// Removal of Transactions
int removeTransaction(vector<Transaction>& trns, const string filename, WINDOW* menu) {
  if (trns.empty()) {
    wclear(menu);
    box(menu, 1, 0);
    wmove(menu, 2, 2);
    wprintw(menu, "There are no transactions!");
    wmove(menu, 4, 2);
    wprintw(menu, "Press any key to continue.");
    wrefresh(menu);
    getch();

    return 1;
  }

  int choice = 0, offset = 0, max_display = 4;
  bool done = false;

  while (!done) {
    wclear(menu);

    box(menu, 1, 0);

    // Draw menu for transaction type
    mvwprintw(menu, 1, 2, "Select transaction (UP/DOWN keys)");

    if (offset > 0) {
      wmove(menu, 2, 13);
      wprintw(menu, "More above");
    }

    if (offset + max_display < trns.size()) {
      wmove(menu, max_display + 3, 13);
      wprintw(menu, "More below");
    }

    int display_count = min((int)trns.size() - offset, max_display);
    for (int i = 0; i < display_count; ++i) {
      int idx = i + offset;
      if (idx == choice) wattron(menu, A_REVERSE);
      mvwprintw(menu, 3 + i, 3, "%s, %s, %.2f", trns[idx].type.c_str(),
                trns[idx].category.c_str(), trns[idx].amount);
      if (idx == choice) wattroff(menu, A_REVERSE);
    }

    wrefresh(menu);

    int c = getch();
    switch (c) {
      case KEY_UP:
        if (choice > 0) {
          choice--;
          if (choice < offset) offset = choice;
        }
        break;
      case KEY_DOWN:
        if (choice < trns.size() - 1) {
          choice++;
          if (choice >= offset + max_display) offset = choice - max_display + 1;
        }
        break;

      case 10:
        trns.erase(trns.begin() + choice);

        if (saveTransactions(trns, filename) == 0) {
          wclear(menu);
          box(menu, 1, 0);
          wmove(menu, 2, 2);
          wprintw(menu, "Transaction removed!");
          wrefresh(menu);
          getch();
        }
        done = true;
        break;

      case 'q':
      case 'Q':
        wclear(menu);
        box(menu, 1, 0);
        wmove(menu, 3, 11);
        wprintw(menu, "Removal");
        wmove(menu, 4, 10);
        wprintw(menu, "Cancelled!");
        wrefresh(menu);
        getch();

        done = true;
        break;
    }
  }
  return 0;
}

// Main Function

int main() {
  initscr();
  noecho();
  cbreak();
  curs_set(0);
  keypad(stdscr, TRUE);

  WINDOW* dashboard = newwin(8, 36, 2, 1);
  WINDOW* menu = newwin(9, 36, 10, 1);
  refresh();

  vector<Transaction> trns;
  string filename = "transactions.csv";

  loadTransactions(trns, filename);

  refresh();

  drawDashboard(dashboard, trns);

  int choice = 0;
  vector<string> menu_options = {"Add Transaction", "Remove Transaction",
                                 "View Transactions", "Exit"};

  while (true) {
    drawDashboard(dashboard, trns);

    wclear(menu);
    box(menu, 1, 0);
    wmove(menu, 1, 7);
    wprintw(menu, "=== MAIN MENU ===");

    // Draw menu
    for (int i = 0; i < menu_options.size(); ++i) {
      if (i == choice) wattron(menu, A_REVERSE);
      mvwprintw(menu, i + 3, 2, menu_options[i].c_str());
      if (i == choice) wattroff(menu, A_REVERSE);
    }

    wrefresh(menu);

    int c = getch();
    switch (c) {
      case KEY_UP:
        if (choice > 0) choice--;
        break;
      case KEY_DOWN:
        if (choice < menu_options.size() - 1) choice++;
        break;
      case 10:
        if (menu_options[choice] == "Add Transaction") {
          addTransaction(trns, filename, menu);
        } else if (menu_options[choice] == "Remove Transaction") {
          removeTransaction(trns, filename, menu);
        } else if (menu_options[choice] == "View Transactions") {
          viewTransactionsList(trns, menu);
        } else if (menu_options[choice] == "Exit") {
          clear();
          saveTransactions(trns, filename);
          wclear(dashboard);
          wclear(menu);
          wrefresh(dashboard);
          wrefresh(menu);
          delwin(dashboard);
          delwin(menu);
          endwin();
          return 0;
        };  // Exit
        break;
    }
  }
}
