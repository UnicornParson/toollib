#include "tools.h"
#include <QChar>
#include <QUuid>
#include <QThread>
#include <qmath.h>

using namespace Tools;

namespace
{
Q_DECL_CONSTEXPR qint64 FILE_BLOCK_SIZE(1024);
const QString SIZE_NAMES[] = {"B", "KB", "MB", "GB", "TB", "PB", "EB"};
CONST_LITERAL DATETIME_FORMAT("yyyy.MM.dd_hh.mm.ss");
Q_DECL_CONSTEXPR uint WHILE_ITERATION_LIMIT(20);
#ifdef PLATFORM_WINDOWS
const QChar pathSeparator('\\');
const QChar invalidPathSeparator('/');
const QLatin1String doubleSeparator("\\\\");
#else
const QChar pathSeparator = QChar('/');
const QChar invalidPathSeparator = QChar('\\');
const QLatin1String doubleSeparator("//");
#endif
}

QString CTools::getPathSeparator()
{
    return pathSeparator;
}

bool CTools::hasLastSlash(const QString& in)
{
    const QString last = in.trimmed().right(1);
    return (last == QString(pathSeparator)) || (last == QString(invalidPathSeparator));
}

QString CTools::removeLastSlash(const QString& in)
{
    if (!hasLastSlash(in))
    {
        return in;
    }
    QString t = in.trimmed();
    return t.left(t.length() - 1);
}

QString CTools::extractFileName(const QString& in)
{
    QString normalizedPath = pathNormalize(in);
    if (normalizedPath.contains(getPathSeparator()))
    {
        return in.right(in.length() - in.lastIndexOf(getPathSeparator()));
    }
    return in;
}

QString CTools::extractFolderName(const QString& in)
{
    QString normalizedPath = pathNormalize(in);
    if (normalizedPath.contains(getPathSeparator()))
    {
        return in.left(in.lastIndexOf(getPathSeparator()));
    }
    return in;
}

QStringList CTools::pathParts(const QString& in)
{
    QString normalizedPath = pathNormalize(in);
    return in.split(getPathSeparator(), QT_SKIP_EMPTY_PARTS);
}

QString CTools::pathNormalize(const QString &in)
{
    uint iteration = 0;
    QString normalized = QString(in).replace(invalidPathSeparator, pathSeparator);
    while(normalized.contains(doubleSeparator))
    {
        ++iteration;
        if (iteration > WHILE_ITERATION_LIMIT)
        {
            LOG_WARNING("potential incorrent while. iteration %u, string %s", iteration, SCHR(normalized));
        }
        normalized.replace(doubleSeparator, pathSeparator);
    }
    return normalized;
}

QString CTools::getUUID()
{
    return QUuid::createUuid().toString();
}

double CTools::dFloor(double val, int count)
{
    double k = static_cast<double>(qFloor(qPow(10, count)));
    double ret = 0.0;
    if (val > 0)
    {
        ret = round(val*k)/k;
    }
    else if (val < 0)
    {
        ret = round(-val*k)/k;
    }
    //LOG_TRACE(QString("dFloor test. val: %1, ret: %2").arg(val, 0, 'g', 100).arg(ret,0, 'g', 100));
    return ret;
}

bool CTools::isStringlistsEquals(const QStringList& left, const QStringList& right)
{
    return CompareList<QString>(left, right);
}
QString CTools::stringTime()
{
    return QDateTime::currentDateTime().toString(DATETIME_FORMAT);
}


bool CTools::checkRequiredKeys(const QtJson::JsonObject& obj, const QStringList& requiredKeys)
{
    bool bRet = true;
    for (const QString& key: requiredKeys)
    {
        if (!obj.contains(key))
        {
            bRet = false;
            break;
        }
    }
    return bRet;
}

QString CTools::readableBytes(quint64 bytes)
{
    if (bytes < 1024)
    {
        return QString("%1").arg(bytes) + SIZE_NAMES[0];
    }
    int index = int(floor(log2(double(bytes)) / 10.0));
    if (index > 6)
    {
        index = 6;
    }
    double val = double(bytes) / pow(2.0, index * 10);
    return QString().asprintf("%0.3f", val) + SIZE_NAMES[index];
}

void CTools::SyncThreadTerminate(QThread* ptr, int delay)
{
    if (ptr == nullptr)
    {
        return;
    }
    ptr->requestInterruption();
    Q_DECL_CONSTEXPR int stepDelay = 100;
    while(ptr->isRunning() && delay > 0)
    {
        QThread::msleep(stepDelay);
        delay -= stepDelay;
    }
    if (ptr->isRunning())
    {
        LOG_ERROR("thread[0x%lld] still running. terminate", intptr_t(ptr));
        ptr->terminate();
    }
}


bool CTools::compareFiles(const QString& left, const QString& right, bool& ok)
{
    ok = false;
    bool ret = false;
    do
    {
        if (left.isEmpty() || right.isEmpty())
        {
            LOG_ERROR("empty file name");
            break;
        }
        QFile f1(left);
        if (!f1.open(QIODevice::ReadOnly))
        {
            LOG_ERROR(QString("cannot open %1 for read").arg(left));
            break;
        }

        QFile f2(right);
        if (!f2.open(QIODevice::ReadOnly))
        {
            LOG_ERROR(QString("cannot open %1 for read").arg(right));
            f1.close();
            break;
        }
        ret = compareFiles(f1, f2, ok);
    }
    while(false);
    return ret;
}

bool CTools::compareFiles(QFile& left, QFile& right, bool& ok)
{
    ok = false;
    bool ret = false;
    bool leftNeedToClose = false;
    bool rightNeedToClose = false;
    do
    {
        if (!left.isOpen())
        {
            if (!left.open(QIODevice::ReadOnly))
            {
                LOG_ERROR(QString("cannot open %1 for read").arg(left.fileName()));
                break;

            }
            leftNeedToClose = true;
        }

        if (!right.isOpen())
        {
            if (!right.open(QIODevice::ReadOnly))
            {
                LOG_ERROR(QString("cannot open %1 for read").arg(left.fileName()));
                break;

            }
            rightNeedToClose = true;
        }
        if (left.size() != right.size())
        {
            ok = true;
            break;
        }
        bool sameData = true;
        while(!left.atEnd() && !right.atEnd())
        {
            QByteArray lblock = left.read(FILE_BLOCK_SIZE);
            QByteArray rblock = right.read(FILE_BLOCK_SIZE);
            if (lblock != rblock)
            {
                sameData = false;
                break;
            }
        }
        ok = true;
        ret = sameData;
    }
    while(false);
    if (leftNeedToClose)
    {
        left.close();
    }
    if (rightNeedToClose)
    {
        right.close();
    }
    return ret;
}
