#include "configmanager.h"
#include "CuteLogger/cuteloggerinc.h"
#include "tools.h"
#include "xmutexguard.h"
#include "performancesensor.h"

using namespace Tools;

namespace
{
CONST_LITERAL CATEGORY_XML_TAG("category");
CONST_LITERAL JSON_COMMENT_PREFIX("#");
}

ConfigManager ConfigManager::defaultManager;

ConfigManager::ConfigManager(QObject *parent) noexcept:
    QObject(parent),
    m_loadingIsActive(false),
    m_affectedParams(0)
{
    connect(this, &ConfigManager::loadingStarted, [this]()
    {
        m_affectedParams = 0;
        m_loadingIsActive = true;
    });
    connect(this, &ConfigManager::loadingFailed, [this]()
    {
        m_loadingIsActive = false;
        LOG_TRACE("loading none. affected: %u", m_loadingIsActive);
        m_affectedParams = 0;
    });
    connect(this, &ConfigManager::loadingDone, [this]()
    {
        m_loadingIsActive = false;
        LOG_TRACE("loading none. affected: %u", m_loadingIsActive);
        m_affectedParams = 0;
    });
}

void ConfigManager::setParam(const QString& path, const QVariant value)
{
    if (path.isEmpty())
    {
        return;
    }
    XMutexGuard<XRecursiveMutex> g(&m_treeMutex);
    if (m_runtimeParams.contains(path.toLower()))
    {
        m_runtimeParams.remove(path.toLower());
    }
    m_runtimeParams.insert(path.toLower(), value);
    emit parameterChanged(path.toLower(), value);
}

uint ConfigManager::count() const
{
    return m_params.size() + m_runtimeParams.size();
}

ConfigManager& ConfigManager::getDefaultManager() noexcept
{
    return defaultManager;
}

QString ConfigManager::removeComments(const QString& content) const
{
    QString ret;
    QStringList lines = content.trimmed().split('\n', QT_SKIP_EMPTY_PARTS);
    for (const QString &line: lines)
    {
        QString s = line.trimmed();
        if (s.isEmpty() || s.startsWith(JSON_COMMENT_PREFIX))
        {
            continue;
        }
        ret.append(s);
    }
    return ret;
}

bool ConfigManager::loadJson(const QString& path)
{
    ADD_PERF_SENSOR_EX(path);
    XMutexGuard<XRecursiveMutex> g(&m_treeMutex);
    m_params.clear();
    bool ret = appendJson(path);
    DESTROY_PERF_SENSOR;
    return ret;
}

bool ConfigManager::appendJson(const QString& path, const QString& mountPoint)
{
    QString npath = CTools::pathNormalize(path);
    ADD_PERF_SENSOR_EX(npath);
    emit loadingStarted();
    bool bRet = false;
    do
    {
        if (npath.isEmpty())
        {
            LOG_WARNING("empty path");
            emit loadingFailed();
            break;
        }
        QFile configFile(npath);
        bool rc = configFile.open(QIODevice::ReadOnly);
        if (!rc)
        {
            LOG_WARNING(QString("Cannot open file %1").arg(npath));
            emit loadingFailed();
            break;
        }

        QString jsonData = QString(configFile.readAll());
        QString cleanData = removeComments(jsonData);
        QtJson::JsonObject obj = QtJson::parse(cleanData, bRet).toMap();
        if (obj.isEmpty() || !bRet)
        {
            LOG_WARNING(QString("empty configuration in %1").arg(npath));
            emit loadingFailed();
            break;
        }
        bRet = appendJson(obj, mountPoint);
    }
    while(false);
    DESTROY_PERF_SENSOR;
    if (bRet)
    {
        emit loadingDone();
    }
    else
    {
        emit loadingFailed();
    }
    return bRet;
}

bool ConfigManager::loadJsonMulti(const QList<ConfigSeqEntry>& queue)
{
    ADD_PERF_SENSOR_EX("json objects seq");
    m_params.clear();
    bool bRet = appendJsonMulti(queue);
    DESTROY_PERF_SENSOR;
    return bRet;
}

