#include "version.h"
#include "memcontrol.h"

using namespace Tools;

Version::Version() noexcept:
    a(0),
    b(0),
    c(0),
    d(0)
{

}

Version::Version(const Version &other) noexcept:
    a(other.a),
    b(other.b),
    c(other.c),
    d(other.d)
{

}

Version::Version(uint na,
                 uint nb,
                 uint nc,
                 uint nd) noexcept:
    a(na), b(nb), c(nc), d(nd)
{

}

bool Version::operator==(Version const& other) const
{
    return (a == other.a) && (b == other.b) && (c == other.c) && (d == other.d);
}

bool Version::operator!=(Version const& other) const { return !operator==(other);}

void Version::reset()
{
    a = 0;
    b = 0;
    c = 0;
    d = 0;
}

Version& Version::operator=(Version const& other)
{
    if (this != &other)
    {
        a = other.a;
        b = other.b;
        c = other.c;
        d = other.d;
    }
    return *this;
}

bool Version::operator>(Version const& other) const
{
    if (a > other.a){return true;}
    else if (b > other.b){return true;}
    else if (c > other.c){return true;}
    else if (d > other.d){return true;}
    return false;
}

bool Version::operator<(Version const& other) const
{
    if (a < other.a){return true;}
    else if (b < other.b){return true;}
    else if (c < other.c){return true;}
    else if (d < other.d){return true;}
    return false;
}

bool Version::operator>=(Version const& other) const
{
    return operator>(other)||operator==(other);
}

bool Version::operator<=(Version const& other) const
{
    return operator<(other)||operator==(other);
}

bool Version::eqOrHigher(uint na,
                         uint nb,
                         uint nc,
                         uint nd)
{
    return operator>=(Version(na, nb,nc,nd));
}

bool Version::loadPart(const QString& s, uint& part)
{
    part = 0;
    QString str = s.trimmed();
    if (str.isEmpty())
    {
        return false;
    }
    bool convertRc = false;
    part = str.toUInt(&convertRc);
    if (!convertRc)
    {
        LOG_ERROR(QString("%1 is not a number").arg(str));
        return false;
    }
    return true;
}

bool Version::fromString(const QString& strVersion)
{
    QStringList list = strVersion.split(".", QT_KEEP_EMPTY_PARTS);
    bool ret = true;

    int size = list.size();
    reset();
    switch(size)
    {
    case 4:
    {
        ret = loadPart(list[3], d);
        if (!ret)
        {
            LOG_ERROR(QString("%1 is not a valid number").arg(list[0]));
            break;
        }
        ATTR_FALLTHROUGH;
    }
    case 3:
    {
        ret = loadPart(list[2], c);
        if (!ret)
        {
            LOG_ERROR(QString("%1 is not a valid number").arg(list[0]));
            break;
        }
        ATTR_FALLTHROUGH;
    }
    case 2:
    {
        ret = loadPart(list[1], b);
        if (!ret)
        {
            LOG_ERROR(QString("%1 is not a valid number").arg(list[0]));
            break;
        }
        ATTR_FALLTHROUGH;
    }
    case 1:
    {
        ret = loadPart(list[0], a);
        if (!ret)
        {
            LOG_ERROR(QString("%1 is not a valid number").arg(list[0]));
            break;
        }
        break;
    }
    default:
    {
        LOG_ERROR(QString("cannot parse version string %1").arg(strVersion));
        ret = false;
        break;
    }
    }
    return ret;
}

QString Version::toString() const
{
    return QString("%1.%2.%3.%4").arg(a).arg(b).arg(c).arg(d);
}

Version VersionFactory::create(uint a, uint b, uint c, uint d)
{
    Version v;
    v.a = a;
    v.b = b;
    v.c = c;
    v.d = d;
    return v;
}

Version VersionFactory::create(const QString& s)
{
    Version v;
    if (!v.fromString(s))
    {
        return create(0,0,0,0);
    }
    return v;
}
