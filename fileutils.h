#ifndef FILEUTILS_H
#define FILEUTILS_H

#include "toolsmacro.h"
#include "json.h"

namespace Tools
{
class FileUtils
{
public:
    FileUtils() = delete;
    FileUtils(FileUtils&) = delete;
    ~FileUtils() = delete;

    static QByteArray readFile(const QString& filename, bool& isOk);
    static QStringList readLines(const QString& filename, bool& isOk, bool trim = true);
    static QStringList dirList(const QString& path, bool recursive = true);
    static bool checkRequiredFiles(const QString& path, const QStringList& reqList);
    static bool writeLines(const QString& filename, const QStringList& lines);
    static bool writeJson(const QString& filename, const QtJson::JsonObject& obj);
    static QtJson::JsonObject readJson(const QString& filename, bool& isOk);
}; // class FileUtils
}; // namespace Tools
#endif // FILEUTILS_H
