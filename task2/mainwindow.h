#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QDateEdit>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include "contactmanager.h"
#include "contactvalidator.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addButton_clicked();
    void on_removeButton_clicked();
    void on_saveButton_clicked();
    void on_contactTable_cellClicked(int row, int column);
    void on_searchEdit_textChanged(const QString &arg1);
    void on_sortFieldCombo_currentIndexChanged(int index);
    void on_sortOrderCombo_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    ContactManager m_contactManager;
    int m_currentRow = -1; // Индекс текущего выбранного контакта для редактирования

    void loadTableData(const QList<Contact>& contacts);
    void populateContactFields(const Contact& contact);
    Contact getContactFromFields() const;
    bool validateFields(bool isNewContact) const;
    void clearFields();
};

#endif // MAINWINDOW_H
