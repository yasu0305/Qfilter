#include "mainwindow.h"

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

#include "filteredcompleter.h"
#include "filterutils.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), timeLabel(new QLabel(this)), timer(new QTimer(this)) {
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

  // fonts
  QFont f = QApplication::font();
  f.setPointSize(qMax(10, f.pointSize()));

  QFont timeFont = f;
  timeFont.setPointSize(qMax(14, f.pointSize() + 4));
  timeLabel->setFont(timeFont);

  // ensure the label won't be clipped when resizing: set minimum size from font metrics
  QFontMetrics tfm(timeFont);
  const QString sample = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
  const int timeW = tfm.horizontalAdvance(sample) + 20; // padding
  const int timeH = tfm.height() + 8;
  timeLabel->setMinimumWidth(timeW);
  timeLabel->setMinimumHeight(timeH);
  timeLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

  lineEdit->setFont(f);

  // Add stretch below the line edit to push everything to the top
  mainLay->addStretch();

  setCentralWidget(w);
  setWindowTitle(QStringLiteral("filter"));
  setFixedSize(600, 400);

  connect(timer, &QTimer::timeout, this, &MainWindow::updateTime);
  timer->start(1000);
  updateTime();
}

MainWindow::~MainWindow() {}

void MainWindow::updateTime() {
  timeLabel->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
}

void MainWindow::onCompleterActivated(const QString &text) {
  qDebug() << "Completer selected:" << text;
}

