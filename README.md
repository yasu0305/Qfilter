FilteredCompleter
=================

Lightweight utility for filtered QLineEdit completion.

Quick usage
-----------

In your Qt widget code:

```cpp
#include "filteredcompleter.h"

// create
auto *fc = new FilteredCompleter(this);
fc->setCandidates(myStringList); // QStringList
fc->attachTo(myLineEdit);

// optional
fc->setCaseSensitivity(Qt::CaseInsensitive);
fc->setCompletionMode(QCompleter::UnfilteredPopupCompletion);

connect(fc, &FilteredCompleter::selectionConfirmed, this, [](const QString &text){
  qDebug() << "confirmed:" << text;
});
```

Notes
-----
- Default filter performs an AND search over space-separated tokens (case-insensitive).
- Arrow-key highlighting does not overwrite the QLineEdit value; Enter or mouse click confirms.

Building
--------

Standard CMake build (project already contains `hello` example):

```bash
mkdir -p build && cd build
cmake ..
cmake --build . -- -j$(nproc)
```
