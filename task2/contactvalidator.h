#ifndef CONTACTVALIDATOR_H
#define CONTACTVALIDATOR_H

#include <QString>
#include <QRegularExpression>
#include <QDate>
#include <QDebug>

class ContactValidator
{
public:
    // Удаление незначимых пробелов (trim)
    static QString sanitizeNamePart(const QString& part);
    static QString sanitizeEmail(const QString& email);

    // Валидация ФИО (Имя, Фамилия, Отчество)
    static bool isNameValid(const QString& namePart);

    // Валидация Телефонного номера
    static bool isPhoneNumberValid(const QString& number);

    // Валидация E-mail
    static bool isEmailValid(const QString& email);

    // Валидация Даты рождения
    static bool isDateOfBirthValid(const QDate& date);
};

#endif // CONTACTVALIDATOR_H
