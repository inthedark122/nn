#include <QTime>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>
#include "neural_network.h"
#include "draw.cpp"

#define length(x)  (sizeof(x)/sizeof(*(x)))

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWindow) {
  ui->setupUi(this);
  getDefaultWeight();
  getMapAction();
  setDefaultParams();
  graphicsScene = new QGraphicsScene(0, 0, 350, 200);
  graphicsNNTeachScene = new QGraphicsScene(0, 0, 350, 200);

  ui->graphicsView->setScene(graphicsScene);
  ui->graphicsView->setRenderHint(QPainter::Antialiasing, true);

  ui->graphicsNNTeach->setScene(graphicsNNTeachScene);
  ui->graphicsNNTeach->setRenderHint(QPainter::Antialiasing, true);

  drawManipulator();
  drawGraph();
}

MainWindow::~MainWindow() {
  delete ui;
}

// Функции обработки кнопок
void MainWindow::calc_action() {
  action_teach();
  drawManipulator();
}
void MainWindow::clear_text() {
  ui->textEdit->clear();
}
void MainWindow::forward_kinematics_click() {
  if (ui->pr_with_input->isChecked()) {
    getParamsAngleFromInputPr();
  }
  setParamsAngleToInputPr();
  forward_kinematics_calc();
}
void MainWindow::forward_kinematics_random() {
  random_params_angle();
  setParamsAngleToInputPr();
}
void MainWindow::nnBeginStudy() {
  action_teach();
}

// Функции получения данных
void MainWindow::random_params_angle() {
  int ar_tmp[3][3];
  ar_tmp[0][0] = 3;
  ar_tmp[0][1] = 4;
  ar_tmp[0][2] = 7;
  ar_tmp[1][0] = 15;
  ar_tmp[1][1] = 20;
  ar_tmp[1][2] = 30;
  ar_tmp[2][0] = 10;
  ar_tmp[2][1] = 30;
  ar_tmp[2][2] = 40;
  int k = rand()%3;
  this->current_angle[0] = ar_tmp[k][0]; //rand()%40;
  this->current_angle[1] = ar_tmp[k][1]; //rand()%40;
  this->current_angle[2] = ar_tmp[k][2]; //rand()%40;
  //this->current_angle[3] = rand()%40;
  //this->current_angle[4] = rand()%40;
}
void MainWindow::getParamsAngleFromInputPr() {
  this->current_angle[0] = ui->pr_Q1->text().toInt();
  this->current_angle[1] = ui->pr_Q2->text().toInt();
  this->current_angle[2] = ui->pr_Q3->text().toInt();
  this->current_angle[3] = ui->pr_Q4->text().toInt();
  this->current_angle[4] = ui->pr_Q5->text().toInt();
}

//Функции записи данных в поля
void MainWindow::setParamsAngleToInputPr() {
  ui->pr_Q1->setText(QString::number(this->current_angle[0]));
  ui->pr_Q2->setText(QString::number(this->current_angle[1]));
  ui->pr_Q3->setText(QString::number(this->current_angle[2]));
  ui->pr_Q4->setText(QString::number(this->current_angle[3]));
  ui->pr_Q5->setText(QString::number(this->current_angle[4]));
  drawManipulator();
}

