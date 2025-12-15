#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Установка заголовков таблицы
    ui->contactTable->setColumnCount(7);
    QStringList headers = {"Фамилия", "Имя", "Отчество", "Адрес", "Дата Рождения", "Email", "Телефоны"};
    ui->contactTable->setHorizontalHeaderLabels(headers);
    ui->contactTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->contactTable->setEditTriggers(QAbstractItemView::NoEditTriggers); // Запретить редактирование в таблице
    ui->contactTable->horizontalHeader()->setStretchLastSection(true); // Растянуть последнюю секцию

    // Инициализация полей сортировки
    ui->sortFieldCombo->addItem("Фамилия", ContactManager::SortBySurname);
    ui->sortFieldCombo->addItem("Имя", ContactManager::SortByName);
    ui->sortFieldCombo->addItem("Email", ContactManager::SortByEmail);
    ui->sortOrderCombo->addItem("По возрастанию", Qt::AscendingOrder);
    ui->sortOrderCombo->addItem("По убыванию", Qt::DescendingOrder);

    // Установка начальной даты для QDateEdit
    ui->dobEdit->setDate(QDate(2000, 1, 1));
    ui->dobEdit->setDisplayFormat("dd.MM.yyyy");

    // Загрузка данных и заполнение таблицы
    m_contactManager.sortContacts(ContactManager::SortBySurname);
    loadTableData(m_contactManager.getAllContacts());
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Заполнение QTableWidget данными из списка контактов
void MainWindow::loadTableData(const QList<Contact>& contacts)
{
    ui->contactTable->setRowCount(0); // Очистить
    ui->contactTable->setRowCount(contacts.size());

    for (int i = 0; i < contacts.size(); ++i) {
        const Contact& c = contacts.at(i);
        ui->contactTable->setItem(i, 0, new QTableWidgetItem(c.surname()));
        ui->contactTable->setItem(i, 1, new QTableWidgetItem(c.name()));
        ui->contactTable->setItem(i, 2, new QTableWidgetItem(c.patronymic()));
        ui->contactTable->setItem(i, 3, new QTableWidgetItem(c.address()));
        ui->contactTable->setItem(i, 4, new QTableWidgetItem(c.dob().toString("dd.MM.yyyy")));
        ui->contactTable->setItem(i, 5, new QTableWidgetItem(c.email()));
        ui->contactTable->setItem(i, 6, new QTableWidgetItem(c.phoneNumbersString()));
    }
    ui->contactTable->resizeColumnsToContents();
}

// Заполнение полей ввода данными выбранного контакта
void MainWindow::populateContactFields(const Contact& contact)
{
    ui->surnameEdit->setText(contact.surname());
    ui->nameEdit->setText(contact.name());
    ui->patronymicEdit->setText(contact.patronymic());
    ui->addressEdit->setText(contact.address());
    ui->dobEdit->setDate(contact.dob());
    ui->emailEdit->setText(contact.email());
    ui->phoneNumbersEdit->setText(contact.phoneNumbers().join("\n")); // Телефоны через новую строку
}

// Извлечение данных из полей ввода
Contact MainWindow::getContactFromFields()const
{
    QString surname = ContactValidator::sanitizeNamePart(ui->surnameEdit->text());
    QString name = ContactValidator::sanitizeNamePart(ui->nameEdit->text());
    QString patronymic = ContactValidator::sanitizeNamePart(ui->patronymicEdit->text());
    QString address = ui->addressEdit->text().trimmed();
    QDate dob = ui->dobEdit->date();
    QString email = ContactValidator::sanitizeEmail(ui->emailEdit->text());

    // Телефоны (разделение по новой строке или запятой)
    QStringList phoneLines = ui->phoneNumbersEdit->toPlainText().split(QRegularExpression("[\\n,;]"), Qt::SkipEmptyParts);
    QStringList phoneNumbers;
    for (const QString& line : phoneLines) {
        phoneNumbers.append(line.trimmed());
    }

    return Contact(name, surname, patronymic, address, dob, email, phoneNumbers);
}

