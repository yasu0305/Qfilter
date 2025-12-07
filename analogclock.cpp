#include "analogclock.h"

#include <QPainter>
#include <QTime>
#include <QTimer>

AnalogClock::AnalogClock(QWidget *parent) : QWidget(parent) {
  QTimer *timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, QOverload<>::of(&QWidget::update));
  timer->start(1000);

  setWindowTitle(tr("アナログ時計"));
  resize(200, 200);
}

void AnalogClock::paintEvent(QPaintEvent *) {
  int side = qMin(width(), height());
  QTime time = QTime::currentTime();

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.translate(width() / 2, height() / 2);
  painter.scale(side / 200.0, side / 200.0);

  // G-SHOCK風カラースキーム
  QColor backgroundColor(20, 20, 20); // マットブラック
  QColor bezelColor(40, 40, 40);      // ダークグレー
  QColor accentOrange(255, 100, 0);   // ネオンオレンジ
  QColor accentLime(180, 255, 0);     // ライムグリーン
  QColor metallic(160, 160, 170);     // メタリックグレー
  QColor textColor(200, 200, 200);    // ライトグレー

  // 外側のベゼル（多層構造）
  painter.setPen(Qt::NoPen);
  painter.setBrush(QColor(60, 60, 60));
  painter.drawEllipse(QPoint(0, 0), 100, 100);

  // ベゼルの立体感
  QRadialGradient bezelGradient(-5, -5, 100);
  bezelGradient.setColorAt(0, QColor(70, 70, 70));
  bezelGradient.setColorAt(0.8, bezelColor);
  bezelGradient.setColorAt(1, QColor(25, 25, 25));
  painter.setBrush(bezelGradient);
  painter.drawEllipse(QPoint(0, 0), 96, 96);

  // 内側のベゼルリング（オレンジアクセント）
  painter.setPen(QPen(accentOrange, 3));
  painter.setBrush(Qt::NoBrush);
  painter.drawEllipse(QPoint(0, 0), 92, 92);

  // 文字盤背景
  QRadialGradient faceGradient(0, 0, 90);
  faceGradient.setColorAt(0, QColor(35, 35, 35));
  faceGradient.setColorAt(1, backgroundColor);
  painter.setBrush(faceGradient);
  painter.setPen(Qt::NoPen);
  painter.drawEllipse(QPoint(0, 0), 88, 88);

  // デジタル時刻表示エリア（上部）
  painter.setBrush(QColor(15, 15, 15));
  painter.setPen(QPen(accentOrange, 1));
  painter.drawRect(-35, -25, 70, 16);

  // デジタル時刻テキスト
  painter.setPen(accentLime);
  QFont digitalFont("Arial", 10, QFont::Bold);
  painter.setFont(digitalFont);
  QString digitalTime = time.toString("hh:mm:ss");
  painter.drawText(QRect(-35, -25, 70, 16), Qt::AlignCenter, digitalTime);

  // メインの時間マーカー（G-SHOCKスタイル）
  for (int i = 0; i < 12; ++i) {
    painter.save();
    painter.rotate(30.0 * i);

    if (i % 3 == 0) {
      // 3,6,9,12時 - 立体的なバー
      // 影
      painter.setPen(Qt::NoPen);
      painter.setBrush(QColor(0, 0, 0, 100));
      painter.drawRect(-4.5, -81, 9, 20);

      // メタリックグラデーション
      QLinearGradient barGradient(-4, -80, 4, -80);
      barGradient.setColorAt(0, QColor(100, 100, 110));
      barGradient.setColorAt(0.5, metallic);
      barGradient.setColorAt(1, QColor(100, 100, 110));
      painter.setBrush(barGradient);
      painter.drawRect(-4, -80, 8, 18);

      // オレンジのハイライト
      painter.setBrush(accentOrange);
      painter.drawRect(-4, -80, 8, 3);
    } else {
      // その他の時間マーカー
      painter.setPen(QPen(metallic, 3));
      painter.drawLine(0, -78, 0, -70);

      // ライムグリーンの小さなドット
      painter.setPen(Qt::NoPen);
      painter.setBrush(accentLime);
      painter.drawEllipse(QPoint(0, -74), 2, 2);
    }
    painter.restore();
  }

  // 分マーカー
  painter.setPen(QPen(QColor(60, 60, 60), 1));
  for (int j = 0; j < 60; ++j) {
    if ((j % 5) != 0) {
      painter.drawLine(0, -84, 0, -82);
    }
    painter.rotate(6.0);
  }

  // 時針（G-SHOCKスタイル - 太く立体的）
  painter.save();
  painter.rotate(30.0 * ((time.hour() % 12) + time.minute() / 60.0));

  // 影
  painter.setPen(Qt::NoPen);
  painter.setBrush(QColor(0, 0, 0, 120));
  QPolygonF hourHandShadow;
  hourHandShadow << QPointF(-7, 8) << QPointF(7, 8) << QPointF(5, -46)
                 << QPointF(-5, -46);
  painter.drawPolygon(hourHandShadow);

  // ベース（黒）
  painter.setBrush(QColor(30, 30, 30));
  QPolygonF hourHandBase;
  hourHandBase << QPointF(-6, 8) << QPointF(6, 8) << QPointF(4, -45)
               << QPointF(-4, -45);
  painter.drawPolygon(hourHandBase);

  // メタリックハイライト
  QLinearGradient hourGradient(-6, 0, 6, 0);
  hourGradient.setColorAt(0, QColor(80, 80, 90));
  hourGradient.setColorAt(0.5, metallic);
  hourGradient.setColorAt(1, QColor(80, 80, 90));
  painter.setBrush(hourGradient);
  QPolygonF hourHand;
  hourHand << QPointF(-5, 8) << QPointF(5, 8) << QPointF(3, -44)
           << QPointF(-3, -44);
  painter.drawPolygon(hourHand);

  // オレンジのアクセント
  painter.setBrush(accentOrange);
  QPolygonF hourAccent;
  hourAccent << QPointF(-2, -38) << QPointF(2, -38) << QPointF(1.5, -44)
             << QPointF(-1.5, -44);
  painter.drawPolygon(hourAccent);
  painter.restore();

  // 分針（G-SHOCKスタイル）
  painter.save();
  painter.rotate(6.0 * (time.minute() + time.second() / 60.0));

  // 影
  painter.setPen(Qt::NoPen);
  painter.setBrush(QColor(0, 0, 0, 120));
  QPolygonF minuteHandShadow;
  minuteHandShadow << QPointF(-5, 8) << QPointF(5, 8) << QPointF(3, -68)
                   << QPointF(-3, -68);
  painter.drawPolygon(minuteHandShadow);

  // ベース
  painter.setBrush(QColor(30, 30, 30));
  QPolygonF minuteHandBase;
  minuteHandBase << QPointF(-4.5, 8) << QPointF(4.5, 8) << QPointF(2.5, -67)
                 << QPointF(-2.5, -67);
  painter.drawPolygon(minuteHandBase);

  // メタリックハイライト
  QLinearGradient minuteGradient(-4, 0, 4, 0);
  minuteGradient.setColorAt(0, QColor(100, 100, 110));
  minuteGradient.setColorAt(0.5, metallic);
  minuteGradient.setColorAt(1, QColor(100, 100, 110));
  painter.setBrush(minuteGradient);
  QPolygonF minuteHand;
  minuteHand << QPointF(-4, 8) << QPointF(4, 8) << QPointF(2, -66)
             << QPointF(-2, -66);
  painter.drawPolygon(minuteHand);

  // ライムグリーンのアクセント
  painter.setBrush(accentLime);
  QPolygonF minuteAccent;
  minuteAccent << QPointF(-1.5, -60) << QPointF(1.5, -60) << QPointF(1, -66)
               << QPointF(-1, -66);
  painter.drawPolygon(minuteAccent);
  painter.restore();

  // 秒針（ネオンオレンジ）
  painter.save();
  painter.rotate(6.0 * time.second());

  // 影
  painter.setPen(QPen(QColor(0, 0, 0, 100), 2));
  painter.drawLine(0, 12, 0, -84);

  // メイン秒針（オレンジ）
  painter.setPen(QPen(accentOrange, 2.5));
  painter.drawLine(0, 10, 0, -82);

  // 先端のライムグリーンドット
  painter.setPen(Qt::NoPen);
  painter.setBrush(accentLime);
  painter.drawEllipse(QPoint(0, -82), 4, 4);

  // 反対側のカウンターウェイト
  painter.setBrush(accentOrange);
  painter.drawEllipse(QPoint(0, 10), 5, 5);
  painter.restore();

  // 中央のビス（G-SHOCKスタイル）
  // 外側のリング
  QRadialGradient centerGradient(0, -1, 12);
  centerGradient.setColorAt(0, QColor(100, 100, 110));
  centerGradient.setColorAt(0.7, QColor(60, 60, 70));
  centerGradient.setColorAt(1, QColor(30, 30, 30));

  painter.setBrush(centerGradient);
  painter.setPen(QPen(QColor(80, 80, 90), 1));
  painter.drawEllipse(QPoint(0, 0), 10, 10);

  // 内側のネジ穴風デザイン
  painter.setBrush(QColor(20, 20, 20));
  painter.setPen(Qt::NoPen);
  painter.drawEllipse(QPoint(0, 0), 6, 6);

  // 十字のネジ溝
  painter.setPen(QPen(QColor(40, 40, 40), 2));
  painter.drawLine(-4, 0, 4, 0);
  painter.drawLine(0, -4, 0, 4);

  // オレンジのハイライト
  painter.setPen(QPen(accentOrange, 0.5));
  painter.setBrush(Qt::NoBrush);
  painter.drawEllipse(QPoint(-1, -1), 8, 8);
}
