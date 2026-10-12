#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class BankManager;
class Account;
class QLabel;
class QTableWidget;
class QStackedWidget;

class MainWindow : public QMainWindow
{
public:
    explicit MainWindow(BankManager *bank, QWidget *parent = nullptr);

private:
    BankManager *bank;
    QStackedWidget *pages;
    QTableWidget *accountsTable;
    QTableWidget *transactionsTable;

    QLabel *totalBalanceLabel;
    QLabel *savingsLabel;
    QLabel *checkingLabel;
    QLabel *accountCountLabel;

    int selectedAccountNumber() const;

    QWidget* buildDashboard();
    QWidget* buildAccountsPage();
    QWidget* buildTransactionsPage();
    QWidget* buildMonthlyPage();
    QWidget* buildAboutPage();

    void refreshDashboard();
    void refreshAccounts();
    void refreshTransactions();
    void deposit();
    void withdraw();
    void createAccount();
    void monthlyUpdate();
    void showAccountDetails();

    void showPage(int index);
};

#endif
