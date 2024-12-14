#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QGraphicsTextItem>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), scene(new QGraphicsScene(this)), m_model(new Model){
    ui->setupUi(this);

    if (!statusBar()) {
        setStatusBar(new QStatusBar(this));
    }

    // show message of status bar
    statusBar()->showMessage("Готов к работе");

    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing); // To smooth the lines
    scene->setSceneRect(0, 0, 1700, 900);

    // Draw the initial coordinate system
    drawCoordinateSystem();

    // forced update of fitInView via QTimer
    QTimer::singleShot(0, this, [this]() {
        ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    });

    // Connecting signals
    connect(ui->stateSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::updateTransitionTable);
    connect(ui->updateButton, &QPushButton::clicked, this, &MainWindow::updateSimulation);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
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
    scene->clear();
    m_coordinateY.clear();

    // Getting parameters
    int states = ui->stateSpinBox->value();
    double end_time = ui->timeSpinBox->value();
    QStringList lambda_list = ui->lambdaInput->text().split(",");
    QStringList alpha_list = ui->alphaInput->text().split(",");

    if (lambda_list.size() != states || alpha_list.size() != states) {
        statusBar()->showMessage("Ошибка: Проверьте ввод данных!");
        return;
    }

    // Read lambdas
    std::vector<double> lambdas;
    for (const auto &val : lambda_list) {
        lambdas.push_back(val.toDouble());
    }
    if (!std::is_sorted(lambdas.begin(), lambdas.end(), std::greater<int>()))
    {
        std::sort(lambdas.begin(), lambdas.end(), std::greater<int>());
        statusBar()->showMessage("Значения случайного процесса были упорядочены по убыванию!");
    }

    // Read alphas
    std::vector<double> alphas;
    for (const auto &val : alpha_list) {
        alphas.push_back(val.toDouble());
    }

    // Reading the transition probabilities by the first random variable
    std::vector<std::vector<double>> first_transitions(states, std::vector<double>(states));
    for (int i = 0; i < states; ++i) {
        for (int j = 0; j < states; ++j) {
            first_transitions[i][j] = ui->firstTransitionTable->item(i, j)->text().toDouble();
            if(first_transitions[i][j] == 0.0)
            {
                statusBar()->showMessage("Ошибка: Проверьте ввод вероятностей!");
                return;
            }
        }
    }

    // Reading the probabilities of transitions by the second random variable
    std::vector<std::vector<double>> second_transitions(states, std::vector<double>(states));
    for (int i = 0; i < states; ++i) {
        for (int j = 0; j < states; ++j) {
            second_transitions[i][j] = ui->secondTransitionTable->item(i, j)->text().toDouble();
            if(second_transitions[i][j] == 0.0)
            {
                statusBar()->showMessage("Ошибка: Проверьте ввод вероятностей!");
                return;
            }
        }
    }

    for(int i = 0; i < states; ++i)
    {
        double first_sum_probabilities = 0;
        double second_sum_probabilities = 0;
        for(int j = 0; j < states; ++j)
        {
            first_sum_probabilities = first_sum_probabilities + first_transitions[i][j];
            second_sum_probabilities = second_sum_probabilities + second_transitions[i][j];
        }
        if(first_sum_probabilities != 1 || second_sum_probabilities != 1)
        {
            statusBar()->showMessage("Ошибка: Проверьте ввод вероятностей!");
            return;
        }
    }

    drawCoordinateSystem(lambdas, end_time);
    visualizeSimulation(lambdas, alphas, first_transitions, second_transitions);
}

void MainWindow::drawCoordinateSystem() {
    scene->setSceneRect(0, 0, 1700, 900); // Установите фиксированные границы сцены

    // Рисуем оси
    scene->addLine(50, 800, 1600, 800); // Ось времени (oX)
    scene->addLine(50, 800, 50, 50);  // Ось лямбд (oY)

    // Добавляем стрелку на конце оси OX
    scene->addLine(1600, 800, 1596, 798); // Линия стрелки вниз
    scene->addLine(1600, 800, 1596, 802); // Линия стрелки вверх

    // Добавляем стрелку на конце оси OY
    scene->addLine(50, 50, 48, 60); // Линия стрелки влево
    scene->addLine(50, 50, 52, 60); // Линия стрелки вправо

    // Метки для оси oX
    for (int i = 0; i <= 10; ++i) {
        int x = 50 + i * 150;
        scene->addLine(x, 803, x, 797);
        auto *textItem = scene->addText(QString::number(i));
        textItem->setPos(x - 10, 810); // Текст под засечкой
    }

    // Метки для оси oY
    for (int i = 1; i <= 5; ++i) {
        int y = 800 - i * 140; // Позиция засечки по оси Y
        scene->addLine(45, y, 55, y); // Засечка
        auto *textItem = scene->addText(QString::number(i));
        textItem->setPos(15, y - 10); // Текст слева от засечки
    }
}

