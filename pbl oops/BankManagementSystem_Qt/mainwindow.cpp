#include "mainwindow.h"
#include "bankmanager.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QTableWidget>
#include <QHeaderView>
#include <QStackedWidget>
#include <QInputDialog>
#include <QMessageBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QDialog>
#include <QDialogButtonBox>
#include <QScrollArea>
#include <QDateTime>
#include <QIcon>

static QString money(double value)
{
    return QString("$%1").arg(value, 0, 'f', 2);
}

MainWindow::MainWindow(BankManager *bank, QWidget *parent)
    : QMainWindow(parent), bank(bank)
{
    setWindowTitle("Bank Management System");
    resize(1250, 760);
    setMinimumSize(1050, 650);

    setStyleSheet(R"(
        QMainWindow, QWidget {
            background: #07111f;
            color: #eef2ff;
            font-family: "Segoe UI";
        }

        QFrame#sidebar {
            background: #0a1626;
            border-right: 1px solid #172a43;
        }

        QLabel#brand {
            font-size: 20px;
            font-weight: bold;
            color: white;
        }

        QLabel#pageTitle {
            font-size: 30px;
            font-weight: bold;
            color: white;
        }

        QLabel#muted {
            color: #8192ad;
        }

        QPushButton#nav {
            text-align: left;
            background: transparent;
            border: none;
            border-radius: 9px;
            padding: 13px 16px;
            color: #91a0b8;
            font-size: 14px;
        }

        QPushButton#nav:hover {
            background: #14243a;
            color: white;
        }

        QPushButton#primary {
            background: #6336e8;
            color: white;
            border: none;
            border-radius: 8px;
            padding: 11px 18px;
            font-weight: bold;
        }

        QPushButton#primary:hover {
            background: #7952ee;
        }

        QPushButton#success {
            background: #087f5b;
            color: white;
            border: none;
            border-radius: 8px;
            padding: 10px 18px;
            font-weight: bold;
        }

        QPushButton#danger {
            background: #a83446;
            color: white;
            border: none;
            border-radius: 8px;
            padding: 10px 18px;
            font-weight: bold;
        }

        QPushButton#secondary {
            background: #14243a;
            color: #d8e0ef;
            border: 1px solid #243853;
            border-radius: 8px;
            padding: 10px 18px;
        }

        QFrame#card {
            background: #0d1b2d;
            border: 1px solid #1b304b;
            border-radius: 13px;
        }

        QLabel#cardTitle {
            color: #8ea0bb;
            font-size: 12px;
            font-weight: bold;
        }

        QLabel#cardValue {
            color: white;
            font-size: 25px;
            font-weight: bold;
        }

        QTableWidget {
            background: #0d1b2d;
            alternate-background-color: #0a1728;
            border: 1px solid #1b304b;
            border-radius: 10px;
            gridline-color: #182b43;
            color: #e7edf8;
            selection-background-color: #34217d;
            selection-color: white;
        }

        QHeaderView::section {
            background: #111f33;
            color: #93a5bf;
            border: none;
            padding: 10px;
            font-weight: bold;
        }

        QComboBox, QLineEdit, QDoubleSpinBox {
            background: #111f33;
            border: 1px solid #263a55;
            border-radius: 8px;
            padding: 10px;
            color: white;
        }

        QMessageBox {
            background: #0d1b2d;
        }
    )");

    QWidget *central = new QWidget;
    setCentralWidget(central);

    QHBoxLayout *root = new QHBoxLayout(central);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    QFrame *sidebar = new QFrame;
    sidebar->setObjectName("sidebar");
    sidebar->setFixedWidth(220);

    QVBoxLayout *side = new QVBoxLayout(sidebar);
    side->setContentsMargins(18, 25, 18, 20);
    side->setSpacing(7);

    QLabel *brand = new QLabel("◈  BMS");
    brand->setObjectName("brand");
    side->addWidget(brand);
    side->addSpacing(28);

    const QStringList navNames = {
        "⌂   Dashboard",
        "▣   Accounts",
        "⇄   Transactions",
        "◷   Monthly Update",
        "▤   About"
    };

    for (int i = 0; i < navNames.size(); ++i)
    {
        QPushButton *button = new QPushButton(navNames[i]);
        button->setObjectName("nav");
        side->addWidget(button);

        connect(button, &QPushButton::clicked, this,
                [this, i]() { showPage(i); });
    }

    side->addStretch();

    QLabel *security = new QLabel(
        "●  SYSTEM SECURE\n"
        "   Demo banking environment");
    security->setStyleSheet(
        "color:#6e82a0;font-size:11px;padding:10px;");
    side->addWidget(security);

    QPushButton *logout = new QPushButton("↪   Logout");
    logout->setObjectName("secondary");
    side->addWidget(logout);

    connect(logout, &QPushButton::clicked, this, &QWidget::close);

    pages = new QStackedWidget;
    pages->addWidget(buildDashboard());
    pages->addWidget(buildAccountsPage());
    pages->addWidget(buildTransactionsPage());
    pages->addWidget(buildMonthlyPage());
    pages->addWidget(buildAboutPage());

    root->addWidget(sidebar);
    root->addWidget(pages);

    refreshDashboard();
    refreshAccounts();
    refreshTransactions();
}

