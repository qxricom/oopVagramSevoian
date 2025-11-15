cmake_minimum_required(VERSION 4.0)
project(Phonebooktask01)

set(CMAKE_CXX_STANDARD 20)

add_executable(Phonebooktask01 main.cpp
        contact.cpp
        contact.h
        phone_book.h
        phone_book.cpp
        utility.cpp
        utility.h)
