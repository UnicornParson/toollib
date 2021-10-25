#include "syslogappender.h"
#include <QNetworkProxy>
SyslogAppender::SyslogAppender()
{

}

SyslogAppender::~SyslogAppender()
{
  disconnect();
}

void SyslogAppender::append(const QDateTime& timeStamp, Logger::LogLevel logLevel, const char* file, int line,
                            const char* function, const QString& category, const QString& message)
{
  if (file == nullptr || function == nullptr)
  {
    return;
  }

  _queueMessage newEntry;
  newEntry.timeStamp = timeStamp;
  newEntry.logLevel = logLevel;
  memcpy(newEntry.file, file, strnlen(file, _queueMessageBuffSize));
  newEntry.line = line;
  memcpy(newEntry.function, file, strnlen(function, _queueMessageBuffSize));
  newEntry.category = category;
  newEntry.message = message;
  queueMutex.lock();
  messageQueue.append(newEntry);
  qDebug() << "queue size: " << messageQueue.size();
  queueMutex.unlock();
}

bool SyslogAppender::connectToHost(const QString& host, quint16 port, QNetworkProxy proxy)
{
  m_bConnected = false;
  do
  {
    if (host.isEmpty() || port == 0)
    {
      break;
    }
    if (QNetworkProxy::NoProxy != proxy.type())
    {
      m_connection.setProxy(proxy);
    }
    m_connection.connectToHost(host, port);
    if (!m_connection.waitForConnected(2000))
    {
      disconnect();
      break;
    }
    /*
    connect(m_connection, SIGNAL(connected()), SLOT(onConnect()));
    connect(m_connection, SIGNAL(disconnected()), SLOT(onSocketDisconnected()));
    connect(m_connection, SIGNAL(error(QAbstractSocket::SocketError)),
            this,
            SLOT(onSlotError(QAbstractSocket::SocketError)));
    */
    if (QAbstractSocket::ConnectedState != m_connection.state())
    {
      qWarning() << QString("socket state is %1; error code: %2")
                    .arg(m_connection.state())
                    .arg(m_connection.error());
      disconnect();
      break;
    }
    isIterrupted = false;
    start();
    m_bConnected = true;
  }
  while(false);
  return m_bConnected;
}

bool SyslogAppender::disconnect()
{
  if (m_connection.isOpen())
  {
    m_connection.close();
  }
  if (isRunning())
  {
    isIterrupted = true;
  }
  m_bConnected = false;
  return m_bConnected;
}

bool SyslogAppender::isConnected()
{
  return m_bConnected;
}

void SyslogAppender::run()
{
  while(!isIterrupted)
  {
    QThread::msleep(20);
    queueMutex.lock();
    if (messageQueue.isEmpty())
    {
      queueMutex.unlock();
      continue;
    }
    _queueMessage currentMessage = messageQueue.takeFirst();
    queueMutex.unlock();


  }
}