QWidget* MainWindow::buildDashboard()
{
    QWidget *page = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 25, 30, 25);
    layout->setSpacing(18);

    QHBoxLayout *header = new QHBoxLayout;

    QVBoxLayout *titles = new QVBoxLayout;
    QLabel *title = new QLabel("Dashboard");
    title->setObjectName("pageTitle");

    QLabel *sub = new QLabel("Welcome back. Here's your banking overview.");
    sub->setObjectName("muted");

    titles->addWidget(title);
    titles->addWidget(sub);

    QPushButton *newAccount = new QPushButton("+  New Account");
    newAccount->setObjectName("primary");

    header->addLayout(titles);
    header->addStretch();
    header->addWidget(newAccount);

    connect(newAccount, &QPushButton::clicked,
            this, &MainWindow::createAccount);

    layout->addLayout(header);

    QGridLayout *cards = new QGridLayout;
    cards->setSpacing(12);

    auto makeCard = [](const QString &titleText, QLabel **value) {
        QFrame *card = new QFrame;
        card->setObjectName("card");
        QVBoxLayout *l = new QVBoxLayout(card);
        l->setContentsMargins(18, 16, 18, 16);

        QLabel *t = new QLabel(titleText);
        t->setObjectName("cardTitle");

        *value = new QLabel("$0.00");
        (*value)->setObjectName("cardValue");

        l->addWidget(t);
        l->addWidget(*value);
        return card;
    };

    QFrame *c1 = makeCard("TOTAL BALANCE", &totalBalanceLabel);
    QFrame *c2 = makeCard("SAVINGS", &savingsLabel);
    QFrame *c3 = makeCard("CHECKING", &checkingLabel);
    QFrame *c4 = makeCard("TOTAL ACCOUNTS", &accountCountLabel);

    cards->addWidget(c1, 0, 0);
    cards->addWidget(c2, 0, 1);
    cards->addWidget(c3, 0, 2);
    cards->addWidget(c4, 0, 3);

    layout->addLayout(cards);

    QHBoxLayout *actions = new QHBoxLayout;

    QPushButton *depositButton = new QPushButton("↓  Deposit");
    depositButton->setObjectName("success");

    QPushButton *withdrawButton = new QPushButton("↑  Withdraw");
    withdrawButton->setObjectName("danger");

    QPushButton *viewButton = new QPushButton("▤  View Transactions");
    viewButton->setObjectName("secondary");

    actions->addWidget(depositButton);
    actions->addWidget(withdrawButton);
    actions->addWidget(viewButton);
    actions->addStretch();

    connect(depositButton, &QPushButton::clicked,
            this, &MainWindow::deposit);
    connect(withdrawButton, &QPushButton::clicked,
            this, &MainWindow::withdraw);
    connect(viewButton, &QPushButton::clicked,
            this, [this]() { showPage(2); });

    layout->addLayout(actions);

    QHBoxLayout *bottom = new QHBoxLayout;

    QFrame *accountsCard = new QFrame;
    accountsCard->setObjectName("card");
    QVBoxLayout *aLayout = new QVBoxLayout(accountsCard);

    QLabel *aTitle = new QLabel("MY ACCOUNTS");
    aTitle->setStyleSheet("font-weight:bold;color:#a5b5cc;");
    aLayout->addWidget(aTitle);

    QTableWidget *mini = new QTableWidget(0, 4);
    mini->setHorizontalHeaderLabels(
        {"Account", "Type", "Owner", "Balance"});
    mini->horizontalHeader()->setStretchLastSection(true);
    mini->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    mini->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mini->setSelectionBehavior(QAbstractItemView::SelectRows);
    mini->setMinimumHeight(240);

    for (const auto &a : bank->getAccounts())
    {
        int r = mini->rowCount();
        mini->insertRow(r);
        mini->setItem(r, 0,
            new QTableWidgetItem(QString::number(a->getAccountNumber())));
        mini->setItem(r, 1,
            new QTableWidgetItem(a->getAccountType()));
        mini->setItem(r, 2,
            new QTableWidgetItem(a->getOwnerName()));
        mini->setItem(r, 3,
            new QTableWidgetItem(money(a->getBalance())));
    }

    aLayout->addWidget(mini);
    bottom->addWidget(accountsCard, 2);

    QFrame *info = new QFrame;
    info->setObjectName("card");
    QVBoxLayout *iLayout = new QVBoxLayout(info);

    QLabel *it = new QLabel("QUICK GUIDE");
    it->setStyleSheet("font-weight:bold;color:#a5b5cc;");

    QLabel *guide = new QLabel(
        "◆ Savings Account\n"
        "Earns monthly interest and must maintain a minimum balance.\n\n"
        "◆ Checking Account\n"
        "Supports an overdraft limit and monthly service fee.\n\n"
        "◆ Monthly Update\n"
        "Runs polymorphic monthly updates on every account.");
    guide->setWordWrap(true);
    guide->setStyleSheet("color:#9babc1;line-height:150%;");

    iLayout->addWidget(it);
    iLayout->addSpacing(8);
    iLayout->addWidget(guide);
    iLayout->addStretch();

    bottom->addWidget(info, 1);
    layout->addLayout(bottom);

    return page;
}

