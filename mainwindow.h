#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QString>
#include <QPushButton>
#include <QLineEdit>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QTimer>
#include "tread.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  QTextEdit textEdit;
  QPushButton cacl;
  QLineEdit iteration;
  Thread *t;
  QTimer *timer = new QTimer(this);
  // Константы

  int total_main_count = 0;

  static const double PI  = 3.141592653589793238463;
  int size_hidden, size_hidden2, size_input, size_outer;
  static const double k_nn = 45, k_pos = 25;
  double hidden[525],
  //       hidden2[125],
         enters[3],
         outer[3],
         outer_fun[3];
  double wEH[24][225],
  //       wHH2[525][525],
         wHO[525][36];
  int current_angle[5];
  int current_position[3];
  int L[5];
  // Обучение
  int teach_data[255][2][3];
  int teach_count = 0;
  int teach_count_data = 0;

  // Константы рисования
  QGraphicsScene *graphicsScene;
  QGraphicsScene *graphicsNNTeachScene;
  int k_iter_graph;
  double draw_matrix[100];

  // Общее
  void set_text(double);

  //Обучение
  void action_teach();
  void countOuter(double []);
  void study(double*, double*);

  //Прямая задача кинематики
  void forward_kinematics_calc();

  //Обратная задача кинемаики


  //Получение параметров
  void random_params_angle();
  void getParamsAngleFromInputPr();
  void setParamsAngleToInputPr();

  // Функции активации
  float funct(float);
  float unfunct(float);

  // Общие функции
  void getMapAction();
  void getDefaultWeight();
  void setDefaultParams();

  // Рисование
  void drawManipulator();
  void drawGraph();

  private slots:
    void tread_action();
    void update_nn();
    void timer_check_fun();

    void calc_action();
    void clear_text();
    void forward_kinematics_click();
    void forward_kinematics_random();
    void nnBeginStudy();

    //Обратная задача кинемаики
    void ob_random_fun();
    void ob_kin_calc_fun();

    // Обучающая выборка
    void teach_add_fun();
    void teach_delete_fun();

private:
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
