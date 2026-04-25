#ifndef CODE_HPP
#define CODE_HPP

#include <string>
#include <iostream>
#include <vector>

// to define the status code of any operation
enum class StatusCode
{
    SUCCESS,
    INSUFFICIENT_BALANCE,
    INVALID_AMOUNT,
    INVALID_ACCOUNT,
    INVALID_PASSWORD,
    WEAK_PASSWORD
};

class Account
{
private:
    int m_account_number;
    std::string m_account_holder_name;
    double m_balance;
    std::vector<double> m_transaction_history;
    std::string m_password;

public:
    Account(int acc_num, const std::string& acc_holder, const std::string& pass);
    void updateTransactions(double amount);
    StatusCode deposit(double amount, const std::string& pass);
    StatusCode withdraw(double amount, const std::string& pass);
    StatusCode printBalance(const std::string& pass);
    StatusCode printTransactionHistory(const std::string& pass);

    friend class Bank;
};

class Bank
{
private:
    std::vector<Account> m_accounts{};
    int m_next_account_num{1000};

    bool passwordStrength(const std::string& pass);

public:
    int getLastAccountNumber();
    StatusCode addAccount(const std::string& acc_holder, const std::string& pass);
    StatusCode bankDeposit(int acc_num, double amount, const std::string& pass);
    StatusCode bankWithdraw(int acc_num, double amount, const std::string& pass);
    StatusCode balanceInquiry(int acc_num, const std::string& pass);
    StatusCode transactionHistory(int acc_num, const std::string& pass);
};

#endif // CODE_HPP