#ifndef LOGSTARTER_H
#define LOGSTARTER_H
#include "cuteloggerinc.h"

class LogStarter
{
public:
  static bool initLogging(const QString& logHome,
                          const QString& LogFileName = QString(),
                          const QString& customCfgPath = QString());
private:
  struct _logCfg
  {
    bool enableFileLogger;
    bool enableConsoleLogger;
    bool enableRemoteLogger;
    bool enableHTTPLogger;
    QString fileLogLevel;
    QString consoleLogLevel;
    QString remoteLogLevel;
    QString httpLogLevel;

    QString filePrefix;
    int fileSizeLimit;
    QString remoteHost;
    quint16 remotePort;

    QString httpUrl;
    quint16 httpPort;
    QString httpTag;
  };
  void fillDefaultCfg();
  bool writeDefaultCfg(const QString& path);
  bool loadCfg(const QByteArray &data);

  _logCfg cfg;
  static LogStarter instance;
  QByteArray defaultCfg;
};

#endif // LOGSTARTER_H
