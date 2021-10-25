#ifndef CRASHTRAP_H
#define CRASHTRAP_H

#ifdef FEATURE_BOOST
#ifndef NO_CRUSH_TRAP
#error("not fully implemented. need to compilate boost")
#include <QtGlobal>
#include <QString>
#include "toolsmacro.h"

namespace Tools
{

#ifdef S_ASSERT_EX
#undef S_ASSERT_EX
#endif
#ifdef S_ASSERT
#undef S_ASSERT
#endif
#define S_ASSERT_EX(cond, msg) Tools::CrashTrap::stableAssert(cond, (QString("[%1] - ").arg(__HERE__).append(msg)))
#define S_ASSERT(cond) S_ASSERT_EX( ##cond )

typedef std::function<void(int /*sig*/, const QString& /* message */)> ExternalCrashHandler_t;

class CrashTrap
{
public:
    static bool init(const QString& crashDumpBase);
    static void stableAssert(bool conditionResult, const QString& messageOnFail = QString());
    static void setExternalCrashHandler(ExternalCrashHandler_t func);
private:
    Q_DISABLE_COPY(CrashTrap)
    CrashTrap();
    ~CrashTrap();
    static CrashTrap& self();
    static void printStackTrace();
    static void crashHandler(int signal, const QString& msg);
    static bool makeDump(const QString& path);
    static void signalHandler( int signum );
    QString m_crashDumpBase;
    ExternalCrashHandler_t m_extHandler;
};
} // namespace Tools
#else // ifndef NO_CRUSH_TRAP
#include "crashtrapnoop.h"
#endif

#else // FEATURE_BOOST
#include "crashtrapnoop.h"
#endif // else FEATURE_BOOST
#endif // CRASHTRAP_H
