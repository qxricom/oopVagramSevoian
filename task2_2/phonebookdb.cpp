#include "phonebookdb.h"
#include <QFile>

PhoneBookDB::PhoneBookDB(const QString& path) {
    // Создаем базу SQLite
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);

    if (!db.open()) {
        qDebug() << " Ошибка подключения к БД:" << db.lastError().text();
        return;
    }

    qDebug() << " Подключение к SQLite успешно!";

    // Создаем таблицу contacts, если её нет
    QSqlQuery query(db);
    QString createTable =
        "CREATE TABLE IF NOT EXISTS contacts ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "second_name TEXT,"
        "name TEXT,"
        "patronymic TEXT,"
        "address TEXT,"
        "birthday TEXT,"
        "email TEXT,"
        "phones TEXT"
        ")";
    if (!query.exec(createTable)) {
        qDebug() << " Ошибка создания таблицы:" << query.lastError().text();
    }
}

bool PhoneBookDB::addContact(const Contact& c) {
    if (!db.isOpen()) {
        qDebug() << "❌ База не открыта!";
        return false;
    }

    QSqlQuery query(db);
    query.prepare(
        "INSERT INTO contacts "
        "(second_name, name, patronymic, address, birthday, email, phones) "
        "VALUES (:second_name, :name, :patronymic, :address, :birthday, :email, :phones)"
        );

    query.bindValue(":second_name", c.secondName);
    query.bindValue(":name", c.name);
    query.bindValue(":patronymic", c.patronymic);
    query.bindValue(":address", c.address);
    query.bindValue(":birthday", c.birthday.toString("yyyy-MM-dd"));
    query.bindValue(":email", c.email);
    query.bindValue(":phones", c.phones.join(","));

    if (!query.exec()) {
        qDebug() << " Ошибка добавления контакта в SQLite:" << query.lastError().text();
        return false;
    }

    qDebug() << " Контакт успешно добавлен:" << c.secondName << c.name;
    return true;
}

QList<Contact> PhoneBookDB::allContacts() {
    QList<Contact> list;

    if (!db.isOpen()) {
        qDebug() << "❌ База не открыта!";
        return list;
    }

    QSqlQuery query("SELECT second_name, name, patronymic, address, birthday, email, phones FROM contacts", db);

    while (query.next()) {
        Contact c;
        c.secondName = query.value(0).toString();
        c.name       = query.value(1).toString();
        c.patronymic = query.value(2).toString();
        c.address    = query.value(3).toString();
        c.birthday   = query.value(4).toDate();
        c.email      = query.value(5).toString();
        c.phones     = query.value(6).toString().split(",", Qt::SkipEmptyParts);
        list.append(c);
    }

    return list;
}

QSqlDatabase PhoneBookDB::getDatabase() const { return db; }
