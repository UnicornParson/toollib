#include "openglhelperprivate.h"
#ifdef FEATURE_OPENGL
using namespace Tools;

MAYBE_UNUSED_ATTR constexpr uint LogBufferSize(512);

OpenglHelperPrivate::OpenglHelperPrivate():
  m_versionFlags(eVersionFlag::FLAGS_COUNT, false)
{

}

QString OpenglHelperPrivate::glVersionString()
{
  const GLubyte * cVersion = glGetString(GL_VERSION);
  if (cVersion == nullptr)
  {
    LOG_WARNING("GL context not initialized");
    return QString();
  }
  return QString().asprintf("%s", cVersion);
}

QString OpenglHelperPrivate::glVendorString()
{
  const GLubyte * ptr = glGetString(GL_VENDOR);
  if (ptr == nullptr)
  {
    LOG_WARNING("GL context not initialized");
    return QString();
  }
  return QString().asprintf("%s", ptr);
}

QString OpenglHelperPrivate::glRendererString()
{
  const GLubyte * ptr = glGetString(GL_RENDER);
  if (ptr == nullptr)
  {
    LOG_WARNING("GL no renderer string");
    return QString();
  }
  return QString().asprintf("%s", ptr);
}

Version OpenglHelperPrivate::glVersion()
{
  QString s = glVersionString();
  QStringList parts = s.split(" ",  QString::SkipEmptyParts);
  if (parts.isEmpty())
  {
    LOG_WARNING(QString("invalid version %1").arg(s));
    return Version();
  }
  Version v;
  if (!v.fromString(parts[0]))
  {
    LOG_WARNING(QString("invalid version %1 cannot parse").arg(s));
    return Version();
  }
  return v;
}


uint OpenglHelperPrivate::makeShader(eShaderType type, const QString& path, bool& ok)
{
  GLuint shader = GL_INVALID_ENUM;
  ok = false;
  do
  {
    QString srcPath = path.trimmed();
    if (srcPath.isEmpty())
    {
      qWarning() << "empty path";
    }

    QFile fragmentShaderFile(srcPath);
    if (!fragmentShaderFile.open(QIODevice::ReadOnly))
    {
      qWarning() << "cannot open " << srcPath;
      break;
    }
    QByteArray fragmentShaderSource = fragmentShaderFile.readAll();
    if (fragmentShaderSource.isEmpty())
    {
      qWarning() << "file" << srcPath << "is empty";
      break;
    }
    uint nativeType = ShaderType::toNative(type);
    GLchar infoLog[LogBufferSize] = {0};
    shader = glCreateShader(nativeType);
    const GLchar* p = fragmentShaderSource.data();
    const GLchar* const * fragmentBuf = &p;
    glShaderSource(shader, 1, fragmentBuf, NULL);
    glCompileShader(shader);
    GLint success = GL_TRUE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success != GL_TRUE)
    {
      glGetShaderInfoLog(shader, LogBufferSize, NULL, infoLog);
      qWarning() << "cannot compile shader" << infoLog << "from " << srcPath << "status: " << success;
      glDeleteShader(shader);
      break;
    }
    else
    {
      qDebug() << "Compile: OK";
    }
    ok = true;
  }
  while(false);
  if (!ok)
  {
    shader = GL_INVALID_ENUM;
  }
  return shader;
}

void OpenglHelperPrivate::updateVersionFlags()
{
#ifdef GLEW_ARB_vertex_program
  m_versionFlags.setBit(eVersionFlag::GLEWARBVertexProgramFlag, GLEW_ARB_vertex_program);
#else
  m_versionFlags.setBit(eVersionFlag::GLEWARBVertexProgramFlag, false);
#endif

#ifdef GLEW_VERSION_1_3
  m_versionFlags.setBit(eVersionFlag::GLEW1_1, true);
#else
  m_versionFlags.setBit(eVersionFlag::GLEW1_1, false);
#endif
#define _setBit(x) \
  m_versionFlags.setBit(eVersionFlag::GLEW##x, __GLEW_VERSION_##x); \
  for(uint i = uint(eVersionFlag::GLEW1_1_andHigher); i < uint(eVersionFlag::GLEW##x##_andHigher); ++i)\
  {m_versionFlags.setBit(eVersionFlag(i), true);}

  _setBit(1_1);
  _setBit(1_2);
  _setBit(1_2_1);
  _setBit(1_3);
  _setBit(1_4);
  _setBit(1_5);
  _setBit(2_0);
  _setBit(2_1);
  _setBit(3_0);
  _setBit(3_1);
  _setBit(3_2);
  _setBit(3_3);
  _setBit(4_0);
  _setBit(4_1);
  _setBit(4_2);
  _setBit(4_3);
  _setBit(4_4);
  _setBit(4_5);
  _setBit(4_6);
#undef _setBit
}

bool OpenglHelperPrivate::checkVersionFlags(const QList<eVersionFlag> &flags)
{
  for (eVersionFlag f: flags)
  {
    if (!m_versionFlags.at(f))
    {
      return false;
    }
  }
  return true;
}

#else // FEATURE_OPENGL
OpenglHelperPrivate():m_versionFlags(1, false){}
void updateVersionFlags(){}
bool checkVersionFlags(const QList<eVersionFlag>& flags)
{
  Q_UNUSED(flags)
  return false;
}
Version glVersion(){return Version();}
QString glVersionString(){return QString();}
QString glVendorString(){return QString();}
QString glRendererString(){return QString();}
uint makeShader(eShaderType type, const QString& path, bool& ok)
{
  Q_UNUSED(type)
  Q_UNUSED(path)
  Q_UNUSED(ok)
  return 0;
}
#endif // FEATURE_OPENGL
