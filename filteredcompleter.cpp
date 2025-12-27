#include "filteredcompleter.h"
#include <QLineEdit>
#include <QAbstractItemView>
#include <QObject>
#include <QStringListModel>
#include <QCompleter>
#include <QSignalBlocker>
#include <QStringList>
#include <QString>
#include <QRegularExpression>

// AND検索用のトークン分割関数
static QStringList splitTokens(const QString &text)
{
  static const QRegularExpression sep("\\s+", QRegularExpression::UseUnicodePropertiesOption);
  return text.split(sep, Qt::SkipEmptyParts);
}
#include <QLineEdit>
#include <QAbstractItemView>
#include <QObject>
#include <QStringListModel>
#include <QCompleter>
#include <QSignalBlocker>

FilteredCompleter::FilteredCompleter(QObject *parent)
    : QObject(parent),
      model(new QStringListModel(this)),
      completer(new QCompleter(model, this)),
      attachedLineEdit(nullptr)
{
  completer->setCaseSensitivity(Qt::CaseInsensitive);
  completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
  QObject::connect(completer, QOverload<const QString &>::of(&QCompleter::activated),
                   this, &FilteredCompleter::onActivated);
}

FilteredCompleter::~FilteredCompleter() = default;

void FilteredCompleter::setCandidates(const QStringList &list)
{
  allCandidates = list;
  model->setStringList(allCandidates);
}

void FilteredCompleter::attachTo(QLineEdit *lineEdit)
{
  if (!lineEdit)
    return;
  attachedLineEdit = lineEdit;
  completer->setWidget(lineEdit);
  QObject::connect(lineEdit, &QLineEdit::textChanged, this, &FilteredCompleter::onTextChanged);
}


void FilteredCompleter::onTextChanged(const QString &text)
{
  QStringList filtered;
  if (text.isEmpty())
  {
    filtered = allCandidates;
  }
  else
  {
    // AND検索: textをスペースで分割し、すべての単語を含む候補のみ抽出
    QStringList tokens = splitTokens(text);
    for (const QString &s : allCandidates)
    {
      bool match = true;
      for (const QString &token : tokens)
      {
        if (!s.contains(token, completer->caseSensitivity()))
        {
          match = false;
          break;
        }
      }
      if (match)
        filtered.append(s);
    }
  }
  model->setStringList(filtered);
  if (!text.isEmpty() && !filtered.isEmpty())
  {
    completer->complete();
  }
}

void FilteredCompleter::onActivated(const QString &text)
{
  emit selectionConfirmed(text);
  if (attachedLineEdit)
  {
    attachedLineEdit->setText(text);
  }
  if (completer->popup())
    completer->popup()->hide();
}
