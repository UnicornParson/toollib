#include "toolstest.h"
#include "test/moptionaltest.h"
#include "test/toollibtestcontext.h"
#include "tools.h"
#include <bitset>
#include "lzma/lzma.h"
#include "Updatable.h"
#include <functional>
#include "moptionaltest.h"
#include "rangestest.h"

using namespace Tools;
using namespace QtJson;

namespace
{
MAYBE_UNUSED_ATTR Q_DECL_CONSTEXPR int ITERATION_COUNT = 200;
MAYBE_UNUSED_ATTR Q_DECL_CONSTEXPR int TEST_BUFF_SIZE = 2000;
MAYBE_UNUSED_ATTR Q_DECL_CONSTEXPR int MAX_LZMA_LEVEL = 9;
MAYBE_UNUSED_ATTR Q_DECL_CONSTEXPR int MIN_LZMA_LEVEL = 0;
MAYBE_UNUSED_ATTR Q_DECL_CONSTEXPR uint UPDATER_ROUNDS_COUNT = 50;
MAYBE_UNUSED_ATTR Q_DECL_CONSTEXPR int  DEFAULT_DELAY = 100;

CONST_LITERAL TestExceptionText("TestExceptionText");
CONST_LITERAL TestRuntimeExceptionText("TestRuntimeExceptionText");
using dirRecord = std::tuple<QLatin1String, QLatin1String, bool>;

namespace DirData
{
const mlist_t<dirRecord> DIR_RECORDS =
{
    std::make_tuple(QLatin1String("/aaa/dddd/"), QLatin1String("/aaa/dddd"), true),
    std::make_tuple(QLatin1String("/aaa/dddd"), QLatin1String("/aaa/dddd"), false),
    std::make_tuple(QLatin1String("/aaa/dddd\\"), QLatin1String("/aaa/dddd"), true),
    std::make_tuple(QLatin1String("/aaa/dddd\\"), QLatin1String("/aaa/dddd"), true)
};

inline QLatin1String _getOriginalDirStr(const dirRecord& record)
{
    return std::get<0>(record);
}

inline QLatin1String _getExpectedDirStr(const dirRecord& record)
{
    return std::get<1>(record);
}

inline bool _getExpectedHasSlash(const dirRecord& record)
{
    return std::get<2>(record);
}
} // namespace DirData

} // constants namespace

QString DownloaderResultFields::toString(DownloaderResultFields::e f)
{
    return Tools::QEnumToString<DownloaderResultFields::e>(f);
}

CONST_LITERAL mixedPath("C:\\aaa//ssss/ffff\\\\d");
#ifdef PLATFORM_WINDOWS
CONST_LITERAL normalizedPath("C:\\aaa\\ssss\\ffff\\d");
#else
CONST_LITERAL normalizedPath("C:/aaa/ssss/ffff/d");
#endif

ToolsTest::ToolsTest():
    ToollibTestBase(),
    m_onUpdatedValue(0),
    m_onUpdateFinishedValue(0),
    m_onUpdateFinishedPrev(0),
    m_onUpdatedPrev(0)
{
    setTestContext(new ToollibTestContext());
    REGISTER_TEST_PLAN;
}

ToolsTest::~ToolsTest()
{
    delete m_context;
    m_context = nullptr;
}

void ToolsTest::TestPlanSetup()
{

}

void ToolsTest::TestPlanCleanup()
{

}



bool ToolsTest::runImpl()
{
    cToolsTest();
    dirPathUtilsTest();
    execTestPlan<MOptionalTest>();
    jsonTest();
    lzmaTest();

    // test disabled. qt lambda issue
    //updatableTest();

    downloaderResult();
    functionSchedulerTest();
    execTestPlan<RangesTest>();
    return true;
}

