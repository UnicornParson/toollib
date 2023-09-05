#ifndef COMPARATORS_H
#define COMPARATORS_H
#include <QList>
#include <QMap>
#include <QHash>
#include <QtMath>
#include "json.h"
#include "ranges.h"

Q_DECL_CONSTEXPR int DEFAULT_COMPARATION_PRECISION(4);

#define compareDouble safeCompare<double>
#define compareFloat safeCompare<float>

namespace Tools
{

template<typename T>
inline bool safeCompare(T left, T right, int precision = DEFAULT_COMPARATION_PRECISION)
{
    const T dif = static_cast<T>(qFabs(left - right));
    const T maxPossibleDif = pow(0.1, static_cast<T>(precision));
    return (dif <= maxPossibleDif);
}

template<typename T>
bool CompareList(QList<T>const& left,
                 QList<T>const& right)
{
    if (left.size() != right.size())
    {
        return false;
    }
    QList<T> tright = right;
    for (const T& item: left)
    {
        if (!tright.contains(item))
        {
            return false;
        }
        tright.removeOne(item);
    }
    return tright.isEmpty();
}

inline bool CompareStringList(const QStringList& left, const QStringList& right)
{
    return CompareList<QString>(left, right);
}

template<typename MapType, typename KeyType>
bool CompareMap(MapType const & left,
                MapType const & right)
{
    if (!CompareList<KeyType>(left.keys(), right.keys()))
    {
        return false;
    }

    foreach_map_const(left)
    {
        if (it->second != right.value(it->first))
        {
            return false;
        }
    }
    return true;
}

template<typename MapType, typename KeyType>
bool CompareMapD(MapType const & left,
                 MapType const & right)
{
    if (!CompareList<KeyType>(left.keys(), right.keys()))
    {
        return false;
    }
    foreach_map_const(left)
    {
        if (!safeCompare<double>(it->second, right.value(it->first)))
        {
            return false;
        }
    }
    return true;
}

template<typename MapType, typename KeyType>
bool CompareMapF(MapType const & left,
                 MapType const & right)
{
    if (!CompareList<KeyType>(left.keys(), right.keys()))
    {
        return false;
    }

    foreach_map_const(left)
    {
        if (!safeCompare<float>(it->second, right.value(it->first)))
        {
            return false;
        }
    }
    return true;
}

inline bool CompareJsonObj(const QtJson::JsonObject& first, const QtJson::JsonObject& second)
{
    return CompareMap<QMap<QString, QVariant>, QString>(first, second);
}

inline bool CompareJsonArray(const QtJson::JsonArray& first, const QtJson::JsonArray& second)
{
    return CompareList<QVariant>(first, second);
}
}
#endif // COMPARATORS_H
