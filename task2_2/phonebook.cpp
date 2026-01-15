#include "phonebook.h"

#include <QFile>
#include <QTextStream>

// Добавление контакта
void PhoneBook::add(const Contact& c)
{
    contacts.append(c);
}

// Удаление контакта
void PhoneBook::remove(int index)
{
    if (index >= 0 && index < contacts.size())
        contacts.removeAt(index);
}

// Получение списка
const QList<Contact>& PhoneBook::all() const
{
    return contacts;
}

// Сохранение в файл
bool PhoneBook::saveToFile(const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&file);
    for (const Contact& c : contacts)
        out << c.toString() << "\n";

    return true;
}

// Загрузка из файла
bool PhoneBook::loadFromFile(const QString& path)
{
    QFile file(path);

    if (!file.exists())
        return true;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    contacts.clear();

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (!line.isEmpty())
            contacts.append(Contact::fromString(line));
    }

    return true;
}