void ToolsTest::cToolsTest()
{
    logTestStart(AutoTestName);
    m_lastFailReason.clear();
    bool result = false;
    do
    {

        //qStringToPChar
        QString msg = CTools::getUUID();
        if (!subTest(AutoTestName, "UUID", (!msg.isEmpty()), "empty UUID"))
        {
            break;
        }
        char* psMsg = SCHR(msg);
        if (!subTest(AutoTestName, "SCHR", (psMsg != nullptr), "qStringToPChar. convertation error. nullptr result"))
        {
            break;
        }

        QString normalizedPathActual = CTools::pathNormalize(mixedPath);
        if (!subTest(AutoTestName,
                     "pathNormalize",
                     (normalizedPathActual == normalizedPath),
                     QString("pathNormalize. convertation error. expected: %1, actual: %2").arg(normalizedPath, normalizedPathActual)))
        {
            break;
        }

        //isStringlistsEquals
        QStringList first;
        QStringList second;
        for (int i = 0; i < ITERATION_COUNT; ++i)
        {
            QString str = QString("%1.%2").arg(CTools::getUUID()).arg(i);
            first.append(str);
            second.append(str);
        }

        bool rc = CTools::isStringlistsEquals(first, second);
        if (!subTest(AutoTestName, "isStringlistsEquals/same", rc, "comparation failed (same lists)"))
        {
            break;
        }
        second.append("eeefsewfe");

        rc = CTools::isStringlistsEquals(first, second);
        if (!subTest(AutoTestName, "isStringlistsEquals/different", !rc, "comparation failed (different lists)"))
        {
            break;
        }

        QHash<QString, QString> hMapFirst;
        QHash<QString, QString> hMapSecond;
        for (int i = 0; i < ITERATION_COUNT; ++i)
        {
            QString key = QString("%1.%2").arg(CTools::getUUID()).arg(i);
            QString val = QString("%1.%2").arg(CTools::getUUID()).arg(i);
            hMapFirst.insert(key, val);
            hMapSecond.insert(key, val);
        }
        rc = CompareMap<QHash <QString, QString>, QString>(hMapFirst, hMapSecond);
        if (!subTest(AutoTestName, "CompareMap/same", rc, "hash map comparation failed (same maps)"))
        {
            break;
        }

        hMapSecond.insert(CTools::getUUID(), CTools::getUUID());
        rc = CompareMap<QHash <QString, QString>, QString>(hMapFirst, hMapSecond);
        if (!subTest(AutoTestName, "CompareMap/different", !rc, "hash map comparation failed (different maps)"))
        {
            LOG_ERROR("hash map comparation failed (different maps)");
            break;
        }

        //compare maps with double value
        QMap<QString, double> dMapFirst;
        QMap<QString, double> dMapSecond;
        for (int i = 0; i < ITERATION_COUNT; ++i)
        {
            QString key = QString("%1.%2").arg(CTools::getUUID()).arg(i);
            double val = double(rand()) / 333.3;
            dMapFirst.insert(key, val);
            dMapSecond.insert(key, val);
        }
        rc = CompareMapD<QMap <QString, double>, QString>(dMapFirst, dMapSecond);
        if (!subTest(AutoTestName, "CompareMapD/same", rc, "double map comparation failed (same maps)"))
        {
            break;
        }

        dMapSecond.insert(CTools::getUUID(), (double(rand()) / 333.3));
        rc = CompareMapD<QMap <QString, double>, QString>(dMapFirst, dMapSecond);
        if (!subTest(AutoTestName, "CompareMapD/different", !rc, "double map comparation failed (different maps)"))
        {
            break;
        }
        result = true;
    }
    while(false);
    logTestEnd(AutoTestName, result, m_lastFailReason);
}
void ToolsTest::jsonTest()
{
    logTestStart(AutoTestName);
    m_lastFailReason.clear();
    bool result = false;
    do
    {
        JsonObject firstObj;
        JsonArray firstArray;
        for (int i = 0; i < ITERATION_COUNT; ++i)
        {
            QString str = QString("%1.%2").arg(time(nullptr)).arg(i);
            firstArray.append(str);
            firstObj.insert(str, QVariant(i));
        }
        if (!subTest(AutoTestName, "JSONfill/emptyTest/object", !firstObj.isEmpty(), "json object is empty"))
        {
            break;
        }
        if (!subTest(AutoTestName, "JSONfill/emptyTest/array", !firstArray.isEmpty(), "json array is empty"))
        {
            break;
        }
        bool success = false;
        QString firstObjStr = QtJson::serializeStr(firstObj, success);

        if (!subTest(AutoTestName, "JSONserializeStr/success/object", success, "json serialization failed"))
        {
            break;
        }
        if (!subTest(AutoTestName, "JSONserializeStr/emptyTest/object", !firstObjStr.isEmpty(), "json serialization string is empty"))
        {
            break;
        }

        QString firstArrayStr = QtJson::serializeStr(firstArray, success);
        if (!subTest(AutoTestName, "JSONserializeStr/success/array", success, "json serialization failed"))
        {
            break;
        }
        if (!subTest(AutoTestName, "JSONserializeStr/emptyTest/array", !firstArrayStr.isEmpty(), "json serialization string is empty"))
        {
            break;
        }

        QtJson::JsonObject secondObj = QtJson::parse(firstObjStr, success).toMap();
        if (!subTest(AutoTestName, "JSONparse/success/object", success, "json parse failed"))
        {
            break;
        }
        if (!subTest(AutoTestName, "JSONparse/emptyTest/object", !secondObj.isEmpty(), "json parse result is empty"))
        {
            break;
        }
        if (!subTest(AutoTestName, "JSONparse/ParseCompare/object", CompareJsonObj(firstObj, secondObj), "json objects are different"))
        {
            break;
        }

        result = true;
    }
    while(false);
    logTestEnd(AutoTestName, result, m_lastFailReason);
}