bool ConfigManager::appendJsonMulti(const QList<ConfigSeqEntry>& queue, const QString& mountPoint)
{
    if (queue.isEmpty())
    {
        return true;
    }
    bool bRet = true;
    m_latestStable = m_params;
    bool restoreStable = false;
    for (const ConfigSeqEntry &entry : queue)
    {
        bool entryRc = false;
        switch(entry.type)
        {
        case ConfigSeqEntry::SkipEntry:
        {
            continue;
        }
        case ConfigSeqEntry::PathBasedEntry:
        {
            entryRc = appendJson(entry.path, mountPoint);
            if (!entryRc)
            {
                LOG_WARNING(QString("Loading %1 failed").arg(entry.name));
                if (entry.required)
                {
                    LOG_WARNING("sequence aborted");
                    bRet = false;
                    restoreStable = true;
                    break;
                }
            }
            break;
        }
        case ConfigSeqEntry::ContentBasedEntry:
        {
            QString cleanData = removeComments(entry.data);
            QtJson::JsonObject obj = QtJson::parse(cleanData, bRet).toMap();
            entryRc = appendJson(obj, mountPoint);
            if (!entryRc)
            {
                LOG_WARNING(QString("Loading %1 failed").arg(entry.name));
                if (entry.required)
                {
                    bRet = false;
                    LOG_WARNING("sequence aborted");
                    restoreStable = true;
                    break;
                }
            }
        }
        }
        if (restoreStable)
        {
            LOG_WARNING("restore last stable configuration");
            m_params = std::move(m_latestStable);
            m_latestStable.clear();
        }
    }
    return bRet;
}

bool ConfigManager::loadJson(const QtJson::JsonObject object)
{
    ADD_PERF_SENSOR_EX("json object");
    m_params.clear();
    bool bRet = appendJson(object);
    DESTROY_PERF_SENSOR;
    return bRet;
}

bool ConfigManager::appendJson(const QtJson::JsonObject object, const QString& mountPoint)
{
    emit loadingStarted();
    bool bRet = true;
    ADD_PERF_SENSOR_EX("json object");
    LOG_TRACE(QString("mount config tree to %1").arg(mountPoint));
    for (const QString &key: object.keys())
    {
        QString fullKey = key.toLower();
        if (!mountPoint.isEmpty())
        {
            fullKey = mountPoint + "/" + key.toLower();
        }
        QVariant value = object.value(key);
        if (value.type() == QVariant::Map)
        {
            bRet = appendJson(value.toMap(), fullKey);
            if (!bRet)
            {
                LOG_WARNING(QString("cannot mount subtree to %1").arg(fullKey));
                emit loadingFailed();
                break;
            }
            continue;
        }
        LOG_TRACE(QString("add param %1").arg(fullKey));
        XMutexGuard<XRecursiveMutex> g(&m_treeMutex);
        m_params.insert(fullKey.toLower(), value);
        ++m_affectedParams;
        emit parameterChanged(fullKey.toLower(), value);
    }
    DESTROY_PERF_SENSOR;
    if (bRet)
    {
        emit loadingDone();
    }
    else
    {
        emit loadingFailed();
    }
    return bRet;
}

bool ConfigManager::loadXML(const QString& path)
{
    LOG_FATAL("this function cannot be called. XML configs temporary not supported");
    XMutexGuard<XRecursiveMutex> g(&m_treeMutex);
    m_params.clear();
    return appendXML(path);
}

