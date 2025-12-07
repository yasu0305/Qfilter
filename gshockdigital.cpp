#include "gshockdigital.h"

#include <QDate>
#include <QPainter>
#include <QTime>
#include <QTimer>

GShockDigital::GShockDigital(QWidget *parent) : QWidget(parent) {
  QTimer *timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, QOverload<>::of(&QWidget::update));
  timer->start(1000);

  setWindowTitle(tr("G-SHOCK デジタル"));
  resize(280, 200);
}

void GShockDigital::paintEvent(QPaintEvent *) {
  QTime time = QTime::currentTime();
  QDate date = QDate::currentDate();

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  // G-SHOCK風カラー
  QColor blackCase(25, 25, 25);
  QColor darkGray(40, 40, 40);
  QColor mediumGray(60, 60, 60);
  QColor lightGray(180, 180, 180);
  QColor redAccent(220, 20, 60);
  QColor yellowAccent(255, 215, 0);
  QColor displayGreen(180, 255, 180);
  QColor displayBg(45, 50, 45);

  int watchWidth = width();
  int watchHeight = height();

  // 外側のケース（黒）
  painter.setBrush(blackCase);
  painter.setPen(Qt::NoPen);
  painter.drawRoundedRect(0, 0, watchWidth, watchHeight, 15, 15);

  // ベゼルの装飾ライン
  painter.setPen(QPen(mediumGray, 2));
  painter.setBrush(Qt::NoBrush);
  painter.drawRoundedRect(8, 8, watchWidth - 16, watchHeight - 16, 10, 10);

  // 上部の装飾バンド
  painter.setBrush(darkGray);
  painter.drawRect(0, 15, watchWidth, 8);
  painter.drawRect(0, watchHeight - 23, watchWidth, 8);

  // G-SHOCKロゴ（上部）
  painter.setPen(redAccent);
  QFont logoFont("Arial", 10, QFont::Bold);
  painter.setFont(logoFont);
  painter.drawText(QRect(20, 25, 80, 15), Qt::AlignLeft | Qt::AlignVCenter,
                   "G-SHOCK");

  // PROTECTION表示
  painter.setPen(lightGray);
  QFont smallFont("Arial", 6);
  painter.setFont(smallFont);
  painter.drawText(QRect(watchWidth - 100, 25, 80, 15),
                   Qt::AlignRight | Qt::AlignVCenter, "SHOCK RESIST");

  // メインディスプレイエリア
  int displayX = 20;
  int displayY = 50;
  int displayWidth = watchWidth - 40;
  int displayHeight = 100;

  // ディスプレイの背景
  painter.setBrush(displayBg);
  painter.setPen(QPen(mediumGray, 1));
  painter.drawRoundedRect(displayX, displayY, displayWidth, displayHeight, 5,
                          5);

  // 内側の装飾枠
  painter.setPen(QPen(darkGray, 1));
  painter.setBrush(Qt::NoBrush);
  painter.drawRoundedRect(displayX + 3, displayY + 3, displayWidth - 6,
                          displayHeight - 6, 3, 3);

  // 曜日と日付表示（上部）
  painter.setPen(displayGreen);
  QFont dayFont("Courier", 9, QFont::Bold);
  painter.setFont(dayFont);

  QString dayOfWeek = date.toString("ddd").toUpper();
  painter.drawText(QRect(displayX + 10, displayY + 8, 60, 15),
                   Qt::AlignLeft | Qt::AlignVCenter, dayOfWeek);

  // 月と日
  QString dateStr = date.toString("MM-dd");
  painter.drawText(QRect(displayX + displayWidth - 70, displayY + 8, 60, 15),
                   Qt::AlignRight | Qt::AlignVCenter, dateStr);

  // メイン時刻表示（中央大きく）
  QFont timeFont("Courier", 32, QFont::Bold);
  painter.setFont(timeFont);

  QString timeStr = time.toString("hh:mm");
  painter.drawText(QRect(displayX + 5, displayY + 25, displayWidth - 10, 45),
                   Qt::AlignCenter, timeStr);

  // 秒表示（右下小さく）
  QFont secFont("Courier", 16, QFont::Bold);
  painter.setFont(secFont);
  QString secStr = time.toString("ss");
  painter.drawText(
      QRect(displayX + displayWidth - 50, displayY + 65, 40, 25),
      Qt::AlignRight | Qt::AlignVCenter, secStr);

  // AM/PM表示（左下）
  painter.setFont(dayFont);
  QString ampm = (time.hour() < 12) ? "AM" : "PM";
  painter.drawText(QRect(displayX + 10, displayY + 72, 40, 20),
                   Qt::AlignLeft | Qt::AlignVCenter, ampm);

  // 機能表示アイコン（下部）
  painter.setPen(yellowAccent);
  QFont iconFont("Arial", 7);
  painter.setFont(iconFont);

  // 各種機能表示
  int iconY = displayY + displayHeight + 10;
  painter.drawText(QRect(30, iconY, 40, 12), Qt::AlignCenter, "LIGHT");
  painter.drawText(QRect(90, iconY, 40, 12), Qt::AlignCenter, "ALARM");
  painter.drawText(QRect(150, iconY, 50, 12), Qt::AlignCenter, "24H");

  // ボタン表示（左右）
  painter.setBrush(darkGray);
  painter.setPen(QPen(mediumGray, 1));

  // 左側ボタン
  painter.drawRect(2, 70, 8, 15);
  painter.drawRect(2, 95, 8, 15);

  // 右側ボタン
  painter.drawRect(watchWidth - 10, 70, 8, 15);
  painter.drawRect(watchWidth - 10, 95, 8, 15);

  // ボタンラベル
  painter.setPen(redAccent);
  painter.setFont(QFont("Arial", 5, QFont::Bold));
  painter.save();
  painter.translate(5, 77);
  painter.rotate(-90);
  painter.drawText(QRect(-10, -3, 20, 6), Qt::AlignCenter, "ADJ");
  painter.restore();

  painter.save();
  painter.translate(5, 102);
  painter.rotate(-90);
  painter.drawText(QRect(-10, -3, 20, 6), Qt::AlignCenter, "MODE");
  painter.restore();

  painter.setPen(yellowAccent);
  painter.save();
  painter.translate(watchWidth - 5, 77);
  painter.rotate(90);
  painter.drawText(QRect(-10, -3, 20, 6), Qt::AlignCenter, "START");
  painter.restore();

  painter.save();
  painter.translate(watchWidth - 5, 102);
  painter.rotate(90);
  painter.drawText(QRect(-10, -3, 20, 6), Qt::AlignCenter, "RESET");
  painter.restore();

  // 装飾的な螺子（四隅）
  painter.setPen(Qt::NoPen);
  painter.setBrush(mediumGray);
  painter.drawEllipse(QPoint(15, 15), 3, 3);
  painter.drawEllipse(QPoint(watchWidth - 15, 15), 3, 3);
  painter.drawEllipse(QPoint(15, watchHeight - 15), 3, 3);
  painter.drawEllipse(QPoint(watchWidth - 15, watchHeight - 15), 3, 3);

  // 螺子の十字
  painter.setPen(QPen(darkGray, 1));
  for (int x : {15, watchWidth - 15}) {
    for (int y : {15, watchHeight - 15}) {
      painter.drawLine(x - 2, y, x + 2, y);
      painter.drawLine(x, y - 2, x, y + 2);
    }
  }
}
