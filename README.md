
# FilteredCompleter



QtのQLineEditに「AND検索」フィルタ付きの補完機能を簡単に追加できる軽量クラスです。

## 特徴
- QLineEditに簡単にアタッチできる
- 候補リストをスペース区切りAND検索でフィルタ
- 大文字小文字区別なし・補完モード固定（UnfilteredPopupCompletion）
- 補完候補の選択確定時にシグナルを発行
- シンプルなAPIと実装

## 使い方


### 1. インスタンス生成
```cpp
#include "filteredcompleter.h"
FilteredCompleter* fc = new FilteredCompleter(this);
```


### 2. 候補リストをセット
```cpp
QStringList candidates = {"apple", "banana", "grape", "orange", "pineapple"};
fc->setCandidates(candidates);
```


### 3. QLineEditにアタッチ
```cpp
fc->attachTo(ui->lineEdit);
```


### 4. 補完確定時のシグナルを受け取る
```cpp
connect(fc, &FilteredCompleter::selectionConfirmed, this, [](const QString &text){
  qDebug() << "Confirmed:" << text;
});
```


## デフォルトの動作
- 入力テキストをスペースで分割し、すべての単語を含む候補のみ表示（AND検索）
- 補完候補の選択確定時にQLineEditのテキストを更新し、`selectionConfirmed`シグナルを発行
- 大文字小文字区別なし、補完モードはUnfilteredPopupCompletionで固定


## サンプルmain.cpp
```cpp
#include <QApplication>
#include <QLineEdit>
#include "filteredcompleter.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  QLineEdit lineEdit;
  lineEdit.show();
  FilteredCompleter fc;
  fc.setCandidates(QStringList{"apple", "banana", "grape", "orange", "pineapple"});
  fc.attachTo(&lineEdit);
  QObject::connect(&fc, &FilteredCompleter::selectionConfirmed, [](const QString &text){
    qDebug() << "Confirmed:" << text;
  });
  return app.exec();
}
```



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

## 環境要件
- Qt6 (Widgets/Core)
- CMake 3.19以上
- g++/clang++ (C++17)
