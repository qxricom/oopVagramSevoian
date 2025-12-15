#include "contact.h"

Contact::Contact(const QString& name, const QString& surname, const QString& patronymic,
                 const QString& address, const QDate& dob, const QString& email,
                 const QStringList& phoneNumbers)
    : m_name(name), m_surname(surname), m_patronymic(patronymic),
    m_address(address), m_dob(dob), m_email(email), m_phoneNumbers(phoneNumbers)
{}

void Contact::addPhoneNumber(const QString& number)
{
    m_phoneNumbers.append(number);
}

// Сериализация в CSV строку
QString Contact::toCsvString() const
{
    // Используем запятую (,) как разделитель, экранируем внутренние запятые в строках, но для простоты пока не будем
    // Разделим телефоны на '|'
    QString phones = m_phoneNumbers.join("|");

    return QString("%1,%2,%3,%4,%5,%6,%7")
        .arg(m_name, m_surname, m_patronymic, m_address, m_dob.toString(Qt::ISODate), m_email, phones);
}

// Десериализация из CSV строки
Contact Contact::fromCsvString(const QString& csvLine)
{
    QStringList parts = csvLine.split(',');

    if (parts.size() < 7) {
        return Contact(); // Возвращаем пустой контакт при ошибке
    }

    Contact contact;
    contact.m_name = parts.value(0);
    contact.m_surname = parts.value(1);
    contact.m_patronymic = parts.value(2);
    contact.m_address = parts.value(3);
    contact.m_dob = QDate::fromString(parts.value(4), Qt::ISODate);
    contact.m_email = parts.value(5);

    // Телефоны были разделены '|'
    contact.m_phoneNumbers = parts.value(6).split('|', Qt::SkipEmptyParts);

    return contact;
}
