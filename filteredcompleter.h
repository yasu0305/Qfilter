#ifndef FILTEREDCOMPLETER_H
#define FILTEREDCOMPLETER_H

#include <QObject>
#include <QStringList>
#include <QStringListModel>
#include <QCompleter>

class QLineEdit;

class FilteredCompleter : public QObject {
  Q_OBJECT
public:
  explicit FilteredCompleter(QObject *parent = nullptr);
  ~FilteredCompleter();

  void setCandidates(const QStringList &list);
  void attachTo(QLineEdit *lineEdit);
  void setCaseSensitivity(Qt::CaseSensitivity cs);
  void setCompletionMode(QCompleter::CompletionMode mode);

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
};

#endif // FILTEREDCOMPLETER_H
