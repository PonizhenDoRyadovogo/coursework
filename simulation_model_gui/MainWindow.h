#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTableWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QStatusBar>

#include "../simulation_model/simulation_model/simulation_model.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void updateTransitionTable(); // Слот для обновления таблицы вероятностей переходов
    void startSimulation();       // Слот для запуска симуляции
    void updateSimulation(); // Добавьте этот слот

protected:
     void resizeEvent(QResizeEvent *event) override;
private:
    void drawCoordinateSystem();  // Метод для отрисовки системы координат
    void visualizeSimulation(std::vector<double>& lambdas, const std::vector<double>& alphas,const std::vector<std::vector<double>>& firstTransitions, const std::vector<std::vector<double>>& secondTransitions);   // Метод для отображения результатов симуляции
    void drawCoordinateSystem(const std::vector<double>& lambdas, const double end_time);
private:
    Ui::MainWindow *ui;           // Указатель на автоматически сгенерированный интерфейс
    QGraphicsScene *scene;        // Графическая сцена для визуализации

    QSpinBox *stateSpinBox;       // Поле для выбора количества состояний
    QLineEdit *lambdaInput;       // Поле для ввода лямбд
    QLineEdit *alphaInput;        // Поле для ввода параметров альфа
    QTableWidget *transitionTable;// Таблица для ввода вероятностей переходов
    Model* m_model = nullptr;
};

#endif // MAINWINDOW_H
