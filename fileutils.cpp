#include "fileutils.h"
#include <QFile>
#include "CuteLogger/cuteloggerinc.h"
#include "performancesensor.h"

using namespace Tools;

QByteArray readFile(const QString& filename, bool& isOk)
{
    ADD_PERF_SENSOR;
    QByteArray ret;
    isOk = false;
    do
    {
        if (filename.isEmpty())
        {
            LOG_WARNING("empty filename");
            break;
        }

        QFile f(filename);
        if (!f.open(QIODevice::ReadOnly))
        {
            LOG_WARNING(QString("cannot open file %1 for read").arg(filename));
            break;
        }
        ret = f.readAll();
        f.close();
        isOk = true;
    }
    while(false);
    DESTROY_PERF_SENSOR;
    return ret;
}

QStringList readLines(const QString& filename, bool& isOk, bool trim = true)
{
    ADD_PERF_SENSOR;
    isOk = false;
    QStringList list;
    do
    {
        QByteArray b = readFile(filename, isOk);

        QStringList tmp = QString(b).split(QRegExp("\n|\r\n|\r"), QT_SKIP_EMPTY_PARTS);
        for(const QString& s: tmp)
        {
            QString s1 = s;
            if(trim)
            {
                s1 = s.trimmed();
            }
            if(!s1.isEmpty())
            {
                list.append(s1);
            }
        }
    }
    while(false);
    if(!isOk)
    {
        list.empty();
    }
    DESTROY_PERF_SENSOR;
    return list;
}

bool writeLines(const QString& filename, const QStringList& lines)
{
    ADD_PERF_SENSOR;
    QString text;
    bool isOk = false;
    do
    {
        if (filename.isEmpty())
        {
            LOG_WARNING("empty filename");
            break;
        }

        for(const QString& s: lines)
        {
            text.append(s).append("\n");
        }

        QFile f(filename);
        if (!f.open(QIODevice::WriteOnly))
        {
            LOG_WARNING(QString("cannot open file %1 for write").arg(filename));
            break;
        }
        f.write(text.toUtf8());
        f.flush();
        f.close();
        isOk = true;
    }
    while(false);
    DESTROY_PERF_SENSOR;
    return isOk;
}
