#ifndef OTHELLO_H
#define OTHELLO_H

#include <QWidget>
#include <QVector>

class Othello : public QWidget {
  Q_OBJECT

public:
  Othello(QWidget *parent = nullptr);

protected:
  void paintEvent(QPaintEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;

private:
  enum Cell { Empty = 0, Black = 1, White = 2 };
  
  QVector<QVector<Cell>> board;
  Cell currentPlayer;
  int cellSize;
  int boardSize;
  int blackScore;
  int whiteScore;
  bool gameOver;

  void initBoard();
  bool isValidMove(int row, int col, Cell player) const;
  QVector<QPair<int, int>> getFlipPositions(int row, int col, Cell player) const;
  void makeMove(int row, int col);
  void flipPieces(int row, int col);
  bool hasValidMoves(Cell player) const;
  void calculateScore();
  void checkGameOver();
  void drawBoard(QPainter &painter);
  void drawPiece(QPainter &painter, int row, int col, Cell piece);
  void drawValidMoves(QPainter &painter);
  void drawGameInfo(QPainter &painter);
};

#endif // OTHELLO_H
