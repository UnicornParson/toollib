#include "interfaces.h"
#include <QFile>

#include "memcontrol.h"
#include "tools.h"
#include "fileutils.h"

using namespace Tools;


DataObject::DataObject(){CONSTRUCTOR_SERSOR(DataObject);}
DataObject::~DataObject(){DESTRUCTOR_SERSOR(DataObject);}
SerializableDataObject::SerializableDataObject(){CONSTRUCTOR_SERSOR(SerializableDataObject);}
SerializableDataObject::~SerializableDataObject(){DESTRUCTOR_SERSOR(SerializableDataObject);}

bool DataObject::isEmpty() const {return false;}

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
bool IJSerializable::deserializeStr(const QString& in)
{
    bool isOk = false;
    QtJson::JsonObject obj = QtJson::parse(in, isOk).toMap();
    if (!isOk)
    {
        LOG_ERROR(QString("cannot parse string %1").arg(in));
        return false;
    }
    return deserialize(obj);
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
        QtJson::JsonObject obj = FileUtils::readJson(path, rc);
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
            LOG_ERROR("write error. %lld written, %lld expected", size, data.size());
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
