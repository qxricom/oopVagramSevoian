#include "contactmanager.h"
#include <QLocale>

ContactManager::ContactManager(QObject *parent)
    : QObject(parent)
{
    // Попытка загрузить данные при инициализации
    loadContacts(m_filename);
}

// Добавление контакта
void ContactManager::addContact(const Contact& contact)
{
    m_contacts.append(contact);
    saveContacts(m_filename);
}

// Удаление контакта
void ContactManager::removeContact(int index)
{
    if (index >= 0 && index < m_contacts.size()) {
        m_contacts.removeAt(index);
        saveContacts(m_filename);
    }
}

// Обновление контакта
void ContactManager::updateContact(int index, const Contact& contact)
{
    if (index >= 0 && index < m_contacts.size()) {
        m_contacts[index] = contact;
        saveContacts(m_filename);
    }
}

// Получение контакта
Contact ContactManager::getContact(int index) const
{
    if (index >= 0 && index < m_contacts.size()) {
        return m_contacts.at(index);
    }
    return Contact(); // Возврат пустого объекта при ошибке
}

// Загрузка контактов из файла (QFile)
bool ContactManager::loadContacts(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open file for reading:" << file.errorString();
        return false;
    }

    m_contacts.clear();
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        Contact contact = Contact::fromCsvString(line);
        if (!contact.surname().isEmpty()) { // Простая проверка на корректность
            m_contacts.append(contact);
        }
    }

    file.close();
    qDebug() << "Loaded" << m_contacts.size() << "contacts.";
    return true;
}

// Сохранение контактов в файл (QFile)
bool ContactManager::saveContacts(const QString& filename) const
{
    QFile file(filename);
    // Для Qt 6 используем QIODeviceBase::WriteOnly
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qWarning() << "Could not open file for writing:" << file.errorString();
        return false;
    }

    QTextStream out(&file);
    for (const Contact& contact : m_contacts) {
        out << contact.toCsvString() << "\n";
    }

    file.close();
    qDebug() << "Saved" << m_contacts.size() << "contacts.";
    return true;
}

// Сортировка контактов
void ContactManager::sortContacts(SortField field, Qt::SortOrder order)
{
    std::sort(m_contacts.begin(), m_contacts.end(),
              [&](const Contact& a, const Contact& b) {
                  QString aValue, bValue;
                  switch (field) {
                  case SortBySurname:
                      aValue = a.surname();
                      bValue = b.surname();
                      break;
                  case SortByName:
                      aValue = a.name();
                      bValue = b.name();
                      break;
                  case SortByEmail:
                      aValue = a.email();
                      bValue = b.email();
                      break;
                  }



                int comparison = QString::compare(aValue, bValue, Qt::CaseInsensitive | Qt::LocaleAware); // Объединяем оба флага

                if (order == Qt::AscendingOrder) {
                    return comparison < 0;
                }else {
                    return comparison > 0;
                }
              });
}

// Поиск контактов по запросу (по нескольким полям)
QList<Contact> ContactManager::searchContacts(const QString& query) const
{
    QList<Contact> results;
    if (query.isEmpty()) {
        return m_contacts;
    }

    // Использование QRegularExpression для нечувствительного к регистру поиска
    QRegularExpression regex(QRegularExpression::escape(query),
                             QRegularExpression::CaseInsensitiveOption);

    for (const Contact& contact : m_contacts) {
        if (regex.match(contact.surname()).hasMatch() ||
            regex.match(contact.name()).hasMatch() ||
            regex.match(contact.patronymic()).hasMatch() ||
            regex.match(contact.email()).hasMatch() ||
            contact.phoneNumbersString().contains(query, Qt::CaseInsensitive))
        {
            results.append(contact);
        }
    }
    return results;
}
