#ifndef BANKMANAGER_H
#define BANKMANAGER_H

#include <QVector>
#include <QString>

#include "account.h"
#include "savingsaccount.h"
#include "checkingaccount.h"

class BankManager
{
private:
    QVector<Account*> accounts;
    int nextAccountNumber;

public:
    BankManager();
    ~BankManager();

    // Prevent accidental copying of the manager and its owned accounts.
    BankManager(const BankManager&) = delete;
    BankManager& operator=(const BankManager&) = delete;

    void loadDemoData();

    SavingsAccount* createSavings(const QString &owner,
                                   double initial,
                                   double rate,
                                   double minimum);

    CheckingAccount* createChecking(const QString &owner,
                                    double initial,
                                    double overdraft,
                                    double fee);

    Account* findAccount(int accountNumber);

    const QVector<Account*>& getAccounts() const;

    double totalBalance() const;
    double totalSavings() const;
    double totalChecking() const;

    void applyMonthlyUpdates(QString &report);
};

#endif
