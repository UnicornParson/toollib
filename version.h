#ifndef VERSION_H
#define VERSION_H
#include "tools.h"

#define VERSION(a,b,c,d) (VersionFactory::create(a, b, c, d))
#define VERSION_S(s) (VersionFactory::create(QString(s)))

namespace Tools
{
class Version
{
public:
    uint a;
    uint b;
    uint c;
    uint d;

    QString toString() const;

    Version() noexcept;
    ~Version() = default;
    Version(uint na,
            uint nb,
            uint nc = 0,
            uint nd = 0) noexcept;

    Version(const Version& other) noexcept;

    ATTR_NODISCARD bool eqOrHigher(uint na,
                                   uint nb = 0,
                                   uint nc = 0,
                                   uint nd = 0);
    ATTR_NODISCARD bool fromString(const QString& strVersion);
    ATTR_NODISCARD bool operator==(Version const& other) const;
    ATTR_NODISCARD bool operator!=(Version const& other) const;
    ATTR_NODISCARD bool operator>(Version const& other) const;
    ATTR_NODISCARD bool operator<(Version const& other) const;
    ATTR_NODISCARD bool operator>=(Version const& other) const;
    ATTR_NODISCARD bool operator<=(Version const& other) const;
    Version& operator=(Version const& other);
    void reset();
protected:

    ATTR_NODISCARD bool loadPart(const QString& s, uint& part);
};

class VersionFactory
{
public:
    static Version create(uint a = 0,
                          uint b = 0,
                          uint c = 0,
                          uint d = 0);

    static Version create(const QString& s);
private:
    DISABLE_INSTANTIATING(VersionFactory)
};
}
#endif // VERSION_H
