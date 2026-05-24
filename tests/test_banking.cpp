#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Account.h"
#include "Transaction.h"

class MockAccount : public Account {
public:
    MockAccount(int id, int balance)
        : Account(id, balance) {}

    MOCK_CONST_METHOD0(GetBalance, int());
    MOCK_METHOD0(Lock, void());
    MOCK_METHOD0(Unlock, void());
};

TEST(Account, Methods) {
    Account acc(1, 1000);

    EXPECT_EQ(acc.GetBalance(), 1000);

    acc.Lock();

    EXPECT_THROW(
        acc.ChangeBalance(100),
        std::runtime_error
    );

    acc.Unlock();

    acc.ChangeBalance(100);

    EXPECT_EQ(acc.GetBalance(), 1100);
}

TEST(Account, NegativeBalanceChange) {
    Account acc(1, 1000);

    acc.ChangeBalance(-300);

    EXPECT_EQ(acc.GetBalance(), 700);
}

TEST(Transaction, SuccessTransfer) {
    Transaction tr;

    Account a1(1, 1000);
    Account a2(2, 1000);

    EXPECT_TRUE(tr.Make(a1, a2, 200));

    EXPECT_EQ(a1.GetBalance(), 800);
    EXPECT_EQ(a2.GetBalance(), 1200);
}

TEST(Transaction, NotEnoughMoney) {
    Transaction tr;

    Account a1(1, 1000);
    Account a2(2, 1000);

    EXPECT_FALSE(tr.Make(a1, a2, 5000));

    EXPECT_EQ(a1.GetBalance(), 1000);
    EXPECT_EQ(a2.GetBalance(), 1000);
}

TEST(Transaction, NegativeSum) {
    Transaction tr;

    Account a1(1, 1000);
    Account a2(2, 1000);

    EXPECT_THROW(
        tr.Make(a1, a2, -100),
        std::invalid_argument
    );
}

TEST(Transaction, SelfTransfer) {
    Transaction tr;

    Account a1(1, 1000);

    EXPECT_THROW(
        tr.Make(a1, a1, 200),
        std::logic_error
    );
}

TEST(Transaction, TooSmallSum) {
    Transaction tr;

    Account a1(1, 1000);
    Account a2(2, 1000);

    EXPECT_THROW(
        tr.Make(a1, a2, 50),
        std::logic_error
    );
}

TEST(Transaction, BoundarySum100) {
    Transaction tr;

    Account a1(1, 1000);
    Account a2(2, 1000);

    EXPECT_TRUE(tr.Make(a1, a2, 100));

    EXPECT_EQ(a1.GetBalance(), 900);
    EXPECT_EQ(a2.GetBalance(), 1100);
}

TEST(Account, DestructorCoverage) {
    Account* acc = new Account(1, 1000);

    delete acc;
}

TEST(Transaction, DestructorCoverage) {
    Transaction* tr = new Transaction();

    delete tr;
}

TEST(Account, Mock) {
    MockAccount m_acc(1, 100);

    EXPECT_CALL(m_acc, GetBalance())
        .WillOnce(testing::Return(100));

    EXPECT_EQ(m_acc.GetBalance(), 100);
}
