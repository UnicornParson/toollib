#ifndef TOOLSMACRO_H
#define TOOLSMACRO_H

// QT includes
#include <QString>
#include <QObject>
#include <QMetaEnum>
#include <QMetaMethod>
#include <QtGlobal>
#include <QRandomGenerator>
#include <QRandomGenerator64>
#include <QDebug>

// compiler check


#ifdef __clang__
#define CLANG_COMPILER
#endif
#ifdef __GNUC__
#define GNUC_COMPILER
#endif
#ifdef _MSC_VER
#define MSVC_COMPILER
#endif
#ifdef __BORLANDC__
#define BORLAND_COMPILER
#endif
#ifdef __MINGW32__
#define MINGW_COMPILER
#endif



#ifdef TOLERATE_GARBAGE_CODE
#define GARBAGE_CODE_MARKER
#else
#define GARBAGE_CODE_MARKER static_assert(false , "IM A GARBAGE CODE! remove me! in " __FILE__);
#endif

//cpp version
#ifdef __cplusplus
#if __cplusplus == 201103L
#define CPP11
#define CPP11SUPPORTED
#elif __cplusplus == 201402L
#define CPP14
#define CPP11SUPPORTED
#define CPP14SUPPORTED
#elif __cplusplus == 201703L
#define CPP17
#define CPP11SUPPORTED
#define CPP14SUPPORTED
#define CPP17SUPPORTED
#elif __cplusplus > 201703L
#define CPP11SUPPORTED
#define CPP14SUPPORTED
#define CPP17SUPPORTED
#endif
#else
#error "NO CPP!"
#endif //#ifdef __cplusplus

#ifdef CPP14SUPPORTED
#define ATTR_NORETURN [[noreturn]]
#define ATTR_DEPRECATED [[deprecated]]
#define ATTR_DEPRECATED_R(reason) [[deprecated(reason)]]
#else
#define ATTR_NORETURN
#define ATTR_DEPRECATED
#define ATTR_DEPRECATED_R(reason)
#endif

#define MARK_USED __attribute__ ((used))

#ifdef CPP17SUPPORTED
#define MAYBE_UNUSED_ATTR [[maybe_unused]]
#define ATTR_NODISCARD [[nodiscard]]
#define ATTR_FALLTHROUGH [[fallthrough]]
#else
#define MAYBE_UNUSED_ATTR
#define ATTR_NODISCARD
#define ATTR_FALLTHROUGH
#endif
#define ATTR_UNUSED MAYBE_UNUSED_ATTR

#define START_REGION(name)
#define END_REGION(name)

#define INLINE __attribute__((always_inline)) inline
#define boolToString(x) ((x) ? "TRUE" : "FALSE")
#define ISNULL2STR(x) ((x == nullptr) ? "NULL" : "NOT NULL")
#define BOOL2STR(x) boolToString(x)
#define BOOLYESNO(x) ((x) ? "YES" : "NO")
#define doubleToString(x) (QString().asprintf("%f", x))
#define ZERO_MEM(a) memset((a), 0, sizeof(a))
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

#define __CLASS__ QString(typeid(this).name())

#ifdef Q_UNUSED
#define UNUSED(x) Q_UNUSED(x)
#else
#define UNUSED(x) (void)(x)
#endif

#define LCHR(l) (l.data())
#define GLOBALBUF Q_DECL_CONSTEXPR static const char
#define CHR(literal) (literal.data())
#define SCHR(s) (s.toUtf8().data())
#define SCHR_CONST(s) (s.toStdString().c_str())
#define BARR(literal) (QByteArray(CHR(literal)))
#define TOQSTR(x) (QString("%1").arg(x))
#define ExprToChr(x) #x
#define THREAD_STR_ID (QString().asprintf("TH%p",(void*)QThread::currentThreadId()))
#define THREAD_CHR_ID CHR(THREAD_STR_ID)
#define STRONG_ASSERT(e) {if((!e)){LOG_ERROR("FATAL! %s:%s.%d %s", __FILE__, Q_FUNC_INFO, __LINE__, static_cast<const char*>("#e") );exit(EXIT_FAILURE);}}
#define CRASH_POINT(msg) {LOG_ERROR(QString("CRASH POINT %1:%2.%3 %4").arg(__FILE__).arg(Q_FUNC_INFO).arg(__LINE__).arg(msg));STRONG_ASSERT(false);}
#define STR2BASE64(s) (QString(s.toLatin1().toBase64()))
#define __HERE__ (QString().asprintf("%s::%s.%d", __FILE__, Q_FUNC_INFO, __LINE__).toStdString().c_str())
#define printfErr(...) fprintf(stderr, __VA_ARGS__)

