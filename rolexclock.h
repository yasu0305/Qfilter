#ifndef ROLEXCLOCK_H
#define ROLEXCLOCK_H

#include <QWidget>

class RolexClock : public QWidget {
  Q_OBJECT

public:
  RolexClock(QWidget *parent = nullptr);

protected:
  void paintEvent(QPaintEvent *event) override;
};

#endif // ROLEXCLOCK_H
