#ifndef TOOLSTEST_H
#define TOOLSTEST_H
#include "unittestbase.h"
#include "Updatable.h"
#include <QObject>

namespace Tools
{

class DownloaderResultFields: QObject
{
    Q_OBJECT
    Q_ENUMS(e)
public:
    enum e
    {
        rc = 0,
        serverCode,
        resultSize,
        targetType,
        rawResult,
        src,
        dst,
        requestDelay,
        FIELDS_COUNT
    };
    Q_ENUM(e)

    static QString toString(DownloaderResultFields::e f);
}; // class DownloaderResultFields

typedef DownloaderResultFields::e eDownloaderResultFields;

class ToolsTest : public UnitTestBase
{
public:
    ToolsTest();
    virtual ~ToolsTest();
    void TestPlanSetup();
    void TestPlanCleanup();
protected:
    bool runImpl();
    void mOptionalTest();
    void cToolsTest();
    void jsonTest();
    void lzmaTest();
    void updatableTest();
    void dirPathUtilsTest();
    void downloaderResult();
    void functionSchedulerTest();
private:
    uint updater(const Updatable<uint>::UpdatableCtx& ctx);

    uint longCalc(uint v);
    uint m_onUpdatedValue;
    uint m_onUpdateFinishedValue;
    uint m_updaterExpectedValue;
    uint m_onUpdateFinishedPrev;
    uint m_onUpdatedPrev;
}; // class ToolsTest
} // namespace Tools
#endif // TOOLSTEST_H
