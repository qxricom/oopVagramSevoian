//
// Created by Ваграм on 11/14/25.
//

#include "utility.h"


string probel(const string &s) {
    const string probel = " \t\n\r\f\v";//табуляция перевод строки возврат коретки перевод стрницы вертикальная табуляция
    size_t start = s.find_first_not_of(probel);
    if (start == string::npos) {// если строка состоит полностью из пробелов
        return "";
    }
    size_t end = s.find_last_not_of(probel);
    return s.substr(start, end - start + 1); //вычисляем длину самого имен(слова) +1 тк сначала ищем длину слова потом само слово
}
Contact input_contact() {
    Contact c;
    string input;

    cout << "Введите фамилию: ";
    getline(cin, input);
    c.secondname = probel(input);

    cout << "Введите имя: ";
    getline(cin, input);
    c.name = probel(input);

    cout << "Введите отчество: ";
    getline(cin, input);
    c.patronymic = probel(input);

    cout << "Введите адрес: ";
    getline(cin, input);
    c.adress = probel(input);

    cout << "Введите дату рождения (ДД.ММ.ГГГГ): ";
    getline(cin, input);
    c.birthday = probel(input);

    cout << "Введите email: ";
    getline(cin, input);
    c.email = probel(input);

    // ---- ввод телефонов ----
    cout << "\nВведите телефоны.\n";
    cout << "Добавить телефон? (y/n): ";
    getline(cin, input);

    while (input == "y" || input == "Y") {
        cout << "Телефон: ";
        getline(cin, input);

        input = probel(input);

        if (!input.empty())
            c.phones.push_back(input);

        cout << "Добавить еще? (y/n): ";
        getline(cin, input);
    }

    // Требование: минимум 1 телефон
    if (c.phones.empty()) {
        cout << "Вы должны ввести хотя бы один телефон." << endl;
        cout << "Введите телефон: ";
        getline(cin, input);
        c.phones.push_back(probel(input));
    }

    return c;
}