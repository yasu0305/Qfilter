#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "analogclock.h"
#include "rolexclock.h"
#include "grandfatherclock.h"
#include "gshockdigital.h"
#include "othello.h"
#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void updateTime();

private:
  Ui::MainWindow *ui;
  class QLabel *timeLabel;
  class QTimer *timer;
};
#endif // MAINWINDOW_H
