#include "logstarter.h"
#include "json.h"
#include <iostream>
#define CFG_NAME "log.cfg"
#include "syslogappender.h"
#include "HTTPAppender.h"
#include "tools.h"
LogStarter LogStarter::instance;

bool LogStarter::initLogging(const QString& logHome, const QString& LogFileName, const QString& customCfgPath)
{
    bool bRet = false;
    do
    {
        instance.fillDefaultCfg();
        if (logHome.isEmpty())
        {
            break;
        }
        QString cfgPath = Tools::CTools::pathNormalize(customCfgPath.trimmed());
        if (cfgPath.isEmpty())
        {
            cfgPath = Tools::CTools::pathNormalize(logHome + CFG_NAME);
        }
        QFile cfgFile(cfgPath);
        if (!cfgFile.exists())
        {
            LOG_DEBUG(QString("log config is not found in %1, write default").arg(cfgPath));
            instance.writeDefaultCfg(cfgPath);
            if (!instance.loadCfg(instance.defaultCfg))
            {
                qWarning("cannot load default log configs");
                break;
            }
            qDebug("default log config is applied");
        }
        else
        {
            LOG_DEBUG(QString("try to load %1").arg(cfgPath));
            if (!cfgFile.open(QFile::ReadOnly))
            {
                qWarning("cannot open log config file to read");
                break;
            }
            if (!instance.loadCfg(cfgFile.readAll()))
            {
                qWarning("cannot load log configs");
                break;
            }
            qDebug("log config is applied");
        }

        if (instance.cfg.enableFileLogger)
        {
            QString logPath = QString(logHome);
            if (LogFileName.isEmpty())
            {
                logPath += QString("%1\\%2.log").arg(instance.cfg.filePrefix, QDateTime::currentDateTime().toString("dd.MM.yyyy_hh.mm.ss"));
            }
            else
            {
                logPath += "\\" + LogFileName;
            }

            FileAppender *fileLogger = new FileAppender(logPath);
            fileLogger->setLogFileSizeLimitMB(instance.cfg.fileSizeLimit);
            fileLogger->setDetailsLevel(instance.cfg.fileLogLevel);
            qDebug("log to %s. level:%s, size limit: %d", SCHR(logPath), SCHR(instance.cfg.fileLogLevel), instance.cfg.fileSizeLimit);
            logger->registerAppender((static_cast<AbstractAppender*>(fileLogger)));
        }

        if (instance.cfg.enableConsoleLogger)
        {
            ConsoleAppender *consoleLogger = new ConsoleAppender();
            consoleLogger->setDetailsLevel(instance.cfg.consoleLogLevel);
            logger->registerAppender((static_cast<AbstractAppender*>(consoleLogger)));
        }
        if (instance.cfg.enableHTTPLogger)
        {
            HTTPAppender *httpAppender = new HTTPAppender();
            httpAppender->setDetailsLevel(instance.cfg.httpLogLevel);
            httpAppender->init(instance.cfg.httpUrl,
                               QNetworkProxy(QNetworkProxy::NoProxy),
                               instance.cfg.httpTag);
            logger->registerAppender((static_cast<AbstractAppender*>(httpAppender)));
        }
        /*
    if (instance.cfg.enableRemoteLogger)
    {
      SyslogAppender *remoteLogger = new SyslogAppender();
      //bool rc = remoteLogger->connectToHost()
      consoleLogger->setDetailsLevel(instance.cfg.consoleLogLevel);
      logger->registerAppender(((AbstractAppender*)consoleLogger));
    }
    */
        bRet = true;
    }
    while(false);
    return bRet;
}



bool LogStarter::writeDefaultCfg(const QString& path)
{
    bool bRet = false;
    do
    {
        fillDefaultCfg();
        if (path.isEmpty())
        {
            break;
        }
        QFile f(path);
        if (!f.open(QFile::WriteOnly))
        {
            qWarning("cannot open log config file to write");
            break;
        }
        f.write(defaultCfg);
        f.flush();
        f.close();
    }
    while(false);
    return bRet;
}

