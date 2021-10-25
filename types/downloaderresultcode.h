#ifndef DOWNLOADERRESULTCODE_H
#define DOWNLOADERRESULTCODE_H

#include <QObject>
#include "baseerrorcodes.h"
namespace Tools
{
/*
class DownloaderResultCode : public QObject
{
    Q_OBJECT
    Q_ENUMS(e)
public:
    enum e
    {
        Unknown = -1,
        Ok,
        InProgress,
        InvalidAddress,
        FileExists,
        IncorrectParam,
        ServerNotRespond,
        TargetNotFound,
        FSError,
        IOError
    };
    Q_ENUM(e)

    static QString toString(DownloaderResultCode::e val);
private:
    explicit DownloaderResultCode(QObject *parent = nullptr);
}; // class DownloaderResultCode

typedef DownloaderResultCode::e eDownloaderResultCode;
*/
} // namespace Tools

#endif // DOWNLOADERRESULTCODE_H
