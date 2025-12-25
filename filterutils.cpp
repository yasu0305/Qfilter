#include "filterutils.h"

QStringList splitTokens(const QString &text) {
  static const QRegularExpression sep("\\s+", QRegularExpression::UseUnicodePropertiesOption);
  return text.split(sep, Qt::SkipEmptyParts);
}

bool defaultAndFilter(const QString &candidate, const QString &text) {
  if (text.isEmpty()) return true;
  const QStringList tokens = splitTokens(text);
  for (const QString &t : tokens) {
    if (!candidate.contains(t, Qt::CaseInsensitive)) return false;
  }
  return true;
}
