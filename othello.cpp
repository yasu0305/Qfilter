#include "othello.h"

#include <QMouseEvent>
#include <QPainter>

Othello::Othello(QWidget *parent)
    : QWidget(parent), boardSize(8), cellSize(50), currentPlayer(Black),
      gameOver(false) {
  initBoard();
  setWindowTitle(tr("Othello Game"));
  resize(cellSize * boardSize + 20, cellSize * boardSize + 120);
}

void Othello::initBoard() {
  board = QVector<QVector<Cell>>(boardSize, QVector<Cell>(boardSize, Empty));
  
  // 初期配置
  int mid = boardSize / 2;
  board[mid - 1][mid - 1] = White;
  board[mid - 1][mid] = Black;
  board[mid][mid - 1] = Black;
  board[mid][mid] = White;
  
  currentPlayer = Black;
  gameOver = false;
  calculateScore();
}

bool Othello::isValidMove(int row, int col, Cell player) const {
  if (row < 0 || row >= boardSize || col < 0 || col >= boardSize)
    return false;
  if (board[row][col] != Empty)
    return false;
  
  return !getFlipPositions(row, col, player).isEmpty();
}

QVector<QPair<int, int>> Othello::getFlipPositions(int row, int col, Cell player) const {
  QVector<QPair<int, int>> flips;
  Cell opponent = (player == Black) ? White : Black;
  
  // 8方向をチェック
  int directions[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
                          {0, 1},   {1, -1}, {1, 0},  {1, 1}};
  
  for (int d = 0; d < 8; d++) {
    int dr = directions[d][0];
    int dc = directions[d][1];
    QVector<QPair<int, int>> tempFlips;
    
    int r = row + dr;
    int c = col + dc;
    
    // 相手の石を探す
    while (r >= 0 && r < boardSize && c >= 0 && c < boardSize &&
           board[r][c] == opponent) {
      tempFlips.append(qMakePair(r, c));
      r += dr;
      c += dc;
    }
    
    // 自分の石で挟めるかチェック
    if (!tempFlips.isEmpty() && r >= 0 && r < boardSize && c >= 0 &&
        c < boardSize && board[r][c] == player) {
      flips.append(tempFlips);
    }
  }
  
  return flips;
}

void Othello::makeMove(int row, int col) {
  if (!isValidMove(row, col, currentPlayer))
    return;
  
  board[row][col] = currentPlayer;
  flipPieces(row, col);
  
  // ターン交代
  currentPlayer = (currentPlayer == Black) ? White : Black;
  
  // 次のプレイヤーが置けない場合はスキップ
  if (!hasValidMoves(currentPlayer)) {
    currentPlayer = (currentPlayer == Black) ? White : Black;
    
    // 両者とも置けない場合はゲーム終了
    if (!hasValidMoves(currentPlayer)) {
      checkGameOver();
    }
  }
  
  calculateScore();
  update();
}

void Othello::flipPieces(int row, int col) {
  QVector<QPair<int, int>> flips = getFlipPositions(row, col, currentPlayer);
  
  for (const auto &pos : flips) {
    board[pos.first][pos.second] = currentPlayer;
  }
}

bool Othello::hasValidMoves(Cell player) const {
  for (int i = 0; i < boardSize; i++) {
    for (int j = 0; j < boardSize; j++) {
      if (isValidMove(i, j, player))
        return true;
    }
  }
  return false;
}

void Othello::calculateScore() {
  blackScore = 0;
  whiteScore = 0;
  
  for (int i = 0; i < boardSize; i++) {
    for (int j = 0; j < boardSize; j++) {
      if (board[i][j] == Black)
        blackScore++;
      else if (board[i][j] == White)
        whiteScore++;
    }
  }
}

void Othello::checkGameOver() {
  gameOver = true;
}

void Othello::mousePressEvent(QMouseEvent *event) {
  if (gameOver)
    return;
  
  int col = (event->position().x() - 10) / cellSize;
  int row = (event->position().y() - 60) / cellSize;
  
  if (row >= 0 && row < boardSize && col >= 0 && col < boardSize) {
    makeMove(row, col);
  }
}

void Othello::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  
  // 背景
  painter.fillRect(rect(), QColor(34, 139, 34)); // 緑の背景
  
  drawGameInfo(painter);
  drawBoard(painter);
  drawValidMoves(painter);
}