QWidget* MainWindow::buildAccountsPage()
{
    QWidget *page = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 25, 30, 25);

    QHBoxLayout *header = new QHBoxLayout;

    QLabel *title = new QLabel("Accounts");
    title->setObjectName("pageTitle");

    QPushButton *newButton = new QPushButton("+  Create Account");
    newButton->setObjectName("primary");

    header->addWidget(title);
    header->addStretch();
    header->addWidget(newButton);

    layout->addLayout(header);

    accountsTable = new QTableWidget(0, 6);
    accountsTable->setHorizontalHeaderLabels(
        {"Account No.", "Type", "Owner", "Balance", "Details", "Action"});
    accountsTable->horizontalHeader()->setStretchLastSection(true);
    accountsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    accountsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    accountsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    layout->addWidget(accountsTable);

    connect(newButton, &QPushButton::clicked,
            this, &MainWindow::createAccount);

    connect(accountsTable, &QTableWidget::cellDoubleClicked,
            this, [this](int, int) { showAccountDetails(); });

    return page;
}

QWidget* MainWindow::buildTransactionsPage()
{
    QWidget *page = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 25, 30, 25);

    QHBoxLayout *header = new QHBoxLayout;

    QLabel *title = new QLabel("Transactions");
    title->setObjectName("pageTitle");

    QLabel *hint = new QLabel("Double-click an account in Accounts to inspect its history.");
    hint->setObjectName("muted");

    header->addWidget(title);
    header->addStretch();
    header->addWidget(hint);

    layout->addLayout(header);

    transactionsTable = new QTableWidget(0, 5);
    transactionsTable->setHorizontalHeaderLabels(
        {"Date & Time", "Type", "Description", "Amount", "Balance"});
    transactionsTable->horizontalHeader()->setStretchLastSection(true);
    transactionsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    transactionsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    transactionsTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    layout->addWidget(transactionsTable);

    return page;
}

