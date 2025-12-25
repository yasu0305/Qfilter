#include "filteredcompleter.h"
#include "filterutils.h"
#include <QLineEdit>
#include <QAbstractItemView>
#include <QRandomGenerator>

FilteredCompleter::FilteredCompleter(QObject *parent) : QObject(parent) {
  model = new QStringListModel(this);
  completer = new QCompleter(model, this);
  completer->setCaseSensitivity(Qt::CaseInsensitive);
  // Use UnfilteredPopupCompletion: we supply a filtered model ourselves
  completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);

  // default filter uses shared utility
  filterFunc = defaultAndFilter;

  connect(completer, QOverload<const QString &>::of(&QCompleter::activated),
          this, &FilteredCompleter::onActivated);
  connect(completer, QOverload<const QString &>::of(&QCompleter::highlighted),
          this, &FilteredCompleter::onHighlighted);
}

FilteredCompleter::~FilteredCompleter() {}

void FilteredCompleter::setCandidates(const QStringList &list) {
  allCandidates = list;
  model->setStringList(allCandidates);
}

void FilteredCompleter::setFilterFunction(std::function<bool(const QString &candidate, const QString &text)> func) {
  if (func) filterFunc = std::move(func);
}

void FilteredCompleter::attachTo(QLineEdit *lineEdit) {
  if (!lineEdit) return;
  attachedLineEdit = lineEdit;
  completer->setWidget(lineEdit);
  connect(lineEdit, &QLineEdit::textChanged, this, &FilteredCompleter::onTextChanged);
}

void FilteredCompleter::setCaseSensitivity(Qt::CaseSensitivity cs) {
  completer->setCaseSensitivity(cs);
}

void FilteredCompleter::setCompletionMode(QCompleter::CompletionMode mode) {
  completer->setCompletionMode(mode);
}

void FilteredCompleter::onTextChanged(const QString &text) {
  QStringList filtered;
  if (text.isEmpty()) {
    filtered = allCandidates;
  } else {
    for (const QString &s : allCandidates) {
      if (filterFunc(s, text)) filtered.append(s);
    }
  }
  model->setStringList(filtered);
  if (!text.isEmpty() && !filtered.isEmpty()) {
    completer->complete();
  }
}

void FilteredCompleter::onActivated(const QString &text) {
  emit selectionConfirmed(text);
  if (attachedLineEdit) {
    attachedLineEdit->blockSignals(true);
    attachedLineEdit->setText(text);
    attachedLineEdit->blockSignals(false);
  }
  if (completer->popup()) completer->popup()->hide();
}

void FilteredCompleter::onHighlighted(const QString & /*text*/) {
  // intentionally empty to avoid highlighted items immediately changing the line edit
}
