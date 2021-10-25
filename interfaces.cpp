#include "interfaces.h"
#include <QFile>

#include "tools.h"

using namespace Tools;

ISerializable::ISerializable() noexcept{}
IJSerializable::IJSerializable() noexcept{}
IPrintable::IPrintable() noexcept{}

ISerializable::~ISerializable() noexcept{}
IJSerializable::~IJSerializable() noexcept{}
IPrintable::~IPrintable() noexcept{}

void IPrintable::print() const
{
    LOG_TRACE(QString("(%1) %2").arg(objectName(), toString()));
}
QString IPrintable::className() const
{
    return "IPrintable" ;
}
QString IPrintable::objectName() const
{
    return QString("%1[%2]").arg(className()).arg(uintptr_t(this));
}
bool IJSerializable::fromFile(const QString& path)
{
  bool rc = false;
  do
  {
    if (path.isEmpty())
    {
      LOG_ERROR("empty path");
      break;
    }
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly))
    {
      LOG_ERROR(QString("cannot open file %1 for read").arg(path));
      break;
    }
    QByteArray data = f.readAll();
    f.close();
    QtJson::JsonObject obj = QtJson::parse(QString(data), rc).toMap();
    if (!rc)
    {
      LOG_ERROR(QString("cannot parse file %1. it's not a valid json").arg(path));
      break;
    }
    rc = deserialize(obj);
  }
  while(false);
  return rc;
}

bool IJSerializable::toFile(const QString& path)
{
  bool rc = false;
  do
  {
    if (path.isEmpty())
    {
      LOG_ERROR("empty path");
      break;
    }
    QByteArray data = QtJson::serialize(serialize(), rc);
    if (!rc)
    {
      LOG_ERROR("cannot serialize");
      break;
    }
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly))
    {
      LOG_ERROR(QString("cannot open file %1 for write").arg(path));
      break;
    }
    qint64 size = f.write(data);
    if (size != data.size())
    {
      LOG_ERROR("write error. %lld written, %d expected", size, data.size());
    }
    else
    {
      LOG_TRACE("write done. size: %lld", size);
      f.flush();
      rc = true;
    }
    f.close();
  }
  while(false);
  return rc;
}
