#include "contactvalidator.h"

// Удаление незначимых пробелов (trim)
QString ContactValidator::sanitizeNamePart(const QString& part)
{
    return part.trimmed();
}

QString ContactValidator::sanitizeEmail(const QString& email)
{
    // Удаляем пробелы вокруг @ и в начале/конце
    QString cleanedEmail = email;
    cleanedEmail.remove(QRegularExpression("\\s*@\\s*")); // Удаляем пробелы вокруг @
    return cleanedEmail.trimmed(); // Удаляем начальные/конечные пробелы
}

// Регулярное выражение для ФИО:
// 1. Должно начинаться на букву ([a-zA-Zа-яА-ЯёЁ])
// 2. Может содержать буквы, цифры, пробелы и дефисы ([- а-яА-ЯёЁ\d]+)
// 3. Не может оканчиваться на дефис/пробел (неявный .trimmed() в sanitizeNamePart)
// 4. Длинное выражение, включая символы из разных алфавитов:
//    ^[\p{L}]+$  - только буквы (для Unicode-совместимости)
//    Мы же используем более строгое:
bool ContactValidator::isNameValid(const QString& namePart)
{
    QString sanitized = sanitizeNamePart(namePart);
    if (sanitized.isEmpty()) {
        return false;
    }

    // Регулярное выражение:
    // ^                           - начало строки
    // [\p{L}]                    - один или более символов-букв (Unicode)
    // (?:                        - не захватывающая группа
    //   [\- ]?                   - опционально один дефис или пробел
    //   [\p{L}\d]                - за ним буква или цифра
    // )* - эта группа может повторяться 0 или более раз
    // $                           - конец строки
    // +?                          - модификатор QRegularExpression::UseUnicodeProperties
    //
    // Более простое, отвечающее требованиям:
    // ^[\p{L}][\p{L}\d\s\-]*[\p{L}\d]$
    // Но требования: "должны начинаться только на буквы, и не могли бы оканчиваться или начинаться на дефис."
    // Изначальный sanitizeNamePart() убирает начальные/конечные пробелы.
    // Проверим, что нет начального/конечного дефиса, и что первый символ - буква.

    // Проверка на дефис в начале/конце (после тримминга пробелов)
    if (sanitized.startsWith('-') || sanitized.endsWith('-')) {
        qDebug() << "Validation failed: Starts or ends with hyphen.";
        return false;
    }

    // Проверка на то, что первый символ - буква
    // \p{L} - любая буква в Unicode
    if (!sanitized.at(0).isLetter()) {
        qDebug() << "Validation failed: First character is not a letter.";
        return false;
    }

    // Проверка, что содержит только разрешенные символы: буквы, цифры, дефис, пробел
    QRegularExpression allowedCharsRegex(QStringLiteral("^[\\p{L}\\d\\s\\-]+$"));
    if (!allowedCharsRegex.match(sanitized).hasMatch()) {
        qDebug() << "Validation failed: Contains disallowed characters.";
        return false;
    }

    return true;
}

// Регулярное выражение для Телефона (различные варианты)
bool ContactValidator::isPhoneNumberValid(const QString& number)
{
    // Удаляем все пробелы для упрощения валидации
    QString cleanedNumber = number;
    cleanedNumber.remove(' ');

    // Регулярное выражение для вариантов:
    // +78121234567 | 88121234567
    // +7(812)1234567 | 8(812)1234567
    // +7(812)123-45-67 | 8(812)123-45-67
    //
    // ^                           - начало
    // (?:\\+7|8)                 - +7 или 8
    // (?:\\(\\d{3}\\))           - (DDD) - опционально
    // |                           - ИЛИ
    // \\d{3}                      - DDD
    // )
    // \\d{7}                      - D......D (7 цифр)
    // |
    // (?:\\+7|8)
    // \\d{3}
    // \\d{7}
    //
    // Попробуем одно универсальное:
    // ^(?:\\+7|8)                   - Начинается с +7 или 8
    // (?:\\(\\d{3}\\))?             - Опционально: (DDD)
    // \\d+                          - Остальные цифры/дефисы
    // (?:\\d{3}-\\d{2}-\\d{2})?     - Опционально: 123-45-67
    //
    // Более точный regex:
    // ^(?:\\+7|8)                          - +7 или 8
    // (?:\\(\\d{3}\\))?                    - Опциональный код в скобках: (DDD)
    // (?:\\d{3}(?:[-\\s]?\\d{2}){2}|\\d{7}) - 123-45-67 или просто 1234567 (после кода/8/+7)
    // $

    QRegularExpression phoneRegex(
        QStringLiteral("^(?:\\+7|8)(?:\\(\\d{3}\\))?\\d{3}(?:[-\\s]?\\d{2}){2}$|^(?:\\+7|8)(?:\\(\\d{3}\\))?\\d{7}$")
        );

    // Удаляем только дефисы и скобки для упрощения подсчёта цифр
    QString digitsOnly = cleanedNumber;
    digitsOnly.remove('(').remove(')').remove('-');

    // Проверка общей структуры
    if (!phoneRegex.match(cleanedNumber).hasMatch()) {
        qDebug() << "Validation failed: Phone number format error.";
        return false;
    }

    return true;
}

// Регулярное выражение для E-mail
bool ContactValidator::isEmailValid(const QString& email)
{
    QString sanitized = sanitizeEmail(email);

    // Регулярное выражение:
    // ^                          - начало
    // [a-zA-Z0-9]+               - имя пользователя (латинские буквы/цифры)
    // @                          - разделитель
    // [a-zA-Z0-9]+               - имя домена (латинские буквы/цифры)
    // (?:\\.[a-zA-Z0-9]+)* - опционально .субдомен, .tld
    // $                          - конец

    // Упрощенный regex по требованиям:
    QRegularExpression emailRegex(QStringLiteral("^[a-zA-Z0-9]+@[a-zA-Z0-9]+(?:\\.[a-zA-Z0-9]+)*$"));

    if (!emailRegex.match(sanitized).hasMatch()) {
        qDebug() << "Validation failed: Email format error.";
        return false;
    }

    return true;
}

// Валидация Даты рождения
bool ContactValidator::isDateOfBirthValid(const QDate& date)
{
    // Дата должна быть меньше текущей даты
    if (date.isValid() && date < QDate::currentDate()) {
        // Проверка на високосные года и дни в месяце уже встроена в QDate::isValid(),
        // но, так как мы получаем QDate, мы уже знаем, что она была создана корректно,
        // поэтому достаточно проверить, что она существует и в прошлом.
        return true;
    }

    qDebug() << "Validation failed: Date of birth is not valid or not in the past.";
    return false;
}
