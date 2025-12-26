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

// Generate `count` random strings of `length` characters each.
// Characters include letters, digits and common symbols.
QStringList generateRandomStrings(int count, int length);

#endif // FILTERUTILS_H
