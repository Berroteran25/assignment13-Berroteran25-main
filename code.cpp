#include "code.h"
#include <iostream>
#include <cctype>

using namespace std;

//Account Class

Account::Account(int acc_num, const string& acc_holder, const string& pass) {
    m_account_number = acc_num;
    m_account_holder_name = acc_holder;
    m_password = pass;
    m_balance = 0.0;
}

void Account::updateTransactions(double amount) {
    m_transaction_history.push_back(amount);
}

StatusCode Account::deposit(double amount, const string& pass) {
    if (pass != m_password) return StatusCode::INVALID_PASSWORD;
    if (amount <= 0) return StatusCode::INVALID_AMOUNT;

    m_balance += amount;
    updateTransactions(amount);
    return StatusCode::SUCCESS;
}

StatusCode Account::withdraw(double amount, const string& pass) {
    if (pass != m_password) return StatusCode::INVALID_PASSWORD;
    if (amount <= 0) return StatusCode::INVALID_AMOUNT;
    if (amount > m_balance) return StatusCode::INSUFFICIENT_BALANCE;

    m_balance -= amount;
    updateTransactions(-amount);
    return StatusCode::SUCCESS;
}

StatusCode Account::printBalance(const string& pass) {
    if (pass != m_password) return StatusCode::INVALID_PASSWORD;

    cout << "Balance for Account Number "
         << m_account_number << ": $" << m_balance << endl;

    return StatusCode::SUCCESS;
}

StatusCode Account::printTransactionHistory(const string& pass) {
    if (pass != m_password) return StatusCode::INVALID_PASSWORD;

    cout << "Transaction History for Account Number "
         << m_account_number << ":" << endl;

    for (size_t i = m_transaction_history.size(); i > 0; i--) {
        cout << m_transaction_history[i - 1];

        if (i - 1 != 0) {
            cout << "\t";
        }
    }

    cout << endl;

    return StatusCode::SUCCESS;
}

//BankSystem Class

bool Bank::passwordStrength(const string& pass) {
    if (pass.length() < 7) return false;

    bool hasUpper = false, hasDigit = false, hasSpecial = false;

    for (char c : pass) {
        if (isupper(c)) hasUpper = true;
        if (isdigit(c)) hasDigit = true;
        if (c == '@' || c == '#' || c == '$' || c == '%') hasSpecial = true;
    }

    return hasUpper && hasDigit && hasSpecial;
}

int Bank::getLastAccountNumber() {
    return m_next_account_num - 1;
}

StatusCode Bank::addAccount(const string& acc_holder, const string& pass) {
    if (!passwordStrength(pass)) return StatusCode::WEAK_PASSWORD;

    Account acc(m_next_account_num, acc_holder, pass);
    m_accounts.push_back(acc);
    m_next_account_num++;

    return StatusCode::SUCCESS;
}

StatusCode Bank::bankDeposit(int acc_num, double amount, const string& pass) {
    for (auto& acc : m_accounts) {
        if (acc.m_account_number == acc_num) {
            return acc.deposit(amount, pass);
        }
    }
    return StatusCode::INVALID_ACCOUNT;
}

StatusCode Bank::bankWithdraw(int acc_num, double amount, const string& pass) {
    for (auto& acc : m_accounts) {
        if (acc.m_account_number == acc_num) {
            return acc.withdraw(amount, pass);
        }
    }
    return StatusCode::INVALID_ACCOUNT;
}

StatusCode Bank::balanceInquiry(int acc_num, const string& pass) {
    for (auto& acc : m_accounts) {
        if (acc.m_account_number == acc_num) {
            return acc.printBalance(pass);
        }
    }
    return StatusCode::INVALID_ACCOUNT;
}

StatusCode Bank::transactionHistory(int acc_num, const string& pass) {
    for (auto& acc : m_accounts) {
        if (acc.m_account_number == acc_num) {
            return acc.printTransactionHistory(pass);
        }
    }
    return StatusCode::INVALID_ACCOUNT;
}