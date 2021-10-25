#ifndef TOOLSEXCEPTION_H
#define TOOLSEXCEPTION_H
#include <stdexcept>
#include <exception>
#include <QString>
#include <QVariant>
#ifdef SECURITY_UTILS_NO_EXCEPT
#define NEW_TOOL_LIB_EXCEPTION(reason)
#define THROW_TOOL_LIB_EXCEPTION(reason)
#define THROW_TOOL_LIB_EXCEPTION_UNSUPPORTED(reason)
#else
#define NEW_TOOL_LIB_EXCEPTION(reason) (ToolLibException(QString(reason), __FILE__, Q_FUNC_INFO, __LINE__))
#define THROW_TOOL_LIB_EXCEPTION(reason) throw NEW_TOOL_LIB_EXCEPTION(reason)
#define THROW_TOOL_LIB_UNSUPPORTED_EXCEPTION THROW_TOOL_LIB_EXCEPTION("function is not supported")
#endif
namespace Tools
{
class ToolLibException : public std::runtime_error
{
public:
  ToolLibException(const QString& reasonStr,
                   const char* file = nullptr,
                   const char* function = nullptr,
                   int line = 0) noexcept;
  virtual ~ToolLibException() = default;

  inline QString reason() const {return m_reason;}
  inline void setReason(const QString& newReason){m_reason = newReason;}

  inline QVariant data() const {return m_data;}
  inline void setReason(const QVariant& newData){m_data = newData;}

  virtual const char* what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT Q_DECL_OVERRIDE;
  virtual QString where() const;
  virtual QString toString() const;
  inline operator QString() const {return toString();}
  virtual inline QString fileName() const {return m_file;}
  virtual inline QString functionName() const {return m_function;}
  virtual inline int line() const {return m_line;}

protected:
  QString m_reason;
  QString m_file;
  QString m_function;
  QVariant m_data;
  int m_line;
}; // class ToolLibException

} // namespace Tools
#endif // TOOLSEXCEPTION_H
