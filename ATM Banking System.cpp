#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class Account {
private:
    int id;
    string name;
    string pin;
    double balance;

public:
    Account(int id, string name, string pin, double balance)
        : id(id), name(name), pin(pin), balance(balance) {}

    string getName() { return name; }
    string getPin() { return pin; }
    double getBalance() { return balance; }
    int getId() { return id; }

    void deposit(double amount) {
        balance += amount;
        cout << "Deposited: " << amount << endl;
    }

    void withdraw(double amount) {
        if (amount > balance) {
            cout << "Insufficient funds!" << endl;
        } else {
            balance -= amount;
            cout << "Withdrawn: " << amount << endl;
        }
    }

    void changePin(string newPin) {
        pin = newPin;
        cout << "PIN successfully changed." << endl;
    }

    void save(ofstream &out) {
        out << id << " " << name << " " << pin << " " << balance << endl;
    }
};

class ATM {
private:
    vector<Account> accounts;
    Account* currentAccount = nullptr;

public:
    void loadAccounts(string filename) {
        ifstream in(filename);
        int id;
        string name, pin;
        double balance;
        while (in >> id >> name >> pin >> balance) {
            accounts.push_back(Account(id, name, pin, balance));
        }
        in.close();
    }

    void saveAccounts(string filename) {
        ofstream out(filename);
        for (auto &acc : accounts) {
            acc.save(out);
        }
        out.close();
    }

    bool login(string name, string pin) {
        for (auto &acc : accounts) {
            if (acc.getName() == name && acc.getPin() == pin) {
                currentAccount = &acc;
                cout << "Login successful!" << endl;
                return true;
            }
        }
        cout << "Invalid credentials." << endl;
        return false;
    }

    void menu() {
        int choice;
        do {
            cout << "\nATM Menu:\n";
            cout << "1. Check Balance\n";
            cout << "2. Deposit\n";
            cout << "3. Withdraw\n";
            cout << "4. Change PIN\n";
            cout << "5. Exit\n";
            cout << "Enter choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    cout << "Balance: " << currentAccount->getBalance() << endl;
                    break;
                case 2: {
                    double amount;
                    cout << "Enter deposit amount: ";
                    cin >> amount;
                    currentAccount->deposit(amount);
                    break;
                }
                case 3: {
                    double amount;
                    cout << "Enter withdrawal amount: ";
                    cin >> amount;
                    currentAccount->withdraw(amount);
                    break;
                }
                case 4: {
                    string newPin;
                    cout << "Enter new PIN: ";
                    cin >> newPin;
                    currentAccount->changePin(newPin);
                    break;
                }
                case 5:
                    cout << "Exiting ATM..." << endl;
                    break;
                default:
                    cout << "Invalid choice." << endl;
            }
        } while (choice != 5);
    }
};

int main() {
    ATM atm;
    atm.loadAccounts("accounts.txt");

    string name, pin;
    cout << "Enter Account Name: ";
    cin >> name;
    cout << "Enter PIN: ";
    cin >> pin;

    if (atm.login(name, pin)) {
        atm.menu();
        atm.saveAccounts("accounts.txt");
    }

    return 0;
}
