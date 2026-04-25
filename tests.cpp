#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "code.h"

TEST_CASE("Bank creates accounts with strong passwords") {
    Bank bank;

    REQUIRE(bank.addAccount("Alice", "Alice@1") == StatusCode::SUCCESS);
    REQUIRE(bank.getLastAccountNumber() == 1000);

    REQUIRE(bank.addAccount("Bob", "Bob@123") == StatusCode::SUCCESS);
    REQUIRE(bank.getLastAccountNumber() == 1001);
}

TEST_CASE("Bank rejects weak passwords") {
    Bank bank;

    REQUIRE(bank.addAccount("Adam", "weak") == StatusCode::WEAK_PASSWORD);
    REQUIRE(bank.addAccount("Adam", "weakpass") == StatusCode::WEAK_PASSWORD);
    REQUIRE(bank.addAccount("Adam", "Weakpass") == StatusCode::WEAK_PASSWORD);
    REQUIRE(bank.addAccount("Adam", "Weak123") == StatusCode::WEAK_PASSWORD);
}

TEST_CASE("Deposit works correctly") {
    Bank bank;
    bank.addAccount("Alice", "Alice@1");

    REQUIRE(bank.bankDeposit(1000, 500, "Alice@1") == StatusCode::SUCCESS);
    REQUIRE(bank.bankDeposit(1000, 0, "Alice@1") == StatusCode::INVALID_AMOUNT);
    REQUIRE(bank.bankDeposit(1000, -10, "Alice@1") == StatusCode::INVALID_AMOUNT);
    REQUIRE(bank.bankDeposit(1000, 100, "Wrong@1") == StatusCode::INVALID_PASSWORD);
    REQUIRE(bank.bankDeposit(9999, 100, "Alice@1") == StatusCode::INVALID_ACCOUNT);
}

TEST_CASE("Withdraw works correctly") {
    Bank bank;
    bank.addAccount("Alice", "Alice@1");

    REQUIRE(bank.bankDeposit(1000, 500, "Alice@1") == StatusCode::SUCCESS);

    REQUIRE(bank.bankWithdraw(1000, 100, "Alice@1") == StatusCode::SUCCESS);
    REQUIRE(bank.bankWithdraw(1000, 0, "Alice@1") == StatusCode::INVALID_AMOUNT);
    REQUIRE(bank.bankWithdraw(1000, -25, "Alice@1") == StatusCode::INVALID_AMOUNT);
    REQUIRE(bank.bankWithdraw(1000, 1000, "Alice@1") == StatusCode::INSUFFICIENT_BALANCE);
    REQUIRE(bank.bankWithdraw(1000, 50, "Wrong@1") == StatusCode::INVALID_PASSWORD);
    REQUIRE(bank.bankWithdraw(9999, 50, "Alice@1") == StatusCode::INVALID_ACCOUNT);
}

TEST_CASE("Balance inquiry and transaction history validate accounts and passwords") {
    Bank bank;
    bank.addAccount("Alice", "Alice@1");

    REQUIRE(bank.bankDeposit(1000, 1000, "Alice@1") == StatusCode::SUCCESS);
    REQUIRE(bank.bankWithdraw(1000, 500, "Alice@1") == StatusCode::SUCCESS);

    REQUIRE(bank.balanceInquiry(1000, "Alice@1") == StatusCode::SUCCESS);
    REQUIRE(bank.balanceInquiry(1000, "Wrong@1") == StatusCode::INVALID_PASSWORD);
    REQUIRE(bank.balanceInquiry(9999, "Alice@1") == StatusCode::INVALID_ACCOUNT);

    REQUIRE(bank.transactionHistory(1000, "Alice@1") == StatusCode::SUCCESS);
    REQUIRE(bank.transactionHistory(1000, "Wrong@1") == StatusCode::INVALID_PASSWORD);
    REQUIRE(bank.transactionHistory(9999, "Alice@1") == StatusCode::INVALID_ACCOUNT);
}