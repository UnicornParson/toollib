#ifndef CONFIGSEQENTRY_H
#define CONFIGSEQENTRY_H
#include "toolsmacro.h"
namespace Tools
{
class ConfigSeqEntry
{
public:
    enum entryType_t: uint
    {
        PathBasedEntry = 0,
        ContentBasedEntry,
        SkipEntry
    };

    QString path;
    QString data;
    QString name;
    entryType_t type = PathBasedEntry;

    // fail sequence and rollback when entry failed
    bool required = true;

    ConfigSeqEntry() noexcept;
    ConfigSeqEntry(const ConfigSeqEntry& other) noexcept;
    ConfigSeqEntry(const QString& entryName,
                   entryType_t entryType,
                   const QString& content,
                   bool req = true) noexcept;
}; // class ConfigSeqEntry
}; // namespace Tools
#endif // CONFIGSEQENTRY_H