//Обучение нейронной сети
void MainWindow::action_teach() {
  int iterat = ui->iteration->text().toInt();
  this->k_iter_graph = 0;
  int k_graph = 1;
  int dec = 0;
  int count = 0;
  for (int cikl=0; cikl < iterat; cikl++) {
    //QTime time;
    //time.start();
    int k = 0;
    draw_matrix[cikl] = 200;
    random_params_angle();
    setParamsAngleToInputPr();
    while (true) {
      int Q1 = this->current_angle[0];
      int Q2 = this->current_angle[1];
      int Q3 = this->current_angle[2];
      int x = this->current_position[0];
      int y = this->current_position[1];
      int z = this->current_position[2];

      double ar[3], ar_answer[3];
      //for (int i=0; i < 8; ++i) { ar[i] = (int)(x+50) & (1 << i) ? 1 : 0; }
      //for (int i=8; i < 16; ++i) { ar[i] = (int)(y+50) & (1 << (i-8)) ? 1 : 0;}
      //for (int i=16; i < 24; ++i) { ar[i] = (int)(z+50)& (1 << (i-16)) ? 1 : 0;}
      QString text_input, text_output;
      ar[0] = x/this->k_pos;
      ar[1] = y/this->k_pos;
      ar[2] = z/this->k_pos;
      for (int i=0; i < 3; i++) { text_input.append(QString::number(ar[i])); }

      //for (int i=0; i < 8; i++) {ar_answer[i] = (int)(Q1) & (1 << i) ? 1 : 0;}
      //for (int i=8; i < 16; i++) {ar_answer[i] = (int)(Q2) & (1 << i-8) ? 1 : 0;}
      //for (int i=16; i < 24; i++) {ar_answer[i] = (int)(Q3) & (1 << i-16) ? 1 : 0;}
      //for (int i=0; i < 24; i++) { text_output.append(QString::number(ar_answer[i])); }
      ar_answer[0] = Q1;
      ar_answer[1] = Q2;
      ar_answer[2] = Q3;
      for (int i=0; i < 3; i++) { text_output.append(QString::number(ar_answer[i])); }

      ui->nnInputValue->setText(text_input);
      ui->nnMathOutputValue->setText(text_output);
      this->study(ar, ar_answer);

      int tmp = 0;
      for (int j = 0; j < size_outer; j++ ) {
        if(ar_answer[j] != round(this->outer_fun[j]*40)) {
          tmp++;
        }
      }
      if (tmp == 0) break;
      if (k > 50) break;
      k++;
      draw_matrix[cikl] -= k_graph;
      dec++;
    }

    //int difference = time.elapsed();
    //ui->textEdit->append("\nВремя 1-го цикла - ");
    //ui->textEdit->append(QString::number(difference));
    //ui->textEdit->append("\n");
    this->k_iter_graph++;
    count++;
  }

  drawGraph();
}

void  MainWindow::countOuter(double *sample) {
  //QTime time;
  //time.start();
  double hidden_tmp[size_hidden];
  for (int i=0; i < size_hidden; i++) {
    hidden[i] = 0;
    for(int j=0; j < size_input; j++) {
      hidden[i] += sample[j]*wEH[j][i];
    };
    hidden_tmp[i] = funct(hidden[i]);
  }
  for (int i=0; i<size_outer; i++) {
    outer[i] = 0;
    for (int j=0; j < size_hidden; j++) {
      outer[i] += hidden_tmp[j]*wHO[j][i];
    }
    outer_fun[i] = funct(outer[i]);
  }
  //int difference = time.elapsed();
  //ui->textEdit->append("Время - ");
  //ui->textEdit->append(QString::number(difference));
}
void MainWindow::study(double *sample, double *answer) {
  countOuter(sample);
  double a = 0.0001;

  // Проверка на правильный вариант
  int tmp = 0;
  for (int j = 0; j < size_outer; j++ ) {
    if(answer[j] != round(this->outer_fun[j]*this->k_nn)) {
      tmp++;
    }
  }
  if (tmp == 0) return;

  // m -> n
  double err_HO[size_hidden][size_outer];
  double G_in_eh[size_hidden];
  for (int i=0; i < size_hidden; i++) G_in_eh[i] = 0;
  for (int i=0; i < size_outer; i++) {
    double tmp = answer[i];
    double G_H2O = answer[i] - outer_fun[i]*this->k_nn; //self.unfunct(self.outer[i])
    for (int j=0; j < this->size_hidden; j++) {
      G_in_eh[j] += G_H2O * this->wHO[j][i];
      err_HO[j][i] = a*a*G_H2O*hidden[j];
    }
  }

  for (int i=0; i < size_hidden; i++) {
    for (int j=0; j < size_outer; j++) {
      wHO[i][j] += err_HO[i][j];
    }
  }

  //k -> l
  double err_EH[size_input][size_hidden];
  for (int i=0; i < size_hidden; i++) {
    //double G_eh = G_in_eh[i]*unfunct(hidden[i]);
    for (int j=0; j < size_input; j++) {
      err_EH[j][i] = a*G_in_eh[i]*int(sample[j]);
    }
  }

  for (int i=0; i < size_input; i++) {
    for (int j=0; j < size_hidden; j++) {
      wEH[i][j] += err_EH[i][j];
    }
  }

  QString text;
  QString answer_text;
  for (int i=0; i < size_outer; i++) {
    answer_text.append(QString::number(int(answer[i]))+" ");
  }
  ui->textEdit->append(answer_text);
  text.append("Результат:");
  text.append("\n");
  for (int i=0; i < size_outer; i++) {
    outer_fun[i] = outer_fun[i]*this->k_nn;
    text.append(QString::number(outer_fun[i]) + " ");
  }
  text.append("\n");
  ui->textEdit->append(text);
  //}
  //return outer_fun;
}

