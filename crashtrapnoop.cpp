#include "crashtrapnoop.h"
#ifndef FEATURE_BOOST
#include "tools.h"
using namespace Tools;

CrashTrapNoop::CrashTrapNoop()
{

}

CrashTrapNoop::~CrashTrapNoop()
{

}

bool CrashTrapNoop::init(const QString& crashDumpBase)
{
    LOG_INFO("Crash trap is disabled in this configurations");
    UNUSED(crashDumpBase)
    return true;
}

void CrashTrapNoop::stableAssert(bool conditionResult, const QString& messageOnFail)
{
    if (!conditionResult)
    {
        LOG_ERROR(messageOnFail);
    }
}

void CrashTrapNoop::setExternalCrashHandler(ExternalCrashHandler_t func)
{
    UNUSED(func)
}
#endif // FEATURE_BOOST
