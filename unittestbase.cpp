#include "unittestbase.h"
#include "CuteLogger/cuteloggerinc.h"
using namespace Tools;

CONST_LITERAL RC_PASSED("  PASSED  ");
CONST_LITERAL RC_FAILED("> FAILED <");

UnitTestBase::UnitTestBase():
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
    LOG_TRACE_LINE;
    LOG_TRACE("TEST RESULTS:");
    LOG_TRACE_LINE;
    uint passed = 0;
    uint failed = 0;
    QStringList failedMessages;
    QStringList passedMessages;
    foreach(QString key, results.keys())
    {
        QString reasonMsg = "";

        TestResult result = results.value(key);
        if (!result.bSuccess)
        {
            reasonMsg = QString("Reason: %3. ").arg((result.reason.isEmpty())?"{EMPTY REASON}":result.reason);
        }
        QString message = QString("%1 - Test %2. %3. Executing time: %4")
                .arg((result.bSuccess) ? RC_PASSED : RC_FAILED,
                     key, result.reason, result.strTime);
        if (result.bSuccess)
        {
            LOG_TRACE(message);
            passedMessages.append(message);
            passed++;
        }
        else
        {
            failedMessages.append(message);
            failed++;
        }
    }
    passedMessages.sort();
    foreach(QString m, passedMessages)
    {
        LOG_ERROR(m);
    }
    LOG_TRACE_LINE;
    QString summary = QString("TEST RESULTS: %1, count: %2, passed: %3, failed: %4")
            .arg((failed == 0) ? RC_PASSED : RC_FAILED)
            .arg(results.size())
            .arg(passed)
            .arg(failed);

    if (failed == 0)
    {
        LOG_TRACE(summary);
    }
    else
    {
        LOG_TRACE_LINE;
        LOG_ERROR("FAILED TESTS:");
        failedMessages.sort();
        foreach(QString m, failedMessages)
        {
            LOG_ERROR(m);
        }
        LOG_TRACE_LINE;
        LOG_ERROR(summary);
    }
    LOG_TRACE_LINE;
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
    foreach(QString key, results.keys())
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
    foreach(QString key, rcMap.keys())
    {
        results.insert(key, rcMap.value(key));
    }
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
