//
// Created by Ваграм on 10/31/25.
//
#include <iostream>
#include <fstream>
#include "phone_book.h"
#include "contact.h"

void phone_book::add_contact(const Contact &contact) {//добавляем и поверяем на дублтрование
    for (const auto &c : contacts) {
        if (c.email == contact.email) {
            cout << "Контакт уже сущесвтует" << endl;
        }
        return;
    }
    contacts.push_back(contact);
    cout << "контакт добавлен" << endl;
}

void phone_book::list_contacts() const {

    if (contacts.empty()) {
        cout << "Справочник пуст" << endl;
    }

    for (size_t i = 0; i < contacts.size(); i++) {
        contacts[i].print((int)i);

    }
}
void phone_book::remove_contact(int item) {
    if (item >= 0 && item < int(contacts.size())) {
        contacts.erase(contacts.begin() + item);
        cout << "Контакт удален" << endl;
    }
    else {
        cout << "Контакт не удален" << endl;
    }
}
void phone_book::save(const string& filename) const {
    ofstream file(filename);
    if (!file) {
        cout << "Файл не открылся"<< filename << endl;
    }
    int k = 0;
    for (const auto &c : contacts) {
        file << c.converting_contact_to_string()<< "\n";
        k++;
    }
    cout << "сохранено" << k << "контакт(ов) в файл" << endl;
}

void phone_book::load(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cout << "ошибка чтения файла"<< endl;
        return;
    }
    contacts.clear();// очищаем справочник
    string line;
    int k = 0;
    while (getline(file, line)) {
        if (line.empty()) {
            continue;// пропуск пустых строчек
        }
        try {
            Contact c = Contact::deserialize(line);
            contacts.push_back(c);
            k++;
        }
        catch (const std::exception &e) {
            cout << "Ошибка чтения строки"<<endl;
            cout <<"причина"<< e.what() << endl;
        }
    }
    cout <<"загружено"<< k <<"контактов"<<filename<<endl;
}