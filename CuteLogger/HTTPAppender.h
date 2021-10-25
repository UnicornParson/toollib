#ifndef HTTPAPPENDER_H
#define HTTPAPPENDER_H
#include "CuteLogger_global.h"
#include "AbstractStringAppender.h"
#include <QTcpSocket>
#include <QString>
#include <QThread>
#include <QObject>
#include <QQueue>
#include <QNetworkProxy>
#include <QNetworkReply>
#include <QSslError>
class HTTPAppender: public QThread, public AbstractStringAppender
{
public:
  HTTPAppender();
  ~HTTPAppender();
  virtual void append(const QDateTime& timeStamp, Logger::LogLevel logLevel, const char* file, int line,
                      const char* function, const QString& category, const QString& message);
  bool init(const QString& host, QNetworkProxy proxy, QString tag);

  void stopAppender();

public slots:
  void replyFinished(QNetworkReply* reply);
  void slotAuthenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator);
  void sslErrors(QNetworkReply* reply, const QList<QSslError> &errors);
protected:
   void run();
private:
  struct _queueMessage
  {
    QDateTime timeStamp;
    Logger::LogLevel logLevel;
    QString file;
    int line;
    QString function;
    QString category;
    QString message;
    Qt::HANDLE tid;

    _queueMessage():
      timeStamp(QDateTime::currentDateTimeUtc()),
      logLevel(Logger::Debug),
      file(""),
      line(0),
      function(""),
      category(""),
      message(""),
      tid(nullptr)
    {
    }

    QString buildDataString();
  };
  static QString base64_encode(QString string);
  QQueue<_queueMessage> messageQueue;
  QMutex queueMutex;
  bool isIterrupted;
  bool m_bInitiated;
  QString m_host;
  QNetworkProxy m_proxy;
  QString m_tag;
};

#endif // HTTPAPPENDER_H
