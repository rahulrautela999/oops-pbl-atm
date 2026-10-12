#include "bankmanager.h"

BankManager::BankManager()
    : nextAccountNumber(1001001)
{
}

BankManager::~BankManager()
{
    // BankManager owns all Account objects stored in the vector.
    qDeleteAll(accounts);
    accounts.clear();
}

void BankManager::loadDemoData()
{
    if (!accounts.isEmpty())
        return;

    SavingsAccount *s = createSavings(
        "Rahul Singh", 8450.0, 5.0, 500.0);

    CheckingAccount *c = createChecking(
        "Rahul Singh", 7000.0, 2000.0, 10.0);

    QString msg;

    s->deposit(500.0, msg);
    s->withdraw(200.0, msg);

    c->deposit(1200.0, msg);
    c->withdraw(350.0, msg);
}

SavingsAccount* BankManager::createSavings(const QString &owner,
                                           double initial,
                                           double rate,
                                           double minimum)
{
    int number = nextAccountNumber++;

    SavingsAccount *account = new SavingsAccount(
        number, owner, initial, rate, minimum);

    accounts.append(account);

    return account;
}

CheckingAccount* BankManager::createChecking(const QString &owner,
                                             double initial,
                                             double overdraft,
                                             double fee)
{
    int number = nextAccountNumber++;

    CheckingAccount *account = new CheckingAccount(
        number, owner, initial, overdraft, fee);

    accounts.append(account);

    return account;
}

Account* BankManager::findAccount(int accountNumber)
{
    for (Account *account : accounts)
    {
        if (account != nullptr &&
            account->getAccountNumber() == accountNumber)
        {
            return account;
        }
    }

    return nullptr;
}

const QVector<Account*>& BankManager::getAccounts() const
{
    return accounts;
}

double BankManager::totalBalance() const
{
    double total = 0.0;

    for (Account *account : accounts)
    {
        if (account != nullptr)
            total += account->getBalance();
    }

    return total;
}

double BankManager::totalSavings() const
{
    double total = 0.0;

    for (Account *account : accounts)
    {
        if (account != nullptr &&
            account->getAccountType() == "Savings Account")
        {
            total += account->getBalance();
        }
    }

    return total;
}

double BankManager::totalChecking() const
{
    double total = 0.0;

    for (Account *account : accounts)
    {
        if (account != nullptr &&
            account->getAccountType() == "Checking Account")
        {
            total += account->getBalance();
        }
    }

    return total;
}

void BankManager::applyMonthlyUpdates(QString &report)
{
    report.clear();

    // Polymorphism:
    // Account* can point to SavingsAccount or CheckingAccount.
    // The correct overridden function is selected at runtime.
    for (Account *account : accounts)
    {
        if (account == nullptr)
            continue;

        QString message;
        account->applyMonthlyUpdate(message);

        report += QString("%1 (%2): %3\n")
                      .arg(account->getAccountNumber())
                      .arg(account->getAccountType())
                      .arg(message);
    }
}
