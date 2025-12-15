#ifndef CONTACTMANAGER_H
#define CONTACTMANAGER_H

#include "contact.h"
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QRegularExpression>

class ContactManager : public QObject
{
    Q_OBJECT
public:
    explicit ContactManager(QObject *parent = nullptr);

    // CRUD
    void addContact(const Contact& contact);
    void removeContact(int index);
    void updateContact(int index, const Contact& contact);

    // Хранение
    bool loadContacts(const QString& filename);
    bool saveContacts(const QString& filename) const;

    // Получение данных
    QList<Contact> getAllContacts() const { return m_contacts; }
    Contact getContact(int index) const;

    // Сортировка
    enum SortField {
        SortBySurname,
        SortByName,
        SortByEmail
    };
    void sortContacts(SortField field, Qt::SortOrder order = Qt::AscendingOrder);

    // Поиск
    QList<Contact> searchContacts(const QString& query) const;

private:
    QList<Contact> m_contacts;
    const QString m_filename = "contacts.dat";
};

#endif // CONTACTMANAGER_H
