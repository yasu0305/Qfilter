#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QLabel;
class QTimer;
class QLineEdit;
class FilteredCompleter;

class MainWindow : public QMainWindow
{
  Q_OBJECT
public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

public slots:
  void updateTime();
  void onCompleterActivated(const QString &text);

private:
  QLabel *timeLabel = nullptr;
  QTimer *timer = nullptr;
  QLineEdit *lineEdit = nullptr;
  FilteredCompleter *filteredCompleter = nullptr;
};

#endif // MAINWINDOW_H