void Othello::drawBoard(QPainter &painter) {
  int offsetX = 10;
  int offsetY = 60;
  
  // ボードの枠
  painter.setPen(QPen(Qt::black, 2));
  painter.setBrush(QColor(34, 139, 34));
  painter.drawRect(offsetX, offsetY, cellSize * boardSize, cellSize * boardSize);
  
  // グリッド線
  painter.setPen(QPen(Qt::black, 1));
  for (int i = 0; i <= boardSize; i++) {
    // 横線
    painter.drawLine(offsetX, offsetY + i * cellSize,
                    offsetX + cellSize * boardSize, offsetY + i * cellSize);
    // 縦線
    painter.drawLine(offsetX + i * cellSize, offsetY,
                    offsetX + i * cellSize, offsetY + cellSize * boardSize);
  }
  
  // 星（ガイドポイント）
  painter.setBrush(Qt::black);
  int starPositions[] = {2, 6};
  for (int row : starPositions) {
    for (int col : starPositions) {
      int x = offsetX + col * cellSize + cellSize / 2;
      int y = offsetY + row * cellSize + cellSize / 2;
      painter.drawEllipse(QPoint(x, y), 3, 3);
    }
  }
  
  // 石を描画
  for (int i = 0; i < boardSize; i++) {
    for (int j = 0; j < boardSize; j++) {
      if (board[i][j] != Empty) {
        drawPiece(painter, i, j, board[i][j]);
      }
    }
  }
}

void Othello::drawPiece(QPainter &painter, int row, int col, Cell piece) {
  int offsetX = 10;
  int offsetY = 60;
  int x = offsetX + col * cellSize + cellSize / 2;
  int y = offsetY + row * cellSize + cellSize / 2;
  int radius = cellSize / 2 - 5;
  
  if (piece == Black) {
    QRadialGradient gradient(x - radius / 3, y - radius / 3, radius);
    gradient.setColorAt(0, QColor(80, 80, 80));
    gradient.setColorAt(1, QColor(0, 0, 0));
    painter.setBrush(gradient);
    painter.setPen(QPen(Qt::black, 2));
  } else {
    QRadialGradient gradient(x - radius / 3, y - radius / 3, radius);
    gradient.setColorAt(0, QColor(255, 255, 255));
    gradient.setColorAt(1, QColor(220, 220, 220));
    painter.setBrush(gradient);
    painter.setPen(QPen(QColor(200, 200, 200), 2));
  }
  
  painter.drawEllipse(QPoint(x, y), radius, radius);
}

void Othello::drawValidMoves(QPainter &painter) {
  if (gameOver)
    return;
  
  int offsetX = 10;
  int offsetY = 60;
  
  painter.setPen(Qt::NoPen);
  painter.setBrush(QColor(255, 255, 0, 100)); // 半透明の黄色
  
  for (int i = 0; i < boardSize; i++) {
    for (int j = 0; j < boardSize; j++) {
      if (isValidMove(i, j, currentPlayer)) {
        int x = offsetX + j * cellSize + cellSize / 2;
        int y = offsetY + i * cellSize + cellSize / 2;
        painter.drawEllipse(QPoint(x, y), 8, 8);
      }
    }
  }
}

void Othello::drawGameInfo(QPainter &painter) {
  // タイトル
  painter.setPen(Qt::white);
  QFont titleFont("Arial", 14, QFont::Bold);
  painter.setFont(titleFont);
  painter.drawText(QRect(10, 5, width() - 20, 25), Qt::AlignCenter, "OTHELLO");
  
  // スコア表示
  QFont scoreFont("Arial", 12, QFont::Bold);
  painter.setFont(scoreFont);
  
  // 黒のスコア
  painter.setBrush(Qt::black);
  painter.setPen(QPen(Qt::white, 2));
  painter.drawEllipse(QPoint(60, 40), 12, 12);
  
  painter.setPen(Qt::white);
  QString blackText = QString("Black: %1").arg(blackScore);
  painter.drawText(QRect(80, 30, 100, 20), Qt::AlignLeft | Qt::AlignVCenter,
                   blackText);
  
  // 白のスコア
  painter.setBrush(Qt::white);
  painter.setPen(QPen(Qt::black, 2));
  painter.drawEllipse(QPoint(220, 40), 12, 12);
  
  painter.setPen(Qt::white);
  QString whiteText = QString("White: %1").arg(whiteScore);
  painter.drawText(QRect(240, 30, 100, 20), Qt::AlignLeft | Qt::AlignVCenter,
                   whiteText);
  
  // 現在のターン表示
  if (!gameOver) {
    painter.setPen(Qt::yellow);
    QString turnText = (currentPlayer == Black) ? "Black's Turn" : "White's Turn";
    painter.drawText(QRect(10, 30, width() - 20, 20),
                    Qt::AlignRight | Qt::AlignVCenter, turnText);
  } else {
    painter.setPen(Qt::yellow);
    QFont gameOverFont("Arial", 14, QFont::Bold);
    painter.setFont(gameOverFont);
    
    QString result;
    if (blackScore > whiteScore)
      result = "Black Wins!";
    else if (whiteScore > blackScore)
      result = "White Wins!";
    else
      result = "Draw!";
    
    painter.drawText(QRect(10, cellSize * boardSize + 70, width() - 20, 30),
                    Qt::AlignCenter, result);
  }
}
