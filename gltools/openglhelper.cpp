#include "openglhelper.h"
#include "memcontrol.h"
#include "openglhelperprivate.h"
using namespace Tools;

OpenglHelperPrivate* OpenglHelper::m_private = nullptr;
QAtomicInt OpenglHelper::m_privateRefs = 0;
XMutex OpenglHelper::m_privateMutex(XMutexModeNonRecursive);

OpenglHelper::OpenglHelper()
{
  ADD_POINTER_SENSOR(this);
  XMUTEX_LOCK(m_privateMutex);
  if (m_private == nullptr)
  {
    m_private = new OpenglHelperPrivate();
    ADD_POINTER_SENSOR(m_private);
  }
  XMUTEX_UNLOCK(m_privateMutex);
}

OpenglHelper::~OpenglHelper()
{
  XMUTEX_LOCK(m_privateMutex);
  if (m_private == nullptr)
  {
    REMOVE_POINTER_SENSOR(m_private);
    delete m_private;
    m_private = nullptr;
  }
  XMUTEX_UNLOCK(m_privateMutex);
  REMOVE_POINTER_SENSOR(this);
}

QString OpenglHelper::glVersionString()
{
  Q_ASSERT(m_private != nullptr);
  return m_private->glVersionString();
}

QString OpenglHelper::glError()
{
  Q_ASSERT(m_private != nullptr);
  return m_private->glError();
}

void OpenglHelper::printGLError()
{
  Q_ASSERT(m_private != nullptr);
  return m_private->printGLError();
}


Version OpenglHelper::glVersion()
{
  Q_ASSERT(m_private != nullptr);
  return m_private->glVersion();
}

QString OpenglHelper::glVendorString()
{
  Q_ASSERT(m_private != nullptr);
  return m_private->glVendorString();
}

QString OpenglHelper::glRendererString()
{
  Q_ASSERT(m_private != nullptr);
  return m_private->glRendererString();
}

void OpenglHelper::updateVersionFlags()
{
  Q_ASSERT(m_private != nullptr);
  return m_private->updateVersionFlags();
}

uint OpenglHelper::makeShader(eShaderType type, const QString& path, bool& ok)
{
  Q_ASSERT(m_private != nullptr);
  return m_private->makeShader(type, path, ok);
}

bool OpenglHelper::checkVersionFlags(const QList<eVersionFlag>& flags)
{
  Q_ASSERT(m_private != nullptr);
  return m_private->checkVersionFlags(flags);
}

