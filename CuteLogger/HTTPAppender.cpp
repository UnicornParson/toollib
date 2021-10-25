#include "HTTPAppender.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
HTTPAppender::HTTPAppender():
  isIterrupted(false),
  m_bInitiated(false),
  m_host(QString("0.0.0.0"))
{

}

HTTPAppender::~HTTPAppender()
{
  stopAppender();
}

void HTTPAppender::stopAppender()
{
  isIterrupted = true;
  m_bInitiated = false;
}

void HTTPAppender::append(const QDateTime& timeStamp,
                          Logger::LogLevel logLevel,
                          const char* file,
                          int line,
                          const char* function,
                          const QString& category,
                          const QString& message)
{
  if (!m_bInitiated || isIterrupted)
  {
    // send thread is not started or destroyed
    return;
  }

  _queueMessage msg;
  msg.category = category;
  msg.file = QString(file);
  msg.function = QString(function);
  msg.line = line;
  msg.logLevel = logLevel;
  msg.message = message;
  msg.timeStamp = timeStamp;
  msg.tid = QThread::currentThreadId();

  queueMutex.lock();
  messageQueue.push_back(msg);
  queueMutex.unlock();
}

bool HTTPAppender::init(const QString& host, QNetworkProxy proxy, QString tag)
{
  m_host = host;
  m_proxy = proxy;
  m_tag = tag;
  m_bInitiated = true;
  this->start();
  return true;
}

void HTTPAppender::run()
{
  if (!m_bInitiated)
    return;
  QQueue<_queueMessage> tmpMessageQueue;
  QNetworkAccessManager *manager = new QNetworkAccessManager(this);
  connect(manager, SIGNAL(finished(QNetworkReply*)),
          this, SLOT(replyFinished(QNetworkReply*)));
  connect(manager, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)),
          this, SLOT(slotAuthenticationRequired(QNetworkReply*,QAuthenticator*)));
  GARBAGE_CODE_MARKER
#if 0 //defined(QT_NO_SSL)
  connect(manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
          this, SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
#endif
  while(!isIterrupted)
  {
    queueMutex.lock();
    tmpMessageQueue = messageQueue;
    messageQueue.clear();
    queueMutex.unlock();

    while(!tmpMessageQueue.isEmpty())
    {
      HTTPAppender::_queueMessage msg = tmpMessageQueue.first();
      tmpMessageQueue.removeFirst();

      QString getStr = QString("%1?action=add&tag=%3&%4")
          .arg(m_host)
          .arg(m_tag)
          .arg(msg.buildDataString());
      manager->get(QNetworkRequest(QUrl(getStr)));
    }
    this->msleep(1);
  }
  manager->deleteLater();
}

QString HTTPAppender::base64_encode(QString string)
{
  return string.toUtf8().toBase64();
}

QString HTTPAppender::_queueMessage::buildDataString()
{
  //incorrcect decoding of symbols "<" and ">"
  for(int i = 0; i < message.size(); ++i)
  {
    int li = message.indexOf('<');
    if (li != -1)
    {
      message.replace(li,1,"(");
    }
    int ri = message.indexOf('>');
    if (ri != -1)
    {
      message.replace(ri,1,")");
    }
    if (li == -1 && ri == -1)
    {
      break;
    }
  }
  QString ret = QString("file=%1&function=%2&line=%3&data=%4&tid=%5")
      .arg(base64_encode(file), base64_encode(function))
      .arg(line)
      .arg(base64_encode(QString("[%1]%2: %3").arg(Logger::levelToString(logLevel), category, message)))
      .arg(QString().asprintf("%p", tid));

  return ret;
}

void HTTPAppender::replyFinished(QNetworkReply* reply)
{
  if (reply != nullptr)
  {
    QString err = reply->errorString();
  }
}

void HTTPAppender::slotAuthenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator)
{
  (void)reply;
  (void)authenticator;
  LOG_DEBUG("Authentication Required");
}

void HTTPAppender::sslErrors(QNetworkReply* reply, const QList<QSslError> &errors)
{
  (void)reply;
  QString errorMsg = "SSL error ";
  foreach(QSslError err, errors)
  {
    errorMsg.append(err.errorString() + "; ");
  }
  LOG_WARNING(errorMsg);
}
