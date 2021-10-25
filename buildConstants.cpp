#include "buildConstants.h"
#include "tools.h"

using namespace Tools;

QString ConstantsHelper::getFeaturesStr(const QString& separator)
{
    QString sList;
    foreach(QString s, BUILDCFG_FEATURES)
    {
        if (s == QString(FEATURES_END) || s.trimmed().isEmpty())
        {
            continue;
        }
        sList += s + separator;
    }
    return sList.trimmed();
}

QString ConstantsHelper::getAtomicAbilitiesStr(const QString& separator)
{
    QString s;
#define _ADDSTR(x) s += QString("%1: %2%3").arg(#x).arg(BOOLYESNO(QAtomicInt::x())).arg(separator)
    _ADDSTR(isFetchAndAddNative);
    _ADDSTR(isFetchAndAddWaitFree);
    _ADDSTR(isFetchAndStoreNative);
    _ADDSTR(isFetchAndStoreWaitFree);
    _ADDSTR(isReferenceCountingNative);
    _ADDSTR(isReferenceCountingWaitFree);
    _ADDSTR(isTestAndSetNative);
    _ADDSTR(isTestAndSetWaitFree);
#undef _ADDSTR
    return s.trimmed();
}

void ConstantsHelper::printBuldConstants()
{
    LOG_TRACE_LINE;
    LOG_TRACE("BuldConstants:");
    LOG_TRACE(QString("\n - Features: \n  -- ").append(getFeaturesStr(";\n  -- ")));
    LOG_TRACE(QString("\n - AtomicAbilities: \n  -- ").append(getAtomicAbilitiesStr(";\n  -- ")));
    LOG_TRACE_LINE;
}
