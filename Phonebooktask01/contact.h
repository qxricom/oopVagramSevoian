//
// Created by Ваграм on 10/31/25.
//

#ifndef PHONEBOOK_CONTACT_H
#define PHONEBOOK_CONTACT_H
#include <iostream>
using namespace std;
#include <string>
#include <vector>

struct Contact {
    string name,secondname,patronymic,adress,birthday,email;
    vector<string> phones;
    void print(int item)const;
    string converting_contact_to_string() const; // перобразование контакта в строку
    static Contact deserialize(const string& line); // создание контката из string& файла
};
#endif //PHONEBOOK_CONTACT_H