void ToolsTest::lzmaTest()
{
    logTestStart(AutoTestName);
    m_lastFailReason.clear();
    bool result = true;
    do
    {
        QByteArray in;
        for (int i = 0; i < TEST_BUFF_SIZE; ++i )
        {
            in.append(static_cast<char>(i % 0xff));
        }

        for (int level = MIN_LZMA_LEVEL; level <= MAX_LZMA_LEVEL; ++level)
        {
            QByteArray out1;
            unsigned char lzmaprop = 0;
            int encodeRc = LZMA::encode2(in, &out1, &lzmaprop, level);
            if (!subTest(AutoTestName,
                         QString("LZMA/Encode/level%1/ok").arg(level),
                         (encodeRc == LZMARC::SZ_OK),
                         QString("encode failed errcode: %1").arg(encodeRc)))
            {
                result = false;
                break;
            }
            if (!subTest(AutoTestName,
                         QString("LZMA/Encode/level%1/outEmpty").arg(level),
                         (!out1.isEmpty()),
                         "output is empty"))
            {
                result = false;
                break;
            }

            QByteArray decodedBuf;
            int decodeRc= LZMA::decode2(out1, &decodedBuf, static_cast<quint64>(in.size()), lzmaprop);
            if (!subTest(AutoTestName,
                         QString("LZMA/Decode/level%1/ok").arg(level),
                         (decodeRc == LZMARC::SZ_OK),
                         QString("decode failed errcode: %1").arg(decodeRc)))
            {
                result = false;
                break;
            }
            if (!subTest(AutoTestName,
                         QString("LZMA/Decode/level%1/outEmpty").arg(level),
                         (!decodedBuf.isEmpty()),
                         "output is empty"))
            {
                result = false;
                break;
            }
            if (!subTest(AutoTestName,
                         QString("LZMA/Decode/level%1/compare.ratio(%2/%3").arg(level).arg(out1.size()).arg(decodedBuf.size()),
                         (decodedBuf.compare(in) == 0),
                         "buffers are different"))
            {
                result = false;
                break;
            }
        }
    }
    while(false);
    logTestEnd(AutoTestName, result, m_lastFailReason);
}

uint ToolsTest::longCalc(uint v)
{
    for (uint i = 0; i < UPDATER_ROUNDS_COUNT; ++i)
    {
        v = qHash(v + i, i);
    }
    return v;
}

void ToolsTest::dirPathUtilsTest()
{
    logTestStart(AutoTestName);
    m_lastFailReason.clear();
    bool result = true;

    for (const dirRecord& record: DirData::DIR_RECORDS)
    {
        QString orig = DirData::_getOriginalDirStr(record);
        QString expected = DirData::_getExpectedDirStr(record);
        bool hasSlash = DirData::_getExpectedHasSlash(record);
        if (!subTest(AutoTestName,
                     QString("PathUtils/").append(orig),
                     (CTools::hasLastSlash(orig) == hasSlash),
                     QString("invalid last slash detection. path %1, expected %2")
                     .arg(orig).arg(hasSlash)))
        {
            result = false;
            break;
        }

        if (!subTest(AutoTestName,
                     QString("PathUtils/").append(orig),
                     (CTools::removeLastSlash(orig) == expected),
                     QString("cannot remove last slash. original path %1, expected %2")
                     .arg(orig, expected)))
        {
            result = false;
            break;
        }
    }

    logTestEnd(AutoTestName, result, m_lastFailReason);
}