// Прямая задача кинематики
void MainWindow::forward_kinematics_calc() {
  QTime time;
  time.start();
  double s1 = this->L[2]*cos(this->current_angle[1]*PI/180 + this->current_angle[2]*PI/180);
  double s2 = this->L[1]*cos(this->current_angle[1]*PI/180);
  double s3 = this->L[0];
  this->current_position[1] = s1 + s2 + s3;
  this->current_position[0] = this->current_position[1]*cos(this->current_angle[0]*PI/180);
  this->current_position[2] = this->current_position[1]*sin(this->current_angle[0]*PI/180);
  ui->pr_x->setText(QString::number(this->current_position[0]));
  ui->pr_y->setText(QString::number(this->current_position[1]));
  ui->pr_z->setText(QString::number(this->current_position[2]));


  //int difference = time.elapsed();
  //ui->textEdit->append("\nВремя - ");
  //ui->textEdit->append(QString::number(difference));
  //ui->textEdit->append("\n");
}

// Обратная задача кинематики
void MainWindow::ob_random_fun() {
  int ar_tmp[3][3];
  ar_tmp[0][0] = 19;
  ar_tmp[0][1] = 20;
  ar_tmp[0][2] = 5;
  ar_tmp[1][0] = 22;
  ar_tmp[1][1] = 23;
  ar_tmp[1][2] = 1;
  ar_tmp[2][0] = 16;
  ar_tmp[2][1] = 17;
  ar_tmp[2][2] = 2;
  int k = rand()%3;
  ui->ob_x->setText(QString::number(ar_tmp[k][0]));
  ui->ob_y->setText(QString::number(ar_tmp[k][1]));
  ui->ob_z->setText(QString::number(ar_tmp[k][2]));
}

void MainWindow::ob_kin_calc_fun() {
  double ar[3];
  ar[0] = this->ui->ob_x->text().toInt();
  ar[1] = this->ui->ob_y->text().toInt();
  ar[2] = this->ui->ob_z->text().toInt();
  countOuter(ar);
  ui->ob_Q1->setText(QString::number(this->outer_fun[0]*40));
  ui->ob_Q2->setText(QString::number(this->outer_fun[1]*40));
  ui->ob_Q3->setText(QString::number(this->outer_fun[2]*40));
}

// Функции активации
float MainWindow::funct(float param) {
  float res = (2/(1 + exp(-param)) - 1); //exp(-param)
  //res = round(param);
  return res;
}
float MainWindow::unfunct(float param) {
  float res = (1+param)*(1-param)*0.1;
  //res = round(param);
  return res;
}