bool LogStarter::loadCfg(const QByteArray& data)
{
    bool bRet = false;
    do
    {
        QtJson::JsonObject cfgObject = QtJson::parse(QString(data), bRet).toMap();
        if (!bRet)
        {
            qWarning("cannot parse log config file");
            break;
        }
        bRet = false;
        const QString fileSectionKey = "FileLogger";
        const QString consoleSectionKey = "ConsoleLogger";
        const QString remoteSectionKey = "RemoteLogger";
        const QString httpSectionKey = "HTTPLogger";
        const QString enableKey = "enable";
        const QString levelKey = "level";
        const QString prefixKey = "prefix";
        const QString hostKey = "host";
        const QString portKey = "port";
        const QString tagKey = "tag";
        const QString urlKey = "url";
        const QString fileSizeLimitKey = "sizeLimit";
        if (!cfgObject.contains(fileSectionKey))
        {
            cfg.enableFileLogger = false;
        }
        else
        {
            QtJson::JsonObject fileCfg = cfgObject.value(fileSectionKey).toMap();
            cfg.enableFileLogger = fileCfg.value(enableKey).toBool();
            cfg.filePrefix = fileCfg.value(prefixKey, QString("LOG_")).toString();
            cfg.fileLogLevel = fileCfg.value(levelKey, QString("trace")).toString();
            cfg.fileSizeLimit = fileCfg.value(fileSizeLimitKey, QString("-1")).toInt();
        }

        if (!cfgObject.contains(consoleSectionKey))
        {
            cfg.enableConsoleLogger = false;
        }
        else
        {
            QtJson::JsonObject consoleCfg = cfgObject.value(consoleSectionKey).toMap();
            cfg.enableConsoleLogger = consoleCfg.value(enableKey).toBool();
            cfg.consoleLogLevel = consoleCfg.value(levelKey, QString("trace")).toString();
        }

        if (!cfgObject.contains(remoteSectionKey))
        {
            cfg.enableConsoleLogger = false;
        }
        else
        {
            QtJson::JsonObject remoteCfg = cfgObject.value(remoteSectionKey).toMap();
            cfg.enableRemoteLogger = remoteCfg.value(enableKey).toBool();
            cfg.remoteLogLevel = remoteCfg.value(levelKey, QString("trace")).toString();
            cfg.remoteHost = remoteCfg.value(hostKey, QString("127.0.0.1")).toString();
            cfg.remotePort = static_cast<quint16>(remoteCfg.value(portKey, 601).toUInt());
        }

        if (!cfgObject.contains(httpSectionKey))
        {
            cfg.enableHTTPLogger = false;
        }
        else
        {
            QtJson::JsonObject remoteCfg = cfgObject.value(httpSectionKey).toMap();
            cfg.enableHTTPLogger = remoteCfg.value(enableKey).toBool();
            cfg.httpLogLevel = remoteCfg.value(levelKey, QString("trace")).toString();
            cfg.httpUrl = remoteCfg.value(urlKey, QString("127.0.0.1")).toString();
            cfg.httpTag = remoteCfg.value(tagKey, QString("DEFAULT_TAG")).toString();
        }
        bRet = true;
    }
    while(false);
    return bRet;
}

void LogStarter::fillDefaultCfg()
{
    /*
    {
      //default log settings
      "FileLogger" :
      {
        "enable": true,
        "prefix": "LOG_",
        "level": "trace"
      }
      "ConsoleLogger" :
      {
        "enable": true,
        "level": "trace"
      }
      "RemoteLogger" :
      {
        "enable": true,
        "level": "trace",
        "host": "127.0.0.1",
        "port":601
      }
      "HTTPLogger" :
      {
        "enable": false,
        "level": "trace",
        "url": "http://127.0.0.1",
        "tag": "DEFAULT_TAG"
      }
    }
  */
    defaultCfg.clear();
    defaultCfg = QByteArray::fromBase64(
                "ICAgIHsKICAgICAgLy9kZWZhdWx0IGxvZyBzZXR0a"
                "W5ncwogICAgICAiRmlsZUxvZ2dlciIgOgogICAgIC"
                "B7CiAgICAgICAgImVuYWJsZSI6IHRydWUsCiAgICA"
                "gICAgInByZWZpeCI6ICJMT0dfIiwKICAgICAgICAi"
                "bGV2ZWwiOiAidHJhY2UiCiAgICAgIH0KICAgICAgI"
                "kNvbnNvbGVMb2dnZXIiIDoKICAgICAgewogICAgIC"
                "AgICJlbmFibGUiOiB0cnVlLAogICAgICAgICJsZXZ"
                "lbCI6ICJ0cmFjZSIKICAgICAgfQogICAgICAiUmVt"
                "b3RlTG9nZ2VyIiA6CiAgICAgIHsKICAgICAgICAiZ"
                "W5hYmxlIjogdHJ1ZSwKICAgICAgICAibGV2ZWwiOi"
                "AidHJhY2UiLAogICAgICAgICJob3N0IjogIjEyNy4"
                "wLjAuMSIsCiAgICAgICAgInBvcnQiOjYwMQogICAg"
                "ICB9CiAgICAgICJIVFRQTG9nZ2VyIiA6CiAgICAgI"
                "HsKICAgICAgICAiZW5hYmxlIjogZmFsc2UsCiAgIC"
                "AgICAgImxldmVsIjogInRyYWNlIiwKICAgICAgICA"
                "idXJsIjogImh0dHA6Ly8xMjcuMC4wLjEiLAogICAg"
                "ICAgICJwb3J0IjogODAsCiAgICAgICAgInRhZyI6I"
                "CJERUZBVUxUX1RBRyIKICAgICAgfQogICAgfQ==");
}
