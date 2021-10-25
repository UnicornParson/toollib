#ifndef CRASHTRAPNOOP_H
#define CRASHTRAPNOOP_H

#include <QtGlobal>
#include <QString>
#include "toolsmacro.h"
namespace Tools
{
#ifndef FEATURE_BOOST
#ifdef S_ASSERT_EX
#undef S_ASSERT_EX
#endif
#ifdef S_ASSERT
#undef S_ASSERT
#endif

#define S_ASSERT_EX(cond, msg) Tools::CrashTrap::stableAssert(cond, (QString("[%1] - ").arg(__HERE__).append(msg)))
#define S_ASSERT(cond) S_ASSERT_EX( ##cond )

typedef std::function<void(int /*sig*/, const QString& /* message */)> ExternalCrashHandler_t;

class CrashTrapNoop
{
public:
    static bool init(const QString& crashDumpBase);
    static void stableAssert(bool conditionResult, const QString& messageOnFail = QString());
    static void setExternalCrashHandler(ExternalCrashHandler_t func);
private:
    Q_DISABLE_COPY(CrashTrapNoop)
    CrashTrapNoop();
    ~CrashTrapNoop();
};

typedef CrashTrapNoop CrashTrap;

#endif // FEATURE_BOOST
} // end namespace Tools
#endif // CRASHTRAPNOOP_H
