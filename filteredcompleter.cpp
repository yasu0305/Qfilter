#include "filteredcompleter.h"
#include <QAbstractItemView>
#include <QCompleter>
#include <QLineEdit>
#include <QRegularExpression>
#include <QStringListModel>

namespace {
// AND検索用のトークン分割
QStringList splitTokens(const QString& text) {
    static const QRegularExpression sep("\\s+", QRegularExpression::UseUnicodePropertiesOption);
    return text.split(sep, Qt::SkipEmptyParts);
}
} // namespace

FilteredCompleter::FilteredCompleter(QObject* parent)
    : QObject(parent), model(new QStringListModel(this)), completer(new QCompleter(model, this)),
      attachedLineEdit(nullptr) {
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    connect(completer, QOverload<const QString&>::of(&QCompleter::activated), this,
            &FilteredCompleter::onActivated);
}

FilteredCompleter::~FilteredCompleter() = default;

void FilteredCompleter::setCandidates(const QStringList& list) {
    allCandidates = list;
    model->setStringList(allCandidates);
}

void FilteredCompleter::attachTo(QLineEdit* lineEdit) {
    if (!lineEdit)
        return;
    attachedLineEdit = lineEdit;
    completer->setWidget(lineEdit);
    connect(lineEdit, &QLineEdit::textChanged, this, &FilteredCompleter::onTextChanged);
}

void FilteredCompleter::onTextChanged(const QString& text) {
    QStringList filtered;
    if (text.isEmpty()) {
        filtered = allCandidates;
    } else {
        const QStringList tokens = splitTokens(text);
        for (const QString& s : allCandidates) {
            bool match = true;
            for (const QString& token : tokens) {
                if (!s.contains(token, completer->caseSensitivity())) {
                    match = false;
                    break;
                }
            }
            if (match)
                filtered.append(s);
        }
    }
    model->setStringList(filtered);
    if (!text.isEmpty() && !filtered.isEmpty()) {
        completer->complete();
    }
}

void FilteredCompleter::onActivated(const QString& text) {
    emit selectionConfirmed(text);
    if (attachedLineEdit)
        attachedLineEdit->setText(text);
    if (completer->popup())
        completer->popup()->hide();
}