// Общие функции
void MainWindow::getMapAction() {
  connect(ui->tread_action, SIGNAL(clicked()), this, SLOT(tread_action()) );

  connect(ui->calc, SIGNAL(clicked()), this, SLOT(calc_action()));
  connect(ui->clear_text, SIGNAL(clicked()), this, SLOT(clear_text()));
  connect(ui->pr_kin_calc, SIGNAL(clicked()), this, SLOT(forward_kinematics_click()) );
  connect(ui->pr_random, SIGNAL(clicked()), this, SLOT(forward_kinematics_random()) );
  connect(ui->nnGetStudy, SIGNAL(clicked()), this, SLOT(nnBeginStudy()) );

  connect(ui->ob_random, SIGNAL(clicked()), this, SLOT(ob_random_fun()) );
  connect(ui->ob_kin_calc, SIGNAL(clicked()), this, SLOT(ob_kin_calc_fun()) );
}

void MainWindow::tread_action() {
  //t->start();
  //t->run();
}

void MainWindow::set_text(double d) {
  this->textEdit.append(QString::number(d));
}

void MainWindow::getDefaultWeight() {
  //t = new Thread(this);
  //t->setParametrs(this->textEdit);

  L[0] = 10; L[1] = 5; L[2] = 9; L[3] = 4; L[4] = 5;
  size_hidden = 125;
  size_input = 3;
  size_outer = 3;
  for(int i=0; i < size_input; i++){
    for(int j=0; j < size_hidden; j++){
      wEH[i][j] = (double)(rand()%100-50)/100000;
    }
  }
  for(int i=0; i < size_hidden; i++){
    for(int j=0; j < size_outer; j++){
      wHO[i][j] = (double)(rand()%100-50)/100000;;
    }
  }
}
void MainWindow::setDefaultParams() {
  ui->L1->setText(QString::number(this->L[0]));
  ui->L2->setText(QString::number(this->L[1]));
  ui->L3->setText(QString::number(this->L[2]));
  ui->L4->setText(QString::number(this->L[3]));
  ui->L5->setText(QString::number(this->L[4]));
  for (int i = 0; i < length(draw_matrix); i++) {
      draw_matrix[i] = 200;
  }
}

// Рисования
void MainWindow::drawManipulator() {
  int k = 7;
  int x = 150, y = 200, s_x =0, s_y = 0;
  //z = 0; s_z = 0;
  graphicsScene->clear();
  s_x = x;
  s_y = y - this->L[0]*k;
  graphicsScene->addLine(QLineF(QPointF(x, y), QPointF(s_x, s_y)),  QPen(Qt::red, 5) );
  graphicsScene->addEllipse(s_x-5, s_y-5, 10, 10, QPen(Qt::red), QBrush(Qt::red));
  x = s_x; y = s_y;
  s_x = x + this->L[1]*sin(this->current_angle[1]*PI/180)*k;
  s_y = y - this->L[1]*cos(this->current_angle[1]*PI/180)*k;
  graphicsScene->addLine(QLineF(QPointF(x, y), QPointF(s_x, s_y)),  QPen(Qt::red, 5) );
  graphicsScene->addEllipse(s_x-5, s_y-5, 10, 10, QPen(Qt::red), QBrush(Qt::red));
  x = s_x; y = s_y;
  s_x = x + this->L[2]*sin(this->current_angle[1]*PI/180 + this->current_angle[2]*PI/180)*k;
  s_y = y - this->L[2]*cos(this->current_angle[1]*PI/180 + this->current_angle[2]*PI/180)*k;
  graphicsScene->addLine(QLineF(QPointF(x, y), QPointF(s_x, s_y)),  QPen(Qt::red, 5) );
  graphicsScene->addEllipse(s_x-5, s_y-5, 10, 10, QPen(Qt::red), QBrush(Qt::red));

  graphicsScene->update();
}

void MainWindow::drawGraph() {
  graphicsNNTeachScene->clear();
  int x=0, y=200, s_x = 2, s_y = draw_matrix[0];
  for (int i = 1; i < k_iter_graph; i++) {
    graphicsNNTeachScene->addLine(QLineF(QPointF(x, y), QPointF(s_x, s_y)),  QPen(Qt::red, 2) );
    x = s_x; y = s_y;
    s_x += 2;
    s_y = draw_matrix[i];
  }
  graphicsNNTeachScene->update();
}