#define ENDL_DOS "\r\n"
#define ENDL_UNIX "\n"
#define CONST_LITERAL MAYBE_UNUSED_ATTR const QLatin1String
#define CONST_STRING MAYBE_UNUSED_ATTR const QString

#define qsprintf QString().asprintf
#ifndef Q_DISABLE_COPY
#define Q_DISABLE_COPY(Class) \
  Class(const Class &) Q_DECL_EQ_DELETE;\
  Class &operator=(const Class &) Q_DECL_EQ_DELETE;
#endif
#ifndef Q_DISABLE_MOVE
#define Q_DISABLE_MOVE(Class) \
  Class(Class &&) = delete; \
  Class &operator=(Class &&) = delete;
#endif
#ifndef Q_DISABLE_COPY_MOVE
#define Q_DISABLE_COPY_MOVE(Class) \
  Q_DISABLE_COPY(Class) \
  Q_DISABLE_MOVE(Class)
#endif

#define DISABLE_INSTANTIATING(Class) \
  Q_DISABLE_COPY_MOVE(Class) \
  Class() = delete; \
  ~Class() = delete;

#define CHECK_FUNC_RESULT(ex) if (! ex) { LOG_ERROR("%s executing error. negative result", #ex); }

#ifndef MAXUINT
#define MAXUINT UINT_MAX
#endif

