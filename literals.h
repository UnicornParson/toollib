#ifndef LITERALS_H
#define LITERALS_H
#include "toolsmacro.h"

namespace Tools
{
INLINE const QString operator"" _s(const char* c)
{
    return QString(c);
}

INLINE const QString operator"" _s(const char* c, size_t sz)
{
    UNUSED(sz)
    return QString(c);
}

}
#endif // LITERALS_H
