#ifndef GSHOCKDIGITAL_H
#define GSHOCKDIGITAL_H

#include <QWidget>

class GShockDigital : public QWidget {
  Q_OBJECT

public:
  GShockDigital(QWidget *parent = nullptr);

protected:
  void paintEvent(QPaintEvent *event) override;
};

#endif // GSHOCKDIGITAL_H