#define PRINTABLE_VAR(var) QString("%1: %2").arg(#var).arg(var)
#define GETSET(type, var) \
  protected: \
  type m_##var; \
  public: \
  inline type Get##var() const {return m_##var; }\
  inline void Set##var(type val) {m_##var = val;}\
  inline type var() const {return m_##var; }

#define GETSETNOTIFY(type, var, listener) \
  protected: \
  type m_##var; \
  public: \
  inline type Get##var() const {return m_##var; }\
  inline void Set##var(type val) {m_##var = val; listener();}\
  inline type var() const {return m_##var; }

#define GETSET_FLAG_ONCHANGE(type, var, flagName) \
  protected: \
  type m_##var; \
  public: \
  inline type Get##var() const {return m_##var; }\
  inline void Set##var(type val) {m_##var = val; flagName = true;}\
  inline type var() const {return m_##var; }

#define OVERRIDABLE_GETSET(type, var) \
  protected: \
  type m_##var; \
  public: \
  virtual type Get##var() const {return m_##var; }\
  virtual void Set##var(type val) {m_##var = val;}\
  type var() const {return Get##var(); }

#define OVERRIDE_SET(type, var) virtual void Set##var(type val) Q_DECL_OVERRIDE
#define OVERRIDE_GET(type, var) virtual type Get##var() const override

#define OVERRIDE_SET_DECL(type, var, owner) void \
  owner::Set##var(type val)
#define OVERRIDE_GET_DECL(type, var, owner) type \
  owner::Get##var() const

#define GETONLY(type, var) \
  protected: \
  type m_##var; \
  public: \
  inline type Get##var() const {return m_##var; }

template<typename QEnum>
INLINE QString QEnumToString (const QEnum value)
{
  return QMetaEnum::fromType<QEnum>().valueToKey(value);
}

#define AddEnumSerializator(type) \
static inline QString type##ToString(e##type e) \
{ \
  return Tools::QEnumToString<e##type>(e); \
}

#define AddEnumSerializatorAsIsName(type) \
static INLINE QString type##ToString(type e) \
{ \
  return Tools::QEnumToString<type>(e); \
}

#define Name2Str(name) #name
#define Name2QLtr(name) QLatin1String(Name2Str(name))
#define Name2QStr(name) QString(Name2Str(name))

#define ENUM2INT(et, e) (static_cast<std::underlying_type_t< et >>(e))

template<typename T>
Q_DECL_CONSTEXPR inline const T Max3(const T& a, const T& b,const T& c)
{
  return qMax<T>(qMax<T>(a, b), c);
}

template<typename T>
Q_DECL_CONSTEXPR inline const T Max4(const T& a, const T& b, const T& c, const T& d)
{
  return qMax<T>(qMax<T>(a, b), qMax<T>(c, d));
}

template<typename T>
Q_DECL_CONSTEXPR inline const T Min3(const T& a, const T& b,const T& c)
{
  return qMin<T>(qMin<T>(a, b), c);
}

template<typename T>
Q_DECL_CONSTEXPR inline const T Min4(const T& a, const T& b, const T& c, const T& d)
{
  return qMin<T>(qMin<T>(a, b), qMin<T>(c, d));
}

MAYBE_UNUSED_ATTR Q_DECL_CONSTEXPR int DEFAULT_THREAD_TERMINATE_DELAY(1000);

#define LITERAL_TO_STRING(s) #s


INLINE quint32 mrand()
{
   return QRandomGenerator64::securelySeeded().generate();
}
INLINE quint64 mrand64()
{
   return QRandomGenerator64::securelySeeded().generate64();
}

// converter for enum classes. used type::val for compare
//#define ENUMVAL2STR_C(valName, enumName, value) if (enumName##::##value == valName){return QString(#value);}

// converter for old-style enums, used value as is
#define ENUMVAL2STR(valName, value) if (valName == value){return QString(#value);}

// QT versions compatibility
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
#define QT_SKIP_EMPTY_PARTS QString::SkipEmptyParts
#define QT_KEEP_EMPTY_PARTS QString::KeepEmptyParts
#else
#define QT_SKIP_EMPTY_PARTS Qt::SkipEmptyParts
#define QT_KEEP_EMPTY_PARTS Qt::KeepEmptyParts
#endif


inline Q_NORETURN void __not_implemented_impl(const QString& here)
{
    const QString s = QString("not implemented place called ") + here;
    qWarning() << s;
    throw std::runtime_error(s.toStdString());
}
#define NOT_IMPLEMENTED __not_implemented_impl(__HERE__);

// build info
#define BUILD_YEAR_CH0 (__DATE__[ 7])
#define BUILD_YEAR_CH1 (__DATE__[ 8])
#define BUILD_YEAR_CH2 (__DATE__[ 9])
#define BUILD_YEAR_CH3 (__DATE__[10])


#define BUILD_MONTH_IS_JAN (__DATE__[0] == 'J' && __DATE__[1] == 'a' && __DATE__[2] == 'n')
#define BUILD_MONTH_IS_FEB (__DATE__[0] == 'F')
#define BUILD_MONTH_IS_MAR (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'r')
#define BUILD_MONTH_IS_APR (__DATE__[0] == 'A' && __DATE__[1] == 'p')
#define BUILD_MONTH_IS_MAY (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'y')
#define BUILD_MONTH_IS_JUN (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'n')
#define BUILD_MONTH_IS_JUL (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'l')
#define BUILD_MONTH_IS_AUG (__DATE__[0] == 'A' && __DATE__[1] == 'u')
#define BUILD_MONTH_IS_SEP (__DATE__[0] == 'S')
#define BUILD_MONTH_IS_OCT (__DATE__[0] == 'O')
#define BUILD_MONTH_IS_NOV (__DATE__[0] == 'N')
#define BUILD_MONTH_IS_DEC (__DATE__[0] == 'D')


#define BUILD_MONTH_CH0 \
    ((BUILD_MONTH_IS_OCT || BUILD_MONTH_IS_NOV || BUILD_MONTH_IS_DEC) ? '1' : '0')

#define BUILD_MONTH_CH1 \
    ( \
        (BUILD_MONTH_IS_JAN) ? '1' : \
        (BUILD_MONTH_IS_FEB) ? '2' : \
        (BUILD_MONTH_IS_MAR) ? '3' : \
        (BUILD_MONTH_IS_APR) ? '4' : \
        (BUILD_MONTH_IS_MAY) ? '5' : \
        (BUILD_MONTH_IS_JUN) ? '6' : \
        (BUILD_MONTH_IS_JUL) ? '7' : \
        (BUILD_MONTH_IS_AUG) ? '8' : \
        (BUILD_MONTH_IS_SEP) ? '9' : \
        (BUILD_MONTH_IS_OCT) ? '0' : \
        (BUILD_MONTH_IS_NOV) ? '1' : \
        (BUILD_MONTH_IS_DEC) ? '2' : \
        /* error default */    '?' \
    )

#define BUILD_DAY_CH0 ((__DATE__[4] >= '0') ? (__DATE__[4]) : '0')
#define BUILD_DAY_CH1 (__DATE__[ 5])



// Example of __TIME__ string: "21:06:19"
//                              01234567

#define BUILD_HOUR_CH0 (__TIME__[0])
#define BUILD_HOUR_CH1 (__TIME__[1])

#define BUILD_MIN_CH0 (__TIME__[3])
#define BUILD_MIN_CH1 (__TIME__[4])

#define BUILD_SEC_CH0 (__TIME__[6])
#define BUILD_SEC_CH1 (__TIME__[7])

#define BUILD_DATETIME __DATE__ __TIME__
#define BUILD_DATETIME_S QString("%1-%2").arg(__DATE__).arg(__TIME__)

#endif // TOOLSMACRO_H
