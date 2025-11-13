#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
#include <limits>
using namespace std;

// ---------- Transaction Class ----------
class Transaction {
public:
    string type;
    double amount;
    string date;

    Transaction(string t, double a) {
        type = t;
        amount = a;
        time_t now = time(0);
        date = ctime(&now);
        date.pop_back(); // remove newline
    }

    void display() const {
        cout << left << setw(15) << type
             << " | Amount: ₹" << setw(10) << amount
             << " | Date: " << date << endl;
    }
};

// ---------- Account Class ----------
class Account {
private:
    string accountNumber;
    double balance;
    vector<Transaction> history;

public:
    Account(string accNum, double initialBalance = 0.0) {
        accountNumber = accNum;
        balance = initialBalance;
    }

    string getAccountNumber() const { return accountNumber; }
    double getBalance() const { return balance; }

    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            history.push_back(Transaction("Deposit", amount));
            cout << "✅ ₹" << amount << " deposited successfully.\n";
        } else {
            cout << "❌ Invalid deposit amount.\n";
        }
    }

    void withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            history.push_back(Transaction("Withdraw", amount));
            cout << "✅ ₹" << amount << " withdrawn successfully.\n";
        } else {
            cout << "❌ Insufficient balance or invalid amount.\n";
        }
    }

    bool transfer(Account &receiver, double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            receiver.balance += amount;
            history.push_back(Transaction("Transfer Out", amount));
            receiver.history.push_back(Transaction("Transfer In", amount));
            cout << "✅ ₹" << amount << " transferred to Account " << receiver.getAccountNumber() << ".\n";
            return true;
        } else {
            cout << "❌ Transfer failed. Check balance and amount.\n";
            return false;
        }
    }

    void showTransactionHistory() const {
        cout << "\n📜 Transaction History for Account " << accountNumber << ":\n";
        cout << "-------------------------------------------------------------\n";
        if (history.empty()) {
            cout << "No transactions yet.\n";
        } else {
            for (const auto &t : history) t.display();
        }
        cout << "-------------------------------------------------------------\n";
    }

    void displayAccountInfo() const {
        cout << "\n🏦 Account Information:\n";
        cout << "Account Number: " << accountNumber << endl;
        cout << "Balance: ₹" << balance << endl;
        cout << "-------------------------------------------------------------\n";
    }
};

// ---------- Customer Class ----------
class Customer {
private:
    string name;
    string id;
    string username;
    Account *account;

public:
    Customer(string n, string i, string user, string accNum)
        : name(n), id(i), username(user) {
        account = new Account(accNum);
    }

    ~Customer() {
        delete account;
    }

    string getName() const { return name; }
    string getId() const { return id; }
    string getUsername() const { return username; }
    Account* getAccount() const { return account; }

    void displayCustomerInfo() const {
        cout << "\n👤 Customer Details:\n";
        cout << "Name: " << name << endl;
        cout << "Customer ID: " << id << endl;
        cout << "Username: " << username << endl;
        account->displayAccountInfo();
    }
};

// ---------- Banking System Class ----------
class BankingSystem {
private:
    vector<Customer*> customers;

    bool isUnique(const string& id, const string& username, const string& accNum) {
        for (auto *c : customers) {
            if (c->getId() == id) {
                cout << "❌ Customer ID already exists!\n";
                return false;
            }
            if (c->getUsername() == username) {
                cout << "❌ Username already taken!\n";
                return false;
            }
            if (c->getAccount()->getAccountNumber() == accNum) {
                cout << "❌ Account number already exists!\n";
                return false;
            }
        }
        return true;
    }

public:
    void createCustomer() {
        string name, id, username, accNum;

        cout << "\nEnter customer name: ";
        cin >> name;
        cout << "Enter customer ID: ";
        cin >> id;
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter new account number: ";
        cin >> accNum;

        if (!isUnique(id, username, accNum)) {
            cout << "⚠️ Please try again with unique details.\n";
            return;
        }

        customers.push_back(new Customer(name, id, username, accNum));
        cout << "✅ Customer and account created successfully!\n";
    }

    Customer* findCustomerByAccount(const string& accNum) {
        for (auto *c : customers) {
            if (c->getAccount()->getAccountNumber() == accNum)
                return c;
        }
        return nullptr;
    }

    void depositMoney() {
        string accNum;
        double amount;
        cout << "\nEnter account number: ";
        cin >> accNum;
        Customer *cust = findCustomerByAccount(accNum);

        if (cust) {
            cout << "Enter amount to deposit: ₹";
            cin >> amount;
            cust->getAccount()->deposit(amount);
        } else {
            cout << "❌ Account not found.\n";
        }
    }

    void withdrawMoney() {
        string accNum;
        double amount;
        cout << "\nEnter account number: ";
        cin >> accNum;
        Customer *cust = findCustomerByAccount(accNum);

        if (cust) {
            cout << "Enter amount to withdraw: ₹";
            cin >> amount;
            cust->getAccount()->withdraw(amount);
        } else {
            cout << "❌ Account not found.\n";
        }
    }

    void transferFunds() {
        string fromAcc, toAcc;
        double amount;

        cout << "\nEnter sender's account number: ";
        cin >> fromAcc;
        cout << "Enter receiver's account number: ";
        cin >> toAcc;
        cout << "Enter amount to transfer: ₹";
        cin >> amount;

        Customer *sender = findCustomerByAccount(fromAcc);
        Customer *receiver = findCustomerByAccount(toAcc);

        if (sender && receiver) {
            sender->getAccount()->transfer(*receiver->getAccount(), amount);
        } else {
            cout << "❌ Invalid account(s). Please check and try again.\n";
        }
    }

    void showAccountDetails() {
        string accNum;
        cout << "\nEnter account number: ";
        cin >> accNum;

        Customer *cust = findCustomerByAccount(accNum);
        if (cust) {
            cust->displayCustomerInfo();
        } else {
            cout << "❌ Account not found.\n";
        }
    }

    void showTransactions() {
        string accNum;
        cout << "\nEnter account number: ";
        cin >> accNum;

        Customer *cust = findCustomerByAccount(accNum);
        if (cust) {
            cust->getAccount()->showTransactionHistory();
        } else {
            cout << "❌ Account not found.\n";
        }
    }

    void menu() {
        int choice;
        do {
            cout << "\n=========== 🏦 BANKING SYSTEM MENU ===========\n";
            cout << "1. Create Customer & Account\n";
            cout << "2. Deposit Money\n";
            cout << "3. Withdraw Money\n";
            cout << "4. Transfer Funds\n";
            cout << "5. View Account Details\n";
            cout << "6. View Transaction History\n";
            cout << "7. Exit\n";
            cout << "Enter your choice: ";

            cin >> choice;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "❌ Invalid input. Please enter a number.\n";
                continue;
            }

            switch (choice) {
                case 1: createCustomer(); break;
                case 2: depositMoney(); break;
                case 3: withdrawMoney(); break;
                case 4: transferFunds(); break;
                case 5: showAccountDetails(); break;
                case 6: showTransactions(); break;
                case 7: cout << "👋 Thank you for using our banking system!\n"; break;
                default: cout << "❌ Invalid choice. Please try again.\n";
            }

        } while (choice != 7);
    }
};

// ---------- Main Function ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    BankingSystem bank;
    bank.menu();

    return 0;
}

