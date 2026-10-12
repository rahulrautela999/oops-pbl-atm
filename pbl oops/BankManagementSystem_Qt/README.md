# Bank Management System — Qt Creator

## Fixed version

This project is designed for Qt Creator using Qt Widgets and C++17.

### Demo login
Username: `admin`
PIN: `1234`

## Open and run

1. Extract the ZIP.
2. Open `BankManagementSystem.pro` in Qt Creator.
3. Select a Desktop kit.
4. Run **Build > Clean All**.
5. Run **Build > Run qmake**.
6. Run **Build Project**.
7. Press **Run**.

## Important fixes in this version

### Fix 1 — `std::unique_ptr` / QVector error

The project originally used:

```cpp
QVector<std::unique_ptr<Account>>
```

Some Qt/compiler combinations can produce a deleted-copy-constructor error when QVector reallocates.

The fixed project uses:

```cpp
QVector<Account*>
```

`BankManager` owns these objects and deletes them in its destructor using `qDeleteAll()`.

### Fix 2 — `MainWindow was not declared`

`main.cpp` now includes:

```cpp
#include "mainwindow.h"
```

so the `MainWindow` class is known before it is created.

## Features

- Modern dark banking dashboard
- Login screen
- Savings accounts
- Checking accounts
- Deposit
- Withdraw
- Transaction history
- Create new accounts
- Monthly updates
- Savings interest
- Checking monthly fee
- Savings minimum-balance rule
- Checking overdraft rule
- OOP abstraction
- Encapsulation
- Inheritance
- Polymorphism

## Team split

Person 1:
- `account.h`
- `account.cpp`

Person 2:
- `savingsaccount.h`
- `savingsaccount.cpp`

Person 3:
- `checkingaccount.h`
- `checkingaccount.cpp`

Person 4:
- `bankmanager.*`
- `loginwindow.*`
- `mainwindow.*`
- `main.cpp`
- integration/testing