QWidget* MainWindow::buildMonthlyPage()
{
    QWidget *page = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 25, 30, 25);

    QLabel *title = new QLabel("Monthly Update");
    title->setObjectName("pageTitle");

    QLabel *sub = new QLabel(
        "Apply the correct monthly rule to every account using polymorphism.");
    sub->setObjectName("muted");

    layout->addWidget(title);
    layout->addWidget(sub);
    layout->addSpacing(20);

    QFrame *card = new QFrame;
    card->setObjectName("card");
    QVBoxLayout *c = new QVBoxLayout(card);

    QLabel *concept = new QLabel(
        "POLYMORPHISM\n\n"
        "Account is an abstract base class. Each derived class provides its "
        "own implementation of applyMonthlyUpdate().\n\n"
        "Savings → adds monthly interest\n"
        "Checking → deducts monthly service fee");
    concept->setWordWrap(true);
    concept->setStyleSheet("color:#aab8cc;font-size:15px;");

    QPushButton *run = new QPushButton("▶  Run Monthly Update");
    run->setObjectName("primary");

    c->addWidget(concept);
    c->addSpacing(20);
    c->addWidget(run, 0, Qt::AlignLeft);

    layout->addWidget(card);

    connect(run, &QPushButton::clicked,
            this, &MainWindow::monthlyUpdate);

    layout->addStretch();
    return page;
}

QWidget* MainWindow::buildAboutPage()
{
    QWidget *page = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 25, 30, 25);

    QLabel *title = new QLabel("About the Project");
    title->setObjectName("pageTitle");

    layout->addWidget(title);
    layout->addSpacing(20);

    QFrame *card = new QFrame;
    card->setObjectName("card");
    QVBoxLayout *c = new QVBoxLayout(card);

    QLabel *text = new QLabel(
        "<h2>Bank Management System</h2>"
        "<p>A Qt Widgets application demonstrating core C++ OOP concepts.</p>"
        "<p><b>Abstraction:</b> Account is an abstract base class.</p>"
        "<p><b>Encapsulation:</b> account data is protected and accessed "
        "through controlled methods.</p>"
        "<p><b>Inheritance:</b> SavingsAccount and CheckingAccount inherit "
        "from Account.</p>"
        "<p><b>Polymorphism:</b> withdraw() and applyMonthlyUpdate() behave "
        "differently for each account type.</p>"
        "<p><b>Team Split:</b> Four members can independently own the base "
        "class, savings class, checking class, and integration/testing.</p>");
    text->setWordWrap(true);
    text->setStyleSheet("color:#aebbd0;font-size:15px;");

    c->addWidget(text);
    layout->addWidget(card);
    layout->addStretch();

    return page;
}

void MainWindow::showPage(int index)
{
    pages->setCurrentIndex(index);

    if (index == 0) refreshDashboard();
    if (index == 1) refreshAccounts();
    if (index == 2) refreshTransactions();
}

int MainWindow::selectedAccountNumber() const
{
    if (!accountsTable || accountsTable->currentRow() < 0)
        return -1;

    bool ok = false;
    int number = accountsTable->item(
        accountsTable->currentRow(), 0)->text().toInt(&ok);

    return ok ? number : -1;
}

void MainWindow::refreshDashboard()
{
    totalBalanceLabel->setText(money(bank->totalBalance()));
    savingsLabel->setText(money(bank->totalSavings()));
    checkingLabel->setText(money(bank->totalChecking()));
    accountCountLabel->setText(QString::number(bank->getAccounts().size()));
}

