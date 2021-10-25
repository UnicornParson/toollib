#ifndef CONCURENTVALUE_H
#define CONCURENTVALUE_H

#include "tools.h"
#include "sharedxmutex.h"

namespace Tools
{
  template< class T >
  class ConcurrentValue
  {
  public:
    explicit ConcurrentValue(const T& val)
    {
      set(val);
    }
    T get()
    {
      SXMUTEX_LOCK_READ(m_smutex);
      T tmp = m_val;
      SXMUTEX_UNLOCK_READ(m_smutex);
      return tmp;
    }
    operator T() {return get();}
    void set(const T& val)
    {
      SXMUTEX_LOCK_WRITE(m_smutex);
      m_val = val;
      SXMUTEX_UNLOCK_WRITE(m_smutex);
    }

    ConcurrentValue& operator= (const T& val)
    {
      set(val);
      return *this;
    }

  private:
    Q_DISABLE_COPY_MOVE(ConcurrentValue)
    T m_val;
    SharedXMutex m_smutex;
  };

  typedef ConcurrentValue<bool> ConcurrentBool;
  typedef ConcurrentValue<int> ConcurrentInt;
  typedef ConcurrentValue<QString> ConcurrentString;
  typedef ConcurrentValue<void*> ConcurrentPointer;
}
#endif // CONCURENTVALUE_H
