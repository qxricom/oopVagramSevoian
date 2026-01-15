#ifndef CONTACT_H
#define CONTACT_H

#include <QString>
#include <QStringList>
#include <QDate>

struct Contact {
    QString secondName;
    QString name;
    QString patronymic;
    QString address;
    QDate birthday;
    QString email;
    QStringList phones;

    // Преобразование контакта в строку
    QString toString() const {
        return QString("%1|%2|%3|%4|%5|%6|%7")
        .arg(secondName,
             name,
             patronymic,
             address,
             birthday.toString("dd-MM-yyyy"),
             email,
             phones.join(","));
    }

    // Создание контакта из строки
    static Contact fromString(const QString& line) {
        QStringList parts = line.split('|');
        Contact c;

        if (parts.size() == 7) {
            c.secondName  = parts[0];
            c.name        = parts[1];
            c.patronymic  = parts[2];
            c.address     = parts[3];
            c.birthday    = QDate::fromString(parts[4], "dd-MM-yyyy");
            c.email       = parts[5];
            c.phones      = parts[6].split(',', Qt::SkipEmptyParts);
        }
        return c;
    }
};

#endif // CONTACT_H
