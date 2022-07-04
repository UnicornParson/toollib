#include "unittestbase.h"
#include "CuteLogger/cuteloggerinc.h"
#include "internal/unittestprinterinternal.h"
#include "tools.h"
#include <cstdlib>

using namespace Tools;

UnitTestBase::UnitTestBase()  noexcept:
    testPlanTime(0),
    m_skipped(false)
{
    m_testPlanName = QString("UnnamedPlan#%1").arg(uintptr_t(this));
    resetResults();
}

UnitTestBase::~UnitTestBase()
{
    resetResults();
}

void UnitTestBase::logTestStart(const QString& testname)
{
    LOG_TRACE(QString("StartTest %1/%2").arg(m_testPlanName, testname));
    m_lastFailReason.clear();
    m_SingleTestTimer.reset();
}

void UnitTestBase::crashExecution(const QString& reason)
{
    qFatal("test system crashed. reason: %s", reason.toUtf8().data());
    std::exit(EXIT_FAILURE);
}

void UnitTestBase::setTestContext(Tools::TestContext* context)
{
    Q_UNUSED(context)
}

Tools::TestContext* UnitTestBase::getContext()
{
    return nullptr;
}

bool UnitTestBase::subTest(const QString& testName, const QString& subTestName, bool condition, const QString& messageOnFail)
{
    QString message = QString("Test %1/%2/%3 %4").arg(m_testPlanName,
                                                      testName,
                                                      subTestName,
                                                      (condition) ? RC_PASSED : RC_FAILED);

    if (condition)
    {
        LOG_TRACE(message);
    }
    else
    {
        LOG_ERROR(message.append(" with reason ").append(messageOnFail));
        m_lastFailReason = messageOnFail;
    }

    QString fullTestName = QString("%1/%2").arg(m_testPlanName, testName);
    if (!subTestName.isEmpty())
    {
        fullTestName += QString("/").append(subTestName);
    }

    TestResult result;
    result.bSuccess = condition;
    result.reason = (condition)?"":QString(messageOnFail);
    result.mstime = m_SingleTestTimer.tickToMs();
    result.strTime = TickCounter::msToString(result.mstime);
    results.insert(fullTestName, result);
    return condition;
}

void UnitTestBase::logTestEnd(const QString& testname, bool bSuccess, const QString& reason)
{
    if (!subTest(testname, "", bSuccess, reason))
    {
        LOG_WARNING(QString("%1 failed").arg(testname));
    }
}

QHash<QString, UnitTestBase::TestResult> UnitTestBase::getResults()
{
    return results;
}

void UnitTestBase::resetResults()
{
    results.clear();
    testPlanTime = 0;
    m_TestPlanTimer.reset();

}

void UnitTestBase::printResults()
{
    printResults(results);
    LOG_TRACE(QString("Time spent %1").arg(getTestPlanTimeStr()));
    LOG_TRACE_LINE;
}

void UnitTestBase::printResults(const QHash<QString, TestResult>& results)
{
    UnitTestPrinterInternal printer;
    QString msg = printer.toPlain(results);
    LOG_TRACE(msg);
}

bool UnitTestBase::resultsToFile(ePrintFormat_t format, const QString& path)
{
    bool bRet = false;
    do
    {
        if (path.isEmpty())
        {
            LOG_WARNING("empty file path");
            break;
        }
    QFile out(path);
    if (!out.open(QIODevice::WriteOnly))
    {
        LOG_WARNING("cannot open "_s + path);
        break;
    }
    out.write(resultsToString(format).toUtf8());
    out.flush();
    out.close();
    bRet = true;
    }
    while(false);
    return bRet;
}

void UnitTestBase::TestPlanSetup()
{

}
void UnitTestBase::TestPlanCleanup()
{

}

QString UnitTestBase::skipReason() const
{
    return m_skipReason;
}

bool UnitTestBase::isSkipped() const
{
    return m_skipped;
}

void UnitTestBase::skip(bool skip, const QString& reason)
{
    LOG_TRACE(QString("%1 test plan %2 reason %3").arg((skip) ? "Skip" : "Restore").arg(m_testPlanName, reason));
    m_skipped = skip;
    m_skipReason = reason;
}

bool UnitTestBase::isPassed() const
{
    bool bPassed = true;
    for (const QString& key: results.keys())
    {
        TestResult result = results.value(key);
        if (!result.bSuccess)
        {
            bPassed = false;
            break;
        }
    }
    return bPassed;
}

void UnitTestBase::appendResults(const QHash<QString, TestResult>& rcMap)
{
    for (const QString& key: rcMap.keys())
    {
        results.insert(key, rcMap.value(key));
    }
}

QString UnitTestBase::resultsToString(ePrintFormat_t format)
{
    UnitTestPrinterInternal printer;
    switch(format)
    {
    case ePrintFormat_t::Plain:{return printer.toPlain(results);}
    case ePrintFormat_t::Json:{return QtJson::serializeStr(printer.toJson(results));}
    case ePrintFormat_t::Html:{return printer.toHtml(results);}
    }
    LOG_WARNING("unknown format");
    return QString();
}

bool UnitTestBase::run()
{
    bool ret = false;
    if (isSkipped())
    {
        LOG_TRACE(QString("test plan %1 is skipped. reason (%2)").arg(m_testPlanName, skipReason()));
    }
    TestPlanSetup();
    m_TestPlanTimer.reset();
    ret = runImpl();
    testPlanTime = m_TestPlanTimer.tickToMs();
    TestPlanCleanup();
    return ret;
}

QString UnitTestBase::getTestPlanTimeStr()
{
    return TickCounter::msToString(testPlanTime);
}

quint64 UnitTestBase::getTestPlanTimeMs()
{
    return testPlanTime;
}
