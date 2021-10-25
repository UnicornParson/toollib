#ifndef MEMCONTROL_H
#define MEMCONTROL_H

#include <typeinfo>

#include <QString>
#include <QFile>

#include "xmutex.h"

#if defined(MEM_CONTROL_LOG) || defined(MEM_CONTROL_CSV)
#define MEM_CONTROL
#endif

namespace Tools
{
#ifdef MEM_CONTROL
class MemControlPrivate
{
public:
    MemControlPrivate() = default;
    virtual ~MemControlPrivate() = default;
    virtual void setTargetDir(const QString& path) { Q_UNUSED(path) }
    virtual void addPtr(const char* file,
                        const char* function,
                        int line,
                        void* x,
                        int size,
                        void* owner,
                        const char* className,
                        const char* comment) = 0;

    virtual void removePtr(const char* file,
                           const char* function,
                           int line,
                           void* x,
                           const char* comment) = 0;
};

class MemControlPrivateConsole: public MemControlPrivate
{
public:
    MemControlPrivateConsole() = default;
    virtual ~MemControlPrivateConsole() Q_DECL_OVERRIDE = default;
    virtual void addPtr(const char* file,
                        const char* function,
                        int line,
                        void* x,
                        int size,
                        void* owner,
                        const char* className,
                        const char* comment) Q_DECL_OVERRIDE;

    virtual void removePtr(const char* file,
                           const char* function,
                           int line,
                           void* x,
                           const char* comment) Q_DECL_OVERRIDE;
};

class MemControlPrivateCSV: public MemControlPrivate
{
public:
    MemControlPrivateCSV() = default;
    virtual ~MemControlPrivateCSV() Q_DECL_OVERRIDE = default;

    virtual void setTargetDir(const QString& path) Q_DECL_OVERRIDE;

    virtual void addPtr(const char* file,
                        const char* function,
                        int line,
                        void* x,
                        int size,
                        void* owner,
                        const char* className,
                        const char* comment) Q_DECL_OVERRIDE;

    virtual void removePtr(const char* file,
                           const char* function,
                           int line,
                           void* x,
                           const char* comment) Q_DECL_OVERRIDE;
private:
    void write(const QString& msg);
    QString m_targetPath;
    QFile m_file;
    XNonRecursiveMutex m_fileMutex;
    XNonRecursiveMutex m_cacheMutex;
    QStringList m_cache;
};

class MemControl
{
public:
    ~MemControl();
    static void setTargetDir(const QString& path);

    static void addPtr(const char* file,
                       const char* function,
                       int line,
                       void* x,
                       int size,
                       void* owner,
                       const char* className,
                       const char* comment);

    static void removePtr(const char* file,
                          const char* function,
                          int line,
                          void* x,
                          const char* comment);

private:
    Q_DISABLE_COPY_MOVE(MemControl)
    static MemControlPrivate* m_private;
    MemControl();
    static void checkPrivate();
};


#define TYPENAME(x) (((x != nullptr) ? typeid(*x).name() : "nullptr"))
#define ADD_POINTER_SENSOR_WOTHIS(x, y) Tools::MemControl::addPtr(__FILE__, __FUNCTION__, __LINE__, static_cast<void*>(x), y, nullptr, TYPENAME(x), "nocomment")
#define ADD_POINTER_SENSOR_CUSTOMSIZE(x, y) Tools::MemControl::addPtr(__FILE__, __FUNCTION__, __LINE__, static_cast<void*>(x), y, static_cast<void*>(this), TYPENAME(x), "nocomment")
#define ADD_POINTER_SENSOR(ptr) ADD_POINTER_SENSOR_CUSTOMSIZE(ptr, sizeof(*ptr))
#define REMOVE_POINTER_SENSOR(x) Tools::MemControl::removePtr(__FILE__, __FUNCTION__, __LINE__, static_cast<void*>(x), "nocomment")
#define ADD_POINTER_SENSOR_EX(/* ptr */x, /* size */y, /* comment */z) Tools::MemControl::addPtr(__FILE__, __FUNCTION__, __LINE__, static_cast<void*>(x), y, static_cast<void*>(this), TYPENAME(x), z)
#define REMOVE_POINTER_SENSOR_EX(x, z) Tools::MemControl::removePtr(__FILE__, __FUNCTION__, __LINE__, static_cast<void*>(x), z)
#define CONSTRUCTOR_SERSOR(name) ADD_POINTER_SENSOR_EX(this, sizeof(name), #name)
#define DESTRUCTOR_SERSOR(name) REMOVE_POINTER_SENSOR_EX(this, #name)

#else // MEM_CONTROL
#define ADD_POINTER_SENSOR_CUSTOMSIZE(x, y)
#define ADD_POINTER_SENSOR_WOTHIS(x, y)
#define ADD_POINTER_SENSOR(x)
#define REMOVE_POINTER_SENSOR(x)
#define ADD_POINTER_SENSOR_EX(x, y, z)
#define REMOVE_POINTER_SENSOR_EX(x, z)
#define CONSTRUCTOR_SERSOR(name)
#define DESTRUCTOR_SERSOR(name)
#endif // MEM_CONTROL else

}
#endif // MEMCONTROL_H
