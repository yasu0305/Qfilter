#include "grandfatherclock.h"

#include <QDate>
#include <QPainter>
#include <QTime>
#include <QTimer>
#include <QtMath>

GrandfatherClock::GrandfatherClock(QWidget *parent)
    : QWidget(parent), pendulumAngle(0), pendulumDirection(1) {
  QTimer *timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, [this]() {
    // 振り子の動き
    pendulumAngle += 0.5 * pendulumDirection;
    if (pendulumAngle > 25) {
      pendulumDirection = -1;
    } else if (pendulumAngle < -25) {
      pendulumDirection = 1;
    }
    update();
  });
  timer->start(50); // 50ms毎に更新（滑らかな動き）

  setWindowTitle(tr("置き時計"));
  resize(280, 480);
}

void GrandfatherClock::paintEvent(QPaintEvent *) {
  QTime time = QTime::currentTime();

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  // 色の定義
  QColor woodDark(101, 67, 33);      // ダークウッド
  QColor woodLight(139, 90, 43);     // ライトウッド
  QColor woodMedium(115, 74, 38);    // ミディアムウッド
  QColor brass(181, 166, 66);        // 真鍮色
  QColor brassLight(205, 190, 80);   // 明るい真鍮
  QColor brassDark(150, 137, 50);    // 暗い真鍮
  QColor cream(245, 238, 215);       // クリーム色の文字盤
  QColor black(20, 20, 20);          // 黒
  QColor gold(218, 165, 32);         // ゴールド

  // 時計のケース（縦長の箱）
  int caseWidth = 200;
  int caseHeight = 380;
  int caseX = (width() - caseWidth) / 2;
  int caseY = 20;

  // ケースの木目グラデーション
  QLinearGradient caseGradient(caseX, 0, caseX + caseWidth, 0);
  caseGradient.setColorAt(0, woodDark);
  caseGradient.setColorAt(0.3, woodMedium);
  caseGradient.setColorAt(0.5, woodLight);
  caseGradient.setColorAt(0.7, woodMedium);
  caseGradient.setColorAt(1, woodDark);

  painter.setBrush(caseGradient);
  painter.setPen(QPen(woodDark, 2));
  painter.drawRoundedRect(caseX, caseY, caseWidth, caseHeight, 10, 10);

  // 装飾的な木彫り風の縁
  painter.setPen(QPen(woodDark, 3));
  painter.setBrush(Qt::NoBrush);
  painter.drawRoundedRect(caseX + 8, caseY + 8, caseWidth - 16,
                          caseHeight - 16, 8, 8);

  // 上部の装飾
  painter.setBrush(woodDark);
  painter.drawRect(caseX + 20, caseY - 15, caseWidth - 40, 20);
  painter.drawRect(caseX + 40, caseY - 25, caseWidth - 80, 15);

  // 文字盤エリア
  int dialSize = 140;
  int dialX = width() / 2;
  int dialY = caseY + 90;

  painter.translate(dialX, dialY);

  // 文字盤の背景（真鍮の枠）
  QRadialGradient brassFrameGradient(-5, -5, dialSize / 2 + 10);
  brassFrameGradient.setColorAt(0, brassLight);
  brassFrameGradient.setColorAt(0.7, brass);
  brassFrameGradient.setColorAt(1, brassDark);

  painter.setPen(Qt::NoPen);
  painter.setBrush(brassFrameGradient);
  painter.drawEllipse(QPoint(0, 0), dialSize / 2 + 8, dialSize / 2 + 8);

  // 文字盤（クリーム色）
  QRadialGradient dialGradient(-10, -10, dialSize / 2);
  dialGradient.setColorAt(0, cream);
  dialGradient.setColorAt(0.8, QColor(235, 228, 205));
  dialGradient.setColorAt(1, QColor(215, 208, 185));

  painter.setBrush(dialGradient);
  painter.setPen(QPen(brass, 2));
  painter.drawEllipse(QPoint(0, 0), dialSize / 2, dialSize / 2);

  // ローマ数字
  painter.setPen(black);
  QFont romanFont("Times New Roman", 16, QFont::Bold);
  painter.setFont(romanFont);

  QStringList romanNumerals = {"XII", "I",  "II",  "III", "IV", "V",
                               "VI",  "VII", "VIII", "IX",  "X",  "XI"};

  for (int i = 0; i < 12; ++i) {
    painter.save();
    painter.rotate(30.0 * i);
    painter.translate(0, -dialSize / 2 + 20);
    painter.rotate(-30.0 * i);
    painter.drawText(QRect(-20, -10, 40, 20), Qt::AlignCenter,
                     romanNumerals[i]);
    painter.restore();
  }

  // 装飾的な内側の円
  painter.setPen(QPen(brass, 1));
  painter.setBrush(Qt::NoBrush);
  painter.drawEllipse(QPoint(0, 0), dialSize / 2 - 25, dialSize / 2 - 25);

  // 飾り模様（四隅）
  painter.setPen(QPen(gold, 1.5));
  for (int i = 0; i < 4; ++i) {
    painter.save();
    painter.rotate(90.0 * i + 45);
    painter.drawLine(0, dialSize / 2 - 35, 0, dialSize / 2 - 28);
    painter.drawArc(-3, dialSize / 2 - 38, 6, 6, 0, 180 * 16);
    painter.restore();
  }

  // 時針（太い真鍮色）
  painter.save();
  painter.rotate(30.0 * ((time.hour() % 12) + time.minute() / 60.0));

  QLinearGradient hourGradient(-4, 0, 4, 0);
  hourGradient.setColorAt(0, brassDark);
  hourGradient.setColorAt(0.5, brass);
  hourGradient.setColorAt(1, brassDark);

  painter.setPen(QPen(brassDark, 1));
  painter.setBrush(hourGradient);

  QPolygonF hourHand;
  hourHand << QPointF(-5, 8) << QPointF(5, 8) << QPointF(3, -35)
           << QPointF(0, -38) << QPointF(-3, -35);
  painter.drawPolygon(hourHand);

  painter.restore();

  // 分針（細長い真鍮色）
  painter.save();
  painter.rotate(6.0 * (time.minute() + time.second() / 60.0));

  QLinearGradient minuteGradient(-3, 0, 3, 0);
  minuteGradient.setColorAt(0, brassDark);
  minuteGradient.setColorAt(0.5, brass);
  minuteGradient.setColorAt(1, brassDark);

  painter.setPen(QPen(brassDark, 1));
  painter.setBrush(minuteGradient);

  QPolygonF minuteHand;
  minuteHand << QPointF(-3.5, 8) << QPointF(3.5, 8) << QPointF(2, -50)
             << QPointF(0, -53) << QPointF(-2, -50);
  painter.drawPolygon(minuteHand);

  painter.restore();

  // 秒針（細い金色）
  painter.save();
  painter.rotate(6.0 * time.second());

  painter.setPen(QPen(QColor(180, 50, 50), 1.5)); // 赤っぽい色
  painter.drawLine(0, 10, 0, -50);

  painter.setBrush(QColor(180, 50, 50));
  painter.setPen(Qt::NoPen);
  painter.drawEllipse(QPoint(0, 0), 3, 3);

  painter.restore();

  // 中央のリベット（真鍮）
  QRadialGradient centerGradient(-1, -1, 5);
  centerGradient.setColorAt(0, brassLight);
  centerGradient.setColorAt(0.7, brass);
  centerGradient.setColorAt(1, brassDark);

  painter.setBrush(centerGradient);
  painter.setPen(QPen(brassDark, 1));
  painter.drawEllipse(QPoint(0, 0), 5, 5);

  painter.resetTransform();

  // 振り子エリア（ガラス窓風）
  int pendulumWindowX = caseX + 30;
  int pendulumWindowY = dialY + 90;
  int pendulumWindowWidth = caseWidth - 60;
  int pendulumWindowHeight = 180;

  // ガラス窓の枠
  painter.setPen(QPen(brass, 3));
  painter.setBrush(QColor(200, 220, 240, 80)); // 半透明ガラス
  painter.drawRoundedRect(pendulumWindowX, pendulumWindowY,
                          pendulumWindowWidth, pendulumWindowHeight, 5, 5);

  // 窓の装飾格子
  painter.setPen(QPen(brass, 1));
  painter.drawLine(pendulumWindowX, pendulumWindowY + pendulumWindowHeight / 2,
                   pendulumWindowX + pendulumWindowWidth,
                   pendulumWindowY + pendulumWindowHeight / 2);

  // 振り子の支点
  int pendulumPivotX = width() / 2;
  int pendulumPivotY = pendulumWindowY + 20;

  // 振り子の棒
  painter.save();
  painter.translate(pendulumPivotX, pendulumPivotY);
  painter.rotate(pendulumAngle);

  // 振り子の棒（真鍮の棒）
  painter.setPen(QPen(brass, 3));
  painter.drawLine(0, 0, 0, 120);

  // 振り子の重り（真鍮の円盤）
  painter.translate(0, 120);

  QRadialGradient pendulumGradient(-3, -3, 25);
  pendulumGradient.setColorAt(0, brassLight);
  pendulumGradient.setColorAt(0.5, brass);
  pendulumGradient.setColorAt(1, brassDark);

  painter.setBrush(pendulumGradient);
  painter.setPen(QPen(brassDark, 2));
  painter.drawEllipse(QPoint(0, 0), 22, 22);

  // 重りの装飾
  painter.setPen(QPen(brassDark, 1));
  painter.drawEllipse(QPoint(0, 0), 15, 15);
  painter.drawEllipse(QPoint(0, 0), 8, 8);

  painter.restore();

  // 支点の装飾
  painter.setBrush(brass);
  painter.setPen(QPen(brassDark, 1));
  painter.drawEllipse(QPoint(pendulumPivotX, pendulumPivotY), 5, 5);

  // 底部の装飾
  painter.setBrush(woodDark);
  painter.setPen(QPen(woodDark, 2));
  painter.drawRect(caseX + 10, caseY + caseHeight - 5, caseWidth - 20, 8);
}
