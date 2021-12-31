#include "xmutexguard.h"

#include <QDebug>

using namespace Tools;

XFunctionLockerGuard::XFunctionLockerGuard(const LockFinction& locker, const UnlockFinction& unlocker):
    m_locker(locker),
    m_unlocker(unlocker)
{
#ifdef MUTEX_TRACE
    qDebug() << "try lock XFunctionLockerGuard" << static_cast<quintptr>(this);
#endif
    m_locker();
#ifdef MUTEX_TRACE
    qDebug() << "lock - done XFunctionLockerGuard" << static_cast<quintptr>(this);
#endif
}

XFunctionLockerGuard::~XFunctionLockerGuard()
{
#ifdef MUTEX_TRACE
    qDebug() << "try unlock XFunctionLockerGuard" << static_cast<quintptr>(this);
#endif
    m_unlocker();
#ifdef MUTEX_TRACE
    qDebug() << "unlock - done XFunctionLockerGuard" << static_cast<quintptr>(this);
#endif
}
