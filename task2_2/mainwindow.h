#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>

#include "contact.h"
#include "phonebook.h"
#include "phonebookdb.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void addContact();
    void removeContact();
    void saveToFile();
    void filterTable();

private:
    QTableWidget *table;
    QLineEdit *searchEdit;

    PhoneBook phoneBook;
    PhoneBookDB db;

    void setupUI();
    void updateTable();
    bool isValid(const Contact& c);
};

#endif // MAINWINDOW_H