void MainWindow::drawCoordinateSystem(const std::vector<double>& lambdas, const double end_time)
{
    scene->setSceneRect(0, 0, 1700, 900);

    // Рисуем оси
    scene->addLine(50, 800, 1600, 800); // Ось времени (oX)
    scene->addLine(50, 800, 50, 50);  // Ось лямбд (oY)

    // Добавляем стрелку на конце оси OX
    scene->addLine(1600, 800, 1596, 798); // Линия стрелки вниз
    scene->addLine(1600, 800, 1596, 802); // Линия стрелки вверх

    // Добавляем стрелку на конце оси OY
    scene->addLine(50, 50, 48, 60); // Линия стрелки влево
    scene->addLine(50, 50, 52, 60); // Линия стрелки вправо

    auto *origin_coordinate = scene->addText(QString::number(0));
    origin_coordinate->setPos(35, 805);

    int numLambdas = lambdas.size();
    // Нахождение минимального и максимального значения лямбды
    double maxLambda = *std::max_element(lambdas.begin(), lambdas.end());
    double minLambda = *std::min_element(lambdas.begin(), lambdas.end());

    if (numLambdas == 2)
    {
        QFont font;
        font.setBold(true);
        scene->addLine(48, 200, 52, 200);
        m_coordinateY.push_back(200);
        QString lambda_text = QString("λ%1 = %2").arg(1).arg(lambdas[0], 0, 'f', 2); // Лямбда с 2 знаками после запятой
        auto* textItem = scene->addText(lambda_text);
        textItem->setScale(0.9);
        textItem->setFont(font);
        textItem->setPos(0, 200 - 10); // Текст слева от засечки

        scene->addLine(48, 650, 52, 650);
        m_coordinateY.push_back(650);
        lambda_text = QString("λ%1 = %2").arg(2).arg(lambdas[1], 0, 'f', 2); // Лямбда с 2 знаками после запятой
        textItem = scene->addText(lambda_text);
        textItem->setScale(0.9);
        textItem->setFont(font);
        textItem->setPos(0, 650 - 10); // Текст слева от засечки
    }
    else
    {
        for (int i = 0; i < numLambdas; ++i) {
            // Нормализуем значение лямбды, чтобы оно соотносилось с диапазоном по оси Y
            // Лямбда, которая максимальна, будет на 50 пикселей (верхний предел оси Y)
            double normalizedY = 800 - (lambdas[i] - minLambda) / (maxLambda - minLambda) * 750.0;
            if (i == 0) {
                // Если это максимальная лямбда, смещаем её немного вниз
                normalizedY += 15;  // Немного ниже
            }
            else if (i == numLambdas - 1) {
                // Если это минимальная лямбда, смещаем её немного вверх
                normalizedY -= 25;  // Немного выше
            }
            // Рисуем засечку
            scene->addLine(48, normalizedY, 52, normalizedY); // Засечка оси
            m_coordinateY.push_back(normalizedY);
            // Отображаем лямбду как текст
            QString lambda_text = QString("λ%1 = %2").arg(i + 1).arg(lambdas[i], 0, 'f', 2); // Лямбда с 2 знаками после запятой
            auto* textItem = scene->addText(lambda_text);
            QFont font;
            font.setBold(true);
            textItem->setScale(0.85);
            textItem->setFont(font);
            textItem->setPos(0, normalizedY - 10); // Текст слева от засечки
        }
    }
    // Отображаем время
    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(2);
    scene->addLine(1550, 798, 1550, 802, pen);
    QString time_text = "end_time = " + QString::number(end_time, 'f', 2);
    auto* textItem = scene->addText(time_text);
    //textItem->setScale(0.5);
    textItem->setPos(1550, 780);
    textItem->setDefaultTextColor(Qt::red);
}

