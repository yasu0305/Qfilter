#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QCompleter>
#include <QStringListModel>
#include <QLineEdit>
#include <QLabel>
#include <QTimer>

// 単純化: 標準の QLineEdit を使用します（CustomLineEdit を削除）

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  bool isUserInput = true;  // CustomLineEditからアクセス可能

public slots:
  void updateTime();
  void onLineEditTextChanged(const QString &text);
  void onCompleterActivated(const QString &text);
  void onCompleterHighlighted(const QString &text);
  void onCompleterActivated(const QString &text);

private:
  QStringList generateRandomStrings();
  QLabel *timeLabel;
  QTimer *timer;
  QLineEdit *lineEdit;
  QStringList randomStrings;
  QStringListModel *completerModel;
  QCompleter *completer;
};
#endif // MAINWINDOW_H
