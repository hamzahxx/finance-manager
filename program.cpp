#include <iostream>
#include <string>

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

    void displayTransaction() {
        cout << "Type: " << type << endl
             << "Catagory: " << catagory << endl
             << "Amount: " << amount << endl;
    }
};

int main() {
    Transaction t1("Income", "Salary", 5000);
    t1.displayTransaction();
    return 0;
}