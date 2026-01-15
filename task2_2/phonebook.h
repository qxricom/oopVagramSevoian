#ifndef PHONEBOOK_H
#define PHONEBOOK_H

#include <QList>
#include <QString>

#include "contact.h"

class PhoneBook {
public:
    bool loadFromFile(const QString& path);
    bool saveToFile(const QString& path);

    void add(const Contact& c);
    void remove(int index);

    const QList<Contact>& all() const;

private:
    QList<Contact> contacts;
};

#endif // PHONEBOOK_H
