#include "toolsexception.h"
#include "toolsmacro.h"

using namespace Tools;

ToolLibException::ToolLibException(const QString& reasonStr,
                                   const char *file,
                                   const char *function,
                                   int line) noexcept:
    std::runtime_error(reasonStr.toStdString()),
    m_reason(reasonStr),
    m_file(file),
    m_function(function),
    m_line(line)
{
    if (m_reason.isEmpty()){m_reason = "[unknown reason]";}
    if (file == nullptr){m_file = "[unknown file]";}
    if (function == nullptr){m_function = "[unknown file]";}
}

QString ToolLibException::where() const
{
    return QString("%1::%2.%3").arg(m_file, m_function).arg(m_line);
}

QString ToolLibException::toString() const
{
    return QString("Exception %1(%2)").arg(QString(what()), where());
}

const char* ToolLibException::what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT
{
    return SCHR(m_reason);
}
