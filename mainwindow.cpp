#include <QStringList>
#include <QString>
#include <QRandomGenerator>
// ...existing code...

QStringList generateRandomStrings(int count, int length)
{
  const QString pool = "abcdefghijklmnopqrstuvwxyz0123456789_/";
  const int poolSize = pool.size();
  QStringList out;
  out.reserve(count);
  for (int i = 0; i < count; ++i)
  {
    QString s;
    s.resize(length);
    for (int j = 0; j < length; ++j)
    {
      int idx = QRandomGenerator::global()->bounded(poolSize);
      s[j] = pool.at(idx);
    }
    out.append(s);
  }
  return out;
}

#include "mainwindow.h"
#include "filteredcompleter.h"
#include <QDateTime>
#include <QLabel>
#include <QTimer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QFont>
#include <QApplication>
#include <QFontMetrics>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      timeLabel(new QLabel(this)),
      timer(new QTimer(this)),
      lineEdit(nullptr),
      filteredCompleter(nullptr)
{
  QWidget *w = new QWidget(this);
  auto *mainLay = new QVBoxLayout(w);
  mainLay->setSpacing(0);
  mainLay->setContentsMargins(8, 8, 8, 8);

  // Top row: time label aligned to the right
  auto *topLay = new QHBoxLayout();
  topLay->setContentsMargins(0, 0, 0, 0);
  topLay->addStretch();
  topLay->addWidget(timeLabel, 0, Qt::AlignRight | Qt::AlignTop);
  mainLay->addLayout(topLay);

  // Line edit below, spanning full width (including under the time label)
  lineEdit = new QLineEdit(this);
  lineEdit->setPlaceholderText(QStringLiteral("Enter text..."));
  lineEdit->setAttribute(Qt::WA_InputMethodEnabled, true);
  mainLay->addWidget(lineEdit);

  // setup filtered completer with 10000 random 20-char candidates
  filteredCompleter = new FilteredCompleter(this);
  const QStringList candidates = ::generateRandomStrings(10000, 20);
  filteredCompleter->setCandidates(candidates);
  filteredCompleter->attachTo(lineEdit);
  connect(filteredCompleter, &FilteredCompleter::selectionConfirmed, this, &MainWindow::onCompleterActivated);

  QFont timeFont = QApplication::font();
  timeFont.setPointSize(14);
  timeLabel->setFont(timeFont);
  QFontMetrics tfm(timeFont);
  const QString sample = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
  const int timeW = tfm.horizontalAdvance(sample) + 20; // padding
  const int timeH = tfm.height() + 8;
  timeLabel->setMinimumWidth(timeW);
  timeLabel->setMinimumHeight(timeH);
  timeLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

  mainLay->addStretch();

  setCentralWidget(w);
  setWindowTitle(QStringLiteral("filter"));
  setFixedSize(600, 400);

  connect(timer, &QTimer::timeout, this, &MainWindow::updateTime);
  timer->start(1000);
  updateTime();
}

MainWindow::~MainWindow() {}

void MainWindow::updateTime()
{
  timeLabel->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
}

void MainWindow::onCompleterActivated(const QString &text)
{
  qDebug() << "Completer selected:" << text;
}
