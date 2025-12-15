#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    // Инициализация Qt Application
    QApplication a(argc, argv);

    // Qt 6.10.1
    // QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    // Для более старых версий может потребоваться:
    // QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // Главное окно
    MainWindow w;
    w.show();

    return a.exec();
}
