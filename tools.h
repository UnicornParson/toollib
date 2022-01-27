#ifndef TOOLS_H
#define TOOLS_H

// CPP includes
#include <climits>
#include <algorithm>
#include <type_traits>
#include <cstdlib>
#include <utility>

// QT includes
#include <QString>
#include <QMutex>
#include <QSemaphore>
#include <QStringList>
#include <QList>
#include <QThread>
#include <QMetaEnum>
#include <QObject>

// Local includes
#include "json.h"
#include "CuteLogger/logglobalsetup.h"
#include "CuteLogger/cuteloggerinc.h"
#include "CuteLogger/logstarter.h"
#include "platformhelper.h"
#include "tickcounter.h"
#include "interfaces.h"
#include "exceptions/toollibexceptions.h"
#include "comparators.h"
#include "objectid.h"
#include "logutils.h"
#include "toolsmacro.h"
#include "buildConstants.h"
#include "functionscheduler.h"
#include "ranges.h"
#include "literals.h"

typedef std::atomic<quint64> counter_t;
typedef qint64 msTime_t;

#ifdef USE_CPP_TIME
inline msTime_t msTime() noexcept
{
  return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}
#else
inline msTime_t msTime() noexcept
{
  return QDateTime::currentMSecsSinceEpoch();
}
#endif

inline QString msTimeStr() noexcept
{
  return QString("%1").arg(msTime());
}

inline QString timeStr() noexcept
{
  return QDateTime::currentDateTime().toString("yyyy-MM-dd  HH:mm:ss");
}

inline QString timeStrFnameCompatible() noexcept
{
  return QDateTime::currentDateTime().toString("yyyy.MM.dd.HH.mm.ss");
}

namespace QtJson
{
    inline bool checkTags(const QtJson::JsonObject& obj, const QStringList& requiredTags)
    {
        for(QString t: requiredTags)
        {
            if (!obj.contains(t))
            {
                return false;
            }
        }
        return true;
    }
}

namespace Tools
{
  // std::optional implementation for pre-C++17 compilers
  template< class T >
  class MOptional
  {
  public:
    explicit MOptional(T& val):d(true, val){}
    MOptional():d(false, T()){}
    MOptional(MOptional& other):d(other.d){}
    inline T& val() {return d.second;}
    inline bool ok() {return d.first;}
    inline operator bool(){return ok();}
    MOptional& operator=(MOptional const& other)
    {
      d = other.d;
      return *this;
    }
  private:
    std::pair<bool, T> d;
  };

  typedef MOptional<QString> OptionalString;

  template<class T, class U>
  inline QTextStream& operator<<(QTextStream& out, const QPair<T, U>& pair) {
      out << "QPair<" << pair.first << pair.second << ">";
      return out;
  }

  template<typename T>
  inline QString anyToString (const T& value)
  {
      QString s;
      QTextStream out(&s);
      out << value;
      return s;
  }

  template<typename QEnum>
  inline QString QEnumToString (const QEnum value)
  {
    return QMetaEnum::fromType<QEnum>().valueToKey(value);
  }

  template<typename QEnum>
  inline QEnum QEnumFromString (const QString& str, bool *ok = nullptr)
  {
    return static_cast<QEnum>(QMetaEnum::fromType<QEnum>().keyToValue(str.toLatin1().data(), ok));
  }

  template<typename QEnum>
  inline QList<QEnum> QEnumToList()
  {
    QList<QEnum> ret;
    QMetaEnum meta = QMetaEnum::fromType<QEnum>();
    for(int i = 0; i < meta.keyCount(); ++i)
    {
      ret.append(meta.value(i));
    }
    return ret;
  }

  template<typename QEnum>
  inline QStringList QEnumToStringList()
  {
    QStringList ret;
    QList<QEnum> l = QEnumToList<QEnum>();
    foreach(QEnum e, l)
    {
      QString s = QEnumToString<QEnum>(e);
      if (!s.isEmpty())
      {
        ret.append(s);
      }
    }
    return ret;
  }

  class CTools
  {
  public:
    ~CTools();
    CTools();
    /* convert path to OS based standart. depends of platform define
     * - #define PLATFORM_WINDOWS
     * - #define PLATFORM_LINUX
     */
    static QString pathNormalize(const QString& in);
    static QString extractFileName(const QString& in);
    static QString extractFolderName(const QString& in);
    static QStringList pathParts(const QString& in);
    static bool hasLastSlash(const QString& in);
    static QString removeLastSlash(const QString& in);
    static QString getPathSeparator();
    static QString getUUID();
    static double dFloor(double val, int count);
    static QString stringTime();
    static bool isStringlistsEquals(const QStringList& left, const QStringList& right);

    static bool checkRequiredKeys(const QtJson::JsonObject& obj, const QStringList& requiredKeys);
    //static bool _selfTest();


    static QString readableBytes(quint64 bytes);
    static bool compareFiles(const QString& left, const QString& right, bool& ok);
    static bool compareFiles(QFile &left, QFile &right, bool& ok);
    static void SyncThreadTerminate(QThread* ptr, int delay = DEFAULT_THREAD_TERMINATE_DELAY);
  private:
    Q_DISABLE_COPY_MOVE(CTools)
  };
  //typedef



} // namespace Tools
#endif // TOOLS_H
