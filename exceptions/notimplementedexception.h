#ifndef NOTIMPLEMENTEDEXCEPTION_H
#define NOTIMPLEMENTEDEXCEPTION_H

#include <QString>

#include "toolsexception.h"

namespace Tools
{
class NotImplementedException : public ToolLibException
{
public:
    NotImplementedException(const char* file, const char* function, int lineNum) noexcept;
}; // class NotImplementedException

#define THROW_NOT_IMPLEMENTED_EXCEPTION throw NotImplementedException(__FILE__, Q_FUNC_INFO, __LINE__)
}; // namespace Tools
#endif // NOTIMPLEMENTEDEXCEPTION_H
