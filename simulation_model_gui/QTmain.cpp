#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow mainWindow; // Создание главного окна
    mainWindow.show();     // Отображение главного окна

    return app.exec();     // Запуск цикла обработки событий
}
