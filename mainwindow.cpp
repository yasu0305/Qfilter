#include "mainwindow.h"
#include <QDateTime>
#include <QLabel>
#include <QTimer>
#include <QRandomGenerator>
#include <QStringList>
#include <QDebug>
#include <QCompleter>
#include <QStringListModel>
#include <QLineEdit>
#include <chrono>
#include <QKeyEvent>
#include <QWidget>
#include <QVBoxLayout>
#include <QApplication>
#include <QAbstractItemView>
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
  
  // スペーサーを追加して下に余白を作る
  mainLayout->addStretch();
  
  // ランダムな20文字の文字列を生成
  randomStrings = generateRandomStrings();
  
  // 生成した文字列をqDebugで改行表示（デバッグ用）
  // 10000個の場合はコメントアウト
  // qDebug() << "Generated random strings:";
  // for (int i = 0; i < randomStrings.size(); ++i) {
  //   qDebug() << QString("[%1] %2").arg(i).arg(randomStrings[i]);
  // }
  qDebug() << "Total:" << randomStrings.size() << "strings generated";
  
  // QLineEditにコンプリーター機能を追加
  completerModel = new QStringListModel(randomStrings, this);
  completer = new QCompleter(completerModel, this);
  completer->setCaseSensitivity(Qt::CaseInsensitive);
  // PopupCompletion にして、矢印で選択しても即時編集されないようにする
  completer->setCompletionMode(QCompleter::PopupCompletion);
  completer->setWidget(lineEdit);
  // activated はマウスクリック／Enterでの確定を受け取る
  
  // QLineEditのテキスト変更シグナルを接続
  connect(lineEdit, &QLineEdit::textChanged, this, &MainWindow::onLineEditTextChanged);
    // マウスで候補をクリックしたときに確定するためのシグナル接続
      connect(completer, QOverload<const QString &>::of(&QCompleter::activated),
        this, &MainWindow::onCompleterActivated);
      // highlighted は選択ハイライト時に発生する（矢印キー移動時）。
      // ここで何もしないことでハイライトによる即時確定を防ぐ。
      connect(completer, QOverload<const QString &>::of(&QCompleter::highlighted),
        this, &MainWindow::onCompleterHighlighted);
  
  // タイマーを開始して時刻を更新
  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &MainWindow::updateTime);
  timer->start(1000);
  updateTime();
}

void MainWindow::onCompleterActivated(const QString &text) {
  if (text.isEmpty()) return;
  // QLineEditのtextChangedを再度トリガーしないようにブロックして設定
  if (lineEdit) {
    lineEdit->blockSignals(true);
    lineEdit->setText(text);
    lineEdit->blockSignals(false);
  }
  if (completer && completer->popup()) completer->popup()->hide();
}

void MainWindow::onCompleterHighlighted(const QString & /*text*/) {
  // intentionally empty: prevent highlighted items from immediately modifying the QLineEdit
}

MainWindow::~MainWindow() {}

void MainWindow::updateTime() {
  QString timeStr = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
  timeLabel->setText(timeStr);
}

void MainWindow::onLineEditTextChanged(const QString &text) {
  // テキストが入力されたときに、マッチするものだけをフィルタリング
  QStringList filteredList;
  
  if (text.isEmpty()) {
    // 空の場合は全て表示
    filteredList = randomStrings;
  } else {
    // テキストが含まれるものだけをフィルタリング
    for (const QString &str : randomStrings) {
      if (str.contains(text, Qt::CaseInsensitive)) {
        filteredList.append(str);
      }
    }
  }
  
  // モデルを更新
  completerModel->setStringList(filteredList);
  
  // テキストが入力されている場合、ポップアップを表示
  if (!text.isEmpty() && !filteredList.isEmpty()) {
    completer->setCompletionPrefix(text);
    completer->complete();
  }
}