void MainWindow::refreshAccounts()
{
    if (!accountsTable)
        return;

    accountsTable->setRowCount(0);

    for (const auto &a : bank->getAccounts())
    {
        int r = accountsTable->rowCount();
        accountsTable->insertRow(r);

        accountsTable->setItem(r, 0,
            new QTableWidgetItem(QString::number(a->getAccountNumber())));
        accountsTable->setItem(r, 1,
            new QTableWidgetItem(a->getAccountType()));
        accountsTable->setItem(r, 2,
            new QTableWidgetItem(a->getOwnerName()));
        accountsTable->setItem(r, 3,
            new QTableWidgetItem(money(a->getBalance())));
        accountsTable->setItem(r, 4,
            new QTableWidgetItem(a->getExtraInfo()));

        QPushButton *view = new QPushButton("View");
        view->setObjectName("secondary");
        accountsTable->setCellWidget(r, 5, view);

        int number = a->getAccountNumber();
        connect(view, &QPushButton::clicked, this,
                [this, number]() {
                    Account *acc = bank->findAccount(number);
                    if (acc)
                    {
                        QMessageBox::information(
                            this,
                            "Account Details",
                            acc->displayInfo());
                    }
                });
    }
}

void MainWindow::refreshTransactions()
{
    if (!transactionsTable)
        return;

    transactionsTable->setRowCount(0);

    for (const auto &a : bank->getAccounts())
    {
        for (const Transaction &t : a->getHistory())
        {
            int r = transactionsTable->rowCount();
            transactionsTable->insertRow(r);

            transactionsTable->setItem(r, 0,
                new QTableWidgetItem(t.dateTime));
            transactionsTable->setItem(r, 1,
                new QTableWidgetItem(t.type));
            transactionsTable->setItem(r, 2,
                new QTableWidgetItem(
                    QString("%1 • A/C %2")
                        .arg(t.description)
                        .arg(a->getAccountNumber())));

            QString amountText =
                (t.amount >= 0 ? "+" : "") +
                money(t.amount);

            transactionsTable->setItem(r, 3,
                new QTableWidgetItem(amountText));
            transactionsTable->setItem(r, 4,
                new QTableWidgetItem(money(t.balanceAfter)));
        }
    }
}

void MainWindow::deposit()
{
    if (bank->getAccounts().isEmpty())
        return;

    QStringList options;

    for (const auto &a : bank->getAccounts())
    {
        options << QString("%1 — %2 — %3")
                       .arg(a->getAccountNumber())
                       .arg(a->getAccountType())
                       .arg(a->getOwnerName());
    }

    bool ok = false;
    QString selected = QInputDialog::getItem(
        this, "Select Account", "Account:", options, 0, false, &ok);

    if (!ok)
        return;

    int number = selected.section(" ", 0, 0).toInt();
    Account *account = bank->findAccount(number);

    if (!account)
        return;

    double amount = QInputDialog::getDouble(
        this,
        "Deposit",
        "Enter deposit amount:",
        500.0,
        0.01,
        100000000.0,
        2,
        &ok);

    if (!ok)
        return;

    QString message;
    bool success = account->deposit(amount, message);

    if (success)
        QMessageBox::information(this, "Deposit Successful", message);
    else
        QMessageBox::warning(this, "Deposit Failed", message);

    refreshDashboard();
    refreshAccounts();
    refreshTransactions();
}

void MainWindow::withdraw()
{
    if (bank->getAccounts().isEmpty())
        return;

    QStringList options;

    for (const auto &a : bank->getAccounts())
    {
        options << QString("%1 — %2 — %3")
                       .arg(a->getAccountNumber())
                       .arg(a->getAccountType())
                       .arg(a->getOwnerName());
    }

    bool ok = false;
    QString selected = QInputDialog::getItem(
        this, "Select Account", "Account:", options, 0, false, &ok);

    if (!ok)
        return;

    int number = selected.section(" ", 0, 0).toInt();
    Account *account = bank->findAccount(number);

    if (!account)
        return;

    double amount = QInputDialog::getDouble(
        this,
        "Withdraw",
        "Enter withdrawal amount:",
        200.0,
        0.01,
        100000000.0,
        2,
        &ok);

    if (!ok)
        return;

    QString message;
    bool success = account->withdraw(amount, message);

    if (success)
        QMessageBox::information(this, "Withdrawal Successful", message);
    else
        QMessageBox::warning(this, "Withdrawal Failed", message);

    refreshDashboard();
    refreshAccounts();
    refreshTransactions();
}