bool ConfigManager::appendXML(const QString& path)
{
    LOG_FATAL("this function cannot be called. XML configs temporary not supported");
    bool ret = false;
    do
    {
        if (path.isEmpty())
        {
            LOG_WARNING("path is empty");
            break;
        }
        LOG_TRACE(QString("Loading configs from %1").arg(path));

        QString npath = CTools::pathNormalize(path);
        QFile configFile(npath);
        bool rc = configFile.open(QIODevice::ReadOnly);
        if (!rc)
        {
            LOG_WARNING(QString("Cannot open file %1").arg(npath));
            break;
        }

        QDomDocument domDoc;
        QString errMsg;
        int errorLine = 0;
        int errorColumn = 0;
        LOG_TRACE(QString("Try to load %1").arg(npath));

        QString XMLData =  QString(configFile.readAll());
        configFile.close();
        if (false == domDoc.setContent(XMLData, &errMsg, &errorLine, &errorColumn))
        {
            LOG_WARNING(QString("XML parsing error. [%1] line %2, column %3")
                        .arg(errMsg)
                        .arg(errorLine)
                        .arg(errorColumn));
            LOG_TRACE_LINE;
            LOG_TRACE(XMLData);
            LOG_TRACE_LINE;

            break;
        }

        QDomElement content = domDoc.firstChildElement("config");
        if (content.isNull())
        {
            LOG_WARNING("invalid document. config section doesn't exist");
            break;
        }

        QDomElement element = content.firstChildElement();
        ret = true;
        while(!element.isNull())
        {
            bool rc = false;
            if (element.nodeName().toLower() == CATEGORY_XML_TAG)
            {
                LOG_TRACE(QString("category %1 is found").arg(element.attribute("name")));
                rc = loadXMLCategory(&element);
            }
            else
            {
                LOG_TRACE("param item is found");
                rc = loadXMLParam(&element);
            }
            if (!rc)
            {
                LOG_WARNING("loading error");
                ret = false;
                break;
            }
            element = element.nextSiblingElement();
        }
    }
    while(false);
    return ret;
}

bool ConfigManager::loadXMLParam(QDomElement* element, const QString &mountPoint)
{
    bool ret = false;
    do
    {

        if (nullptr == element || element->isNull())
        {
            LOG_WARNING("element is null");
            break;
        }

        QString paramName = QString("%1/%2").arg(mountPoint, element->nodeName());
        LOG_TRACE(QString("add param %1").arg(paramName));
        XMutexGuard<XRecursiveMutex> g(&m_treeMutex);
        m_params.insert(paramName.toLower(), QVariant(element->text()));
        ret = true;
    }
    while(false);
    return ret;
}

bool ConfigManager::loadXMLCategory(QDomElement* element, const QString &mountPoint)
{
    bool ret = false;
    do
    {
        if (element->isNull())
        {
            LOG_WARNING("element is null");
            break;
        }
        QString tmpPath(mountPoint);
        QString elementName = element->attribute("name");
        if (mountPoint.isEmpty())
        {
            tmpPath = elementName;
        }
        else
        {
            tmpPath = QString("%1/%2").arg(mountPoint, elementName);
        }
        if (elementName.isEmpty())
        {
            LOG_WARNING("category without name attribute");
            break;
        }
        QDomElement subelement = element->firstChildElement();

        ret = true;
        while(!subelement.isNull())
        {
            bool rc = false;

            if (subelement.nodeName().toLower() == CATEGORY_XML_TAG)
            {
                LOG_TRACE(QString("category %1 is found in %2").arg(subelement.attribute("name"), tmpPath));
                rc = loadXMLCategory(&subelement, tmpPath);
            }
            else
            {
                LOG_TRACE(QString("param item %1 is found in %2").arg(subelement.nodeName().toLower(), tmpPath));
                rc = loadXMLParam(&subelement, tmpPath);
            }
            if (!rc)
            {
                LOG_WARNING("loading error");
                ret = false;
                break;
            }
            subelement = subelement.nextSiblingElement();
        }
    }
    while(false);
    return ret;
}

bool ConfigManager::contains(const QString& path)
{
    XMutexGuard<XRecursiveMutex> g(&m_treeMutex);
    return m_params.contains(path.toLower()) || m_runtimeParams.contains(path.toLower());
}

