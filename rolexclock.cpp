#include "rolexclock.h"

#include <QDate>
#include <QPainter>
#include <QTime>
#include <QTimer>

RolexClock::RolexClock(QWidget *parent) : QWidget(parent) {
  QTimer *timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, QOverload<>::of(&QWidget::update));
  timer->start(1000);

  setWindowTitle(tr("ロレックス時計"));
  resize(200, 200);
}

void RolexClock::paintEvent(QPaintEvent *) {
  int side = qMin(width(), height());
  QTime time = QTime::currentTime();

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.translate(width() / 2, height() / 2);
  painter.scale(side / 200.0, side / 200.0);

  // ロレックス風カラースキーム
  QColor champagneGold(218, 165, 32); // シャンパンゴールド
  QColor deepGold(184, 134, 11);      // 濃いゴールド
  QColor darkGreen(0, 100, 0);        // ダークグリーン（サブマリーナー風）
  QColor white(255, 255, 255);        // ホワイト
  QColor cream(250, 250, 240);        // クリーム色
  QColor blackDial(15, 15, 15);       // ブラック文字盤
  QColor silver(192, 192, 192);       // シルバー

  // 外側のゴールドベゼル
  QRadialGradient outerBezelGradient(-3, -3, 100);
  outerBezelGradient.setColorAt(0, champagneGold);
  outerBezelGradient.setColorAt(0.7, deepGold);
  outerBezelGradient.setColorAt(1, QColor(140, 100, 8));

  painter.setPen(Qt::NoPen);
  painter.setBrush(outerBezelGradient);
  painter.drawEllipse(QPoint(0, 0), 100, 100);

  // ベゼルの刻み（60分表示）
  for (int i = 0; i < 60; i++) {
    painter.save();
    painter.rotate(6.0 * i);

    if (i % 5 == 0) {
      // 5分ごとに大きな刻み
      painter.setPen(QPen(blackDial, 2));
      painter.drawLine(0, -96, 0, -92);
    }
    painter.restore();
  }

  // 内側の文字盤
  QRadialGradient dialGradient(0, -10, 90);
  dialGradient.setColorAt(0, QColor(25, 25, 25));
  dialGradient.setColorAt(1, blackDial);

  painter.setBrush(dialGradient);
  painter.setPen(QPen(champagneGold, 2));
  painter.drawEllipse(QPoint(0, 0), 88, 88);

  // ゴールドのインナーリング
  painter.setPen(QPen(champagneGold, 1.5));
  painter.setBrush(Qt::NoBrush);
  painter.drawEllipse(QPoint(0, 0), 85, 85);

  // ロレックスロゴ風の王冠（上部）
  painter.save();
  painter.setPen(Qt::NoPen);
  painter.setBrush(champagneGold);

  // 王冠の中央
  QPolygonF crown;
  crown << QPointF(-4, -50) << QPointF(4, -50) << QPointF(3, -55)
        << QPointF(-3, -55);
  painter.drawPolygon(crown);

  // 王冠の突起
  QPolygonF crownTop;
  crownTop << QPointF(-2, -55) << QPointF(2, -55) << QPointF(0, -58);
  painter.drawPolygon(crownTop);

  painter.drawEllipse(QPoint(-5, -52), 1.5, 1.5);
  painter.drawEllipse(QPoint(5, -52), 1.5, 1.5);
  painter.restore();

  // 日付窓（3時位置）
  painter.setBrush(white);
  painter.setPen(QPen(champagneGold, 1));
  painter.drawRect(64, -8, 16, 14);

  // サイクロプスレンズ風の枠
  painter.setBrush(Qt::NoBrush);
  painter.setPen(QPen(QColor(200, 200, 200, 100), 2));
  painter.drawRect(63, -9, 18, 16);

  // 日付テキスト
  painter.setPen(blackDial);
  QFont dateFont("Arial", 10, QFont::Bold);
  painter.setFont(dateFont);
  QString dateStr = QDate::currentDate().toString("dd");
  painter.drawText(QRect(64, -8, 16, 14), Qt::AlignCenter, dateStr);

  // 時間マーカー（ゴールドのバー）
  for (int i = 0; i < 12; ++i) {
    painter.save();
    painter.rotate(30.0 * i);

    if (i == 3) {
      // 3時（日付窓があるのでスキップ）
      painter.restore();
      continue;
    }

    // ゴールドのグラデーション
    QLinearGradient markerGradient(-3, -78, 3, -78);
    markerGradient.setColorAt(0, deepGold);
    markerGradient.setColorAt(0.5, champagneGold);
    markerGradient.setColorAt(1, deepGold);

    painter.setPen(Qt::NoPen);
    painter.setBrush(markerGradient);

    // 12時は少し大きく
    if (i == 0) {
      painter.drawRect(-4, -82, 8, 12);
      // 白い縁取り
      painter.setBrush(white);
      painter.drawRect(-3.5, -81.5, 7, 11);
      painter.setBrush(markerGradient);
      painter.drawRect(-3, -81, 6, 10);
    } else {
      painter.drawRect(-3, -80, 6, 10);
      // 白い縁取り
      painter.setBrush(white);
      painter.drawRect(-2.5, -79.5, 5, 9);
      painter.setBrush(markerGradient);
      painter.drawRect(-2, -79, 4, 8);
    }
    painter.restore();
  }

  // 微細な分マーカー
  painter.setPen(QPen(QColor(100, 100, 100), 0.5));
  for (int j = 0; j < 60; ++j) {
    if ((j % 5) != 0) {
      painter.save();
      painter.rotate(6.0 * j);
      painter.drawLine(0, -82, 0, -80);
      painter.restore();
    }
  }

  // 時針（ゴールド、メルセデス針風）
  painter.save();
  painter.rotate(30.0 * ((time.hour() % 12) + time.minute() / 60.0));

  // 影
  painter.setPen(Qt::NoPen);
  painter.setBrush(QColor(0, 0, 0, 80));
  QPolygonF hourHandShadow;
  hourHandShadow << QPointF(-5, 10) << QPointF(5, 10) << QPointF(3, -48)
                 << QPointF(-3, -48);
  painter.drawPolygon(hourHandShadow);

  // ゴールドベース
  QLinearGradient hourGradient(-4, 0, 4, 0);
  hourGradient.setColorAt(0, deepGold);
  hourGradient.setColorAt(0.5, champagneGold);
  hourGradient.setColorAt(1, deepGold);
  painter.setBrush(hourGradient);

  QPolygonF hourHand;
  hourHand << QPointF(-4, 8) << QPointF(4, 8) << QPointF(2.5, -46)
           << QPointF(-2.5, -46);
  painter.drawPolygon(hourHand);

  // メルセデスマーク風の先端
  painter.setBrush(white);
  QPolygonF mercedeTip;
  mercedeTip << QPointF(-2, -42) << QPointF(2, -42) << QPointF(0, -46);
  painter.drawPolygon(mercedeTip);

  painter.restore();

  // 分針（ゴールド）
  painter.save();
  painter.rotate(6.0 * (time.minute() + time.second() / 60.0));

  // 影
  painter.setPen(Qt::NoPen);
  painter.setBrush(QColor(0, 0, 0, 80));
  QPolygonF minuteHandShadow;
  minuteHandShadow << QPointF(-4, 10) << QPointF(4, 10) << QPointF(2, -72)
                   << QPointF(-2, -72);
  painter.drawPolygon(minuteHandShadow);

  // ゴールドベース
  QLinearGradient minuteGradient(-3.5, 0, 3.5, 0);
  minuteGradient.setColorAt(0, deepGold);
  minuteGradient.setColorAt(0.5, champagneGold);
  minuteGradient.setColorAt(1, deepGold);
  painter.setBrush(minuteGradient);

  QPolygonF minuteHand;
  minuteHand << QPointF(-3.5, 8) << QPointF(3.5, 8) << QPointF(2, -70)
             << QPointF(-2, -70);
  painter.drawPolygon(minuteHand);

  // 白い先端
  painter.setBrush(white);
  QPolygonF minuteTip;
  minuteTip << QPointF(-1.5, -66) << QPointF(1.5, -66) << QPointF(1, -70)
            << QPointF(-1, -70);
  painter.drawPolygon(minuteTip);

  painter.restore();

  // 秒針（ゴールド、細身）
  painter.save();
  painter.rotate(6.0 * time.second());

  // 影
  painter.setPen(QPen(QColor(0, 0, 0, 60), 1.5));
  painter.drawLine(0, 12, 0, -82);

  // ゴールドの秒針
  painter.setPen(QPen(champagneGold, 1.5));
  painter.drawLine(0, 10, 0, -80);

  // 先端のダイヤモンド型
  painter.setPen(Qt::NoPen);
  painter.setBrush(champagneGold);
  QPolygonF secondTip;
  secondTip << QPointF(0, -84) << QPointF(1.5, -80) << QPointF(0, -76)
            << QPointF(-1.5, -80);
  painter.drawPolygon(secondTip);

  // カウンターウェイト
  painter.drawEllipse(QPoint(0, 10), 4, 4);
  painter.restore();

  // 中央のキャップ（ゴールド）
  QRadialGradient centerGradient(-1, -1, 8);
  centerGradient.setColorAt(0, champagneGold);
  centerGradient.setColorAt(0.8, deepGold);
  centerGradient.setColorAt(1, QColor(140, 100, 8));

  painter.setBrush(centerGradient);
  painter.setPen(QPen(champagneGold, 1));
  painter.drawEllipse(QPoint(0, 0), 7, 7);

  // ロレックス王冠マーク（中央）
  painter.setBrush(blackDial);
  painter.setPen(Qt::NoPen);
  painter.drawEllipse(QPoint(0, 0), 4, 4);

  // 小さな王冠
  painter.setBrush(champagneGold);
  QPolygonF smallCrown;
  smallCrown << QPointF(-1.5, 0) << QPointF(1.5, 0) << QPointF(1, -2)
             << QPointF(-1, -2);
  painter.drawPolygon(smallCrown);
  painter.drawEllipse(QPoint(0, -2), 0.8, 0.8);
}
