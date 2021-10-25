#include "toolstest.h"
#include "tools.h"
#include <bitset>
#include "lzma/lzma.h"
#include "Updatable.h"

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
    const QList<dirRecord> DIR_RECORDS =
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
  m_onUpdatedValue(0),
  m_onUpdateFinishedValue(0),
  m_onUpdateFinishedPrev(0),
  m_onUpdatedPrev(0)
{
  REGISTER_TEST_PLAN;
}

ToolsTest::~ToolsTest()
{

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
  mOptionalTest();
  jsonTest();
  lzmaTest();
  updatableTest();
  downloaderResult();
  functionSchedulerTest();
  return true;
}

void ToolsTest::cToolsTest()
{
  logTestStart(QString(Q_FUNC_INFO));
  m_lastFailReason.clear();
  bool result = false;
  do
  {

    //qStringToPChar
    QString msg = CTools::getUUID();
    if (!subTest(QString(Q_FUNC_INFO), "UUID", (!msg.isEmpty()), "empty UUID"))
    {
      break;
    }
    char* psMsg = SCHR(msg);
    if (!subTest(QString(Q_FUNC_INFO), "SCHR", (psMsg != nullptr), "qStringToPChar. convertation error. nullptr result"))
    {
      break;
    }

    QString normalizedPathActual = CTools::pathNormalize(mixedPath);
    if (!subTest(QString(Q_FUNC_INFO),
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
    if (!subTest(QString(Q_FUNC_INFO), "isStringlistsEquals/same", rc, "comparation failed (same lists)"))
    {
      break;
    }
    second.append("eeefsewfe");

    rc = CTools::isStringlistsEquals(first, second);
    if (!subTest(QString(Q_FUNC_INFO), "isStringlistsEquals/different", !rc, "comparation failed (different lists)"))
    {
      break;
    }

    QHash<QString, QString> hMapFirst;
    QHash<QString, QString> hMapSecond;
    for(int i = 0; i < ITERATION_COUNT; ++i)
    {
      QString key = QString("%1.%2").arg(CTools::getUUID()).arg(i);
      QString val = QString("%1.%2").arg(CTools::getUUID()).arg(i);
      hMapFirst.insert(key, val);
      hMapSecond.insert(key, val);
    }
    rc = CompareMap<QHash <QString, QString>, QString>(hMapFirst, hMapSecond);
    if (!subTest(QString(Q_FUNC_INFO), "CompareMap/same", rc, "hash map comparation failed (same maps)"))
    {
      break;
    }

    hMapSecond.insert(CTools::getUUID(), CTools::getUUID());
    rc = CompareMap<QHash <QString, QString>, QString>(hMapFirst, hMapSecond);
    if (!subTest(QString(Q_FUNC_INFO), "CompareMap/different", !rc, "hash map comparation failed (different maps)"))
    {
      LOG_ERROR("hash map comparation failed (different maps)");
      break;
    }

    //compare maps with double value
    QMap<QString, double> dMapFirst;
    QMap<QString, double> dMapSecond;
    for(int i = 0; i < ITERATION_COUNT; ++i)
    {
      QString key = QString("%1.%2").arg(CTools::getUUID()).arg(i);
      double val = double(rand()) / 333.3;
      dMapFirst.insert(key, val);
      dMapSecond.insert(key, val);
    }
    rc = CompareMapD<QMap <QString, double>, QString>(dMapFirst, dMapSecond);
    if (!subTest(QString(Q_FUNC_INFO), "CompareMapD/same", rc, "double map comparation failed (same maps)"))
    {
      break;
    }

    dMapSecond.insert(CTools::getUUID(), (double(rand()) / 333.3));
    rc = CompareMapD<QMap <QString, double>, QString>(dMapFirst, dMapSecond);
    if (!subTest(QString(Q_FUNC_INFO), "CompareMapD/different", !rc, "double map comparation failed (different maps)"))
    {
      break;
    }
    result = true;
  }
  while(false);
  logTestEnd(QString(Q_FUNC_INFO), result, m_lastFailReason);
}

void ToolsTest::mOptionalTest()
{
  // TODO: implement
}

void ToolsTest::jsonTest()
{
  logTestStart(QString(Q_FUNC_INFO));
  m_lastFailReason.clear();
  bool result = false;
  do
  {
    JsonObject firstObj;
    JsonArray firstArray;
    for(int i = 0; i < ITERATION_COUNT; ++i)
    {
      QString str = QString("%1.%2").arg(time(nullptr)).arg(i);
      firstArray.append(str);
      firstObj.insert(str, QVariant(i));
    }
    if (!subTest(QString(Q_FUNC_INFO), "JSONfill/emptyTest/object", !firstObj.isEmpty(), "json object is empty"))
    {
      break;
    }
    if (!subTest(QString(Q_FUNC_INFO), "JSONfill/emptyTest/array", !firstArray.isEmpty(), "json array is empty"))
    {
      break;
    }
    bool success = false;
    QString firstObjStr = QtJson::serializeStr(firstObj, success);

    if (!subTest(QString(Q_FUNC_INFO), "JSONserializeStr/success/object", success, "json serialization failed"))
    {
      break;
    }
    if (!subTest(QString(Q_FUNC_INFO), "JSONserializeStr/emptyTest/object", !firstObjStr.isEmpty(), "json serialization string is empty"))
    {
      break;
    }

    QString firstArrayStr = QtJson::serializeStr(firstArray, success);
    if (!subTest(QString(Q_FUNC_INFO), "JSONserializeStr/success/array", success, "json serialization failed"))
    {
      break;
    }
    if (!subTest(QString(Q_FUNC_INFO), "JSONserializeStr/emptyTest/array", !firstArrayStr.isEmpty(), "json serialization string is empty"))
    {
      break;
    }

    QtJson::JsonObject secondObj = QtJson::parse(firstObjStr, success).toMap();
    if (!subTest(QString(Q_FUNC_INFO), "JSONparse/success/object", success, "json parse failed"))
    {
      break;
    }
    if (!subTest(QString(Q_FUNC_INFO), "JSONparse/emptyTest/object", !secondObj.isEmpty(), "json parse result is empty"))
    {
      break;
    }
    if (!subTest(QString(Q_FUNC_INFO), "JSONparse/ParseCompare/object", CompareJsonObj(firstObj, secondObj), "json objects are different"))
    {
      break;
    }

    result = true;
  }
  while(false);
  logTestEnd(QString(Q_FUNC_INFO), result, m_lastFailReason);
}

void ToolsTest::lzmaTest()
{
  logTestStart(QString(Q_FUNC_INFO));
  m_lastFailReason.clear();
  bool result = true;
  do
  {
    QByteArray in;
    for(int i = 0; i < TEST_BUFF_SIZE; ++i )
    {
      in.append(static_cast<char>(i % 0xff));
    }

    QByteArray header1;
    for (int level = MIN_LZMA_LEVEL; level <= MAX_LZMA_LEVEL; ++level)
    {
      QByteArray out1;
      unsigned char lzmaprop = 0;
      int encodeRc = LZMA::encode2(in, &out1, &lzmaprop, level);
      if (!subTest(QString(Q_FUNC_INFO),
                   QString("LZMA/Encode/level%1/ok").arg(level),
                   (encodeRc == LZMARC::SZ_OK),
                   QString("encode failed errcode: %1").arg(encodeRc)))
      {
        result = false;
        break;
      }
      if (!subTest(QString(Q_FUNC_INFO),
                   QString("LZMA/Encode/level%1/outEmpty").arg(level),
                   (!out1.isEmpty()),
                   "output is empty"))
      {
        result = false;
        break;
      }

      QByteArray decodedBuf;
      int decodeRc= LZMA::decode2(out1, &decodedBuf, static_cast<quint64>(in.size()), lzmaprop);
      if (!subTest(QString(Q_FUNC_INFO),
                   QString("LZMA/Decode/level%1/ok").arg(level),
                   (decodeRc == LZMARC::SZ_OK),
                   QString("decode failed errcode: %1").arg(decodeRc)))
      {
        result = false;
        break;
      }
      if (!subTest(QString(Q_FUNC_INFO),
                   QString("LZMA/Decode/level%1/outEmpty").arg(level),
                   (!decodedBuf.isEmpty()),
                   "output is empty"))
      {
        result = false;
        break;
      }
      if (!subTest(QString(Q_FUNC_INFO),
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
  logTestEnd(QString(Q_FUNC_INFO), result, m_lastFailReason);
}

uint ToolsTest::longCalc(uint v)
{
  for(uint i = 0; i < UPDATER_ROUNDS_COUNT; ++i)
  {
    v = qHash(v + i, i);
    LOG_TRACE("longCalc. iteration %u val %u", i, v);
  }
  return v;
}

void ToolsTest::dirPathUtilsTest()
{
  logTestStart(QString(Q_FUNC_INFO));
  m_lastFailReason.clear();
  bool result = true;

  foreach(dirRecord record, DirData::DIR_RECORDS)
  {
    QString orig = DirData::_getOriginalDirStr(record);
    QString expected = DirData::_getExpectedDirStr(record);
    bool hasSlash = DirData::_getExpectedHasSlash(record);
    if (!subTest(QString(Q_FUNC_INFO),
                 QString("PathUtils/").append(orig),
                 (CTools::hasLastSlash(orig) == hasSlash),
                 QString("invalid last slash detection. path %1, expected %2")
                 .arg(orig).arg(hasSlash)))
    {
      result = false;
      break;
    }

    if (!subTest(QString(Q_FUNC_INFO),
                 QString("PathUtils/").append(orig),
                 (CTools::removeLastSlash(orig) == expected),
                 QString("cannot remove last slash. original path %1, expected %2")
                 .arg(orig).arg(expected)))
    {
      result = false;
      break;
    }
  }

  logTestEnd(QString(Q_FUNC_INFO), result, m_lastFailReason);
}

void ToolsTest::downloaderResult()
{
  logTestStart(QString(Q_FUNC_INFO));
  m_lastFailReason.clear();
  bool result = true;
  do
  {
    // TODO: implement
    std::bitset<eDownloaderResultFields::FIELDS_COUNT> changes;
  }
  while(false);
  logTestEnd(QString(Q_FUNC_INFO), result, m_lastFailReason);
}

void ToolsTest::functionSchedulerTest()
{
  logTestStart(QString(Q_FUNC_INFO));
  m_lastFailReason.clear();

  bool result = false;
  do
  {
    FunctionScheduler::reset();
    FunctionScheduler::setVerbose(false);
    if (!subTest(QString(Q_FUNC_INFO),
                 "verboseFalse",
                 !FunctionScheduler::verbose(),
                 QString("invalid verbose (read after set) actual %1, expected %2")
                 .arg(FunctionScheduler::verbose()).arg(false))){break;}
    FunctionScheduler::setVerbose(true);
    if (!subTest(QString(Q_FUNC_INFO),
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
      QThread::msleep(delay);
      LOG_DEBUG("throw test exception");
      throw TestExceptionText.data();
    };
    auto throwableRuntimeExceptionRoutine = []()
    {
      unsigned long delay = (mrand() % DEFAULT_DELAY) + delayOffset;
      QThread::msleep(delay);
      LOG_DEBUG("throw test exception");
      throw std::runtime_error(TestRuntimeExceptionText.data());
    };

    for(int i = 0; i < ITERATION_COUNT; ++i)
    {
      scheduleOnce(throwableRoutine, DEFAULT_DELAY, ec, rec);
      scheduleOnce(throwableRuntimeExceptionRoutine, DEFAULT_DELAY, ec, rec);
    }
    uint waitingCount = 0;
    while(!FunctionScheduler::isEmpty())
    {
      LOG_DEBUG("wait %u", waitingCount);
      waitingCount++;
      QThread::msleep(DEFAULT_DELAY);
    }

    if (!subTest(QString(Q_FUNC_INFO),
                 "exceptionsRaised",
                 exceptionsRaised == ITERATION_COUNT,
                 QString("missed exceptions actual %1, expected %2")
                 .arg(exceptionsRaised).arg(ITERATION_COUNT))){break;}
    if (!subTest(QString(Q_FUNC_INFO),
                 "exceptionsRaised",
                 runtimeErrorsRaised == ITERATION_COUNT,
                 QString("missed runtimeexceptions actual %1, expected %2")
                 .arg(runtimeErrorsRaised).arg(ITERATION_COUNT))){break;}
    auto cleanRoutine = []()
    {
      LOG_DEBUG("cleanRoutine_%llu", msTime());
    };

    FunctionScheduler::reset();
    for(int i = 0; i < ITERATION_COUNT; ++i)
    {
      setInterval(cleanRoutine, DEFAULT_DELAY, ec, rec);
    }

    // now new exceptions should be raised
    if (!subTest(QString(Q_FUNC_INFO),
                 "noNewExceptionsRaised",
                 ((exceptionsRaised == ITERATION_COUNT) && (runtimeErrorsRaised == ITERATION_COUNT)),
                 QString("unexpected exceptions"))){break;}
    FunctionScheduler::destroy();
    if (!subTest(QString(Q_FUNC_INFO),
                 "emptyAfterDestroy",
                 FunctionScheduler::isEmpty(),
                 QString("FunctionScheduler not empty. count: %1").arg(FunctionScheduler::refs()))){break;}
    result = true;
  }
  while(false);
  logTestEnd(QString(Q_FUNC_INFO), result, m_lastFailReason);
}

void ToolsTest::updatableTest()
{
  logTestStart(QString(Q_FUNC_INFO));
  m_lastFailReason.clear();
  bool result = true;
  do
  {
    auto OnValueUpdatedFunct = [this](const uint& newVal, const uint& prevVal)
    {
      LOG_TRACE("OnValueUpdatedFunct. new: %u, prev: %u", newVal, prevVal);
      m_onUpdatedValue = newVal;
      m_onUpdatedPrev = prevVal;
    };
    auto OnValueUpdateFinishedFunct = [this](const uint& newVal, const uint& prevVal)
    {
      LOG_TRACE("OnValueUpdateFinishedFunct. new: %u, prev: %u", newVal, prevVal);
      m_onUpdateFinishedValue = newVal;
      m_onUpdateFinishedPrev = prevVal;
    };
    auto updater = [this](const Updatable<uint>::UpdatableCtx& ctx)
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
    const uint firstValue = 10;
    Updatable<uint> upd(firstValue);
    m_updaterExpectedValue = longCalc(firstValue);
    upd.setOnValueUpdatedFunct(OnValueUpdatedFunct);
    upd.setOnUpdateFinishedFunct(OnValueUpdateFinishedFunct);
    upd.setUpdater(updater);

    if (!subTest(QString(Q_FUNC_INFO),
                 QString("Updatable/beforeRun"),
                 (upd.get() == firstValue),
                 QString("unexpected value after set. expected %1, actual %2").arg(firstValue).arg(upd.get())))
    {
      result = false;
      break;
    }
    upd.requestUpdate();
    upd.wait();

    if (!subTest(QString(Q_FUNC_INFO),
                 QString("Updatable/onUpdatedValue"),
                 (m_onUpdatedValue == m_updaterExpectedValue),
                 QString("unexpected value received in setOnValueUpdatedFunct callback. expected %1, actual %2")
                 .arg(m_updaterExpectedValue).arg(m_onUpdatedValue)))
    {
      result = false;
      break;
    }

    if (!subTest(QString(Q_FUNC_INFO),
                 QString("Updatable/onUpdatedPrev"),
                 (m_onUpdatedPrev == firstValue),
                 QString("unexpected prev value received in setOnValueUpdatedFunct callback. expected %1, actual %2")
                 .arg(firstValue).arg(m_onUpdatedPrev)))
    {
      result = false;
      break;
    }

    if (!subTest(QString(Q_FUNC_INFO),
                 QString("Updatable/onUpdateFinishedValue"),
                 (m_onUpdateFinishedPrev == firstValue),
                 QString("unexpected prev received in setOnUpdateFinishedFunct callback. expected %1, actual %2")
                 .arg(firstValue).arg(m_onUpdateFinishedPrev)))
    {
      result = false;
      break;
    }

    if (!subTest(QString(Q_FUNC_INFO),
                 QString("Updatable/onUpdateFinishedValue"),
                 (m_onUpdateFinishedValue == m_updaterExpectedValue),
                 QString("unexpected value received in setOnUpdateFinishedFunct callback. expected %1, actual %2")
                 .arg(m_updaterExpectedValue).arg(m_onUpdateFinishedValue)))
    {
      result = false;
      break;
    }

    if (!subTest(QString(Q_FUNC_INFO),
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
  logTestEnd(QString(Q_FUNC_INFO), result, m_lastFailReason);
}
