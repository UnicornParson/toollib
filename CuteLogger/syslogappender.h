#ifndef SYSLOGAPPENDER_H
#define SYSLOGAPPENDER_H
#include "CuteLogger_global.h"
#include "AbstractStringAppender.h"
#include <QTcpSocket>
#include <QString>
#include <QThread>
#include <QObject>
#include <QQueue>
#define _queueMessageBuffSize 512
class SyslogAppender : public QThread, public AbstractStringAppender
{
  //Q_OBJECT
public:
  SyslogAppender();
  ~SyslogAppender();
  virtual void append(const QDateTime& timeStamp, Logger::LogLevel logLevel, const char* file, int line,
                      const char* function, const QString& category, const QString& message);
  bool connectToHost(const QString& host, quint16 port, QNetworkProxy proxy);
  bool disconnect();
  bool isConnected();
  void run();

private:

  struct _queueMessage
  {
    QDateTime timeStamp;
    Logger::LogLevel logLevel;
    char file[_queueMessageBuffSize];
    int line;
    char function[_queueMessageBuffSize];
    QString category;
    QString message;

    _queueMessage()
    {
      timeStamp = QDateTime::currentDateTimeUtc();
      memset(file, 0, _queueMessageBuffSize);
      memset(function, 0, _queueMessageBuffSize);
      line = 0;
      category = QString();
      message = QString();
    }
  };



  QQueue<_queueMessage> messageQueue;
  QMutex queueMutex;
  bool isIterrupted;
  bool m_bConnected;
  QString m_host;
  quint16 m_port;
  QTcpSocket m_connection;

};

#endif // SYSLOGAPPENDER_H
