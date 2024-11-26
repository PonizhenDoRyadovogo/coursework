#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QGraphicsTextItem>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), scene(new QGraphicsScene(this)) {
    ui->setupUi(this);

    if (!statusBar()) {
        setStatusBar(new QStatusBar(this));
    }

    // Отображение сообщения
    statusBar()->showMessage("Готов к работе");

    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing); // Для сглаживания линий
    scene->setSceneRect(0, 0, 600, 400);

    // Рисуем начальную координатную систему
    drawCoordinateSystem();

    // Принудительное обновление fitInView через QTimer
    QTimer::singleShot(0, this, [this]() {
        ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    });

    // Подключение сигналов
    connect(ui->stateSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::updateTransitionTable);
    connect(ui->updateButton, &QPushButton::clicked, this, &MainWindow::updateSimulation);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event); // Сначала передайте событие базовому классу

    // Установите новую область отображения сцены
    ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::updateTransitionTable() {
    int states = ui->stateSpinBox->value();
    ui->firstTransitionTable->setRowCount(states);
    ui->firstTransitionTable->setColumnCount(states);
    ui->secondTransitionTable->setRowCount(states);
    ui->secondTransitionTable->setColumnCount(states);
}

void MainWindow::updateSimulation() {
    // Очистка сцены
    scene->clear();
    drawCoordinateSystem();

    // Получение параметров
    int states = ui->stateSpinBox->value();
    QStringList lambdaList = ui->lambdaInput->text().split(",");
    QStringList alphaList = ui->alphaInput->text().split(",");

    if (lambdaList.size() != states || alphaList.size() != states) {
        statusBar()->showMessage("Ошибка: Проверьте ввод данных!");
        return;
    }

    // Чтение лямбд
    std::vector<double> lambdas;
    for (const auto &val : lambdaList) {
        lambdas.push_back(val.toDouble());
    }

    // Чтение вероятностей переходов
    std::vector<std::vector<double>> transitions(states, std::vector<double>(states));
    for (int i = 0; i < states; ++i) {
        for (int j = 0; j < states; ++j) {
            transitions[i][j] = ui->firstTransitionTable->item(i, j)->text().toDouble();
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
    scene->setSceneRect(0, 0, 600, 400); // Установите фиксированные границы сцены

    // Рисуем оси
    scene->addLine(50, 300, 550, 300); // Ось времени (oX)
    scene->addLine(50, 300, 50, 50);  // Ось лямбд (oY)

    // Добавляем стрелку на конце оси OX
    scene->addLine(550, 300, 540, 290); // Линия стрелки вниз
    scene->addLine(550, 300, 540, 310); // Линия стрелки вверх

    // Добавляем стрелку на конце оси OY
    scene->addLine(50, 50, 40, 60); // Линия стрелки влево
    scene->addLine(50, 50, 60, 60); // Линия стрелки вправо

    // Метки для оси oX
    for (int i = 0; i <= 10; ++i) {
        int x = 50 + i * 50;
        if(i != 10)
        {
        scene->addLine(x, 295, x, 305);
        }
        auto *textItem = scene->addText(QString::number(i));
        textItem->setPos(x - 10, 310); // Текст под засечкой
    }

    // Метки для оси oY
    for (int i = 1; i <= 5; ++i) {
        int y = 300 - i * 50; // Позиция засечки по оси Y
        if(i != 5)
        {
        scene->addLine(45, y, 55, y); // Засечка
        }
        auto *textItem = scene->addText(QString::number(i));
        textItem->setPos(15, y - 10); // Текст слева от засечки
    }
}

void MainWindow::visualizeSimulation(const std::vector<double>& lambdas, const std::vector<std::vector<double>>& transitions) {
    // Убедимся, что лямбды отображаются в первой четверти
    for (size_t i = 0; i < lambdas.size(); ++i) {
        // Текст для отображения лямбд
        QString lambdaText = QString("λ%1 = %2").arg(i + 1).arg(lambdas[i], 0, 'f', 2); // 2 знака после запятой
        auto *textItem = scene->addText(lambdaText);

        // Позиционируем текст
        int xPosition = 5;                  // Небольшой отступ от оси Y
        int yPosition = 300 - i * 50 - 20;  // Смещение вверх от метки оси Y
        textItem->setPos(xPosition, yPosition);
    }

    // Добавьте визуализацию переходов между состояниями
    for (size_t i = 0; i < transitions.size(); ++i) {
        for (size_t j = 0; j < transitions[i].size(); ++j) {
            if (transitions[i][j] > 0) {
                // Пример отрисовки стрелки или линии для перехода
                int xStart = 50 + i * 50;  // Исходное состояние
                int xEnd = 50 + j * 50;    // Конечное состояние
                int yLevel = 300 - i * 50;

                // Добавляем линию или визуализацию перехода
                scene->addLine(xStart, yLevel, xEnd, yLevel - 10, QPen(Qt::black));
            }
        }
    }
}

