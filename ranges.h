#ifndef RANGES_H
#define RANGES_H

#include "toolsmacro.h"
#include <QList>

namespace Tools::Ranges
{
template< class T >
inline bool inRange(const T& val, const T& rangeMin, const T& rangeMax) noexcept
{
    return (val <= rangeMax) && (val >= rangeMin);
}



template< typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
inline QList<T> range(const T& min, const T& max, const T& step) noexcept
{
    QList<T> r;
    for (T i = min; i < max; i += step)
    {
        r << i;
    }
    return r;
}

template< typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
inline QList<T> range(const T& max) noexcept
{
    return range(T(0), max, T(1));
}

template< typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
inline QList<T> range(const T& min, const T& max) noexcept
{
    return range(min, max, T(1));
}

template< class T >
inline bool inScope(const T& val, const QList<T>& scope) noexcept
{
    foreach(const T& elem, scope)
    {
        if (elem == val)
        {
            return true;
        }
    }
    return false;
}
template< class T >
inline bool notInScope(const T& val, const QList<T>& scope) noexcept
{
    return !inScope<T>(val, scope);
}

template< class T >
Q_DECL_CONSTEXPR inline T fitToLimits(T val, T min, T max) noexcept
{
    if (val < qMin<T>(min, max)){return qMin<T>(min, max);}
    else if (val > qMax<T>(min, max)){return qMax<T>(min, max);}
    return val;
}

template< class T >
Q_DECL_CONSTEXPR inline T boundedSumm(T a, T b, T min, T max) noexcept
{
    return fitToLimits<T>((a + b), min, max);
}

template< class T >
Q_DECL_CONSTEXPR inline T boundedSub(T a, T b, T min, T max) noexcept
{
    return fitToLimits<T>((a - b), min, max);
}

} // namespace Tools::Ranges
#endif // RANGES_H
