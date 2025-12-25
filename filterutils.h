#ifndef FILTERUTILS_H
#define FILTERUTILS_H

#include <QString>
#include <QStringList>
#include <QRegularExpression>

// Default AND-search filter: returns true if candidate contains all tokens
// obtained by splitting 'text' on whitespace (case-insensitive).
bool defaultAndFilter(const QString &candidate, const QString &text);

// Helper to split tokens from text (unicode-aware whitespace)
QStringList splitTokens(const QString &text);

#endif // FILTERUTILS_H
