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
  // Константы

  int total_main_count = 0;

  static const double PI  = 3.141592653589793238463;
  int size_hidden, size_hidden2, size_input, size_outer;
  static const double k_nn = 40, k_pos = 20;
  double hidden[125],
         hidden2[125],
         enters[24],
         outer[16],
         outer_fun[16];
  double wEH[24][525],
         wHH2[525][525],
         wHO[525][36];
  int current_angle[5];
  int current_position[3];
  int L[5];

  // Константы рисования
  QGraphicsScene *graphicsScene;
  QGraphicsScene *graphicsNNTeachScene;
  int k_iter_graph;
  int draw_matrix[100];

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

    void calc_action();
    void clear_text();
    void forward_kinematics_click();
    void forward_kinematics_random();
    void nnBeginStudy();

    //Обратная задача кинемаики
    void ob_random_fun();
    void ob_kin_calc_fun();
private:
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
