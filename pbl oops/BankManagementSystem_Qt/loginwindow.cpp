#include "loginwindow.h"
#include "bankmanager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QFrame>

LoginWindow::LoginWindow(BankManager *bank, QWidget *parent)
    : QDialog(parent), bank(bank)
{
    setWindowTitle("Bank Management System - Secure Login");
    setFixedSize(920, 560);

    setStyleSheet(R"(
        QDialog {
            background: #07111f;
            color: #f5f7ff;
        }
        QLabel {
            color: #eef2ff;
        }
        QLineEdit {
            background: #111e31;
            border: 1px solid #263956;
            border-radius: 9px;
            padding: 12px;
            color: white;
            font-size: 14px;
        }
        QLineEdit:focus {
            border: 1px solid #7c4dff;
        }
        QPushButton {
            background: #6336e8;
            color: white;
            border: none;
            border-radius: 9px;
            padding: 13px;
            font-weight: bold;
        }
        QPushButton:hover {
            background: #7952ee;
        }
    )");

    auto *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    auto *left = new QFrame;
    left->setStyleSheet("background:#0d1b2d;");
    auto *leftLayout = new QVBoxLayout(left);
    leftLayout->setContentsMargins(55, 50, 55, 50);

    QLabel *logo = new QLabel("◈");
    logo->setAlignment(Qt::AlignCenter);
    logo->setStyleSheet(
        "background:#6537e8;border-radius:35px;color:white;"
        "font-size:34px;font-weight:bold;");
    logo->setFixedSize(70, 70);

    QLabel *title = new QLabel("Welcome Back");
    title->setStyleSheet("font-size:30px;font-weight:bold;");
    title->setAlignment(Qt::AlignCenter);

    QLabel *sub = new QLabel("Secure access to your banking dashboard");
    sub->setWordWrap(true);
    sub->setAlignment(Qt::AlignCenter);
    sub->setStyleSheet("color:#91a1bb;font-size:14px;");

    userEdit = new QLineEdit;
    userEdit->setPlaceholderText("Username");
    userEdit->setText("admin");

    pinEdit = new QLineEdit;
    pinEdit->setPlaceholderText("4-digit PIN");
    pinEdit->setEchoMode(QLineEdit::Password);
    pinEdit->setText("1234");

    QPushButton *loginButton = new QPushButton("LOGIN  →");

    QLabel *hint = new QLabel("Demo login:  admin  /  1234");
    hint->setAlignment(Qt::AlignCenter);
    hint->setStyleSheet("color:#71839f;font-size:12px;");

    leftLayout->addStretch();
    leftLayout->addWidget(logo, 0, Qt::AlignHCenter);
    leftLayout->addSpacing(22);
    leftLayout->addWidget(title);
    leftLayout->addWidget(sub);
    leftLayout->addSpacing(30);
    leftLayout->addWidget(userEdit);
    leftLayout->addSpacing(10);
    leftLayout->addWidget(pinEdit);
    leftLayout->addSpacing(15);
    leftLayout->addWidget(loginButton);
    leftLayout->addSpacing(10);
    leftLayout->addWidget(hint);
    leftLayout->addStretch();

    auto *right = new QFrame;
    right->setStyleSheet(
        "background:qlineargradient(x1:0,y1:0,x2:1,y2:1,"
        "stop:0 #15113b,stop:0.55 #20105b,stop:1 #071b43);");

    auto *rightLayout = new QVBoxLayout(right);
    rightLayout->setContentsMargins(55, 55, 55, 55);

    QLabel *bankIcon = new QLabel("♜");
    bankIcon->setAlignment(Qt::AlignCenter);
    bankIcon->setStyleSheet("font-size:100px;color:#b58cff;");

    QLabel *bankTitle = new QLabel("BANK");
    bankTitle->setAlignment(Qt::AlignCenter);
    bankTitle->setStyleSheet("font-size:40px;font-weight:bold;color:white;");

    QLabel *welcome = new QLabel(
        "Manage your savings and checking accounts\n"
        "from one elegant dashboard.");
    welcome->setAlignment(Qt::AlignCenter);
    welcome->setStyleSheet("font-size:17px;color:#bfc8df;");

    QLabel *features = new QLabel(
        "✓  Deposits & Withdrawals\n"
        "✓  Transaction History\n"
        "✓  Monthly Interest & Fees\n"
        "✓  Secure Account Controls");
    features->setStyleSheet("font-size:14px;color:#aeb9d0;line-height:150%;");

    rightLayout->addStretch();
    rightLayout->addWidget(bankIcon);
    rightLayout->addWidget(bankTitle);
    rightLayout->addWidget(welcome);
    rightLayout->addSpacing(35);
    rightLayout->addWidget(features);
    rightLayout->addStretch();

    mainLayout->addWidget(left, 1);
    mainLayout->addWidget(right, 1);

    connect(loginButton, &QPushButton::clicked,
            this, &LoginWindow::attemptLogin);
    connect(pinEdit, &QLineEdit::returnPressed,
            this, &LoginWindow::attemptLogin);
}

void LoginWindow::attemptLogin()
{
    if (userEdit->text() == "admin" && pinEdit->text() == "1234")
    {
        accept();
        return;
    }

    QMessageBox::warning(
        this,
        "Login Failed",
        "Incorrect username or PIN.\n\n"
        "For this demo use:\n"
        "Username: admin\n"
        "PIN: 1234");
}