void ToolsTest::downloaderResult()
{
    logTestStart(AutoTestName);
    m_lastFailReason.clear();
    bool result = true;
    do
    {
        // TODO: implement
        //std::bitset<eDownloaderResultFields::FIELDS_COUNT> changes;
    }
    while(false);
    logTestEnd(AutoTestName, result, m_lastFailReason);
}

void ToolsTest::functionSchedulerTest()
{
    logTestStart(AutoTestName);
    m_lastFailReason.clear();

    bool result = false;
    do
    {
        FunctionScheduler::reset();
        FunctionScheduler::setVerbose(false);
        if (!subTest(AutoTestName,
                     "verboseFalse",
                     !FunctionScheduler::verbose(),
                     QString("invalid verbose (read after set) actual %1, expected %2")
                     .arg(FunctionScheduler::verbose()).arg(false))){break;}
        FunctionScheduler::setVerbose(true);
        if (!subTest(AutoTestName,
                     "verboseTrue",
                     FunctionScheduler::verbose(),
                     QString("invalid verbose (read after set) actual %1, expected %2")
                     .arg(FunctionScheduler::verbose()).arg(true))){break;}
        int exceptionsRaised = 0;
        int runtimeErrorsRaised = 0;
        FunctionScheduler::ExceptonCallback_t ec = [&exceptionsRaised](const std::exception& e)
        {
            LOG_DEBUG("ExceptonCallback %s", e.what());
            ++exceptionsRaised;
        };
        FunctionScheduler::RuntimeErrorCallback_t rec = [&runtimeErrorsRaised](const std::runtime_error& re)
        {
            LOG_DEBUG("RuntimeErrorCallback %s", re.what());
            ++runtimeErrorsRaised;
        };
        const int delayOffset = 500;

        auto throwableRoutine = []()
        {
            unsigned long delay = (mrand() % DEFAULT_DELAY) + delayOffset;
            mssleep(delay);
            LOG_DEBUG("throw test exception");
            throw TestExceptionText.data();
        };
        auto throwableRuntimeExceptionRoutine = []()
        {
            unsigned long delay = (mrand() % DEFAULT_DELAY) + delayOffset;
            mssleep(delay);
            LOG_DEBUG("throw test exception");
            throw std::runtime_error(TestRuntimeExceptionText.data());
        };

        for (int i = 0; i < ITERATION_COUNT; ++i)
        {
            scheduleOnce(throwableRoutine, DEFAULT_DELAY, ec, rec);
            scheduleOnce(throwableRuntimeExceptionRoutine, DEFAULT_DELAY, ec, rec);
        }
        uint waitingCount = 0;
        while(!FunctionScheduler::isEmpty())
        {
            LOG_DEBUG("wait %u", waitingCount);
            waitingCount++;
            mssleep(DEFAULT_DELAY);
        }

        if (!subTest(AutoTestName,
                     "exceptionsRaised",
                     exceptionsRaised == ITERATION_COUNT,
                     QString("missed exceptions actual %1, expected %2")
                     .arg(exceptionsRaised).arg(ITERATION_COUNT))){break;}
        if (!subTest(AutoTestName,
                     "exceptionsRaised",
                     runtimeErrorsRaised == ITERATION_COUNT,
                     QString("missed runtimeexceptions actual %1, expected %2")
                     .arg(runtimeErrorsRaised).arg(ITERATION_COUNT))){break;}
        auto cleanRoutine = []()
        {
            LOG_DEBUG("cleanRoutine_%llu", msTime());
        };

        FunctionScheduler::reset();
        for (int i = 0; i < ITERATION_COUNT; ++i)
        {
            setInterval(cleanRoutine, DEFAULT_DELAY, ec, rec);
        }

        // now new exceptions should be raised
        if (!subTest(AutoTestName,
                     "noNewExceptionsRaised",
                     ((exceptionsRaised == ITERATION_COUNT) && (runtimeErrorsRaised == ITERATION_COUNT)),
                     QString("unexpected exceptions"))){break;}
        FunctionScheduler::destroy();
        if (!subTest(AutoTestName,
                     "emptyAfterDestroy",
                     FunctionScheduler::isEmpty(),
                     QString("FunctionScheduler not empty. count: %1").arg(FunctionScheduler::refs()))){break;}
        result = true;
    }
    while(false);
    logTestEnd(AutoTestName, result, m_lastFailReason);
}

