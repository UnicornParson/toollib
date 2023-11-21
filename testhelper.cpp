#include "testhelper.h"
#include <QUuid>
#include "CuteLogger/Logger.h"
#include "qglobal.h"
#include "qjsonobject.h"
#include "securityutils.h"
#include "xmutexguard.h"

using namespace Tools;
using namespace QtJson;

namespace
{
MAYBE_UNUSED_ATTR constexpr int itemsCount = 10;
MAYBE_UNUSED_ATTR constexpr int tagsCount = 3;
MAYBE_UNUSED_ATTR constexpr int finalNodesCount = 3;
MAYBE_UNUSED_ATTR constexpr int jsonNodesCount = 3;
}

TestHelper::TestHelper()
{

}
TestHelper::~TestHelper()
{

}

QString TestHelper::camelizeStrRegister(const QString& in)
{
    QString out;
    for (int i = 0; i < in.size(); ++i)
    {
        out.append((i % 2 == 0)?(in.at(i).toUpper()):(in.at(i).toLower()));
    }
    return out;
}

double TestHelper::getRandDouble()
{
    return SecurityUtils::randDouble(0, 10000);
}

int TestHelper::getRandInt()
{
    return SecurityUtils::rand(INT_MIN, INT_MAX);
}

unsigned int TestHelper::getRandUInt()
{
    return static_cast<unsigned int>(SecurityUtils::rand(0, INT_MAX));
}

quint64 TestHelper::getRandUInt64()
{
    return ((static_cast<quint64>(getRandUInt()) << 32) | static_cast<quint64>(getRandUInt())) & 0x1fffffffffffffff;
}

QString TestHelper::getUnicRandomStr(int size)
{
    static quint64 salt = 0;
    QByteArray data = QUuid::createUuid().toByteArray();
    data.append(reinterpret_cast<char*>(&salt), sizeof(quint64));
    salt++;
    data.append(TOQSTR(QDateTime::currentMSecsSinceEpoch()).toLatin1());
    data.append(TOQSTR(getRandDouble()).toLatin1());
    QString ret = QString(data.toHex());
    if (size > 0)
    {
        ret = QString(data.toHex()).right(size);
    }

    return ret;
}

QString TestHelper::getShortUnicRandomStr()
{
    static quint64 salt = 0;
    QString ret = QString("str%1").arg(salt);
    salt++;
    return ret;
}

QtJson::JsonObject TestHelper::generateJSONObject(int depth)
{
    JsonObject ret;
    for (int i = 0; i < finalNodesCount; ++i)
    {
        QString nodeName = getShortUnicRandomStr();
        QString nodeValue = getShortUnicRandomStr();
        if (ret.contains(nodeName))
        {
            i--;
            continue;
        }
        ret.insert(nodeName, nodeValue);
    }
    if (depth > 0)
    {
        depth--;
        for (int i = 0; i < jsonNodesCount; ++i)
        {
            QString nodeName = getShortUnicRandomStr();
            if (ret.contains(nodeName))
            {
                i--;
                continue;
            }
            JsonObject subnode = generateJSONObject(depth);
            ret.insert(nodeName, subnode);
        }
    }
    return ret;
}

quint64 TestHelper::counter()
{
    static quint64 c = 0;
    XMutexGuard mg(&m_counterMutex);
    return c++;
}
