QT += widgets

CONFIG += c++17
TEMPLATE = app
TARGET = BankManagementSystem

SOURCES += \
    main.cpp \
    account.cpp \
    savingsaccount.cpp \
    checkingaccount.cpp \
    bankmanager.cpp \
    loginwindow.cpp \
    mainwindow.cpp

HEADERS += \
    account.h \
    savingsaccount.h \
    checkingaccount.h \
    bankmanager.h \
    loginwindow.h \
    mainwindow.h