uint ToolsTest::updater(const Updatable<uint>::UpdatableCtx& ctx)
{
    TickCounter t;
    uint v = longCalc(ctx.prev);
    if (m_updaterExpectedValue == v)
    {
        LOG_TRACE("updater. new: %u, prev: %u. time: %llu ms", v, ctx.prev, t.tickToMs());
    }
    else
    {
        LOG_ERROR("updater. not expected result. new: %u prev: %u. time: %llu ms", v, ctx.prev, t.tickToMs());
    }
    return v;
};
void ToolsTest::onValueUpdatedFunct(const uint& newVal, const uint& prevVal)
{
    LOG_TRACE("OnValueUpdatedFunct. new: %u, prev: %u", newVal, prevVal);
    m_onUpdatedValue = newVal;
    m_onUpdatedPrev = prevVal;
}
void ToolsTest::onValueUpdateFinishedFunct(const uint& newVal, const uint& prevVal)
{
    LOG_TRACE("OnValueUpdateFinishedFunct. new: %u, prev: %u", newVal, prevVal);
    m_onUpdateFinishedValue = newVal;
    m_onUpdateFinishedPrev = prevVal;
}
void ToolsTest::updatableTest()
{
    logTestStart(AutoTestName);
    m_lastFailReason.clear();
    bool result = true;
    do
    {
        auto OnValueUpdatedFunct = [this](const uint& newVal, const uint& prevVal)
        {
            onValueUpdatedFunct(newVal, prevVal);
        };
        auto OnValueUpdateFinishedFunct = [this](const uint& newVal, const uint& prevVal)
        {
            onValueUpdateFinishedFunct(newVal, prevVal);
        };

        const uint firstValue = 10;
        Updatable<uint> upd(firstValue);
        m_updaterExpectedValue = longCalc(firstValue);

        upd.setOnValueUpdatedFunct(OnValueUpdatedFunct);
        upd.setOnUpdateFinishedFunct(OnValueUpdateFinishedFunct);
        const Updatable<uint>::UpdaterFunct_t& updater = std::bind(&ToolsTest::updater, this, std::placeholders::_1);
        upd.setUpdater(updater);

        if (!subTest(AutoTestName,
                     QString("Updatable/beforeRun"),
                     (upd.get() == firstValue),
                     QString("unexpected value after set. expected %1, actual %2").arg(firstValue).arg(upd.get())))
        {
            result = false;
            break;
        }
        upd.requestUpdate();
        upd.wait();

        if (!subTest(AutoTestName,
                     QString("Updatable/onUpdatedValue"),
                     (m_onUpdatedValue == m_updaterExpectedValue),
                     QString("unexpected value received in setOnValueUpdatedFunct callback. expected %1, actual %2")
                     .arg(m_updaterExpectedValue).arg(m_onUpdatedValue)))
        {
            result = false;
            break;
        }

        if (!subTest(AutoTestName,
                     QString("Updatable/onUpdatedPrev"),
                     (m_onUpdatedPrev == firstValue),
                     QString("unexpected prev value received in setOnValueUpdatedFunct callback. expected %1, actual %2")
                     .arg(firstValue).arg(m_onUpdatedPrev)))
        {
            result = false;
            break;
        }

        if (!subTest(AutoTestName,
                     QString("Updatable/onUpdateFinishedValue"),
                     (m_onUpdateFinishedPrev == firstValue),
                     QString("unexpected prev received in setOnUpdateFinishedFunct callback. expected %1, actual %2")
                     .arg(firstValue).arg(m_onUpdateFinishedPrev)))
        {
            result = false;
            break;
        }

        if (!subTest(AutoTestName,
                     QString("Updatable/onUpdateFinishedValue"),
                     (m_onUpdateFinishedValue == m_updaterExpectedValue),
                     QString("unexpected value received in setOnUpdateFinishedFunct callback. expected %1, actual %2")
                     .arg(m_updaterExpectedValue).arg(m_onUpdateFinishedValue)))
        {
            result = false;
            break;
        }

        if (!subTest(AutoTestName,
                     QString("Updatable/getAfterUpdate"),
                     (upd == m_updaterExpectedValue),
                     QString("unexpected value after update expected %1, actual %2")
                     .arg(m_updaterExpectedValue).arg(m_onUpdateFinishedValue)))
        {
            result = false;
            break;
        }
    }
    while(false);
    logTestEnd(AutoTestName, result, m_lastFailReason);
}
