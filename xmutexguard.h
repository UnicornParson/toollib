#ifndef XMUTEXGUARD_H
#define XMUTEXGUARD_H
#include "xmutex.h"
#include "tools.h"

#include <functional>

namespace Tools
{
template<class M>
class XMutexGuard
{
public:
    explicit XMutexGuard(M* m) noexcept :
        m_mutex(m)
    {
        if (m_mutex != nullptr)
        {
            XMUTEXPTR_LOCK(m_mutex);
        }
        else
        {
            LOG_WARNING("null mutex (lock)");
        }
    }

    ~XMutexGuard() noexcept
    {
        if (m_mutex != nullptr)
        {
            XMUTEXPTR_UNLOCK(m_mutex);
        }
        else
        {
            LOG_WARNING("null mutex (unlock)");
        }
    }
    XMutexGuard(const M&) = delete;
    XMutexGuard(const M&&) = delete;
    XMutexGuard() = delete;
    XMutexGuard& operator=(const XMutexGuard&) = delete;
    inline M* mutex() noexcept { return m_mutex;}
private:
    M* m_mutex;
}; // class XMutexGuard

/*!
    \class XFunctionLockerGuard
    \brief Alternative of mutexguard for user specified lock/unlock functions
    */
class XFunctionLockerGuard
{
public:
    using LockFinction = std::function<void()>;
    using UnlockFinction = std::function<void()>;
    XFunctionLockerGuard(const LockFinction& locker, const UnlockFinction& unlocker);
    ~XFunctionLockerGuard();
    XFunctionLockerGuard(XFunctionLockerGuard&) = delete;
    XFunctionLockerGuard(XFunctionLockerGuard&&) = delete;
    XFunctionLockerGuard& operator=(XFunctionLockerGuard&) = delete;
private:
    const LockFinction& m_locker;
    const LockFinction& m_unlocker;

}; // class XFunctionLockerGuard

}; // namespace Tools
#endif // XMUTEXGUARD_H
