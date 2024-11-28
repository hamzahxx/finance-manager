# Finance Manager Program

The **Finance Manager Program** is a command-line-based application written in C++ that helps users manage their personal finances. It allows users to track income and expenses, view financial summaries, and maintain a persistent record of transactions.

## Features

### 1. Add Transactions

- Record new transactions by specifying:
  - **Type**: Income or Expense
  - **Category**: E.g., Food, Rent, Salary
  - **Amount**: Positive numeric value
- Validates inputs to ensure only correct data is saved.

### 2. Delete Transactions

- Remove specific transactions by selecting their corresponding ID from the list of recorded transactions.

### 3. View Transactions

- Display a detailed list of all transactions, including:
  - **Type**: Income or Expense
  - **Category**: Transaction category
  - **Amount**: Transaction amount
- Provides a financial summary:
  - Total Income
  - Total Expenses
  - Net Balance

### 4. Persistent Storage

- Transactions are stored in a CSV file (`transactions.csv`) for future reference.
- The program loads saved data automatically when restarted.

### 5. Input Validation and Error Handling

- Ensures invalid inputs (e.g., non-numeric values, negative amounts) are handled gracefully.
- Provides clear feedback for user errors.

---

## How to Use

#### Step 1: Clone or download this repository to your local machine.

#### Step 2: Compile the program using a C++ compiler, e.g.:

```bash
g++ -o FinanceManager main.cpp
```

#### Step 3: Run the compiled program.

```
./FinanceManager
```

#### Step 4: Enter the number of the action you want to carry out.

```
1. Add a Transaction
2. Delete a Transaction
3. View all Transactions
4. Exit
```

## Example Usage

### Adding a Transaction

Choose by entering "1".

```
1. Add a Transaction.
```

Enter "1" or "2" depending on the type of transaction.

```Type
Transaction type:
   1. Income
   2. Expense
```

You can enter the category like e.g. _rent, salary_.

```Category
Type the category of tranasaction:
```

Lastly you enter the amount of transaction

```
Enter the amount:
```

and you are done!!!

### Deleting a transaction

Choose by entering "2", the following will be returned

```
Transaction no: 1
Type: Income
Category: Salary
Amount: 10000
------------------

Transaction no: 2
Type: Expense
Category: Gym
Amount: 1000
------------------
```

Then type the transaction number of the transaction you want to remove.

```
Enter the id of the transaction to be removed:
```

Once you enter the id the transaction will be deleted.

### View Transactions

See a detailed list of recorded transactions and a financial summary by typing "3"

```
Transaction no: 1
Type: Income
Category: Salary
Amount: 10000
------------------

---- Summary ----
Total Income: ₹10000
Total Expense: ₹0
Net Balance: ₹10000
```

### Exit

Type "4" to end the program.

## Requirements

To run this project, you will need to add the following

`C++ Compiler (e.g., GCC or Clang).`

`Basic knowledge of navigating a terminal`

## Contributing

Feel free to submit issues or fork this repository to contribute. Pull requests are welcome!

## License

This project is licensed under the MIT License. See the LICENSE file for details.
