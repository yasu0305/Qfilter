#include "filterutils.h"
#include <QRandomGenerator>

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

QStringList generateRandomStrings(int count, int length) {
  const QString pool = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()-_=+[]{};:,.<>/?|~";
  const int poolSize = pool.size();
  QStringList out;
  out.reserve(count);
  for (int i = 0; i < count; ++i) {
    QString s;
    s.resize(length);
    for (int j = 0; j < length; ++j) {
      int idx = QRandomGenerator::global()->bounded(poolSize);
      s[j] = pool.at(idx);
    }
    out.append(s);
  }
  return out;
}
