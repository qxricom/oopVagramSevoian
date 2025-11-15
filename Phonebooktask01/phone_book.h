//
// Created by Ваграм on 10/31/25.
//

#ifndef PHONEBOOK_PHONE_BOOK_H
#define PHONEBOOK_PHONE_BOOK_H
#include <iostream>
#include "contact.h"
#include <string>
#include <vector>
using namespace std;


class phone_book {
private:
    vector<Contact> contacts;
public:
    void add_contact(const Contact& contact);
    void list_contacts() const;
    void remove_contact(int item);
    void save(const string& filename)const;
    void load(const string& filename);

};
#endif //PHONEBOOK_PHONE_BOOK_H