// Валидация всех полей
bool MainWindow::validateFields(bool isNewContact) const
{
    QString errorMsg;
    Contact tempContact = getContactFromFields();

    // Обязательные поля для создания (isNewContact) и редактирования
    bool mandatoryValid = true;

    if (!ContactValidator::isNameValid(tempContact.surname())) {
        errorMsg += "Фамилия: некорректно. Должна начинаться на букву, не иметь дефисов в начале/конце.\n";
        mandatoryValid = false;
    }
    if (!ContactValidator::isNameValid(tempContact.name())) {
        errorMsg += "Имя: некорректно. Должно начинаться на букву, не иметь дефисов в начале/конце.\n";
        mandatoryValid = false;
    }

    // Отчество не обязательное поле, но если заполнено, то должно быть валидным
    if (!tempContact.patronymic().isEmpty() && !ContactValidator::isNameValid(tempContact.patronymic())) {
        errorMsg += "Отчество: некорректно. Должно начинаться на букву, не иметь дефисов в начале/конце.\n";
    }

    if (!ContactValidator::isEmailValid(tempContact.email())) {
        errorMsg += "Email: некорректный формат.\n";
        mandatoryValid = false;
    }

    if (!ContactValidator::isDateOfBirthValid(tempContact.dob())) {
        errorMsg += "Дата рождения: некорректна (должна быть в прошлом).\n";
    }

    // Телефоны
    if (isNewContact) {
        // Для создания нужен хотя бы 1 валидный номер
        int validPhonesCount = 0;
        for (const QString& phone : tempContact.phoneNumbers()) {
            if (ContactValidator::isPhoneNumberValid(phone)) {
                validPhonesCount++;
            }
        }
        if (validPhonesCount < 1) {
            errorMsg += "Телефон: необходимо указать хотя бы 1 корректный телефонный номер.\n";
            mandatoryValid = false;
        }
    } else {
        // Для редактирования все заполненные номера должны быть валидны
        for (const QString& phone : tempContact.phoneNumbers()) {
            if (!phone.isEmpty() && !ContactValidator::isPhoneNumberValid(phone)) {
                errorMsg += QString("Телефон: Номер '%1' имеет некорректный формат.\n").arg(phone);
            }
        }
    }

    if (!errorMsg.isEmpty()) {
        QMessageBox::warning(this, "Ошибка валидации", errorMsg);
    }

    return errorMsg.isEmpty() && mandatoryValid;
}

// Очистка полей
void MainWindow::clearFields()
{
    ui->surnameEdit->clear();
    ui->nameEdit->clear();
    ui->patronymicEdit->clear();
    ui->addressEdit->clear();
    ui->emailEdit->clear();
    ui->dobEdit->setDate(QDate(2000, 1, 1));
    ui->phoneNumbersEdit->clear();
    m_currentRow = -1;
}

// Слот: Добавить контакт
void MainWindow::on_addButton_clicked()
{
    if (validateFields(true)) {
        Contact newContact = getContactFromFields();
        m_contactManager.addContact(newContact);
        clearFields();
        on_sortFieldCombo_currentIndexChanged(ui->sortFieldCombo->currentIndex()); // Обновить и отсортировать
        QMessageBox::information(this, "Успех", "Контакт успешно добавлен.");
    }
}

// Слот: Удалить контакт
void MainWindow::on_removeButton_clicked()
{
    if (m_currentRow != -1) {
        // Мы удаляем контакт из *отфильтрованного/отсортированного* списка.
        // Нужно найти оригинальный индекс (это сложно в QTableWidget без модели).
        // Для простоты, пока будем удалять по индексу в текущей таблице, если нет поиска.
        // Если был поиск, то удаляем элемент из списка, который сейчас отображается.

        // **Упрощение:** Если поиск активен, мы не можем просто удалить по индексу.
        // В реальном проекте тут нужна QAbstractTableModel.
        // Примем, что удаление работает только когда поиск пуст.

        if (!ui->searchEdit->text().isEmpty()) {
            QMessageBox::warning(this, "Внимание", "Удаление невозможно при активном поиске. Очистите поле поиска.");
            return;
        }

        int selectedRow = ui->contactTable->currentRow();
        if (selectedRow != -1) {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Подтверждение", "Вы уверены, что хотите удалить выбранный контакт?",
                                          QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::Yes) {
                m_contactManager.removeContact(selectedRow);
                clearFields();
                loadTableData(m_contactManager.getAllContacts());
                QMessageBox::information(this, "Успех", "Контакт удален.");
            }
        }
    } else {
        QMessageBox::warning(this, "Ошибка", "Выберите контакт для удаления.");
    }
}

