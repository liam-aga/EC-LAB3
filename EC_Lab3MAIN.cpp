/**
 * @Author: Liam Aga
 * KU ID: 3050093
 * Date Created: 11/26/23
 * Last Modified: 11/26/23, 9:00pm
 * Extra Credit Lab 3: Banking system program featuring deposits,
 * withdrawals, and overdraft. Coded in C++ .
 */

#include <iostream>
#include <iomanip>
#include <string>

// Base classes for all types of bank accounts
class Account {
protected:
    std::string accountNumber;
    std::string accountHolder;
    double balance;

public:
    Account(const std::string& number, const std::string& holder, double initialBalance)
            : accountNumber(number), accountHolder(holder), balance(initialBalance) {}

    // Function to display account details
    virtual void displayDetails() const {
        std::cout << "Account Details for Account (ID: " << accountNumber << "):" << std::endl;
        std::cout << "   Holder: " << accountHolder << std::endl;
        std::cout << "   Balance: $" << std::fixed << std::setprecision(2) << balance;
    }
    // Display deposit money into account
    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            std::cout << "Deposit successful. " << std::endl;
        } else {
            std::cout << "Invalid deposit amount." << std::endl;
        }
    }
    // Function for withdrawing money
    virtual void withdraw(double amount) {
        if (amount > 0 && balance >= amount) {
            balance -= amount;
            std::cout << "Withdrawal successful. New balance: $" << balance << std::endl;
        } else {
            std::cout << "Invalid withdrawal amount or insufficient balance." << std::endl;
        }
    }
    // Operator overloading to allow transferring money between accounts
    friend Account operator+(const Account& lhs, const Account& rhs) {
        Account result = lhs;
        result.balance += rhs.balance;
        return result;
    }
    // Operator overloading to display account details
    friend std::ostream& operator<<(std::ostream& os, const Account& account) {
        os << "Account Details for Account (ID: " << account.accountNumber << "):" << std::endl;
        os << "   Holder: " << account.accountHolder << std::endl;
        os << "   Balance: $" << std::fixed << std::setprecision(2) << account.balance;
        return os;
    }

    const std::string& getAccountNumber() const { return accountNumber; }
    const std::string& getAccountHolder() const { return accountHolder; }
    double getBalance() const { return balance; }
};
//  Derived class for savings accounts
class SavingsAccount : public Account {
private:
    double interestRate;

public:
    SavingsAccount(const std::string& number, const std::string& holder, double initialBalance, double rate)
            : Account(number, holder, initialBalance), interestRate(rate) {}
    // Override function to display savings account details
    void displayDetails() const override {
        Account::displayDetails();
        std::cout << "   Interest Rate: " << std::fixed << std::setprecision(2) << (interestRate * 100) << "%" << std::endl;
    }
    // Override function to withdraw money from savings account
    void withdraw(double amount) override {
        const double minBalance = 100.0;
        if (amount > 0 && (balance - amount) >= minBalance) {
            balance -= amount;
            std::cout << "Withdrawal successful." << std::endl;
        } else {
            std::cout << "Invalid withdrawal amount or insufficient balance (below minimum)." << std::endl;
        }
    }
};
//  Derived class for current accounts
class CurrentAccount : public Account {
private:
    double overdraftLimit;

public:
    // Constructor
    CurrentAccount(const std::string& number, const std::string& holder, double initialBalance, double limit)
            : Account(number, holder, initialBalance), overdraftLimit(limit) {}

    // Override function to display current account details
    void displayDetails() const override {
        Account::displayDetails();
        std::cout << std::endl << "   Overdraft Limit: $" << overdraftLimit << std::endl;
    }

    // Override function to withdraw money from current account
    void withdraw(double amount) override {
        if (amount > 0 && (balance - amount) >= -overdraftLimit) {
            balance -= amount;
            std::cout << "Withdrawal successful." << std::endl;
        } else {
            std::cout << "Invalid withdrawal amount or overdraft limit exceeded." << std::endl;
        }
    }

    // Assignment operator to handle assignments from Account objects
    CurrentAccount& operator=(const Account& other) {
        if (this != &other) {
            const CurrentAccount* otherCurrent = dynamic_cast<const CurrentAccount*>(&other);
            if (otherCurrent) {
                accountNumber = other.getAccountNumber();
                accountHolder = other.getAccountHolder();
                balance = other.getBalance();
                overdraftLimit = otherCurrent->overdraftLimit;
            } else {
                std::cerr << "Error: Attempted to assign an Account to a CurrentAccount." << std::endl;
            }
        }
        return *this;
    }

    // Overload operator+ to handle addition of Account and CurrentAccount
    friend CurrentAccount operator+(const CurrentAccount& lhs, const Account& rhs) {
        CurrentAccount result = lhs;
        // Consider overdraft limit in the calculation
        result.balance += std::min(rhs.getBalance(), lhs.overdraftLimit);
        return result;
    }
};




int main() {
    SavingsAccount savings("S123", "John Doe", 1000, 0.02);
    CurrentAccount current("C456", "Jane Doe", 2000, 500);

    savings.displayDetails();
    current.displayDetails();

    savings.deposit(500);
    current.withdraw(1000);

    savings.displayDetails();
    current.displayDetails();

    current = current + savings;

    savings.displayDetails();
    current.displayDetails();

    return 0;
}
