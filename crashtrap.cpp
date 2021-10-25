#include "crashtrap.h"

#ifdef FEATURE_BOOST
#ifndef NO_CRUSH_TRAP

#include "tools.h"
#include <signal.h>
#include <QPair>
#include <QDir>
#define BOOST_STACKTRACE_LINK
#include <boost/stacktrace.hpp>
using namespace Tools;
Q_DECL_CONSTEXPR size_t dumpBufSize = 1024 * 1024 * 2; //2M
typedef QPair<int, QLatin1String> SignalPait_t;
#define __MAKE_SIG_PAIR(sig) QPair<int, QLatin1String>(sig, QLatin1String( #sig ))
const QList<SignalPait_t> HANDLED_SIGNALS = {__MAKE_SIG_PAIR(SIGABRT),
                                             __MAKE_SIG_PAIR(SIGFPE),
                                             __MAKE_SIG_PAIR(SIGILL),
                                             __MAKE_SIG_PAIR(SIGSEGV),
                                             __MAKE_SIG_PAIR(SIGTERM)};
#undef __MAKE_SIG_PAIR
#define CHR_SRACKTRACE (to_string(boost::stacktrace::stacktrace()).c_str())
CrashTrap::CrashTrap():
    m_crashDumpBase(".")
{
    m_extHandler = [](int /*sig*/, const QString& /* message */){};
}


CrashTrap::~CrashTrap()
{

}

CrashTrap& CrashTrap::self()
{
    static CrashTrap instance;
    return instance;
}

void CrashTrap::printStackTrace()
{
    LOG_ERROR("trace: %s", CHR_SRACKTRACE);
}

void CrashTrap::setExternalCrashHandler(ExternalCrashHandler_t func)
{
    self().m_extHandler = func;
}

void CrashTrap::crashHandler(int signal, const QString& msg)
{
    LOG_TRACE_LINE;
    LOG_ERROR(msg);
    self().m_extHandler(signal, msg);
    printStackTrace();
    QString dumpname = QString("%1/%2.%3.bt.txt").arg(self().m_crashDumpBase).arg(THREAD_STR_ID).arg(time(nullptr));
    bool rc = makeDump(dumpname);
    if (rc)
    {
        LOG_TRACE(QString("dump is saved to ").append(dumpname));
    }
    else
    {
        LOG_TRACE(QString("cannot save dump to ").append(dumpname));
    }
    LOG_TRACE_LINE;
}

void CrashTrap::signalHandler( int signum )
{
    QString msg = QString("unknown signal %d").arg(signum);
    foreach(SignalPait_t p, HANDLED_SIGNALS)
    {
        if (p.first == signum)
        {
            msg = QString("on signal %1(%2)").arg(p.second).arg(p.first);
            break;
        }
    }
    crashHandler(signum, msg);
}

bool CrashTrap::init(const QString &crashDumpBase)
{
    QString base = CTools::removeLastSlash(crashDumpBase);
    if (!base.isEmpty())
    {
        self().m_crashDumpBase = base;
    }
    if (!QDir().exists(self().m_crashDumpBase))
    {
        bool mkdirRc = QDir().mkdir(self().m_crashDumpBase);
        if (!mkdirRc)
        {
            LOG_ERROR("cannot create %s ", SCHR(self().m_crashDumpBase));
            return false;
        }
    }
    LOG_TRACE(QString("crash dumps will be saved to ").append(crashDumpBase));
    foreach(SignalPait_t p, HANDLED_SIGNALS)
    {
        LOG_TRACE(QString("add trap to %1(%2)").arg(p.second).arg(p.first));
        ::signal(p.first, &CrashTrap::signalHandler);
    }
    return true;
}

void CrashTrap::stableAssert(bool conditionResult, const QString& messageOnFail)
{
    if (conditionResult)
    {
        return;
    }
    crashHandler(0, QString("ASSERT: ").append(messageOnFail));
    exit(SIGABRT);
}

bool CrashTrap::makeDump(const QString& path)
{
    size_t sz = boost::stacktrace::safe_dump_to("./ddd.dump");
    return (sz != 0);
    QFile f(CTools::pathNormalize(path));
    bool ret = false;
    do
    {
        if (!f.open(QIODevice::WriteOnly))
        {
            break;
        }
        ret = true;
        char* buf = new char[dumpBufSize];
        size_t sz = boost::stacktrace::safe_dump_to(buf, dumpBufSize);
        buf[dumpBufSize - 1] = 0;
        if (sz == 0)
        {
            ret = false;
        }
        sz = f.write(buf, strnlen(buf, dumpBufSize));
        if (sz == 0)
        {
            ret = false;
        }
        f.flush();
        f.close();
    }
    while(false);
    return ret;
}
#endif // ifndef NO_CRUSH_TRAP
#endif // FEATURE_BOOST

