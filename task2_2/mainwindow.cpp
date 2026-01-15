#include "mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>
#include <QRegularExpression>
#include <QDir>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    db(QDir::homePath() + "/Desktop/phonebook.db") // теперь база на Desktop
{
    if (!db.getDatabase().isOpen()) {
        qDebug() << " База НЕ подключена при запуске!";
    } else {
        qDebug() << " База подключена!";
    }

    setupUI();
    phoneBook.loadFromFile("contacts.txt");
    updateTable();
}

void MainWindow::setupUI()
{
    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);

    searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText("Поиск по фамилии или email...");
    connect(searchEdit, &QLineEdit::textChanged, this, &MainWindow::filterTable);
    layout->addWidget(searchEdit);

    table = new QTableWidget(0, 7);
    table->setHorizontalHeaderLabels({
        "Фамилия", "Имя", "Отчество", "Адрес",
        "Дата рожд.", "Email", "Телефоны"
    });
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setSortingEnabled(true);
    layout->addWidget(table);

    QHBoxLayout *buttons = new QHBoxLayout();
    QPushButton *addBtn  = new QPushButton("Добавить");
    QPushButton *delBtn  = new QPushButton("Удалить");
    QPushButton *saveBtn = new QPushButton("Сохранить");

    buttons->addWidget(addBtn);
    buttons->addWidget(delBtn);
    buttons->addWidget(saveBtn);
    layout->addLayout(buttons);

    setCentralWidget(central);
    setWindowTitle("Телефонная книга");
    resize(1000, 600);

    connect(addBtn,  &QPushButton::clicked, this, &MainWindow::addContact);
    connect(delBtn,  &QPushButton::clicked, this, &MainWindow::removeContact);
    connect(saveBtn, &QPushButton::clicked, this, &MainWindow::saveToFile);
}

bool MainWindow::isValid(const Contact& c)
{
    QRegularExpression nameRx(
        "^[a-zA-Zа-яА-Я]([a-zA-Zа-яА-Я0-9\\s]*[a-zA-Zа-яА-Я0-9])?$");

    QRegularExpression emailRx(
        "^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");

    QRegularExpression phoneRx(
        "^(\\+7|8)\\(?\\d{3}\\)?\\d{3}-?\\d{2}-?\\d{2}$");

    if (c.secondName.isEmpty() || !nameRx.match(c.secondName).hasMatch())
        return false;

    if (c.name.isEmpty() || !nameRx.match(c.name).hasMatch())
        return false;

    if (c.email.isEmpty() || !emailRx.match(c.email).hasMatch())
        return false;

    if (c.phones.isEmpty())
        return false;

    for (const QString& p : c.phones)
        if (!phoneRx.match(p).hasMatch())
            return false;

    if (!c.birthday.isValid() || c.birthday >= QDate::currentDate())
        return false;

    return true;
}

void MainWindow::updateTable()
{
    table->setSortingEnabled(false);
    table->setRowCount(0);

    const auto &contacts = phoneBook.all();
    for (int i = 0; i < contacts.size(); ++i) {
        const Contact &c = contacts.at(i);

        table->insertRow(i);
        table->setItem(i, 0, new QTableWidgetItem(c.secondName));
        table->setItem(i, 1, new QTableWidgetItem(c.name));
        table->setItem(i, 2, new QTableWidgetItem(c.patronymic));
        table->setItem(i, 3, new QTableWidgetItem(c.address));
        table->setItem(i, 4, new QTableWidgetItem(c.birthday.toString("dd.MM.yyyy")));
        table->setItem(i, 5, new QTableWidgetItem(c.email));
        table->setItem(i, 6, new QTableWidgetItem(c.phones.join(", ")));
    }

    table->setSortingEnabled(true);
}

void MainWindow::addContact()
{
    Contact c;
    bool ok;

    // --- Фамилия (обязательная)
    c.secondName = QInputDialog::getText(this, "Ввод", "Фамилия:", QLineEdit::Normal, "", &ok).trimmed();
    if (!ok || c.secondName.isEmpty()) return;

    // --- Имя (обязательное)
    c.name = QInputDialog::getText(this, "Ввод", "Имя:", QLineEdit::Normal, "", &ok).trimmed();
    if (!ok || c.name.isEmpty()) return;

    // --- Отчество (необязательное)
    c.patronymic = QInputDialog::getText(this, "Ввод", "Отчество (необязательно):", QLineEdit::Normal, "", &ok).trimmed();
    if (!ok) return;

    // --- Адрес (необязательный)
    c.address = QInputDialog::getText(this, "Ввод", "Адрес (необязательно):", QLineEdit::Normal, "", &ok).trimmed();
    if (!ok) return;

    // --- Email (обязательный)
    c.email = QInputDialog::getText(this, "Ввод", "Email:", QLineEdit::Normal, "", &ok).trimmed();
    if (!ok || c.email.isEmpty()) return;

    // --- Телефон (необязательный, можно несколько)
    QString phone = QInputDialog::getText(this, "Ввод", "Телефон (необязательно):", QLineEdit::Normal, "", &ok).trimmed();
    if (!ok) return;
    if (!phone.isEmpty())
        c.phones << phone;

    // --- Дата рождения (необязательная)
    QString dateStr = QInputDialog::getText(this, "Ввод", "Дата рождения (дд-мм-гггг, необязательно):", QLineEdit::Normal, "", &ok).trimmed();
    if (!ok) return;
    if (!dateStr.isEmpty())
        c.birthday = QDate::fromString(dateStr, "dd-MM-yyyy");

    // --- Проверка и вывод отладки
    bool valid = isValid(c);
    qDebug() << "Добавление контакта:"
             << "valid:" << valid
             << "Фамилия:" << c.secondName
             << "Имя:" << c.name
             << "Email:" << c.email
             << "Телефоны:" << c.phones
             << "Дата рождения:" << c.birthday;

    if (!valid) {
        QMessageBox::warning(this, "Ошибка", "Некорректные данные контакта.\nТелефон и дата рождения теперь необязательны.");
        // Можно игнорировать проверку телефонов и даты, если нужно
        // valid = true;  // расскомментировать, чтобы добавлялись все контакты
    }

    // --- Сохраняем в локальную книгу и в PostgreSQL
    phoneBook.add(c);

    bool dbOk = db.addContact(c);
    if (!dbOk) {
        QMessageBox::critical(this, "Ошибка БД", "Не удалось добавить контакт в базу данных! Проверьте консоль.");
    }

    updateTable();
}


void MainWindow::removeContact()
{
    int row = table->currentRow();
    if (row >= 0) {
        phoneBook.remove(row);
        updateTable();
    }
}

void MainWindow::saveToFile()
{
    QString path = QDir::homePath() + "/Desktop/contacts.txt";
    phoneBook.saveToFile(path);
}

void MainWindow::filterTable()
{
    QString text = searchEdit->text().toLower();

    for (int i = 0; i < table->rowCount(); ++i) {
        bool match =
            table->item(i, 0)->text().toLower().contains(text) ||
            table->item(i, 5)->text().toLower().contains(text);

        table->setRowHidden(i, !match);
    }
}
