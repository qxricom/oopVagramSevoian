#ifndef CONTACT_H
#define CONTACT_H

#include <QString>
#include <QStringList>
#include <QDate>

class Contact
{
public:
    Contact() = default;
    Contact(const QString& name, const QString& surname, const QString& patronymic,
            const QString& address, const QDate& dob, const QString& email,
            const QStringList& phoneNumbers);

    // Геттеры
    QString name() const { return m_name; }
    QString surname() const { return m_surname; }
    QString patronymic() const { return m_patronymic; }
    QString address() const { return m_address; }
    QDate dob() const { return m_dob; }
    QString email() const { return m_email; }
    QStringList phoneNumbers() const { return m_phoneNumbers; }
    QString phoneNumbersString() const { return m_phoneNumbers.join(", "); } // Для отображения в таблице

    // Сеттеры
    void setName(const QString& name) { m_name = name; }
    void setSurname(const QString& surname) { m_surname = surname; }
    void setPatronymic(const QString& patronymic) { m_patronymic = patronymic; }
    void setAddress(const QString& address) { m_address = address; }
    void setDob(const QDate& dob) { m_dob = dob; }
    void setEmail(const QString& email) { m_email = email; }
    void setPhoneNumbers(const QStringList& phoneNumbers) { m_phoneNumbers = phoneNumbers; }
    void addPhoneNumber(const QString& number);

    // Сериализация для записи в файл (для простоты - CSV)
    QString toCsvString() const;
    static Contact fromCsvString(const QString& csvLine);

private:
    QString m_name;
    QString m_surname;
    QString m_patronymic;
    QString m_address;
    QDate m_dob;
    QString m_email;
    QStringList m_phoneNumbers;
};

#endif // CONTACT_H
