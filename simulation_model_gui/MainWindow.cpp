#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QGraphicsTextItem>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), scene(new QGraphicsScene(this)) {
    ui->setupUi(this);

    if (!statusBar()) {
        setStatusBar(new QStatusBar(this));
    }

    // Отображение сообщения
    statusBar()->showMessage("Готов к работе");

    ui->graphicsView->setScene(scene);

    // Подключение сигналов
    connect(ui->stateSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::updateTransitionTable);
    connect(ui->updateButton, &QPushButton::clicked, this, &MainWindow::updateSimulation);

    // Рисуем начальную координатную систему
    drawCoordinateSystem();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::updateTransitionTable() {
    int states = ui->stateSpinBox->value();
    ui->transitionTable->setRowCount(states);
    ui->transitionTable->setColumnCount(states);
}

void MainWindow::updateSimulation() {
    // Очистка сцены
    scene->clear();
    drawCoordinateSystem();

    // Получение параметров
    int states = ui->stateSpinBox->value();
    QStringList lambdaList = ui->lambdaInput->text().split(",");
    QStringList alphaList = ui->alphaInput->text().split(",");

//    if (lambdaList.size() != states || alphaList.size() != states) {
//        ui->statusbar->showMessage("Ошибка: Проверьте ввод данных!");
//        return;
//    }

    // Чтение лямбд
    std::vector<double> lambdas;
    for (const auto &val : lambdaList) {
        lambdas.push_back(val.toDouble());
    }

    // Чтение вероятностей переходов
    std::vector<std::vector<double>> transitions(states, std::vector<double>(states));
    for (int i = 0; i < states; ++i) {
        for (int j = 0; j < states; ++j) {
            transitions[i][j] = ui->transitionTable->item(i, j)->text().toDouble();
        }
    }

    // Визуализация
    visualizeSimulation(lambdas, transitions);
}

void MainWindow::startSimulation() {
    // Здесь ваша логика для запуска симуляции
    //ui->statusBar->showMessage("Симуляция запущена!");
}

void MainWindow::drawCoordinateSystem() {
    // Рисуем оси
    scene->addLine(50, 300, 500, 300); // Ось времени (oX)
    scene->addLine(50, 300, 50, 50);  // Ось лямбд (oY)

    // Метки для оси oX
    for (int i = 0; i <= 10; ++i) {
        int x = 50 + i * 50;
        scene->addLine(x, 295, x, 305);
        auto *textItem = scene->addText(QString::number(i));
        textItem->setPos(x - 10, 310); // Текст под засечкой
    }

    // Метки для оси oY
    for (int i = 1; i <= 5; ++i) {
        int y = 300 - i * 50; // Позиция засечки по оси Y
        scene->addLine(45, y, 55, y); // Засечка
        auto *textItem = scene->addText(QString::number(i));
        textItem->setPos(15, y - 10); // Текст слева от засечки
    }
}

void MainWindow::visualizeSimulation(const std::vector<double>& lambdas, const std::vector<std::vector<double>>& transitions) {
    for (size_t i = 0; i < lambdas.size(); ++i) {
        scene->addText(QString("λ%1 = %2").arg(i + 1).arg(lambdas[i]))
            ->setPos(-40, 300 - i * 50);
    }
    // Добавить визуализацию переходов
}
