#ifndef SHAREDXMUTEX_H
#define SHAREDXMUTEX_H
#include "tools.h"
#include "internal/sharedxmutexnative.h"
#include "internal/sharedxmutexwrp.h"
namespace Tools
{
#ifdef CPP17SUPPORTED
  typedef SharedXMutexWrp SharedXMutex;
#else
  typedef SharedXMutexNative SharedXMutex;
#endif //CPP14PLUS

#define SXMUTEX_LOCK_READ(x) x.lockRead(__FILE__, Q_FUNC_INFO, __LINE__)
#define SXMUTEX_UNLOCK_READ(x) x.unlockRead(__FILE__, Q_FUNC_INFO, __LINE__)
#define SXMUTEX_LOCK_WRITE(x) x.lockWrite(__FILE__, Q_FUNC_INFO, __LINE__)
#define SXMUTEX_UNLOCK_WRITE(x) x.unlockWrite(__FILE__, Q_FUNC_INFO, __LINE__)
}
#endif // SHAREDXMUTEX_H
