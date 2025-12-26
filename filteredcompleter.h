#ifndef FILTEREDCOMPLETER_H
#define FILTEREDCOMPLETER_H

#include <QObject>
#include <QStringList>
#include <QCompleter>
#include <functional>

/*
  FilteredCompleter

  Lightweight reusable helper that provides filtered completion for a QLineEdit.

  Usage summary:
  - Create an instance with a QObject parent (e.g. the window):
    auto *fc = new FilteredCompleter(this);
  - Provide the candidate list:
    fc->setCandidates(myStringList);
  - Attach to a QLineEdit:
    fc->attachTo(lineEdit);
  - (Optional) customize behavior:
    fc->setCaseSensitivity(Qt::CaseInsensitive);
    fc->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    fc->setFilterFunction(customFunc);
  - Listen for confirmed selection:
    connect(fc, &FilteredCompleter::selectionConfirmed, this, &MyClass::onConfirmed);

  Behavior:
  - By default performs an AND-search on space-separated tokens (case-insensitive).
  - Arrow-key highlighted items do not alter the QLineEdit text.
  - Activation via Enter or mouse emits `selectionConfirmed` and updates the attached
  QLineEdit text (signals are blocked during update to avoid re-filtering).

  The filtering logic can be reused independently by calling `setFilterFunction`.
*/

class QLineEdit;
class QStringListModel;

class FilteredCompleter : public QObject {
  Q_OBJECT
public:
  explicit FilteredCompleter(QObject *parent = nullptr);
  ~FilteredCompleter();

  void setCandidates(const QStringList &list);
  void attachTo(QLineEdit *lineEdit);
  void setCaseSensitivity(Qt::CaseSensitivity cs);
  void setCompletionMode(QCompleter::CompletionMode mode);
  void setFilterFunction(std::function<bool(const QString &candidate, const QString &text)> func);

signals:
  void selectionConfirmed(const QString &text);

private slots:
  void onTextChanged(const QString &text);
  void onActivated(const QString &text);
  void onHighlighted(const QString &text);

private:
  QStringList allCandidates;
  QStringListModel *model = nullptr;
  QCompleter *completer = nullptr;
  QLineEdit *attachedLineEdit = nullptr;
  std::function<bool(const QString &candidate, const QString &text)> filterFunc;
};

#endif // FILTEREDCOMPLETER_H
