#ifndef UNITTESTBASE_H
#define UNITTESTBASE_H
#include <QString>
#include <QHash>

#include "exceptions/notimplementedexception.h"
#include "exceptions/toolsexception.h"
#include "testcontext.h"
#include "tickcounter.h"
#include "toolsmacro.h"
#include "testhelper.h"
#include "comparators.h"

#define REGISTER_TEST_PLAN m_testPlanName = QString(typeid(this).name())

#define AutoTestName QString(Q_FUNC_INFO)
#define AutoCompareEq(A, E) ((A) == (E)), ("unexpected %1. expected %2 actual %3"_s.arg( #A ).arg(A).arg(E))
#define AutoCompareNEq(A, E) ((A) != (E)), ("unexpected %1. expected NOT %2 actual %3"_s.arg( #A ).arg(A).arg(E))

#define AutoCompareEqDouble(A, E) (Tools::compareDouble((A), (E))), ("unexpected %1. expected %2 actual %3"_s.arg( #A ).arg(A).arg(E))
#define AutoCompareNEqDouble(A, E) (!Tools::compareDouble((A), (E))), ("unexpected %1. expected NOT %2 actual %3"_s.arg( #A ).arg(A).arg(E))

#define AutoCompareEqFloat(A, E) (Tools::compareFloat((A), (E))), ("unexpected %1. expected %2 actual %3"_s.arg( #A ).arg(A).arg(E))
#define AutoCompareNEqFloat(A, E) (!Tools::compareFloat((A), (E))), ("unexpected %1. expected NOT %2 actual %3"_s.arg( #A ).arg(A).arg(E))

#define TestCheckContains(Obj, Key) (Obj.contains(Key)), ("%1 does not contains %2").arg( #Obj ).arg(Key)
#define TestCheckNotContains(Obj, Key) (!(Obj.contains(Key))), ("%1 contains %2").arg( #Obj ).arg(Key)

#define SubTestEq(A, B) if (!subTest(AutoTestName, QString("test %1 value l.%2"_s).arg( #A ).arg(__LINE__),AutoCompareEq(A, B))){break;}
#define SubTestNEq(A, B) if (!subTest(AutoTestName, QString("test %1 value l.%2"_s).arg( #A ).arg(__LINE__),AutoCompareNEq(A, B))){break;}

#define SubTestEqDouble(A, B) if (!subTest(AutoTestName, QString("test %1 value l.%2"_s).arg( #A ).arg(__LINE__), AutoCompareEqDouble(A, B))){break;}
#define SubTestNEqDouble(A, B) if (!subTest(AutoTestName, QString("test %1 value l.%2"_s).arg( #A ).arg(__LINE__),AutoCompareNEqDouble(A, B))){break;}

#define SubTestEqFloat(A, B) if (!subTest(AutoTestName, QString("test %1 value l.%2"_s).arg( #A ).arg(__LINE__), AutoCompareEqFloat(A, B))){break;}
#define SubTestNEqFloat(A, B) if (!subTest(AutoTestName, QString("test %1 value l.%2"_s).arg( #A ).arg(__LINE__),AutoCompareNEqFloat(A, B))){break;}

#define SubTestTrue(A) if (!subTest(AutoTestName, QString("test %1 value l.%2"_s).arg( #A ).arg(__LINE__),AutoCompareEq(A, true))){break;}
#define SubTestFalse(A) if (!subTest(AutoTestName, QString("test %1 value"_s).arg( #A ).arg(__LINE__),AutoCompareEq(A, false))){break;}

#define SubTestContains(Obj, Key) if (!subTest(AutoTestName, QString("test %1 in %2 l.%3"_s).arg( #Key ).arg( #Obj ).arg(__LINE__),TestCheckContains(Obj, Key))){break;}
#define SubTestNotContains(Obj, Key) if (!subTest(AutoTestName, QString("test %1 NOT in %2 l.%3"_s).arg( #Key ).arg( #Obj ).arg(__LINE__),TestCheckNotContains(Obj, Key))){break;}

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
