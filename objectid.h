#ifndef OBJECTID_H
#define OBJECTID_H
#include "xmutex.h"
#include <QString>
namespace Tools
{
  class ObjectId
  {
  public:
    ObjectId();
    ObjectId(const ObjectId& other);
    inline QString str() const {return m_str;}
    inline quint64 id() const {return m_id;}
  private:
    static quint64 s_nextId;
    static Tools::XNonRecursiveMutex s_idMutex;
    void updateStr();
    quint64 m_id;
    QString m_str;
  };
}
#endif // OBJECTID_H
