#ifndef BADWAYTRAP_H
#define BADWAYTRAP_H
#include <atomic>
namespace Tools
{
#ifdef BADWAY_TRAP_FEATURE

class BadwayTrapConfig
{
public:
    static inline bool isTrapEnabled()
    {
        return m_trapEnabled.load();
    }

    static inline void enableTrap()
    {
        m_trapEnabled.store(true);
    }

    static inline void disableTrap()
    {
        m_trapEnabled.store(false);
    }

    static inline void setEnabledTrap(bool v)
    {
        m_trapEnabled.store(v);
    }
private:
    static std::atomic_bool m_trapEnabled;
};



#define BADWAY_TRAP(msg) if(Tools::BadwayTrapConfig::isTrapEnabled()){ qFatal("SOMETHING IS WRONG HERE %s (%s)", __HERE__, QString(msg).toUtf8().data());}
#else
#define BADWAY_TRAP(msg) if(Tools::BadwayTrapConfig::isTrapEnabled()){ qWarning("(%s) %s", __HERE__, SCHR_CONST(QString(msg)));}
#endif // BADWAY_TRAP
} // namespace Tools
#endif // BADWAYTRAP_H
