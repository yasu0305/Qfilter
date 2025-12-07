#ifndef GRANDFATHERCLOCK_H
#define GRANDFATHERCLOCK_H

#include <QWidget>

class GrandfatherClock : public QWidget {
  Q_OBJECT

public:
  GrandfatherClock(QWidget *parent = nullptr);

protected:
  void paintEvent(QPaintEvent *event) override;

private:
  double pendulumAngle;
  int pendulumDirection;
};

#endif // GRANDFATHERCLOCK_H