bool ConfigManager::checkParam(const QString& path, const ConfigParamChecker_func& checker)
{
    XMutexGuard<XRecursiveMutex> g(&m_treeMutex);

    if (m_params.contains(path.toLower()))
    {
        return checker(m_params[path.toLower()]);
    }

    if (m_runtimeParams.contains(path.toLower()))
    {
        return checker(m_runtimeParams[path.toLower()]);
    }
    return false;
}


QVariant ConfigManager::getParam(const QString& path, bool& isOk)
{
    ADD_PERF_SENSOR_EX(path);
    QVariant ret;
    isOk = false;
    do
    {
        if (path.isEmpty())
        {
            LOG_WARNING("empty path");
            break;
        }
        XMutexGuard<XRecursiveMutex> g(&m_treeMutex);
        if (m_runtimeParams.contains(path.toLower()))
        {
            ret = m_runtimeParams.value(path.toLower());
            isOk = true;
            break;
        }
        ret = getParamOrigin(path, isOk);
    }
    while(false);
    DESTROY_PERF_SENSOR;
    return ret;
}

bool ConfigManager::getFlag(const QString& path, bool defaultValue)
{
    bool ret = defaultValue;
    QString val = getParam(path, ret).toString().trimmed().toLower();
    if (ret)
    {
        ret = (val == "true");
    }
    return ret;
}

QVariant ConfigManager::getParamOrigin(const QString& path, bool& isOk)
{
    ADD_PERF_SENSOR_EX(path);
    QVariant ret;
    isOk = false;
    do
    {
        if (path.isEmpty())
        {
            LOG_WARNING("empty path");
            break;
        }
        XMutexGuard<XRecursiveMutex> g(&m_treeMutex);
        if (!m_params.contains(path.toLower()))
        {
            LOG_WARNING(QString("param %1 is not found").arg(path.toLower()));
            break;
        }
        ret = m_params.value(path.toLower());
        isOk = true;
    }
    while(false);
    DESTROY_PERF_SENSOR;
    return ret;
}

bool ConfigManager::getFlagOrigin(const QString& path, bool defaultValue)
{
    bool ret = defaultValue;
    QString val = getParamOrigin(path, ret).toString().trimmed().toLower();
    if (ret)
    {
        ret = (val == "true");
    }
    return ret;
}

QStringList ConfigManager::getDump()
{
    ADD_PERF_SENSOR;
    QStringList list;
    XMutexGuard<XRecursiveMutex> g(&m_treeMutex);
    for (const QString& key: m_params.keys())
    {
        bool ok = false;
        QVariant actual = getParam(key, ok);
        QString sActual = (ok) ? actual.toString() : "[[MISSED]]";
        QVariant origin = getParamOrigin(key, ok);
        QString sOrigin = (ok) ? origin.toString() : "[[MISSED]]";
        list.append(QString("[%1] >> actual:%2, origin:%3").arg(key, sActual, sOrigin));
    }
    LOG_TRACE("%lld parameters are dumped", m_params.size());
    DESTROY_PERF_SENSOR;
    return list;
}

void ConfigManager::printDump()
{
    for (const QString& line: getDump())
    {
        LOG_TRACE(line);
    }
}

QString ConfigManager::getDumpString()
{
    QString dumpTest;
    for (const QString& line: getDump())
    {
        dumpTest += line + "\n";
    }
    return dumpTest;
}

bool ConfigManager::dumpPlain(const QString& path)
{
    ADD_PERF_SENSOR_EX(path);
    bool bRet = false;
    do
    {
        if (path.isEmpty())
        {
            LOG_ERROR("empty path");
            break;
        }
        QFile out(path);
        bool openRc = out.open(QIODevice::WriteOnly);
        if (!openRc)
        {
            LOG_ERROR(QString("cannot open file %1 for write").arg(path));
            break;
        }
        XMutexGuard<XRecursiveMutex> g(&m_treeMutex);
        out.write(getDumpString().toLatin1());
        out.flush();
        out.close();
        bRet = true;
    }
    while(false);
    DESTROY_PERF_SENSOR;
    return bRet;
}

