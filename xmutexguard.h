#ifndef XMUTEXGUARD_H
#define XMUTEXGUARD_H
#include "xmutex.h"
#include "tools.h"
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

    inline M* mutex() noexcept { return m_mutex;}
private:
    M* m_mutex;
}; // class XMutexGuard
}; // namespace Tools
#endif // XMUTEXGUARD_H
