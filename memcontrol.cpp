#include "memcontrol.h"
#include "CuteLogger/cuteloggerinc.h"
#include "tools.h"
using namespace Tools;
#ifdef MEM_CONTROL
namespace
{
CONST_LITERAL FILENAME("memcontrol.csv");
CONST_LITERAL CSV_HEADER("action;file;function;line;ptr;size;owner;className;comment\n");
}
MemControlPrivate* MemControl::m_private = nullptr;


//-----------------------------------------------------------------------------------------------------------------------
// MemControlPrivateConsole impl
//-----------------------------------------------------------------------------------------------------------------------

void MemControlPrivateConsole::addPtr(const char* file,
                                      const char* function,
                                      int line,
                                      void* x,
                                      int size,
                                      void* owner,
                                      const char* className,
                                      const char* comment)
{
    LOG_TRACE("[MEMCONTROL] ADD %s::%s::%d ptr: %p size: %d owner: %p class: %s comment: %s",
              file,
              function,
              line,
              x,
              size,
              owner,
              className,
              comment);
}

void MemControlPrivateConsole::removePtr(const char* file,
                                         const char* function,
                                         int line,
                                         void* x,
                                         const char* comment)
{
    LOG_TRACE("[MEMCONTROL] REMOVE %s::%s::%d ptr: %p  comment: %s",
              file,
              function,
              line,
              x,
              comment);
}

//-----------------------------------------------------------------------------------------------------------------------
// MemControlPrivateCSV impl
//-----------------------------------------------------------------------------------------------------------------------

void MemControlPrivateCSV::addPtr(const char* file,
                                  const char* function,
                                  int line,
                                  void* x,
                                  int size,
                                  void* owner,
                                  const char* className,
                                  const char* comment)
{
    QString msg = QString("A;%1;%2;%3;%4;%5;%6;%7;%8\n")
            .arg(file)
            .arg(function)
            .arg(line)
            .arg(intptr_t(x))
            .arg(size)
            .arg(intptr_t(owner))
            .arg(className)
            .arg(QString(comment).replace(";", ","));
    write(msg);
}

void MemControlPrivateCSV::removePtr(const char* file,
                                     const char* function,
                                     int line,
                                     void* x,
                                     const char* comment)
{
    QString msg = QString("R;%1;%2;%3;%4;0;0;none;%5\n")
            .arg(file)
            .arg(function)
            .arg(line)
            .arg(intptr_t(x))
            .arg(QString(comment).replace(";", "."));
    write(msg);
}

void MemControlPrivateCSV::setTargetDir(const QString& path)
{
    do
    {
        if (path.isEmpty())
        {
            LOG_ERROR("empty path");
            break;
        }
        if (m_targetPath == path)
        {
            break;
        }
        m_targetPath = path;
        XMUTEX_LOCK(m_fileMutex);
        if (m_file.isOpen())
        {
            m_file.close();
        }
        QString filePath = CTools::pathNormalize(m_targetPath + "/" + FILENAME);
        uint index = 0;
        while (QFile::exists(filePath))
        {
            filePath = CTools::pathNormalize(m_targetPath + "/" + QString::number(index) + "_" + FILENAME);
        }
        m_file.setFileName(filePath);
        LOG_TRACE(QString("write mem contol data to %1").arg(filePath));
        if (!m_file.open(QIODevice::WriteOnly))
        {
            LOG_ERROR(QString("cannot open file %1 to write. reason: %2").arg(filePath, m_file.errorString()));
            XMUTEX_UNLOCK(m_fileMutex);
            break;
        }
        m_file.write(QString(CSV_HEADER).toLatin1());
        m_file.flush();
        XMUTEX_UNLOCK(m_fileMutex);
    }
    while(false);
}

void MemControlPrivateCSV::write(const QString& msg)
{
    if (!m_file.isOpen())
    {
        XMUTEX_LOCK(m_cacheMutex);
        m_cache.append(msg);
        LOG_WARNING("target file is not openeg. write to cache. cached messages: %lld", m_cache.size());
        XMUTEX_UNLOCK(m_cacheMutex);
        return;
    }
    bool dirtyCache = false;
    QStringList chacheCopy;
    XMUTEX_LOCK(m_cacheMutex);
    if (!m_cache.isEmpty())
    {
        dirtyCache = true;
        chacheCopy.swap(m_cache);
    }
    XMUTEX_UNLOCK(m_cacheMutex);

    XMUTEX_LOCK(m_fileMutex);
    if (dirtyCache)
    {
        for (const QString& record: m_cache)
        {
            m_file.write(record.toUtf8());
        }
    }
    m_file.write(msg.toLatin1());
    m_file.flush();
    XMUTEX_UNLOCK(m_fileMutex);
}

//-----------------------------------------------------------------------------------------------------------------------
// MemControl impl
//-----------------------------------------------------------------------------------------------------------------------



MemControl::MemControl()
{

}

MemControl::~MemControl()
{

}
void MemControl::checkPrivate()
{
    if (m_private == nullptr)
    {
#ifdef MEM_CONTROL_LOG
        m_private = new MemControlPrivateConsole();
#elif defined(MEM_CONTROL_CSV)
        m_private = new MemControlPrivateCSV();
#endif
    }
}
void MemControl::addPtr(const char* file,
                        const char* function,
                        int line,
                        void* x,
                        int size,
                        void* owner,
                        const char* className,
                        const char* comment)
{
#ifdef MEM_CONTROL
    checkPrivate();
    if (m_private != nullptr)
    {
        m_private->addPtr(file, function, line, x, size, owner, className, comment);
    }
    else
    {
        LOG_ERROR("Mem control is enabled but target is not selected. check build keys");
    }
#else
    UNUSED(file)
    UNUSED(function)
    UNUSED(line)
    UNUSED(x)
    UNUSED(size)
    UNUSED(owner)
    UNUSED(className)
    UNUSED(comment)
#endif
}

void MemControl::removePtr(const char* file,
                           const char* function,
                           int line,
                           void* x,
                           const char* comment)
{
    checkPrivate();
    if (m_private != nullptr)
    {
        m_private->removePtr(file, function, line, x, comment);
    }
    else
    {
        LOG_ERROR("Mem control is enabled but target is not selected. check build keys");
    }
}

void MemControl::setTargetDir(const QString& path)
{
    checkPrivate();
    if (m_private != nullptr)
    {
        m_private->setTargetDir(path);
    }
    else
    {
        LOG_ERROR("Mem control is enabled but target is not selected. check build keys");
    }
}
#endif // MEM_CONTROL