void MainWindow::createAccount()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Create New Account");
    dialog.setMinimumWidth(440);
    dialog.setStyleSheet(
        "QDialog{background:#0d1b2d;color:white;}"
        "QLabel{color:#dce5f5;}"
        "QLineEdit,QDoubleSpinBox,QComboBox{"
        "background:#111f33;color:white;border:1px solid #263a55;"
        "border-radius:8px;padding:9px;}"
        "QPushButton{background:#6336e8;color:white;border:none;"
        "border-radius:8px;padding:10px 16px;}");

    QVBoxLayout *l = new QVBoxLayout(&dialog);

    QLabel *title = new QLabel("Create New Account");
    title->setStyleSheet("font-size:22px;font-weight:bold;");

    QLineEdit *owner = new QLineEdit;
    owner->setPlaceholderText("Owner name");

    QComboBox *type = new QComboBox;
    type->addItems({"Savings Account", "Checking Account"});

    QDoubleSpinBox *initial = new QDoubleSpinBox;
    initial->setRange(0, 100000000);
    initial->setDecimals(2);
    initial->setValue(1000);

    QDoubleSpinBox *rate = new QDoubleSpinBox;
    rate->setRange(0, 100);
    rate->setDecimals(2);
    rate->setValue(5);

    QDoubleSpinBox *minimum = new QDoubleSpinBox;
    minimum->setRange(0, 100000000);
    minimum->setValue(500);

    QDoubleSpinBox *overdraft = new QDoubleSpinBox;
    overdraft->setRange(0, 100000000);
    overdraft->setValue(2000);

    QDoubleSpinBox *fee = new QDoubleSpinBox;
    fee->setRange(0, 100000);
    fee->setValue(10);

    l->addWidget(title);
    l->addSpacing(10);
    l->addWidget(new QLabel("Owner Name"));
    l->addWidget(owner);
    l->addWidget(new QLabel("Account Type"));
    l->addWidget(type);
    l->addWidget(new QLabel("Initial Deposit"));
    l->addWidget(initial);
    l->addWidget(new QLabel("Interest Rate (Savings)"));
    l->addWidget(rate);
    l->addWidget(new QLabel("Minimum Balance (Savings)"));
    l->addWidget(minimum);
    l->addWidget(new QLabel("Overdraft Limit (Checking)"));
    l->addWidget(overdraft);
    l->addWidget(new QLabel("Monthly Fee (Checking)"));
    l->addWidget(fee);

    QDialogButtonBox *buttons =
        new QDialogButtonBox(
            QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    l->addSpacing(8);
    l->addWidget(buttons);

    connect(buttons, &QDialogButtonBox::accepted,
            &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected,
            &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted)
        return;

    if (owner->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, "Invalid Input",
                             "Please enter the owner's name.");
        return;
    }

    if (type->currentIndex() == 0)
    {
        bank->createSavings(
            owner->text().trimmed(),
            initial->value(),
            rate->value(),
            minimum->value());
    }
    else
    {
        bank->createChecking(
            owner->text().trimmed(),
            initial->value(),
            overdraft->value(),
            fee->value());
    }

    refreshDashboard();
    refreshAccounts();
    refreshTransactions();

    QMessageBox::information(
        this, "Account Created",
        "The new account was created successfully.");
}

void MainWindow::monthlyUpdate()
{
    QString report;
    bank->applyMonthlyUpdates(report);

    refreshDashboard();
    refreshAccounts();
    refreshTransactions();

    QMessageBox::information(
        this,
        "Monthly Update Complete",
        report);
}

void MainWindow::showAccountDetails()
{
    int number = selectedAccountNumber();

    if (number < 0)
        return;

    Account *account = bank->findAccount(number);

    if (!account)
        return;

    QMessageBox::information(
        this,
        "Account Details",
        account->displayInfo());
}
