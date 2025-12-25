#include "filterutils.h"
#include <QDebug>
#include <cassert>

int main() {
  // basic tests
  assert(defaultAndFilter("hello world", "hello"));
  assert(defaultAndFilter("hello world", "world"));
  assert(defaultAndFilter("hello world", "hello world"));
  assert(!defaultAndFilter("hello", "world"));
  assert(defaultAndFilter("a b c d", "b c"));
  assert(defaultAndFilter("sr_mon_dcct/current", "sr_mon_dcct/current"));

  qDebug() << "filterutils tests passed";
  return 0;
}
