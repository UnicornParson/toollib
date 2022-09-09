#ifndef UNITTESTBASE_H
#define UNITTESTBASE_H
#include <QString>
#include <QHash>

#include "exceptions/notimplementedexception.h"
#include "exceptions/toolsexception.h"
#include "testcontext.h"
#include "tickcounter.h"
#include "toolsmacro.h"

#define REGISTER_TEST_PLAN m_testPlanName = QString(typeid(this).name())

class UnitTestBase
{
public:
    struct TestResult
    {
        bool bSuccess;
        QString reason;
        QString strTime;
        quint64 mstime;
    };

    enum class ePrintFormat_t
    {
        Plain,
        Json,
        Html
    };

    UnitTestBase() noexcept;
    virtual ~UnitTestBase();
    static void printResults(const QHash<QString, TestResult>& results);

    ATTR_NODISCARD bool run();
    ATTR_NODISCARD QHash<QString, TestResult> getResults();
    void appendResults(const QHash<QString, TestResult>& rcMap);
    void resetResults();
    void printResults();
    void skip(bool skip = true, const QString& reason = QString());
    ATTR_NODISCARD bool isPassed() const;
    ATTR_NODISCARD bool isSkipped() const;
    ATTR_NODISCARD QString skipReason() const;
    ATTR_NODISCARD QString getTestPlanTimeStr();
    ATTR_NODISCARD quint64 getTestPlanTimeMs();
    virtual void TestPlanSetup();
    virtual void TestPlanCleanup();
    virtual Tools::TestContext* getContext();
    virtual void setTestContext(Tools::TestContext* context);
    QString resultsToString(ePrintFormat_t format);
    bool resultsToFile(ePrintFormat_t format, const QString& path);
protected:
    [[ noreturn ]] void crashExecution(const QString& reason);

    template < class T >
    void execTestPlan()
    {
        bool ret = false;
        T plan;
        QString planName(typeid(plan).name());
        QString basicTestName = QString("TestPlan[%1]").arg(planName);
        QString exceptionTestName = QString("TestPlanNoExcept[%1]").arg(planName);
        logTestStart(basicTestName);
        try
        {
            plan.setTestContext(getContext());
            ret = plan.run();
            appendResults(plan.getResults());
        }
        catch (const Tools::NotImplementedException& e)
        {
            logTestStart(exceptionTestName);
            logTestEnd(exceptionTestName, false, e.toString());
            ret = false;
        }
        catch (const Tools::ToolLibException& e)
        {
            logTestStart(exceptionTestName);
            logTestEnd(exceptionTestName, false, e.toString());
            ret = false;
        }
        logTestEnd(basicTestName, ret, "");
    }

    virtual bool runImpl() = 0;
    void logTestStart(const QString& testname);
    void logTestEnd(const QString& testname, bool bSuccess, const QString& reason = QString());
    ATTR_NODISCARD bool subTest(const QString& testName,
                                const QString& subTestName,
                                bool condition,
                                const QString& messageOnFail = QString());


    Tools::TickCounter m_TestPlanTimer;
    Tools::TickCounter m_SingleTestTimer;
    QHash<QString, TestResult> results;
    quint64 testPlanTime;
    QString m_lastFailReason;
    bool m_skipped;
    QString m_skipReason;
    QString m_testPlanName;
};

#endif // UNITTESTBASE_H
