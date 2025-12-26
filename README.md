FilteredCompleter
=================

Lightweight utility for filtered QLineEdit completion.

特徴
----
- QLineEdit 用の効率的で再利用可能なフィルタ付き補完
- フィルタロジックはカスタマイズ可能（デフォルトはトークンAND検索）
- Qt Widgets/Core 以外のUI依存なし
- 大量候補リストにも対応
- 確定時のみシグナル発行（途中でテキストが書き換わらない）

最近のリファクタ
----------------
- ヘッダ依存を最小化しビルドを高速化
- QSignalBlocker による安全なシグナルブロック
- MainWindow 側での重複テキスト更新を排除

使い方
------
Qtウィジェットコード例：

```cpp
#include "filteredcompleter.h"

// インスタンス生成
auto *fc = new FilteredCompleter(this);
fc->setCandidates(myStringList); // QStringList
fc->attachTo(myLineEdit);

// オプション
fc->setCaseSensitivity(Qt::CaseInsensitive);
fc->setCompletionMode(QCompleter::UnfilteredPopupCompletion);

connect(fc, &FilteredCompleter::selectionConfirmed, this, [](const QString &text){
  qDebug() << "確定:" << text;
});
```

備考
----
- デフォルトフィルタは空白区切りトークンのAND検索（大文字小文字無視）
- 矢印キーでのハイライトではQLineEdit値は書き換わらず、Enterやクリックで確定

ビルド
------
標準CMakeビルド（`hello`サンプル付き）:

```bash
mkdir -p build && cd build
cmake ..
cmake --build . -- -j$(nproc)
```

テスト
------
ユニットテスト（filterutils_test）:

```bash
cmake --build . --target filterutils_test
./filterutils_test
```

実行例
------
アプリ起動:

```bash
cmake --build . --target hello
./hello
```

環境要件
--------
- Qt6 (Widgets/Core)
- CMake 3.19以上
- g++/clang++ (C++17)