// Слот: Сохранить (Обновить) контакт
void MainWindow::on_saveButton_clicked()
{
    if (m_currentRow != -1) {
        if (validateFields(false)) { // false - не новый контакт
            Contact updatedContact = getContactFromFields();

            // Снова: если поиск/сортировка активны, индекс в таблице не совпадает с индексом в Manager.
            // Примем, что редактируем только при пустом поиске.
            if (!ui->searchEdit->text().isEmpty()) {
                QMessageBox::warning(this, "Внимание", "Редактирование невозможно при активном поиске. Очистите поле поиска.");
                return;
            }

            m_contactManager.updateContact(m_currentRow, updatedContact);
            clearFields();
            on_sortFieldCombo_currentIndexChanged(ui->sortFieldCombo->currentIndex()); // Обновить и отсортировать
            QMessageBox::information(this, "Успех", "Контакт успешно обновлен.");
        }
    } else {
        QMessageBox::warning(this, "Ошибка", "Выберите контакт для сохранения (обновления).");
    }
}

// Слот: Выбор контакта в таблице
void MainWindow::on_contactTable_cellClicked(int row, int /*column*/)
{
    // Опять же, если поиск/сортировка активны, индекс в таблице - это индекс в отфильтрованном/отсортированном списке.
    // Если поиск активен, этот индекс не соответствует ContactManager::getContact(row).
    // Будем считать, что при клике мы всегда хотим редактировать.
    // Если поиск активен, то редактирование не сработает (см. on_saveButton_clicked).

    if (ui->searchEdit->text().isEmpty()) {
        m_currentRow = row;
        Contact contact = m_contactManager.getContact(row);
        populateContactFields(contact);
    } else {
        // Если поиск активен, нам нужно найти оригинал контакта.
        // Это можно сделать, например, по уникальному полю (Email) или просто не разрешать редактирование/удаление при поиске.
        QMessageBox::information(this, "Внимание", "Для редактирования или удаления контакта сначала очистите поле поиска.");
    }
}

// Слот: Поиск
void MainWindow::on_searchEdit_textChanged(const QString &arg1)
{
    QList<Contact> results = m_contactManager.searchContacts(arg1.trimmed());
    loadTableData(results);
    clearFields(); // Очистить поля ввода при смене режима (поиск/нет поиска)
    m_currentRow = -1; // Сброс выбранного
}

// Слот: Сортировка по полю
void MainWindow::on_sortFieldCombo_currentIndexChanged(int /*index*/)
{
    if (ui->searchEdit->text().isEmpty()) {
        ContactManager::SortField field = (ContactManager::SortField)ui->sortFieldCombo->currentData().toInt();
        Qt::SortOrder order = (Qt::SortOrder)ui->sortOrderCombo->currentData().toInt();

        m_contactManager.sortContacts(field, order);
        loadTableData(m_contactManager.getAllContacts());
    } else {
        // Если активен поиск, сортируем результаты поиска (поиск уже отработал в textChanged)
        on_searchEdit_textChanged(ui->searchEdit->text());
    }
    clearFields();
    m_currentRow = -1;
}

// Слот: Сортировка по порядку
void MainWindow::on_sortOrderCombo_currentIndexChanged(int /*index*/)
{
    on_sortFieldCombo_currentIndexChanged(ui->sortFieldCombo->currentIndex()); // Просто переиспользовать слот
}
