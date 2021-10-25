#ifndef IDOWNLOADEROBSERVER_H
#define IDOWNLOADEROBSERVER_H

#include <QString>

#include "downloaderresultcode.h"
#include "downloaderresult.h"

namespace Tools
{
class IDownloaderObserver
{
public:
    IDownloaderObserver();
    virtual ~IDownloaderObserver();
    virtual void OnFinish(const QString& emitterId, const DownloaderResult& result) = 0;
    virtual void OnProgress(const QString& emitterId, const QString& target, float percent) = 0;
    virtual void OnError(const QString& emitterId, const QString& msg, eBaseErrorCodes code) = 0;
};
}
#endif // IDOWNLOADEROBSERVER_H
