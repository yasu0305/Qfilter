#include "mainwindow.h"
#include <QDateTime>
#include <QLabel>
#include <QTimer>
#include <QRandomGenerator>
#include <QStringList>
#include <QDebug>
#include "filteredcompleter.h"
#include <QLineEdit>
#include <QWidget>
#include <QVBoxLayout>
#include <QApplication>
#include <QScreen>

// 単純化: カスタム keyPressEvent の実装を削除し、標準の QLineEdit を使用します

QStringList MainWindow::generateRandomStrings() {
  QStringList result;
  const QString characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()_+-=[]{}|;:',.<>?/";
  QRandomGenerator generator(QRandomGenerator::securelySeeded());
  
  for (int i = 0; i < 9999; ++i) {
    QString randomString;
    for (int j = 0; j < 20; ++j) {
      int index = generator.bounded(characters.length());
      randomString.append(characters[index]);
    }
    result.append(randomString);
  }
  
  // 特定の文字列を追加
  result.append("sr_mon_dcct/current");
  
  return result;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
  setWindowTitle("Main Window");
  setGeometry(100, 100, 600, 400);
  
  // 中央ウィジェットを作成
  QWidget *centralWidget = new QWidget(this);
  setCentralWidget(centralWidget);
  
  // メインレイアウトを作成
  QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
  mainLayout->setContentsMargins(20, 20, 20, 20);
  mainLayout->setSpacing(10);
  
  // トップレイアウト（ラベルと時刻）
  QHBoxLayout *topLayout = new QHBoxLayout();
  QLabel *label = new QLabel("Enter text:");
  label->setStyleSheet("font-size: 12px; font-weight: bold;");
  topLayout->addWidget(label);
  
  // 時刻ラベル
  timeLabel = new QLabel(this);
  timeLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  timeLabel->setStyleSheet("color: black; padding: 5px 10px; font-size: 16px; font-weight: bold;");
  topLayout->addWidget(timeLabel);
  mainLayout->addLayout(topLayout);
  
  // 標準の QLineEdit を使用
  lineEdit = new QLineEdit(this);
  lineEdit->setPlaceholderText("Type here...");
  lineEdit->setStyleSheet("font-size: 11px;");
  mainLayout->addWidget(lineEdit);

  // 追加の QLineEdit を作成（同じ候補で独立して動作）
  lineEdit2 = new QLineEdit(this);
  lineEdit2->setPlaceholderText("Another input...");
  lineEdit2->setStyleSheet("font-size: 11px;");
  mainLayout->addWidget(lineEdit2);
  
  // スペーサーを追加して下に余白を作る
  mainLayout->addStretch();
  
  // ランダムな20文字の文字列を生成
  randomStrings = generateRandomStrings();
  
  // FilteredCompleter を作成して接続（1つ目）
  filteredCompleter = new FilteredCompleter(this);
  filteredCompleter->setCandidates(randomStrings);
  filteredCompleter->attachTo(lineEdit);
  filteredCompleter->setCaseSensitivity(Qt::CaseInsensitive);
  filteredCompleter->setCompletionMode(QCompleter::UnfilteredPopupCompletion);

  // 2つ目の QLineEdit 用の FilteredCompleter を作成して接続（独立）
  filteredCompleter2 = new FilteredCompleter(this);
  filteredCompleter2->setCandidates(randomStrings);
  filteredCompleter2->attachTo(lineEdit2);
  filteredCompleter2->setCaseSensitivity(Qt::CaseInsensitive);
  filteredCompleter2->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
  
  // タイマーを開始して時刻を更新
  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &MainWindow::updateTime);
  timer->start(1000);
  updateTime();
}

void MainWindow::onCompleterActivated(const QString &text) {
  if (text.isEmpty()) return;
  if (lineEdit) {
    lineEdit->blockSignals(true);
    lineEdit->setText(text);
    lineEdit->blockSignals(false);
  }
}

MainWindow::~MainWindow() {}

void MainWindow::updateTime() {
  QString timeStr = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
  timeLabel->setText(timeStr);
}

// filtering is handled by FilteredCompleter; no local implementation needed
