#ifndef IDFACTORY_H
#define IDFACTORY_H
#include <QAtomicInteger>
#include "tools.h"
namespace Tools
{
class IdFactory
{
public:
  IdFactory();
  IdFactory(const IdFactory& other);
  IdFactory& operator=(const IdFactory& other);
  virtual ~IdFactory() = default;
  virtual quint64 newId();
  virtual quint64 getLastId() const;
  void reset(quint64 firstId = 1);
private:
  QAtomicInteger<quint64> nextId;
}; // class IdFactory
} // namespace Tools
#endif // IDFACTORY_H
