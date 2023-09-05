#ifndef STRINGUTILS_H
#define STRINGUTILS_H
#include "toolsmacro.h"
#include "json.h"

namespace Tools
{

template<class T, class U>
inline QTextStream& operator<<(QTextStream& out, const QPair<T, U>& pair) {
    out << "QPair<" << pair.first << pair.second << ">";
    return out;
}

template<class A, class B>
inline QTextStream& operator<<(QTextStream& out, const std::tuple<A, B>& t) {
    out << "tuple<" << std::get<0>(t) << std::get<1>(t) << ">";
    return out;
}

template<class A, class B, class C>
inline QTextStream& operator<<(QTextStream& out, const std::tuple<A, B, C>& t) {
    out << "tuple<" << std::get<0>(t) << std::get<1>(t) << std::get<2>(t) << ">";
    return out;
}

inline QTextStream& operator<<(QTextStream& out, const QVariantList& l)
{
    out << "QVariantList[";
    bool first = true;
    for(const QVariant& v: l)
    {
        if(!first)
        {
            out << ", ";
        }
        first = false;
        out << v.toString();
    }
    out << "]";
    return out;
}

inline QTextStream& operator<<(QTextStream& out, const QVariant& v)
{
    if(!v.isNull())
    {
        out << v.toString();
    }
    else
    {
        out << "null";
    }
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


class StringUtils
{
public:
    StringUtils() = delete;
    template< class T >
    static QString toString(const QList<T>& l)
    {
        QString str;
        bool first = true;
        for(const T& v: l)
        {
            if(!first)
            {
                str.append(", ");
            }
            first = false;
            str.append(anyToString(v));
        }
        return str;
    }

    template< class T, class U >
    static QString toString(const QHash<T, U>& l)
    {
        QString str;
        bool first = true;
        for(const T& k: l.keys())
        {
            if(!first)
            {
                str.append(", ");
            }
            first = false;
            str.append(QString("[(%1) => (%2)]").arg(anyToString(k), anyToString(l[k])));
        }
        return str;
    }

    static QString toString(const QtJson::JsonArray& ja);
    static QString toString(const QtJson::JsonObject& jo);

    static QStringList splitLines(const QString& in);
}; // class StringUtils
}; // namespace Tools
#endif // STRINGUTILS_H
