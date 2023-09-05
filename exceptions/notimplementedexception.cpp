#include "notimplementedexception.h"
#include <string>
#include "toolsmacro.h"
using namespace Tools;

namespace
{
CONST_LITERAL ExceptionName("NotImplementedException");
}

NotImplementedException::NotImplementedException(const char* file, const char* function, int lineNum) noexcept:
    ToolLibException(ExceptionName)
{
    UNUSED(file)
    UNUSED(function)
    UNUSED(lineNum)
}

