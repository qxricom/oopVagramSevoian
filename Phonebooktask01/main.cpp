#include <iostream>
#include "phone_book.h"
#include "contact.h"
using namespace std;

Contact create_contact_from_input() {
    Contact c;
    cout << "Введите фамилию: ";
    getline(cin, c.secondname);// getline для того чтобы ссчитывал всю строку полностью
    cout << "Введите имя: ";
    getline(cin, c.name);
    cout << "Введите отчество (если есть): ";
    getline(cin, c.patronymic);
    cout << "Введите адрес: ";
    getline(cin, c.adress);
    cout << "Введите дату рождения (например, 27-02-2006): ";
    getline(cin, c.birthday);
    cout << "Введите email: ";
    getline(cin, c.email);

    // телефоны
    string phone;
    c.phones.clear();
    while (true) {
        cout << "Введите телефон (или пустую строку для завершения): ";
        getline(cin, phone);
        if (phone.empty()) break;
        c.phones.push_back(phone);
    }

    return c;
}

int main() {
    setlocale(LC_ALL, "Russian");// библиотека для русских букв

    phone_book book;
    book.load("contact.txt"); // загружаем при старте

    while (true) {// цикл до того момента пока не будет выбрана команда 5
        cout << "\n===== Телефонный справочник =====\n";
        cout << "1. Показать все контакты\n";
        cout << "2. Добавить контакт\n";
        cout << "3. Удалить контакт\n";
        cout << "4. Сохранить в файл\n";
        cout << "5. Выйти\n";
        cout << "Выберите пункт: ";

        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        cin.ignore(); // очистка \n после числа

        if (choice == 1) {
            book.list_contacts();
        } else if (choice == 2) {
            Contact c = create_contact_from_input();
            book.add_contact(c);
        } else if (choice == 3) {
            int idx;
            cout << "Введите номер контакта для удаления: ";
            cin >> idx;
            cin.ignore();
            book.remove_contact(idx - 1);
        } else if (choice == 4) {
            book.save("contact.txt");
        } else if (choice == 5) {
            cout << "Сохранить изменения перед выходом? (y/n): ";
            char ans;
            cin >> ans;
            if (ans == 'y' || ans == 'Y')
                book.save("contact.txt");
            break;
        } else {
            cout << "Неверный пункт меню!\n";
        }
    }

    cout << "Выход из программы...\n";
    return 0;
}