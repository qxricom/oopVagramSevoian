//
// Created by Ваграм on 11/4/25.
//
#include "contact.h"
#include <sstream>
using namespace std;

void Contact::print(int item) const {
    cout << "-------------------------" << endl;
    cout << "количество " <<item+1 << endl;
    cout << "Имя:" <<name << endl;
    cout <<"Фамилия:" <<secondname << endl;
    cout << "Отчество:"<<patronymic << endl;
    cout << "Адрес:"<<adress << endl;
    cout <<"дата рождения:" <<birthday << endl;
    cout << "почта:"<<email << endl;
    if (phones.empty() == 0) {
        cout << "телефонов нет" <<endl;
    }else {
        cout <<"телефоны:"<<endl;
        for (size_t i = 1; i < phones.size(); i++) {
            cout <<"("<<i+1<<")"<< phones[i] << endl;
        }
    }
    cout << "-------------------------" << endl;
}
string Contact::converting_contact_to_string() const {
    ostringstream ss;
    ss << secondname << "|" <<name<<"|"<<patronymic<<"|"<<adress<<"|"<<birthday<<"|"<<email<<"|"<<endl;
    for (size_t i = 0; i < phones.size(); i++) {
        ss << phones[i];
        if (i != phones.size() - 1) {
            ss << "," <<endl;
        }
    }
    return ss.str();

}

Contact Contact::deserialize(const string& line) {
    Contact c;
    string phone_str;
    stringstream ss(line);
    getline(ss, c.secondname, '|');
    getline(ss, c.name, '|');
    getline(ss,c.patronymic, '|');
    getline(ss, c.adress, '|');
    getline(ss, c.birthday, '|');
    getline(ss, c.email, '|');
    getline(ss, phone_str);


    stringstream ps(phone_str);
    string phone;
    while (getline(ps, phone, ',')) {
        if (!phone.empty())
            c.phones.push_back(phone);
    }
    return c;

}