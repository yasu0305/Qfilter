#include "mainwindow.h"
#include "analogclock.h"
#include "rolexclock.h"
#include "grandfatherclock.h"
#include "gshockdigital.h"
#include "othello.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QLabel>
#include <QTimer>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  timeLabel = new QLabel(this);
  timeLabel->move(10, 10);
  timeLabel->resize(200, 30);

  AnalogClock *gshockClock = new AnalogClock(this);
  gshockClock->move(50, 50);
  gshockClock->show();

  RolexClock *rolexClock = new RolexClock(this);
  rolexClock->move(270, 50);
  rolexClock->show();

  GrandfatherClock *grandfatherClock = new GrandfatherClock(this);
  grandfatherClock->move(490, 50);
  grandfatherClock->show();

  GShockDigital *gshockDigital = new GShockDigital(this);
  gshockDigital->move(50, 280);
  gshockDigital->show();

  Othello *othello = new Othello(this);
  othello->move(350, 280);
  othello->show();

  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &MainWindow::updateTime);
  timer->start(1000);
  updateTime();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::updateTime() {
  timeLabel->setText(
      QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
}
