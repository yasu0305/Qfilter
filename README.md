# Qfilter: AND検索付きQLineEdit補完

QtのQLineEditに「AND検索」フィルタ付きの補完機能を簡単に追加できる、最小構成のクラスです。

## 特徴
- QLineEditにattachするだけで使える
- 候補リストをスペース区切りAND検索でフィルタ
- 大文字小文字区別なし・UnfilteredPopupCompletion固定
- 補完候補の選択確定時にシグナル発行
- シンプルなAPI・実装

## 使い方
1. インスタンス生成
2. 候補リストをセット
3. QLineEditにアタッチ
4. selectionConfirmedシグナルを受け取る

### サンプル
```cpp
#include "filteredcompleter.h"
FilteredCompleter* fc = new FilteredCompleter(this);
fc->setCandidates(QStringList{"apple", "banana", "grape", "orange", "pineapple"});
fc->attachTo(ui->lineEdit);
connect(fc, &FilteredCompleter::selectionConfirmed, this, [](const QString &text){
  qDebug() << "Confirmed:" << text;
});
```

### mainwindowでの利用例（抜粋）
```cpp
filteredCompleter = new FilteredCompleter(this);
filteredCompleter->setCandidates(generateRandomStrings(10000, 20));
filteredCompleter->attachTo(lineEdit);
connect(filteredCompleter, &FilteredCompleter::selectionConfirmed, this, &MainWindow::onCompleterActivated);
```

## デフォルト動作
- 入力テキストをスペースで分割し、すべての単語を含む候補のみ表示（AND検索）
- 補完候補の選択確定時にQLineEditのテキストを更新し、`selectionConfirmed`シグナルを発行
- 大文字小文字区別なし、補完モードはUnfilteredPopupCompletionで固定

## ビルド・実行
標準CMakeビルド:
```bash
mkdir -p build && cd build
cmake ..
cmake --build . -- -j$(nproc)
./qfilter
```

## ライセンス
MIT License

## 動作環境
- Qt6 (Widgets/Core)
- CMake 3.19以上
- g++/clang++ (C++17)
