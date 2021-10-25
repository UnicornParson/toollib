#include "configseqentry.h"
using namespace Tools;

ConfigSeqEntry::ConfigSeqEntry() noexcept :
    ConfigSeqEntry(QString("entry%1").arg(uintptr_t(this)), entryType_t::PathBasedEntry, QString(), true){}

ConfigSeqEntry::ConfigSeqEntry(const ConfigSeqEntry& other) noexcept:
    path(other.path),
    data(other.data),
    name(other.name),
    type(other.type),
    required(other.required)
{}

ConfigSeqEntry::ConfigSeqEntry(const QString& entryName,
                               entryType_t entryType,
                               const QString& content,
                               bool req) noexcept:
    name(entryName),
    type(entryType),
    required(req)
{
    switch(type)
    {
    case entryType_t::ContentBasedEntry:
    {
        data = content;
        break;
    }
    case entryType_t::PathBasedEntry:
    {
        path = content;
        break;
    }
    default:
    case entryType_t::SkipEntry:
    {}
    }
}