void MainWindow::visualizeSimulation(std::vector<double>& lambdas, std::vector<double>& alphas, const std::vector<std::vector<double>>& firstTransitions, const std::vector<std::vector<double>>& secondTransitions) {
    delete m_model;

    double begin_time = 0.0;
    int states = ui->stateSpinBox->value();
    double end_time = ui->timeSpinBox->value();
    m_model = new Model(states, lambdas);
    m_model->setParamsRandomVariable(alphas);
    m_model->setTransitionProbabilities(firstTransitions, secondTransitions);

    double max_time = end_time;

    std::vector<double> result_vector;
    result_vector.push_back(begin_time);
    int initial_state = m_model->_lotteryInitialState();
    m_model->m_states[initial_state].m_count++;
    NumberRV number_RV = NumberRV::First;
    double tao = m_model->_timeSpentState(initial_state, number_RV);
    begin_time = begin_time + tao;
    result_vector.push_back(begin_time);

    //rendering the initial iteration
    QPen pen_state;//задаем кисть потолще
    pen_state.setWidth(4);
    double old_normalizedX = (tao / max_time) * (1550.0 - 50.0) + 50;
    scene->addEllipse(old_normalizedX - 3, 800 - 3, 6, 6);
    auto* textItem = scene->addText(QString::number(tao));
    textItem->setPos(old_normalizedX - 15, 810);
    //textItem->setScale(0.5);
    scene->addLine(50, m_coordinateY[initial_state], old_normalizedX, m_coordinateY[initial_state], pen_state);

    //displaying dotted lines
    QPen pen;//для вертикальной пуунктирной линии
    pen.setStyle(Qt::DashLine);
    //scene->addLine(old_normalizedX, m_coordinateY[initial_state], old_normalizedX, 800, pen);

    QPen pen_arrow;//задаем кисть потолще для отрисовки стрелок
    pen_arrow.setWidth(2);

    int current_state = initial_state;
    int i = 0;
    double old_begin_time = begin_time;
    while (begin_time <= end_time)
    {
        current_state = m_model->_lotteryState(current_state, number_RV);
        assert(current_state != INT_MAX);
        //add a counter that counts the number of stays in the state
        m_model->m_states[current_state].m_count++;
        tao = m_model->_timeSpentState(current_state, number_RV);
        begin_time = begin_time + tao;

        if (begin_time <= end_time)
        {
            result_vector.push_back(begin_time);
            double new_normalizedX = (begin_time / max_time) * (1550.0 - 50.0) + 50;
            scene->addEllipse(new_normalizedX - 3, 800 - 3, 6, 6);
            auto* textItem = scene->addText(QString::number(begin_time));

            if(std::abs(old_begin_time - begin_time) < 0.05)
            {
                i += 10;
            }
            else
            {
                i = 0;
            }

            textItem->setPos(new_normalizedX - 10, 810 + i);
            //textItem->setScale(0.75);
            scene->addLine(old_normalizedX, m_coordinateY[current_state], new_normalizedX, m_coordinateY[current_state], pen_state);

            if(initial_state > current_state)
            {
                //отрисовка вертикальных линий
                scene->addLine(old_normalizedX, m_coordinateY[current_state], old_normalizedX, 800, pen);
                //отрисовка стрелки
                scene->addLine(old_normalizedX, m_coordinateY[current_state], old_normalizedX - 7, m_coordinateY[current_state] + 10, pen_arrow);
                scene->addLine(old_normalizedX, m_coordinateY[current_state], old_normalizedX + 7, m_coordinateY[current_state] + 10, pen_arrow);
                //отрисовка по какой СВ перешли
                if(number_RV == NumberRV::First)
                {
                    auto* textRV = scene->addText("λ");
                    textRV->setFont(QFont("Arial", 16));
                    textRV->setPos(old_normalizedX - 20, m_coordinateY[current_state] + 15);
                    //textRV->setRotation(-90);
                }
                else
                {
                    auto* textRV = scene->addText("α");
                    textRV->setFont(QFont("Arial", 16));
                    textRV->setPos(old_normalizedX - 20, m_coordinateY[current_state] + 15);
                    //textRV->setRotation(-90);
                }
            }
            else if(current_state > initial_state)
            {
                //отрисовка вертикальных линий
                scene->addLine(old_normalizedX, m_coordinateY[initial_state], old_normalizedX, 800, pen);
                //отрисовка стрелки
                scene->addLine(old_normalizedX, m_coordinateY[current_state], old_normalizedX - 7, m_coordinateY[current_state] - 10, pen_arrow);
                scene->addLine(old_normalizedX, m_coordinateY[current_state], old_normalizedX + 7, m_coordinateY[current_state] - 10, pen_arrow);
                //отрисовка по какой СВ перешли
                if(number_RV == NumberRV::First)
                {
                    auto* textRV = scene->addText("λ");
                    textRV->setFont(QFont("Arial", 16));
                    textRV->setPos(old_normalizedX - 20, m_coordinateY[current_state] - 40);
                    //textRV->setRotation(90);
                }
                else
                {
                    auto* textRV = scene->addText("α");
                    textRV->setFont(QFont("Arial", 16));
                    textRV->setPos(old_normalizedX - 20, m_coordinateY[current_state] - 40);
                    //textRV->setRotation(90);
                }
            }
            else
            {
                //отрисовка вертикальных линий
                scene->addLine(old_normalizedX, m_coordinateY[initial_state], old_normalizedX, 800, pen);
                //отрисовка стрелки
                scene->addLine(old_normalizedX, m_coordinateY[current_state], old_normalizedX - 10, m_coordinateY[current_state] - 7, pen_arrow);
                scene->addLine(old_normalizedX, m_coordinateY[current_state], old_normalizedX - 10, m_coordinateY[current_state] + 7, pen_arrow);
                //отрисовка по какой СВ перешли
                if(number_RV == NumberRV::First)
                {
                    auto* textRV = scene->addText("λ");
                    textRV->setFont(QFont("Arial", 16));
                    textRV->setPos(old_normalizedX - 10, m_coordinateY[current_state] - 40);
                }
                else
                {
                    auto* textRV = scene->addText("α");
                    textRV->setFont(QFont("Arial", 16));
                    textRV->setPos(old_normalizedX - 10, m_coordinateY[current_state] - 40);
                }
            }
            old_normalizedX = new_normalizedX;
            initial_state = current_state;
            old_begin_time = begin_time;
        }
    }
    scene->addLine(old_normalizedX, m_coordinateY[initial_state], old_normalizedX, 800, pen);//отрисовка вертикальной линии у последнего состояния
}

