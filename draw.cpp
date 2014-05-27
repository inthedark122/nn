/*
void MainWindow::drawManipulator() {
  int k = 7;
  int x = 150, y = 200, z = 0, s_x =0, s_y = 0, s_z = 0;
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
*/
