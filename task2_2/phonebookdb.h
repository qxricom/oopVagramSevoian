#ifndef PHONEBOOKDB_H
#define PHONEBOOKDB_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "contact.h"

class PhoneBookDB {
public:
    PhoneBookDB(const QString& path = "phonebook.db"); // файл SQLite

    bool addContact(const Contact& c);
    QList<Contact> allContacts();
    QSqlDatabase getDatabase() const;

private:
    QSqlDatabase db;
};

#endif // PHONEBOOKDB_H
