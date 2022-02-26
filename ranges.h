#ifndef RANGES_H
#define RANGES_H

#include "toolsmacro.h"
#include <QList>
#include <QFutureSynchronizer>
#include <tuple>
#include <QString>
#include <QtConcurrent>

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

template< typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
QList<T> chain(QList<QList<T> > ranges) noexcept
{
    QList<T> r;
    int sz = 1;
    for(const QList<T>& l: ranges)
    {
        sz += l.size();
    }
    r.reserve(sz);
    for(const QList<T>& l: ranges)
    {
        r.append(l);
    }
//    auto variantLessThan = [](const T &v1, const T &v2){return v1 < v2;}
    std::sort(r.begin(), r.end());
    return r;
}


template< class T >
inline bool inScope(const T& val, const QList<T>& scope) noexcept
{
    for (const T& elem: scope)
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




template< class in_t, class out_t>
QList<out_t> inline mapSingleThread(QList<in_t> in, const std::function<out_t(const in_t&)>& functor)
{
    QList<out_t> out;
    out.reserve(in.size());
    for(const in_t& i: in)
    {
        out.append(functor(i));
    }
    return out;
}

template< class in_t, class out_t>
QList<out_t> inline mapMT(QList<in_t> in, const std::function<out_t(const in_t&)>& functor)
{
    QList<out_t> out;
    out.reserve(in.size());
    QFutureSynchronizer<out_t> sync;
    for(const in_t& i: in)
    {
        sync.addFuture(QtConcurrent::run([&functor](const in_t& i){return functor(i);}, i));
    }
    sync.waitForFinished();
    auto futures = sync.futures();
    for(const QFuture<out_t>& f: futures)
    {
        out.append(f.result());
    }
    return out;
}


template< class in_t, class out_t>
QList<out_t> inline map(QList<in_t> in, const std::function<out_t(const in_t&)>& functor)
{
#ifdef FEATURE_MULTITHREADS
    return mapMT(in, functor);
#else
    return mapSingleThread(in, functor);
#endif

}


template< class T>
QList<T> inline filter(QList<T> in, const std::function<bool(const T&)>& functor)
{
    QList<T> out;
    out.reserve(in.size());
    for(const T& i: in)
    {
        if(functor(i)){out.append(i);}
    }
    return out;
}

} // namespace Tools::Ranges
#endif // RANGES_H
