#ifndef TREAD_H
#define TREAD_H

#include <QWidget>
#include <QThread>
#include <QTextEdit>
#include <QMainWindow>

class Thread : public QThread {
  Q_OBJECT
  public:
    Thread(QMainWindow* parent);
    QTextEdit text;
    QMainWindow* tmp;
    virtual void run();
};
#endif
