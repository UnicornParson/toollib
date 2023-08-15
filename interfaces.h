#ifndef INTERFACES
#define INTERFACES

#include <QByteArray>
#include <json.h>
#include <QObject>
#include "toolsmacro.h"


class ISerializable
{
public:
    [[deprecated]] ISerializable() noexcept;
    [[deprecated]] virtual ~ISerializable() noexcept;
    [[deprecated]] virtual QByteArray serialize() const = 0;
    [[deprecated]] virtual bool deserialize(const QByteArray& in) = 0;
};

class IJSerializable
{
public:
    IJSerializable() noexcept;
    virtual ~IJSerializable() noexcept;
    virtual QtJson::JsonObject serialize() = 0;
    virtual bool deserialize(const QtJson::JsonObject& in) = 0;
    ATTR_NODISCARD virtual bool fromFile(const QString& path);
    ATTR_NODISCARD virtual bool toFile(const QString& path);
};

class IPrintable
{
public:
    IPrintable() noexcept;
    virtual ~IPrintable() noexcept;
    virtual void print() const;
    virtual QString toString() const = 0;
    virtual QString className() const;
    virtual QString objectName() const;

};

#define PRINTABLE_OBJECT_IMPL(c) \
    public: \
    virtual QString className() const Q_DECL_OVERRIDE {return #c ;} \
    virtual QString objectName() const Q_DECL_OVERRIDE {return QString("%1[%2]").arg(className()).arg(intptr_t(this));}

class DataObject
{
public:
    DataObject();
    virtual ~DataObject();

    virtual void reset() = 0;
};

#endif // INTERFACES

